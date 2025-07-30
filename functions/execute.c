/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:24:05 by dgessner          #+#    #+#             */
/*   Updated: 2025/07/30 20:24:11 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <fcntl.h>
#include <sys/wait.h>

/* ========================================================================== */
/*                          Internal builtin helpers                          */
/* ========================================================================== */

static int builtin_echo(char **args)
{
    int i;
    int newline;

    newline = 1;
    i = 1;
    if (args[1] && strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return (0);
}

static int builtin_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    return (0);
}

static int builtin_cd(char **args)
{
    const char *path;

    path = args[1];
    if (!path)
        path = getenv("HOME");
    if (!path || chdir(path) != 0)
    {
        perror("cd");
        return (1);
    }
    return (0);
}

extern char **environ;

static int builtin_env(void)
{
    char **env;

    env = environ;
    while (env && *env)
    {
        printf("%s\n", *env);
        env++;
    }
    return (0);
}

static int builtin_exit(char **args)
{
    (void)args;
    exit(0);
    return (0);
}

static int builtin_export(char **args)
{
    int     i;
    char    *eq;

    if (!args[1])
        return (builtin_env());
    i = 1;
    while (args[i])
    {
        eq = strchr(args[i], '=');
        if (eq)
        {
            *eq = '\0';
            setenv(args[i], eq + 1, 1);
        }
        i++;
    }
    return (0);
}

static int builtin_unset(char **args)
{
    int i;

    i = 1;
    while (args[i])
    {
        unsetenv(args[i]);
        i++;
    }
    return (0);
}

static int is_builtin(const char *cmd)
{
    if (!cmd)
        return (0);
    return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0
        || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "env") == 0
        || strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0
        || strcmp(cmd, "unset") == 0);
}

static int exec_builtin(t_cmd_node *node)
{
    if (strcmp(node->cmd[0], "echo") == 0)
        return (builtin_echo(node->cmd));
    else if (strcmp(node->cmd[0], "cd") == 0)
        return (builtin_cd(node->cmd));
    else if (strcmp(node->cmd[0], "pwd") == 0)
        return (builtin_pwd());
    else if (strcmp(node->cmd[0], "env") == 0)
        return (builtin_env());
    else if (strcmp(node->cmd[0], "exit") == 0)
        return (builtin_exit(node->cmd));
    else if (strcmp(node->cmd[0], "export") == 0)
        return (builtin_export(node->cmd));
    else if (strcmp(node->cmd[0], "unset") == 0)
        return (builtin_unset(node->cmd));
    return (1);
}

/* ========================================================================== */
/*                           Redirection utilities                            */
/* ========================================================================== */

