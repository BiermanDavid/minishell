/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 21:05:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/17 21:05:00 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

/**
 * Expands filename and prepares for redirection.
 * Returns expanded filename or NULL if no expansion needed.
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
 * Restores original filename after redirection attempt.
 * Cleans up expanded filename if it was allocated.
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
 * Executes the appropriate redirection based on type.
 * Returns 0 on success, -1 on failure.
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
