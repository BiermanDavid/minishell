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

/**
 * Establishes input redirection from file to standard input stream.
 * Opens specified file in read-only mode and duplicates file descriptor
 * to stdin, enabling command to read from file instead of terminal input.
 * Essential for shell input redirection operator implementation.
 * @param f File node containing target filename for input source
 * @return 0 on successful redirection setup, -1 on file access failure
 */
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

/**
 * Establishes output redirection from standard output to file.
 * Creates or truncates target file and duplicates file descriptor to stdout,
 * directing all command output to file instead of terminal display.
 * Implements shell output redirection with file creation and truncation.
 * @param f File node containing target filename for output destination
 * @return 0 on successful redirection setup, -1 on file creation failure
 */
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

/**
 * Establishes append redirection from standard output to file end.
 * Opens target file in append mode, preserving existing content while
 * redirecting stdout to file end for cumulative output operations.
 * Implements shell append redirection without data loss from truncation.
 * @param f File node containing target filename for append destination
 * @return 0 on successful redirection setup, -1 on file access failure
 */
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

/**
 * Implements here document redirection using inter-process pipe communication.
 * Creates pipe pair, writes collected heredoc content to write end, then
 * redirects read end to stdin for command input. Enables inline text
 * input without temporary files during shell here document processing.
 * @param f File node containing heredoc delimiter for content collection
 * @return 0 on successful pipe redirection, -1 on pipe creation failure
 */
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
 * Processes complete redirection sequence for command execution.
 * Iterates through redirection list, expands filenames with environment
 * variables, executes each redirection operation, then restores original
 * filenames. Ensures proper file descriptor setup before command execution.
 * @param files Linked list of redirection operations to apply
 * @param envp Environment variables array for filename expansion
 * @return 0 on successful redirection application, -1 on any operation failure
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
