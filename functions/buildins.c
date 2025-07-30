/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buildins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 17:37:07 by dgessner          #+#    #+#             */
/*   Updated: 2025/07/30 19:47:12 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"
#include <errno.h>

void	our_echo(t_cmd_node *cmd_node)
{
	int		i;
	bool	n_flag;

	if (!cmd_node || !cmd_node->cmd)
		return ;
	i = 1;
	n_flag = false;
	if (cmd_node->cmd[i] && strcmp(cmd_node->cmd[i], "-n") == 0)
	{
		n_flag = true;
		i++;
	}
	while (cmd_node->cmd[i])
	{
		printf("%s", cmd_node->cmd[i]);
		if (cmd_node->cmd[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
}

void	our_cd(t_cmd_node *cmd_node)
{
	char	*oldpwd;
	char	*newpwd;
	const char *target;

	if (!cmd_node || !cmd_node->cmd)
		return ;

	oldpwd = getcwd(NULL, 0);
	if (!cmd_node->cmd[1])
		target = getenv("HOME");
	else if (strcmp(cmd_node->cmd[1], "-") == 0)
	{
		target = getenv("OLDPWD");
		if (!target)
		{
			fprintf(stderr, "minishell: cd: OLDPWD not set\n");
			free(oldpwd);
			return ;
		}
		printf("%s\n", target);
	}
	else
		target = cmd_node->cmd[1];

	if (!target || chdir(target) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", target, strerror(errno));
		free(oldpwd);
		return ;
	}

	newpwd = getcwd(NULL, 0);
	if (oldpwd)
		setenv("OLDPWD", oldpwd, 1);
	if (newpwd)
		setenv("PWD", newpwd, 1);
	free(oldpwd);
	free(newpwd);
}

void	search_command(t_cmd_node *cmd_node)
{
	if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd[0])
		return ;
	if (strcmp(cmd_node->cmd[0], "echo") == 0)
		our_echo(cmd_node);
	else if (strcmp(cmd_node->cmd[0], "cd") == 0)
		our_cd(cmd_node);
	else
		printf("minishell: %s: command not found\n", cmd_node->cmd[0]);
}
