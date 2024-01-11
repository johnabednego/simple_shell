#include "shell.h"

/**
 * abed_exit - abedAllExits the shell
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: abedAllExits with a given exit status
 *         (0) if info.argv[0] != "exit"
 */
int abed_exit(info_t *info)
{
	int exitcheck;

	if (info->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = abedErratoi(info->argv[1]);
		if (exitcheck == -1)
		{
			info->status = 2;
			abedErrorPrint(info, "Illegal number: ");
			abedAllEput(info->argv[1]);
			abedCharEput('\n');
			return (1);
		}
		info->err_num = abedErratoi(info->argv[1]);
		return (-2);
	}
	info->err_num = -1;
	return (-2);
}

/**
 * abed_cd - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int abed_cd(info_t *info)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_puts("TODO: >>getcwd failure emsg here<<\n");
	if (!info->argv[1])
	{
		dir = abedabedEnvGet(info, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = abedabedEnvGet(info, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (_strcmp(info->argv[1], "-") == 0)
	{
		if (!abedabedEnvGet(info, "OLDPWD="))
		{
			_puts(s);
			_putchar('\n');
			return (1);
		}
		_puts(abedabedEnvGet(info, "OLDPWD=")), _putchar('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = abedabedEnvGet(info, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(info->argv[1]);
	if (chdir_ret == -1)
	{
		abedErrorPrint(info, "can't cd to ");
		abedAllEput(info->argv[1]), abedCharEput('\n');
	}
	else
	{
		abedEnvSet(info, "OLDPWD", abedabedEnvGet(info, "PWD="));
		abedEnvSet(info, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * abed_help - changes the current directory of the process
 * @info: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int abed_help(info_t *info)
{
	char **arg_array;

	arg_array = info->argv;
	_puts("help call works. Function not yet implemented \n");
	if (0)
		_puts(*arg_array); /* temp att_unused workaround */
	return (0);
}
