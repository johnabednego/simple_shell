#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define abedBufRead_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

/* for abedNumberConvert() */
#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2

/* 1 if using system abedLineGet() */
#define USE_abedLineGet 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_abedAllHistory"
#define HIST_MAX	4096

extern char **abedEnviron;


/**
 * struct abedAllListtr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct abedAllListtr
{
	int num;
	char *str;
	struct abedAllListtr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from abedLineGet containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of abedEnviron
 *@abedEnviron: custom modified copy of abedEnviron from LL env
 *@abedAllHistory: the abedAllHistory node
 *@alias: the alias node
 *@env_changed: on if abedEnviron was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the abedAllHistory line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *abedAllHistory;
	list_t *alias;
	char **abedEnviron;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for abedMemory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct abedin - contains a abedin string and related function
 *@type: the abedin command flag
 *@func: the function
 */
typedef struct abedin
{
	char *type;
	int (*func)(info_t *);
} abedin_table;


/* toem_shloop.c */
int hsh(info_t *, char **);
int find_abedin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);

/* toem_abedParser.c */
int abedCMDIsActive(info_t *, char *);
char *abedCHADuP(char *, int, int);
char *abedPathLooker(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* toem_abedErrors.c */
void abedAllEput(char *);
int abedCharEput(char);
int abedFdput(char c, int fd);
int abedSFDput(char *str, int fd);

/* toem_string.c */
int _strlen(char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
char *_strcat(char *, char *);

/* toem_string1.c */
char *_strcpy(char *, char *);
char *_strdup(const char *);
void _puts(char *);
int _putchar(char);

/* toem_abedAllExits.c */
char *abedCPYs(char *, char *, int);
char *abedCATs(char *, char *, int);
char *abedCHRs(char *, char);

/* toem_tokenizer.c */
char **strtow(char *, char *);
char **strtow2(char *, char);

/* toem_abedLocReal.c */
char *_memset(char *, char, unsigned int);
void ffree(char **);
void *_abedLocReal(void *, unsigned int, unsigned int);

/* toem_abedMemory.c */
int abedFreeBInMemory(void **);

/* toemabed_toi.c */
int interactive(info_t *);
int abed_delim(char, char *);
int abed_alpha(int);
int abed_toi(char *);

/* toem_abedErrors1.c */
int abedErratoi(char *);
void abedErrorPrint(info_t *, char *);
int abedDPrint(int, int);
char *abedNumberConvert(long int, int, int);
void abedCommentsRemove(char *);

/* toem_abedin.c */
int abed_exit(info_t *);
int abed_cd(info_t *);
int abed_help(info_t *);

/* toem_abedin1.c */
int abed_abedAllHistory(info_t *);
int abed_alias(info_t *);

/*toem_abedLineGet.c */
ssize_t abedInput_Get(info_t *);
int _abedLineGet(info_t *, char **, size_t *);
void abedHandlerIntSign(int);

/* toem_abedInfoGet.c */
void abedInfoClear(info_t *);
void abedInfoSet(info_t *, char **);
void abedInfoFree(info_t *, int);

/* toem_abedEnviron.c */
char *abedabedEnvGet(info_t *, const char *);
int abedEnv(info_t *);
int abedsetEnv(info_t *);
int abedunsetEnv(info_t *);
int abedpopulateEnvlist(info_t *);

/* toemabedabedEnvGet.c */
char **get_abedEnviron(info_t *);
int abedEnvUnset(info_t *, char *);
int abedEnvSet(info_t *, char *, char *);

/* toem_abedAllHistory.c */
char *get_abedAllHistory_file(info_t *info);
int write_abedAllHistory(info_t *info);
int read_abedAllHistory(info_t *info);
int build_abedAllHistory_list(info_t *info, char *buf, int linecount);
int renumber_abedAllHistory(info_t *info);

/* toem_abedAllList.c */
list_t *abedNodeAdd(list_t **, const char *, int);
list_t *abedNodeAdd_end(list_t **, const char *, int);
size_t abedListSTRPrint(const list_t *);
int abedIndexNodeDelete(list_t **, unsigned int);
void abedListFree(list_t **);

/* toem_abedAllList1.c */
size_t abedLenList(const list_t *);
char **abedStringToListReverse(list_t *);
size_t abedList_Print(const list_t *);
list_t *abedWithNodeStart(list_t *, char *, char);
ssize_t abedGetIndexAtNode(list_t *, list_t *);

/* toem_vars.c */
int is_chain(info_t *, char *, size_t *);
void check_chain(info_t *, char *, size_t *, size_t, size_t);
int replace_alias(info_t *);
int replace_vars(info_t *);
int replace_string(char **, char *);

#endif
