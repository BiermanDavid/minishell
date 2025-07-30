/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/07/30 21:47:14 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "execute.h"
// #include "shell.h"
// #include "../libft/libft.h"
// #include <sys/wait.h>

// /**
//  * Frees a NULL terminated array of strings.
//  */
// static void    free_split(char **arr)
// {
//     size_t  i;

//     if (!arr)
//         return ;
//     i = 0;
//     while (arr[i])
//     {
//         free(arr[i]);
//         i++;
//     }
//     free(arr);
// }

// /**
//  * Searches PATH for executable corresponding to cmd.
//  * Returns allocated path string or NULL if not found.
//  */
// static char    *find_cmd(char *cmd)
// {
//     char    **paths;
//     char    *full;
//     char    *tmp;
//     int     i;

//     paths = ft_split(getenv("PATH"), ':');
//     i = 0;
//     if (!paths || access(cmd, X_OK) == 0)
//         return (cmd);
//     while (paths[i])
//     {
//         tmp = ft_strjoin(paths[i], "/");
//         full = ft_strjoin(tmp, cmd);
//         free(tmp);
//         if (access(full, X_OK) == 0)
//             return (full);
//         free(full);
//         i++;
//     }
//     free_split(paths);
//     return (NULL);
// }

// /**
//  * Executes one command in a child process.
//  * in_fd/out_fd specify the pipe ends to use.
//  */
// static void    child_process(t_cmd_node *cmd, int in_fd,
//                              int out_fd, char **envp)
// {
//     char    **argv;
//     char    *cmd_path;

//     dup2(in_fd, 0);
//     dup2(out_fd, 1);
//     argv = cmd->cmd;
//     cmd_path = find_cmd(argv[0]);
//     if (!cmd_path)
//     {
//         fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
//         exit(1);
//     }
//     execve(cmd_path, argv, envp);
//     perror("execve failed");
//     exit(1);
// }

// /**
//  * Forks a new process to execute one stage of the pipeline.
//  */
// static void    spawn_stage(t_cmd_node *cmd, int in_fd,
//                           int out_fd, char **envp)
// {
//     if (fork() == 0)
//         child_process(cmd, in_fd, out_fd, envp);
// }

// /**
//  * Executes a list of commands connected by pipes.
//  */
// void    run_piped_commands(t_cmd_list *cmd_list, char **envp)
// {
//     int         pipe_fd[2];
//     int         in_fd;
//     t_cmd_node  *cmd;

//     in_fd = 0;
//     cmd = cmd_list->head;
//     while (cmd && cmd->next)
//     {
//         pipe(pipe_fd);
//         spawn_stage(cmd, in_fd, pipe_fd[1], envp);
//         close(pipe_fd[1]);
//         if (in_fd != 0)
//             close(in_fd);
//         in_fd = pipe_fd[0];
//         cmd = cmd->next;
//     }
//     spawn_stage(cmd, in_fd, 1, envp);
//     if (in_fd != 0)
//         close(in_fd);
//     while (wait(NULL) > 0)
//         ;
// }
