#include "shell.h"

/**
 * abedCPYstrInString1 - copies a string
 * @dest: the destination
 * @src: the source
 *
 * Return: pointer to destination
 */
char *abedCPYstrInString1(char *dest, char *src)
{
	int i = 0;

	if (dest == src || src == 0)
		return (dest);
	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = 0;
	return (dest);
}

/**
 * abedDUPstrInString1 - duplicates a string
 * @str: the string to duplicate
 *
 * Return: pointer to the duplicated string
 */
char *abedDUPstrInString1(const char *str)
{
	int length = 0;
	char *ret;

	if (str == NULL)
		return (NULL);
	while (*str++)
		length++;
	ret = malloc(sizeof(char) * (length + 1));
	if (!ret)
		return (NULL);
	for (length++; length--;)
		ret[length] = *--str;
	return (ret);
}

/**
 *abedAllPutInString1 - prints an input string
 *@str: the string to be printed
 *
 * Return: Nothing
 */
void abedAllPutInString1(char *str)
{
	int i = 0;

	if (!str)
		return;
	while (str[i] != '\0')
	{
		abedCHARPutInString1(str[i]);
		i++;
	}
}

/**
 * abedCHARPutInString1 - writes the character c to stdout
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int abedCHARPutInString1(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(1, buf, i);
		i = 0;
	}
	if (c != BUF_FLUSH)
		buf[i++] = c;
	return (1);
}
