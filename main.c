#include "shell.h"

/**
 * main - entry point
 * @ac: arg count
 * @av: arg vector
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	info_t info[] = { INFO_INIT };
	int fd = 2;

	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				abedAllEput(av[0]);
				abedAllEput(": 0: Can't open ");
				abedAllEput(av[1]);
				abedCharEput('\n');
				abedCharEput(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = fd;
	}
	abedpopulateEnvlist(info);
	read_abedAllHistory(info);
	hsh(info, av);
	return (EXIT_SUCCESS);
}
