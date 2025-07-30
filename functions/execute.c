/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabierma <dabierma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 15:41:18 by dabierma          #+#    #+#             */
/*   Updated: 2025/07/30 15:41:19 by dabierma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "shell.h"

/**
 * Main execution manager function.
 * Takes parsed command list and handles execution.
 */
int	execution_manager(t_cmd_list *cmd_list)
{
	if (!cmd_list || !cmd_list->head)
		return (-1);
	print_parsed_commands(cmd_list);
	return (0);
}

/**
 * Prints all parsed commands in a clean, readable format.
 * Shows command structure, arguments, and redirections clearly.
 */
void	print_parsed_commands(t_cmd_list *cmd_list)
{
	t_cmd_node	*current;
	t_file_node	*file;
	int			cmd_index;
	int			arg_index;

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
