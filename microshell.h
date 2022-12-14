#ifndef MICROSHELL_H
# define MICROSHELL_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>


typedef enum	e_type
{
	PIPE = 1,
	SEMICOLON,
	EXEC
}	t_type;

typedef struct	s_tree
{
	t_type			type;
	char			**cmd;
	struct s_tree	*left;
    struct s_tree	*right;
}	t_tree;

void tree_debug(t_tree *tree, int level);
int semico_check(char ***av);
int fork1();
int pipe1(int fd[2]);
t_tree	*parse_cmd(char ***av);
void	exec_ast(t_tree *tree, char **env);
void	putstr(int fd, char *str);
t_tree	*tree_constructor(t_type type, char **cmd, t_tree *left, t_tree *right);

#endif
