#include "shell.h"

/**
 * abedErratoi - converts a string to an integer
 * @s: the string to be converted
 * Return: 0 if no numbers in string, converted number otherwise
 *       -1 on error
 */
int abedErratoi(char *s)
{
	int i = 0;
	unsigned long int result = 0;

	if (*s == '+')
		s++;  /* TODO: why does this make main return 255? */
	for (i = 0;  s[i] != '\0'; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			result *= 10;
			result += (s[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * abedErrorPrint - prints an error message
 * @info: the parameter & return info struct
 * @estr: string containing specified error type
 * Return: 0 if no numbers in string, converted number otherwise
 *        -1 on error
 */
void abedErrorPrint(info_t *info, char *estr)
{
	abedAllEput(info->fname);
	abedAllEput(": ");
	abedDPrint(info->line_count, STDERR_FILENO);
	abedAllEput(": ");
	abedAllEput(info->argv[0]);
	abedAllEput(": ");
	abedAllEput(estr);
}

/**
 * abedDPrint - function prints a decimal (integer) number (base 10)
 * @input: the input
 * @fd: the filedescriptor to write to
 *
 * Return: number of characters printed
 */
int abedDPrint(int input, int fd)
{
	int (*_abedCHARPutInString1)(char) = abedCHARPutInString1;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		_abedCHARPutInString1 = abedCharEput;
	if (input < 0)
	{
		_abs_ = -input;
		_abedCHARPutInString1('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			_abedCHARPutInString1('0' + current / i);
			count++;
		}
		current %= i;
	}
	_abedCHARPutInString1('0' + current);
	count++;

	return (count);
}

/**
 * abedNumberConvert - converter function, a clone of itoa
 * @num: number
 * @base: base
 * @flags: argument flags
 *
 * Return: string
 */
char *abedNumberConvert(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * abedCommentsRemove - function replaces first instance of '#' with '\0'
 * @buf: address of the string to modify
 *
 * Return: Always 0;
 */
void abedCommentsRemove(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
