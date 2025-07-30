/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 21:12:35 by dgessner          #+#    #+#             */
/*   Updated: 2025/07/30 21:43:30 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <fcntl.h>
#include <sys/wait.h>

/* ========================================================================== */
/*                          Internal builtin helpers                          */
/* ========================================================================== */
//PRINT ERROR IMPLEMENTATION NEEDS TO BE ADDED

int builtin_echo(char **args)
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

int builtin_pwd(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    return (0);
}

int builtin_cd(char **args)
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

int builtin_env(void)
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

int builtin_exit(char **args)
{
    (void)args;
    exit(0);
    return (0);
}

int builtin_export(char **args)
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

int builtin_unset(char **args)
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

int is_builtin(const char *cmd)
{
    if (!cmd)
        return (0);
    return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0
        || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "env") == 0
        || strcmp(cmd, "exit") == 0 || strcmp(cmd, "export") == 0
        || strcmp(cmd, "unset") == 0);
}

int exec_builtin(t_cmd_node *node)
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