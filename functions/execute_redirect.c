/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dgessner <dgessner@student.42heilbronn.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/03 10:00:00 by dgessner          #+#    #+#             */
/*   Updated: 2025/08/04 21:52:00 by dgessner         ###   ########.fr       */
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
	char	*heredoc_content;

	if (pipe(pfd) == -1)
		return (-1);
	heredoc_content = collect_heredoc_content(f->filename);
	if (!heredoc_content)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (-1);
	}
	write(pfd[1], heredoc_content, ft_strlen(heredoc_content));
	close(pfd[1]);
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
	free(heredoc_content);
	return (0);
}

int	apply_redirections(t_file_list *files)
{
	t_file_node	*f;

	if (files)
		f = files->head;
	else
		f = NULL;
	if (!files)
		return (0);
	if (!files->head)
		return (0);
	while (f)
	{
		if (f->redir_type == REDIR_IN && redir_in(f) == -1)
			return (-1);
		else if (f->redir_type == REDIR_OUT && redir_out(f) == -1)
			return (-1);
		else if (f->redir_type == REDIR_APPEND && redir_append(f) == -1)
			return (-1);
		else if (f->redir_type == REDIR_HEREDOC && redir_heredoc(f) == -1)
			return (-1);
		f = f->next;
	}
	return (0);
}
