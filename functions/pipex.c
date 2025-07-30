/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 19:51:40 by dgessner          #+#    #+#             */
/*   Updated: 2025/07/30 19:53:15 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"

static char	*find_cmd(char *cmd, char **envp)
{
	char	**paths = ft_split(getenv("PATH"), ':');
	char	*full, *tmp;
	int		i = 0;

	if (!paths || access(cmd, X_OK) == 0)
		return (cmd);
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, X_OK) == 0)
			return (full);
		free(full);
		i++;
	}
	free_split(paths);
	return (NULL);
}

static void	child_process(t_cmd_node *cmd, int in_fd, int out_fd, char **envp)
{
	char	**argv;
	char	*cmd_path;

	dup2(in_fd, 0);
	dup2(out_fd, 1);
	argv = cmd->cmd;
	cmd_path = find_cmd(argv[0], envp);
	if (!cmd_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", argv[0]);
		exit(1);
	}
	execve(cmd_path, argv, envp);
	perror("execve failed");
	exit(1);
}

void	run_piped_commands(t_cmd_list *cmd_list, char **envp)
{
	int			pipe_fd[2];
	int			in_fd = 0;
	pid_t		pid;
	t_cmd_node	*cmd = cmd_list->head;

	while (cmd && cmd->next)
	{
		pipe(pipe_fd);
		if ((pid = fork()) == 0)
		{
			close(pipe_fd[0]);
			child_process(cmd, in_fd, pipe_fd[1], envp);
		}
		close(pipe_fd[1]);
		if (in_fd != 0)
			close(in_fd);
		in_fd = pipe_fd[0];
		cmd = cmd->next;
	}
	// last command (no pipe after it)
	if ((pid = fork()) == 0)
		child_process(cmd, in_fd, 1, envp);
	if (in_fd != 0)
		close(in_fd);
	while (wait(NULL) > 0)
		;
}
