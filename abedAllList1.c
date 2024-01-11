#include "shell.h"

/**
 * abedLenList - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t abedLenList(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * abedStringToListReverse - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **abedStringToListReverse(list_t *head)
{
	list_t *node = head;
	size_t i = abedLenList(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(abedLENstr(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = abedCPYstrInString1(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * abedList_Print - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t abedList_Print(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		abedAllPutInString1(abedNumberConvert(h->num, 10, 0));
		abedCHARPutInString1(':');
		abedCHARPutInString1(' ');
		abedAllPutInString1(h->str ? h->str : "(nil)");
		abedAllPutInString1("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * abedWithNodeStart - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *abedWithNodeStart(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = abedWithStarts(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * abedGetIndexAtNode - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t abedGetIndexAtNode(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}
