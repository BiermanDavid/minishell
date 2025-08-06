/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/06 18:12:43 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void	exec_command(t_cmd_node *node, char **env)
{
	char	**paths;

	if (ft_strchr(node->cmd[0], '/'))
		exec_absolute_path(node, env);
	paths = get_path_dirs(env);
	try_path_execution(node, env, paths);
	free_split(paths);
}

int	exec_external(t_cmd_node *node, char **env)
{
	pid_t	pid;
	int		status;

	set_minimal_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (apply_redirections(node->files) == -1)
			exit(1);
		exec_command(node, env);
		exit(127);
	}
	else if (pid < 0)
	{
		initialize_shell_signals();
		perror("fork");
		return (1);
	}
	waitpid(pid, &status, 0);
	initialize_shell_signals();
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	exec_builtin_redir(t_cmd_node *node, char ***envp)
{
	int	saved_in;
	int	saved_out;
	int	result;

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
	result = exec_builtin(node, envp);
	dup2(saved_in, STDIN_FILENO);
	dup2(saved_out, STDOUT_FILENO);
	close(saved_in);
	close(saved_out);
	return (result);
}

int	handle_assignment(t_cmd_node *node, char ***envp)
{
	char	*eq;

	eq = ft_strchr((char *)node->cmd[0], '=');
	*eq = '\0';
	env_set(envp, node->cmd[0], eq + 1);
	*eq = '=';
	return (0);
}

int	is_variable_assignment(t_cmd_node *node)
{
	return (is_assignment(node->cmd[0]) && node->cmd[1] == NULL);
}
