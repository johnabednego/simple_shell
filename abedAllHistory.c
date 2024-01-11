#include "shell.h"

/**
 * get_abedAllHistory_file - gets the abedAllHistory file
 * @info: parameter struct
 *
 * Return: allocated string containg abedAllHistory file
 */

char *get_abedAllHistory_file(info_t *info)
{
	char *buf, *dir;

	dir = abedabedEnvGet(info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (abedLENstr(dir) + abedLENstr(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	abedCPYstrInString1(buf, dir);
	abedCATstrInString(buf, "/");
	abedCATstrInString(buf, HIST_FILE);
	return (buf);
}

/**
 * write_abedAllHistory - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int write_abedAllHistory(info_t *info)
{
	ssize_t fd;
	char *filename = get_abedAllHistory_file(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->abedAllHistory; node; node = node->next)
	{
		abedSFDput(node->str, fd);
		abedFdput('\n', fd);
	}
	abedFdput(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_abedAllHistory - reads abedAllHistory from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int read_abedAllHistory(info_t *info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_abedAllHistory_file(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_abedAllHistory_list(info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_abedAllHistory_list(info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (info->histcount-- >= HIST_MAX)
		abedIndexNodeDelete(&(info->abedAllHistory), 0);
	renumber_abedAllHistory(info);
	return (info->histcount);
}

/**
 * build_abedAllHistory_list - adds entry to a abedAllHistory linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the abedAllHistory linecount, histcount
 *
 * Return: Always 0
 */
int build_abedAllHistory_list(info_t *info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (info->abedAllHistory)
		node = info->abedAllHistory;
	abedNodeAdd_end(&node, buf, linecount);

	if (!info->abedAllHistory)
		info->abedAllHistory = node;
	return (0);
}

/**
 * renumber_abedAllHistory - renumbers the abedAllHistory linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_abedAllHistory(info_t *info)
{
	list_t *node = info->abedAllHistory;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}
