#include "shell.h"

/**
 * abedInfoClear - initializes info_t struct
 * @info: struct address
 */
void abedInfoClear(info_t *info)
{
	info->arg = NULL;
	info->argv = NULL;
	info->path = NULL;
	info->argc = 0;
}

/**
 * abedInfoSet - initializes info_t struct
 * @info: struct address
 * @av: argument vector
 */
void abedInfoSet(info_t *info, char **av)
{
	int i = 0;

	info->fname = av[0];
	if (info->arg)
	{
		info->argv = abedTOWstrInTokenizer(info->arg, " \t");
		if (!info->argv)
		{

			info->argv = malloc(sizeof(char *) * 2);
			if (info->argv)
			{
				info->argv[0] = abedDUPstrInString1(info->arg);
				info->argv[1] = NULL;
			}
		}
		for (i = 0; info->argv && info->argv[i]; i++)
			;
		info->argc = i;

		abedAliasReplaceInVars(info);
		replace_abedVars(info);
	}
}

/**
 * abedInfoFree - frees info_t struct fields
 * @info: struct address
 * @all: true if freeing all fields
 */
void abedInfoFree(info_t *info, int all)
{
	ffree(info->argv);
	info->argv = NULL;
	info->path = NULL;
	if (all)
	{
		if (!info->cmd_buf)
			free(info->arg);
		if (info->env)
			abedListFree(&(info->env));
		if (info->abedAllHistory)
			abedListFree(&(info->abedAllHistory));
		if (info->alias)
			abedListFree(&(info->alias));
		ffree(info->abedEnviron);
			info->abedEnviron = NULL;
		abedFreeBInMemory((void **)info->cmd_buf);
		if (info->readfd > 2)
			close(info->readfd);
		abedCHARPutInString1(BUF_FLUSH);
	}
}
