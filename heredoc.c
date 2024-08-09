/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 00:14:21 by minjungk          #+#    #+#             */
/*   Updated: 2024/08/09 21:42:40 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//#include <stdio.h>

int	heredoc(char *word)
{
	int		pipes[2];
	size_t	word_len;
	char	*line;

	if (word == NULL || pipe(pipes) == -1)
		return (-1);
	word_len = ft_strlen(word);
	line = get_next_line(STDIN_FILNO);
	while (line)
	{
		if (ft_strncmp(line, word, word_len) == 0
			&& (line[word_len] == '\n' || line[word_len] == '\0'))
			break ;
		if (write(pipes[1], line, ft_strlen(line)) < 0)
		{
			close(pipes[0]);
			pipes[0] = -1;
			break ;
		}
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	free(line);
	close(pipes[1]);
	return (pipes[0]);
}

/*
   int     main(int argc, char **argv)
   {
   int             fd;
   int             ret;
   char    buf[1024];

   if (argc != 2 || argv == NULL)
   return (1);
   fd = heredoc(argv[1]);
   if (fd < 0)
   return (1);
   ret = read(fd, buf, sizeof(buf));
   while (ret > 0)
   {
   printf("%.*s", ret, buf);
   ret = read(fd, buf, sizeof(buf));
   }
   close(fd);
   return (0);
   }
 */
