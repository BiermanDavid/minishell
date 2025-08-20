/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_process.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 05:29:16 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/**
 * Checks if command is a shell operator that should not be executed.
 * Returns 1 if it's an invalid command, 0 if it's valid.
 */
static int	is_shell_operator(const char *cmd)
{
	if (!cmd || !*cmd)
		return (1);
	if (ft_strncmp(cmd, ">>", 3) == 0 || ft_strncmp(cmd, "<<", 3) == 0
		|| ft_strncmp(cmd, ">", 2) == 0 || ft_strncmp(cmd, "<", 2) == 0
		|| ft_strncmp(cmd, "|", 2) == 0 || ft_strncmp(cmd, "&", 2) == 0)
		return (1);
	return (0);
}

/**
 * Resolves and executes command using absolute path or PATH search.
 * 
 * Detects absolute/relative paths via '/' character, executes directly
 * or searches PATH directories for executable. Child process only.
 * 
 * @param node Command node containing command array to execute
 * @param env Environment variables array for execution context
 */
void	exec_command(t_cmd_node *node, char **env)
{
	char	**paths;

	if (is_shell_operator(node->cmd[0]))
		exit(print_command_not_found(node->cmd[0]));
	if (ft_strchr(node->cmd[0], '/'))
		exec_absolute_path(node, env);
	paths = get_path_dirs(env);
	try_path_execution(node, env, paths);
	free_split(paths);
}

/**
 * Executes external command in child process with signals and redirections.
 * Forks, sets up child signals, applies redirections, then executes command.
 * Parent waits for child and returns exit status.
 * @param node Command node with command array and redirection list
 * @param env Environment variables array for execution
 * @return Child process exit status (0=success, 1-255=errors, 127=not found)
 */
int	exec_external(t_cmd_node *node, char **env)
{
	pid_t	pid;
	int		status;

	set_minimal_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		if (apply_redirections(node->files, env) == -1)
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

/**
 * Executes a builtin command with redirection support.
 * Saves and restores file descriptors around execution.
 * Returns the exit status of the builtin command.
 */
int	exec_builtin_redir(t_cmd_node *node, char ***envp)
{
	int	saved_in;
	int	saved_out;
	int	result;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	if (apply_redirections(node->files, *envp) == -1)
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

/**
 * Handles variable assignment commands (VAR=value).
 * Sets the environment variable to the specified value.
 * Returns 0 on success.
 */
int	handle_assignment(t_cmd_node *node, char ***envp)
{
	char	*eq;

	eq = ft_strchr((char *)node->cmd[0], '=');
	*eq = '\0';
	env_set(envp, node->cmd[0], eq + 1);
	*eq = '=';
	return (0);
}
