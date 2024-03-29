#include "shell.h"

/**
 * get_abedEnviron - returns the string array copy of our abedEnviron
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **get_abedEnviron(info_t *info)
{
	if (!info->abedEnviron || info->env_changed)
	{
		info->abedEnviron = abedStringToListReverse(info->env);
		info->env_changed = 0;
	}

	return (info->abedEnviron);
}

/**
 * abedEnvUnset - Remove an abedEnvironment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int abedEnvUnset(info_t *info, char *var)
{
	list_t *node = info->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = abedWithStarts(node->str, var);
		if (p && *p == '=')
		{
			info->env_changed = abedIndexNodeDelete(&(info->env), i);
			i = 0;
			node = info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (info->env_changed);
}

/**
 * abedEnvSet - Initialize a new abedEnvironment variable,
 *             or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int abedEnvSet(info_t *info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buf = malloc(abedLENstr(var) + abedLENstr(value) + 2);
	if (!buf)
		return (1);
	abedCPYstrInString1(buf, var);
	abedCATstrInString(buf, "=");
	abedCATstrInString(buf, value);
	node = info->env;
	while (node)
	{
		p = abedWithStarts(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buf;
			info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	abedNodeAdd_end(&(info->env), buf, 0);
	free(buf);
	info->env_changed = 1;
	return (0);
}
