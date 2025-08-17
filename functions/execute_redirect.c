/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/17 21:16:18 by dgessner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"
#include "parse.h"

int	redir_in(t_file_node *f)
{
	int	fd;

	fd = open(f->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(f->filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redir_out(t_file_node *f)
{
	int	fd;

	fd = open(f->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(f->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redir_append(t_file_node *f)
{
	int	fd;

	fd = open(f->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(f->filename);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redir_heredoc(t_file_node *f)
{
	int		pfd[2];
	char	*content;

	if (pipe(pfd) == -1)
		return (-1);
	content = collect_heredoc_content(f->filename);
	if (content)
	{
		write(pfd[1], content, ft_strlen(content));
		free(content);
	}
	close(pfd[1]);
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
	return (0);
}

/**
 * Applies all redirections for a command with variable expansion.
 * Returns 0 on success, -1 on failure.
 */
int	apply_redirections(t_file_list *files, char **envp)
{
	t_file_node	*f;
	char		*expanded_filename;
	char		*original_filename;

	if (!files || !files->head)
		return (0);
	f = files->head;
	while (f)
	{
		expanded_filename = prepare_redirection_filename(f, envp,
				&original_filename);
		if (execute_redirection(f) == -1)
		{
			restore_filename(f, expanded_filename, original_filename);
			return (-1);
		}
		restore_filename(f, expanded_filename, original_filename);
		f = f->next;
	}
	return (0);
}
