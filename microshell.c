/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablaamim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:52:14 by ablaamim          #+#    #+#             */
/*   Updated: 2023/01/24 17:52:24 by ablaamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

void putstr(int fd, char *str)
{
	while (*str)
		write(fd, str++, 1);
}

int	fork1()
{
	int pid = fork();

	if (pid == -1)
		write(2, "error : fatal\n", sizeof("error : fatal\n"));
	return (pid);
}

int	pipe1(int fd[2])
{
	int	ret;

	ret = pipe(fd);
	if (ret == -1)
		write(2, "error : fatal\n", sizeof("error : fatal\n"));
	return (ret);
}

void	cd(char **cmd)
{
	int	ac;

	ac = 0;
	while (cmd[ac])
		ac++;
	if (ac != 2)
	{
		write(2, "error: cd: bad arguments\n", sizeof("error: cd: bad arguments"));
		return ;
	}
	int ret = chdir(cmd[1]);
	
	if (ret == -1)
	{
		write(2, "error: cd: cannot change directory to ", sizeof("error: cd: cannot change directory to"));
		putstr(2, cmd[1]);
		write(2, "\n", 1);
		return ;
	}
	return ;
}

void	exec_ast(t_tree *tree, char **env)
{
	if (!tree)
		return ;

	if (tree->type == EXEC)
	{
		if (strcmp(tree->cmd[0], "cd") == 0)
		{
			return cd(tree->cmd);
		}
		int pid = fork1();
		if (pid == 0)
		{
			execve(tree->cmd[0], tree->cmd, env);
			write(2, "error: cannot execute ", sizeof("error: cannot execute"));
			putstr(2, tree->cmd[0]);
			write(2, "\n", 1);
			exit(1);
		}
		waitpid(pid, 0x0 ,0x0);
		return ;
	}
	if (tree->type == SEMICOLON)
	{
		exec_ast(tree->left, env);
		exec_ast(tree->right, env);
		return ;
	}
	if (tree->type == PIPE)
	{
		int pfd[2];
		pipe1(pfd);
		if (fork1() == 0)
		{
			close(1);
			dup2(pfd[1], 1);
			close(pfd[0]);
			close(pfd[0]);
			exec_ast(tree->left, env);
			exit(0);
		}
		if (fork1() == 0)
		{
			close(0);
			dup2(pfd[0], 0);
			close(pfd[0]);
			close(pfd[1]);
			exec_ast(tree->right, env);
			exit(0);
		}
		close(pfd[0]);
		close(pfd[1]);
		waitpid(-1, 0, 0);
		waitpid(-1, 0, 0);
		return ;
	}
	return ;
}

int	semico_check(char ***av)
{
	int flag;

	flag = 0x0;
	while (**av && strcmp(**av, ";") == 0)
	{
		flag = 1;
		**av = 0x0;
		(*av)++;
	}
	return flag;
}

t_tree	*parse_list(char ***av)
{
	t_tree	*ret;

	if (!**av)
		return (0x0);
	ret = tree_constructor(EXEC, *av, 0, 0);
	while (**av && !strcmp(**av, ";") && !strcmp(**av, "|"))
		(*av)++;
	return (ret);
}

t_tree *parse_pline(char ***av)
{
	t_tree	*ret;

	if (!**av)
		return (0x0);
	ret = parse_list(av);
	if (!ret)
		return (0x0);
	while (**av && strcmp(**av, "|") == 0)
	{
		**av = 0x0;
		(*av)++;
		ret = tree_constructor(PIPE, 0, ret, NULL);
		ret->right = parse_list(av);
		if (!ret->right)
			return (0x0);
	}
	return (ret);
}

t_tree	*parse_cmd(char ***av)
{
	t_tree *ret;

	if (!**av)
		return (0x0);
	ret = parse_pline(av);
	if (!ret)
		return (0x0);
	if (**av && semico_check(av))
	{
		ret = tree_constructor(SEMICOLON, 0, ret, parse_cmd(av));
	}
	return ret;
}

t_tree *tree_constructor(t_type type, char **cmd, t_tree *left, t_tree *right)
{
	t_tree *ret = malloc(sizeof(t_tree));

	if (!ret)
		write(2, "error : fatal\n", 14);
	ret->type = type;
	ret->cmd = cmd;
	ret->right = right;
	ret->left = left;
	return ret;
}

void tree_debug(t_tree *tree, int level)
{
	if (!tree)
		return ;
	for (int i = 0; i < level; i++)
		printf("\t\t");
	if (tree->type == PIPE)
		printf("PIPE\n");
	else if (tree->type == SEMICOLON)
		printf("SEMICO\n");
	else if (tree->type == EXEC)
	{
		printf("EXEC: ");
		for (int i = 0; tree->cmd[i]; i++)
			printf(" %s  ", tree->cmd[i]);
		printf("\n");
	}
	else
		printf("UNKNOWN CMD\n");
	tree_debug(tree->left, level + 1);
	tree_debug(tree->right, level + 1);
}
