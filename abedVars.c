#include "shell.h"

/**
 * abedChainIsInVars - test if current char in buffer is a chain delimeter
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int abedChainIsInVars(info_t *info, char *buf, size_t *p)
{
	size_t j = *p;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		info->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* found end of this command */
	{
		buf[j] = 0; /* replace semicolon with null */
		info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * abedChainCheckInVars - checks we should continue chaining based on last status
 * @info: the parameter struct
 * @buf: the char buffer
 * @p: address of current position in buf
 * @i: starting position in buf
 * @len: length of buf
 *
 * Return: Void
 */
void abedChainCheckInVars(info_t *info, char *buf, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (info->cmd_buf_type == CMD_AND)
	{
		if (info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (info->cmd_buf_type == CMD_OR)
	{
		if (!info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * abedAliasReplaceInVars - replaces an aliases in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int abedAliasReplaceInVars(info_t *info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = abedWithNodeStart(info->alias, info->argv[0], '=');
		if (!node)
			return (0);
		free(info->argv[0]);
		p = abedCHRs(node->str, '=');
		if (!p)
			return (0);
		p = abedDUPstrInString1(p + 1);
		if (!p)
			return (0);
		info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_abedVars - replaces abedVars in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_abedVars(info_t *info)
{
	int i = 0;
	list_t *node;

	for (i = 0; info->argv[i]; i++)
	{
		if (info->argv[i][0] != '$' || !info->argv[i][1])
			continue;

		if (!abedCMPstr(info->argv[i], "$?"))
		{
			abedStringReplaceInVars(&(info->argv[i]),
				abedDUPstrInString1(abedNumberConvert(info->status, 10, 0)));
			continue;
		}
		if (!abedCMPstr(info->argv[i], "$$"))
		{
			abedStringReplaceInVars(&(info->argv[i]),
				abedDUPstrInString1(abedNumberConvert(getpid(), 10, 0)));
			continue;
		}
		node = abedWithNodeStart(info->env, &info->argv[i][1], '=');
		if (node)
		{
			abedStringReplaceInVars(&(info->argv[i]),
				abedDUPstrInString1(abedCHRs(node->str, '=') + 1));
			continue;
		}
		abedStringReplaceInVars(&info->argv[i], abedDUPstrInString1(""));

	}
	return (0);
}

/**
 * abedStringReplaceInVars - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int abedStringReplaceInVars(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