static int apply_redirections(t_file_list *files)
{
    t_file_node *f;
    int         fd;
    int         pfd[2];

    f = files ? files->head : NULL;
    while (f)
    {
        if (f->redir_type == REDIR_IN)
        {
            fd = open(f->filename, O_RDONLY);
            if (fd < 0)
            {
                perror(f->filename);
                return (-1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        else if (f->redir_type == REDIR_OUT)
        {
            fd = open(f->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0)
            {
                perror(f->filename);
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (f->redir_type == REDIR_APPEND)
        {
            fd = open(f->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0)
            {
                perror(f->filename);
                return (-1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        else if (f->redir_type == REDIR_HEREDOC)
        {
            if (pipe(pfd) == -1)
                return (-1);
            write(pfd[1], f->filename, strlen(f->filename));
            close(pfd[1]);
            dup2(pfd[0], STDIN_FILENO);
            close(pfd[0]);
        }
        f = f->next;
    }
    return (0);
}

/* ========================================================================== */
/*                             Process execution                              */
/* ========================================================================== */

static int exec_external(t_cmd_node *node)
{
    pid_t   pid;
    int     status;

    pid = fork();
    if (pid == 0)
    {
        setup_child_signals();
        if (apply_redirections(node->files) == -1)
            exit(1);
        execvp(node->cmd[0], node->cmd);
        perror(node->cmd[0]);
        exit(127);
    }
    else if (pid < 0)
    {
        perror("fork");
        return (1);
    }
    waitpid(pid, &status, 0);
    return (WIFEXITED(status) ? WEXITSTATUS(status) : 1);
}

static int execute_single(t_cmd_node *node)
{
    int saved_in;
    int saved_out;
    int result;

    if (!node->cmd || !node->cmd[0])
        return (0);
    if (is_builtin(node->cmd[0]))
    {
        saved_in = dup(STDIN_FILENO);
        saved_out = dup(STDOUT_FILENO);
        if (apply_redirections(node->files) == -1)
        {
            dup2(saved_in, STDIN_FILENO);
            dup2(saved_out, STDOUT_FILENO);
            close(saved_in);
            close(saved_out);
            return (1);
        }
        result = exec_builtin(node);
        dup2(saved_in, STDIN_FILENO);
        dup2(saved_out, STDOUT_FILENO);
        close(saved_in);
        close(saved_out);
        return (result);
    }
    return (exec_external(node));
}

/* ========================================================================== */
/*                        Pipeline execution (simple)                         */
/* ========================================================================== */

static t_cmd_node *exec_pipeline(t_cmd_node *start)
{
    int         prev_fd;
    int         pipefd[2];
    pid_t       pids[64];
    int         count;
    t_cmd_node *node;

    prev_fd = STDIN_FILENO;
    count = 0;
    node = start;
    while (node && node->cmd_type == CMD_PIPE)
    {
        if (pipe(pipefd) == -1)
            return (node->next);
        pids[count] = fork();
        if (pids[count] == 0)
        {
            setup_child_signals();
            dup2(prev_fd, STDIN_FILENO);
            dup2(pipefd[1], STDOUT_FILENO);
            if (prev_fd != STDIN_FILENO)
                close(prev_fd);
            close(pipefd[0]);
            if (apply_redirections(node->files) == -1)
                exit(1);
            if (is_builtin(node->cmd[0]))
                exit(exec_builtin(node));
            execvp(node->cmd[0], node->cmd);
            perror(node->cmd[0]);
            exit(127);
        }
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
        close(pipefd[1]);
        prev_fd = pipefd[0];
        node = node->next;
        count++;
    }
    /* last command */
    pids[count] = fork();
    if (pids[count] == 0)
    {
        setup_child_signals();
        dup2(prev_fd, STDIN_FILENO);
        if (prev_fd != STDIN_FILENO)
            close(prev_fd);
        if (apply_redirections(node->files) == -1)
            exit(1);
        if (is_builtin(node->cmd[0]))
            exit(exec_builtin(node));
        execvp(node->cmd[0], node->cmd);
        perror(node->cmd[0]);
        exit(127);
    }
    if (prev_fd != STDIN_FILENO)
        close(prev_fd);
    count++;
    while (count-- > 0)
        waitpid(pids[count], NULL, 0);
    return (node->next);
}

/* ========================================================================== */
/*                       Public execution entry point                         */
/* ========================================================================== */

int execution_manager(t_cmd_list *cmd_list)
{
    t_cmd_node *node;

    if (!cmd_list || !cmd_list->head)
        return (-1);
    node = cmd_list->head;
    while (node)
    {
        if (node->cmd_type == CMD_PIPE)
            node = exec_pipeline(node);
        else
        {
            execute_single(node);
            node = node->next;
        }
    }
    return (0);
}

/* ========================================================================== */
/*                    Debug function to print parsed commands                 */
/* ========================================================================== */

void print_parsed_commands(t_cmd_list *cmd_list)
{
    t_cmd_node *current;
    t_file_node *file;
    int         cmd_index;
    int         arg_index;

    if (!cmd_list || !cmd_list->head)
    {
        printf("No commands to execute.\n");
        return ;
    }
    printf("\n========== PARSED COMMANDS ==========\n");
    current = cmd_list->head;
    cmd_index = 1;
    while (current)
    {
        printf("\n--- Command %d ---\n", cmd_index);
        if (current->cmd_type == CMD_PIPE)
            printf("Type: PIPE (connects to next command)\n");
        else if (current->cmd_type == CMD_BACKGROUND)
            printf("Type: BACKGROUND (&)\n");
        else if (current->cmd_type == CMD_SIMPLE)
            printf("Type: SIMPLE\n");
        else
            printf("Type: OTHER\n");
        if (current->cmd && current->cmd[0])
        {
            printf("Program: %s\n", current->cmd[0]);
            if (current->cmd[1])
            {
                printf("Arguments:\n");
                arg_index = 1;
                while (current->cmd[arg_index])
                {
                    printf("  [%d] %s\n", arg_index, current->cmd[arg_index]);
                    arg_index++;
                }
            }
            else
                printf("Arguments: (none)\n");
        }
        else
            printf("Program: (empty command)\n");
        if (current->files && current->files->head)
        {
            printf("Redirections:\n");
            file = current->files->head;
            while (file)
            {
                if (file->redir_type == REDIR_IN)
                    printf("  INPUT:  < %s\n", file->filename);
                else if (file->redir_type == REDIR_OUT)
                    printf("  OUTPUT: > %s\n", file->filename);
                else if (file->redir_type == REDIR_APPEND)
                    printf("  APPEND: >> %s\n", file->filename);
                else if (file->redir_type == REDIR_HEREDOC)
                    printf("  HEREDOC: << (content stored)\n");
                file = file->next;
            }
        }
        else
            printf("Redirections: (none)\n");
        current = current->next;
        cmd_index++;
    }
    printf("\n====================================\n\n");
}
