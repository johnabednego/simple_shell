#include "shell.h"

/**
 * hsh - main shell loop
 * @info: the parameter & return info struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int abedin_ret = 0;

	while (r != -1 && abedin_ret != -2)
	{
		abedInfoClear(info);
		if (interactive(info))
			_puts("$ ");
		abedCharEput(BUF_FLUSH);
		r = abedInput_Get(info);
		if (r != -1)
		{
			abedInfoSet(info, av);
			abedin_ret = find_abedin(info);
			if (abedin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');
		abedInfoFree(info, 0);
	}
	write_abedAllHistory(info);
	abedInfoFree(info, 1);
	if (!interactive(info) && info->status)
		exit(info->status);
	if (abedin_ret == -2)
	{
		if (info->err_num == -1)
			exit(info->status);
		exit(info->err_num);
	}
	return (abedin_ret);
}

/**
 * find_abedin - finds a abedin command
 * @info: the parameter & return info struct
 *
 * Return: -1 if abedin not found,
 *			0 if abedin executed successfully,
 *			1 if abedin found but not successful,
 *			-2 if abedin signals exit()
 */
int find_abedin(info_t *info)
{
	int i, built_in_ret = -1;
	abedin_table abedintbl[] = {
		{"exit", abed_exit},
		{"env", abedEnv},
		{"help", abed_help},
		{"abedAllHistory", abed_abedAllHistory},
		{"setenv", abedsetEnv},
		{"unsetenv", abedunsetEnv},
		{"cd", abed_cd},
		{"alias", abed_alias},
		{NULL, NULL}
	};

	for (i = 0; abedintbl[i].type; i++)
		if (_strcmp(info->argv[0], abedintbl[i].type) == 0)
		{
			info->line_count++;
			built_in_ret = abedintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - finds a command in PATH
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!abed_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = abedPathLooker(info, abedabedEnvGet(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
		if ((interactive(info) || abedabedEnvGet(info, "PATH=")
			|| info->argv[0][0] == '/') && abedCMDIsActive(info, info->argv[0]))
			fork_cmd(info);
		else if (*(info->arg) != '\n')
		{
			info->status = 127;
			abedErrorPrint(info, "not found\n");
		}
	}
}

/**
 * fork_cmd - forks a an exec thread to run cmd
 * @info: the parameter & return info struct
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_abedEnviron(info)) == -1)
		{
			abedInfoFree(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
		if (WIFEXITED(info->status))
		{
			info->status = WabedAllExitsTATUS(info->status);
			if (info->status == 126)
				abedErrorPrint(info, "Permission denied\n");
		}
	}
}
