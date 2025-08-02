/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 20:24:05 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/02 22:00:27 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int g_exit_status = 0;

/* ========================================================================= */
/*                           Redirection Utilities                           */
/* ========================================================================= */

int redir_in(t_file_node *f)
{
    int fd = open(f->filename, O_RDONLY);
    if (fd < 0)
	{
        perror(f->filename);
        return (-1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return (0);
}

int redir_out(t_file_node *f)
{
    int fd = open(f->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror(f->filename);
        return (-1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

int redir_append(t_file_node *f)
{
    int fd = open(f->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror(f->filename);
        return (-1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

int redir_heredoc(t_file_node *f)
{
    int pfd[2];

    if (pipe(pfd) == -1)
        return (-1);
    write(pfd[1], f->filename, ft_strlen(f->filename));
    close(pfd[1]);
    dup2(pfd[0], STDIN_FILENO);
    close(pfd[0]);
    return (0);
}

int apply_redirections(t_file_list *files)
{
    t_file_node *f = files ? files->head : NULL;
    while (f)
    {
        if (f->redir_type == REDIR_IN && redir_in(f) == -1)
            return (-1);
        else if (f->redir_type == REDIR_OUT && redir_out(f) == -1)
            return (-1);
        else if (f->redir_type == REDIR_APPEND && redir_append(f) == -1)
            return (-1);
        else if (f->redir_type == REDIR_HEREDOC && redir_heredoc(f) == -1)
            return (-1);
        f = f->next;
    }
    return (0);
}

/* ========================================================================= */
/*                           Process Execution                               */
/* ========================================================================= */

int exec_external(t_cmd_node *node)
{
    pid_t pid;
    int status;

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

int exec_builtin_redir(t_cmd_node *node)
{
    int saved_in = dup(STDIN_FILENO);
    int saved_out = dup(STDOUT_FILENO);
    int result;

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

int execute_single(t_cmd_node *node)
{
    if (!node->cmd || !node->cmd[0])
        return (0);
    if (is_builtin(node->cmd[0]))
        return (exec_builtin_redir(node));
    return (exec_external(node));
}

/* ========================================================================= */
/*                       Public Execution Entry Point                        */
/* ========================================================================= */

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
			g_exit_status = execute_single(node);
            node = node->next;
        }
    }
    return (0);
}
