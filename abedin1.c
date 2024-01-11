#include "shell.h"

/**
 * abed_abedAllHistory - displays the abedAllHistory list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int abed_abedAllHistory(info_t *info)
{
	abedList_Print(info->abedAllHistory);
	return (0);
}

/**
 * abedalias - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int abedalias(info_t *info, char *str)
{
	char *p, c;
	int ret;

	p = abedCHRs(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = abedIndexNodeDelete(&(info->alias),
		abedGetIndexAtNode(info->alias, abedWithNodeStart(info->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * abedalias_set - sets alias to string
 * @info: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int abedalias_set(info_t *info, char *str)
{
	char *p;

	p = abedCHRs(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (abedalias(info, str));

	abedalias(info, str);
	return (abedNodeAdd_end(&(info->alias), str, 0) == NULL);
}

/**
 * abedalias_print - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int abedalias_print(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = abedCHRs(node->str, '=');
		for (a = node->str; a <= p; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(p + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * abed_alias - mimics the alias abedin (man alias)
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int abed_alias(info_t *info)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (info->argc == 1)
	{
		node = info->alias;
		while (node)
		{
			abedalias_print(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; info->argv[i]; i++)
	{
		p = abedCHRs(info->argv[i], '=');
		if (p)
			abedalias_set(info, info->argv[i]);
		else
			abedalias_print(abedWithNodeStart(info->alias, info->argv[i], '='));
	}

	return (0);
}
