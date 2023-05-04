/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_binary.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/05 00:03:50 by minjungk          #+#    #+#             */
/*   Updated: 2023/05/05 01:54:01 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	**getpath(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
		++i;
	}
	return (NULL);
}

void	replace_binary(char **envp, char **cmd)
{
	int		i;
	char	*tmp[2];
	char	**path;

	if (envp == NULL || cmd == NULL)
		return ;
	path = getpath(envp);
	i = 0;
	while (path && path[i])
	{
		tmp[0] = ft_strjoin(path[i], "/");
		tmp[1] = ft_strjoin(tmp[0], *cmd);
		if (access(tmp[1], F_OK | X_OK) == 0)
		{
			free(*cmd);
			*cmd = ft_strdup(tmp[1]);
		}
		free(tmp[0]);
		free(tmp[1]);
		free(path[i]);
		++i;
	}
	free(path);
}
