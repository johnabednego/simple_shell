#include "shell.h"

/**
 * abedBufInput - buffers chained commands
 * @info: parameter struct
 * @buf: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t abedBufInput(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*abedFreeBInMemory((void **)info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, abedHandlerIntSign);
#if USE_abedLineGet
		r = abedLineGet(buf, &len_p, stdin);
#else
		r = _abedLineGet(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			info->linecount_flag = 1;
			abedCommentsRemove(*buf);
			build_abedAllHistory_list(info, *buf, info->histcount++);
			/* if (abedCHRs(*buf, ';')) is this a command chain? */
			{
				*len = r;
				info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * abedInput_Get - gets a line minus the newline
 * @info: parameter struct
 *
 * Return: bytes read
 */
ssize_t abedInput_Get(info_t *info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->arg), *p;

	abedCHARPutInString1(BUF_FLUSH);
	r = abedBufInput(info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buf position */
		p = buf + i; /* get pointer for return */

		abedChainCheckInVars(info, buf, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (abedChainIsInVars(info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (abedLENstr(p)); /* return length of current command */
	}

	*buf_p = buf; /* else not a chain, pass back buffer from _abedLineGet() */
	return (r); /* return length of buffer from _abedLineGet() */
}

/**
 * abedBufRead - reads a buffer
 * @info: parameter struct
 * @buf: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t abedBufRead(info_t *info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(info->readfd, buf, abedBufRead_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _abedLineGet - gets the next line of input from STDIN
 * @info: parameter struct
 * @ptr: address of pointer to buffer, pabedLocRealated or NULL
 * @length: size of pabedLocRealated ptr buffer if not NULL
 *
 * Return: s
 */
int _abedLineGet(info_t *info, char **ptr, size_t *length)
{
	static char buf[abedBufRead_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = abedBufRead(info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = abedCHRs(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _abedLocReal(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		abedCATs(new_p, buf + i, k - i);
	else
		abedCPYs(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * abedHandlerIntSign - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void abedHandlerIntSign(__attribute__((unused))int sig_num)
{
	abedAllPutInString1("\n");
	abedAllPutInString1("$ ");
	abedCHARPutInString1(BUF_FLUSH);
}
