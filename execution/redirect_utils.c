/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:05:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/20 06:20:11 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Expands filename tokens and prepares file node for redirection operation.
 * Processes environment variables, wildcards, 
 * and special characters in filename.
 * Preserves original filename pointer for restoration after redirection attempt.
 * @param f File node containing filename to expand
 * @param envp Environment variables array for expansion context
 * @param original_filename Pointer to store original filename for restoration
 * @return Expanded filename string if expansion occurred, NULL if no expansion
 */
char	*prepare_redirection_filename(t_file_node *f, char **envp,
		char **original_filename)
{
	char	*expanded_filename;

	expanded_filename = expand_token(f->filename, envp);
	if (expanded_filename)
	{
		*original_filename = f->filename;
		f->filename = expanded_filename;
	}
	return (expanded_filename);
}

/**
 * Restores original filename and deallocates expanded filename memory.
 * Ensures proper cleanup of dynamically allocated filename strings after
 * redirection processing completes, preventing memory leaks in file operations.
 * @param f File node to restore original filename to
 * @param expanded_filename Allocated expanded filename string to free
 * @param original_filename Original filename pointer to restore
 */
void	restore_filename(t_file_node *f, char *expanded_filename,
		char *original_filename)
{
	if (expanded_filename)
	{
		free(f->filename);
		f->filename = original_filename;
	}
}

/**
 * Dispatches redirection execution to appropriate handler function.
 * Routes file descriptor manipulation based on redirection operator type,
 * supporting input (<), output (>), append (>>), and heredoc (<<) operations.
 * @param f File node containing redirection type and target filename
 * @return 0 on successful redirection setup, -1 on file operation failure
 */
int	execute_redirection(t_file_node *f)
{
	if (f->redir_type == REDIR_IN)
		return (redir_in(f));
	else if (f->redir_type == REDIR_OUT)
		return (redir_out(f));
	else if (f->redir_type == REDIR_APPEND)
		return (redir_append(f));
	else if (f->redir_type == REDIR_HEREDOC)
		return (redir_heredoc(f));
	return (0);
}
