/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minjungk <minjungk@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/16 10:17:59 by minjungk          #+#    #+#             */
/*   Updated: 2023/01/18 00:30:51 by minjungk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	ft_except(int condition, char *file, int line)
{
	if (condition == 0)
		return ;
	ft_putstr_fd(file, STDERR_FILENO);
	ft_putstr_fd(":", STDERR_FILENO);
	ft_putnbr_fd(line, STDERR_FILENO);
	perror(":");
	exit(EXIT_FAILURE);
}

static char	**getpath(char *envp[])
{
	int	i;

	i = -1;
	while (envp && envp[++i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (ft_split(envp[i] + 5, ':'));
	}
	return (NULL);
}

static void	setcmd(char *envp[], char **cmd)
{
	int		i;
	char	*tmp[2];
	char	**path;

	path = getpath(envp);
	i = -1;
	while (path && path[++i])
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
	}
	free(path);
}

static void	pipeline(int in, char *cmd, int out, char *envp[])
{
	int		i;
	char	**sp;

	ft_except(dup2(in, STDIN_FILENO) < 0, __FILE__, __LINE__);
	ft_except(close(in) < 0, __FILE__, __LINE__);
	ft_except(dup2(out, STDOUT_FILENO) < 0, __FILE__, __LINE__);
	ft_except(close(out) < 0, __FILE__, __LINE__);
	sp = ft_split(cmd, ' ');
	ft_except(sp == NULL, __FILE__, __LINE__);
	if (access(sp[0], F_OK | X_OK) == -1)
		setcmd(envp, &sp[0]);
	ft_except(execve(sp[0], sp, envp) < 0, __FILE__, __LINE__);
	i = -1;
	while (sp[++i])
		free(sp[i]);
	free(sp);
}

int	main(int argc, char *argv[], char *envp[])
{
	pid_t		pid;
	int			fd;
	int			pipes[2];

	ft_except(argc != 5, __FILE__, __LINE__);
	ft_except(pipe(pipes) == -1, __FILE__, __LINE__);
	pid = fork();
	if (pid == 0)
	{
		close(pipes[STDIN_FILENO]);
		fd = open(argv[1], O_RDONLY);
		ft_except(fd == -1, __FILE__, __LINE__);
		pipeline(fd, argv[2], pipes[STDOUT_FILENO], envp);
	}
	else
	{
		close(pipes[STDOUT_FILENO]);
		fd = open(argv[4], O_CREAT | O_TRUNC | O_WRONLY, 0644);
		ft_except(fd == -1, __FILE__, __LINE__);
		pipeline(pipes[STDIN_FILENO], argv[3], fd, envp);
	}
	close(fd);
	close(pipes[STDIN_FILENO]);
	close(pipes[STDOUT_FILENO]);
	exit(EXIT_SUCCESS);
}
