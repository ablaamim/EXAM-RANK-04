/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ablaamim <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/14 20:09:21 by ablaamim          #+#    #+#             */
/*   Updated: 2023/01/01 22:42:10 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell.h"

int main(int ac, char **av, char **env)
{
	(void)	env;

	if (ac != 2)
	{
		write(2, "error: bad arguments\n", sizeof("error: bad arguments"));
		return (EXIT_FAILURE);
	}
	else
	{
		av++;
		t_tree *tree = parse_cmd(&av);
		tree_debug(tree, 0);
		exec_ast(tree, env);
	}
		return (EXIT_SUCCESS);
}
