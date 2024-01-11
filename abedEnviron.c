#include "shell.h"

/**
 * abedEnv - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int abedEnv(info_t *info)
{
	abedListSTRPrint(info->env);
	return (0);
}

/**
 * abedabedEnvGet - gets the value of an environ variable
 * @info: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *abedabedEnvGet(info_t *info, const char *name)
{
	list_t *node = info->env;
	char *p;

	while (node)
	{
		p = abedWithStarts(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * abedsetEnv - Initialize a new environment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int abedsetEnv(info_t *info)
{
	if (info->argc != 3)
	{
		abedAllEput("Incorrect number of arguements\n");
		return (1);
	}
	if (abedEnvSet(info, info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * abedunsetEnv - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int abedunsetEnv(info_t *info)
{
	int i;

	if (info->argc == 1)
	{
		abedAllEput("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= info->argc; i++)
		abedEnvUnset(info, info->argv[i]);

	return (0);
}

/**
 * abedpopulateEnvlist - populates env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int abedpopulateEnvlist(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		abedNodeAdd_end(&node, environ[i], 0);
	info->env = node;
	return (0);
}
