/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 20:09:57 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/02 21:41:31 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

/* ========================================================================= */
/*                        Debug Utilities                                    */
/* ========================================================================= */

void print_files(t_file_node *file)
{
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

void print_args(char **args)
{
    int i = 1;

    if (!args[1])
    {
        printf("Arguments: (none)\n");
        return;
    }
    printf("Arguments:\n");
    while (args[i])
    {
        printf("  [%d] %s\n", i, args[i]);
        i++;
    }
}

void print_cmd_type(t_cmd_node *cmd)
{
    if (cmd->cmd_type == CMD_PIPE)
        printf("Type: PIPE (connects to next command)\n");
    else if (cmd->cmd_type == CMD_BACKGROUND)
        printf("Type: BACKGROUND (&)\n");
    else if (cmd->cmd_type == CMD_SIMPLE)
        printf("Type: SIMPLE\n");
    else
        printf("Type: OTHER\n");
}

void print_command(t_cmd_node *cmd, int index)
{
    printf("\n--- Command %d ---\n", index);
    print_cmd_type(cmd);
    if (cmd->cmd && cmd->cmd[0])
    {
        printf("Program: %s\n", cmd->cmd[0]);
        print_args(cmd->cmd);
    }
    else
        printf("Program: (empty command)\n");
    if (cmd->files && cmd->files->head)
    {
        printf("Redirections:\n");
        print_files(cmd->files->head);
    }
    else
        printf("Redirections: (none)\n");
}

void print_parsed_commands(t_cmd_list *cmd_list)
{
    t_cmd_node *cur;
    int index = 1;

    if (!cmd_list || !cmd_list->head)
    {
        printf("No commands to execute.\n");
        return;
    }
    printf("\n========== PARSED COMMANDS ==========\n");
    cur = cmd_list->head;
    while (cur)
    {
        print_command(cur, index++);
        cur = cur->next;
    }
    printf("\n====================================\n\n");
}
