#include <stdio.h> /* printf()*/
#include <unistd.h> /* access() */
#include <stdlib.h> /* malloc() */
#include <assert.h> /* assert() */
#include <stddef.h> /* size_t */

#define NUM_OF_OPERATIONS 5
#define STR_LIMIT 100
#define TMP_FILE "tmp_file_dont_look"

typedef enum	status
{
	EXIT,
	SUCCESS,
	DEFAULT,
	NO_FILE,
	NO_ARGUMENT,
	OPEN_FAIL,
	GET_STRING_FAIL,
	MALLOC_FAIL,
	WRITING_FAIL,
	CLOSE_FAIL,
	GET_STR_FAIL
}		status_t;

typedef status_t (*func_ptr_t)(const char *, const char *);

typedef struct	operations
{
	char *str;
	int (*comper_func)(const char *, const char *);
	status_t (*action_func)(const char *, const char *);

}		operation_t;


static void ExplainProgram(void)
{
	printf("\n-- Welcome to \"Logger\"\n");
	printf("-- Your file will be created when you will enter a string\n");
	printf("-- Strings have limitaion of 100 characters\n\n");
	printf("-- You have %d commands at your service:\n", (NUM_OF_OPERATIONS - 1));
	printf("-- \"-exit\"   - will leave the program\n");
	printf("-- \"-count\"  - will print the number of lines in the file\n");
	printf("-- \"-remove\" - will delete the file\n");
	printf("-- \"<...\"    - any string that will begin with '<' will be typed in the top of the file\n\n");
}

static int CheckStatusForErrors(status_t status)
{
	switch (status)
	{
		case EXIT:
			return EXIT;

		case SUCCESS:
			return SUCCESS;

		case DEFAULT:
			return DEFAULT;

		case NO_FILE:
			printf("-- Error - missing a file\n");
			printf("-- When using \"-count\" or \"-remove\" a file must exist\n");
			printf("-- Try typing a random string or use \"<...\" to crate one\n\n");
			return NO_FILE;

		case NO_ARGUMENT:
			printf("\n-- Error - problem with arguments\n");
			printf("-- Logger expects one argument\n");
			printf("-- Usage: ./a.out \"file_name\"\n\n");
			break ;

		case OPEN_FAIL:
			printf("-- Error - fopen()\n-- Could not open file\n\n");
			break ;

		case GET_STRING_FAIL:
			printf("-- Error - fgets()\n-- Could not get string from stdin\n\n");
			break ;

		case MALLOC_FAIL:
			printf("-- Error - malloc()\n-- Could not allocate memory with malloc()\n\n");
			break ;

		case WRITING_FAIL:
			printf("-- Error - fputs()\n-- Could not write to file\n\n");
			break ;

		case CLOSE_FAIL:
			printf("-- Error - fclose()\n-- Could not close the file\n\n");
			break ;

		case GET_STR_FAIL:
			printf("-- Error - fgets()\n-- Could not get string\n\n");
			break ;

	}
	return -1;
}

static int StrCmp(const char *s1, const char *s2)
{
	assert(s1);
	assert(s2);

	if ('<' == *s1 && *s1 == *s2)
	{
		return 0;
	}
	while (*s1 && *s1 == *s2)
	{
		++s1;
		++s2;
	}

	return (*s1 - *s2);
}

static status_t ExitProgram(const char *str, const char *file_name)
{
	if (file_name){}
	if (str){}
	return EXIT;
}

static status_t WriteStringToEndOfFile(const char *str, const char *file_name)
{
	FILE *fd = 0;

	assert(str);
	assert(file_name);

	if (!(fd = fopen(file_name, "a")))
	{
		return OPEN_FAIL;
	}
	if (EOF == fputs(str, fd))
	{
		fclose(fd);
		return WRITING_FAIL;
	}
	if (EOF == fclose(fd))
	{
		return CLOSE_FAIL;
	}

	return SUCCESS;
}

static status_t CountNewLineInFile(const char *str, const char *file_name)
{
	FILE *fd = 0;
	char tmp[STR_LIMIT];
	char *stoper = 0;
	size_t count = -1;

	assert(file_name);
	if (str){}

	
	if (!(fd = fopen(file_name, "r")))
	{
		return NO_FILE;
	}
	do
	{
		++count;
		stoper = fgets(tmp, STR_LIMIT, fd);
	} while (NULL != stoper);
	if (EOF == fclose(fd))
	{
		return CLOSE_FAIL;
	}
	printf("%ld Lines\n", count);

	return SUCCESS;
}

static status_t RemoveFile(const char *str, const char *file_name)
{
	assert(file_name);

	if (str){}

	if (access(file_name, F_OK) != -1)
	{
		remove(file_name);
		return SUCCESS;
	}

	return NO_FILE;
}

static int OpenFilesAndCreateCopy(FILE **source_fd, FILE **copy_fd, const char *file_name)
{
	int ret = 0;

	if (access(file_name, F_OK) == -1)
	{
		if (!(*source_fd = fopen(file_name, "a+")))
		{
			ret = -1;
		}
		fclose(*source_fd);
	}
	if (!(*source_fd = fopen(file_name, "r+")))
	{
		ret = -1;
	}
	if (!(*copy_fd = fopen(TMP_FILE, "w+")))
	{
		fclose(*source_fd);
		ret = -1;
	}
	fclose(*copy_fd);
	if (!(*copy_fd = fopen(TMP_FILE, "r+")))
	{
		fclose(*source_fd);
		fclose(*copy_fd);
		ret = -1;
	}

	return ret;
}

static int WriteFromSrcFileToDestFile(FILE **dest_file, FILE **src_file)
{
	char ch = 0;

	while (EOF != (ch = fgetc(*src_file)))
	{
		if (EOF == fputc(ch, *dest_file))
		{
			return -1;
		}
	}

	return 1;
}

static status_t WriteStringToTopOfFile(const char *user_string, const char *file_name)
{
	FILE *source_fd = 0;
	FILE *copy_fd = 0;

	assert(user_string);
	assert(file_name);

	if (OpenFilesAndCreateCopy(&source_fd, &copy_fd, file_name))
	{
		return OPEN_FAIL;
	}
	if (!(WriteFromSrcFileToDestFile(&copy_fd, &source_fd)))
	{
		return WRITING_FAIL;
	}
	rewind(copy_fd);
	rewind(source_fd);
	if (EOF == fputs((user_string + 1), source_fd))
	{
		return WRITING_FAIL;
	}
	if (!(WriteFromSrcFileToDestFile(&source_fd, &copy_fd)))
	{
		return WRITING_FAIL;
	}
	if (EOF == fclose(copy_fd))
	{
		fclose(source_fd);
		return CLOSE_FAIL;
	}
	if (EOF == fclose(source_fd))
	{
		return CLOSE_FAIL;
	}
	remove(TMP_FILE);

	return SUCCESS;
}

static status_t InitializeOperationArray(operation_t *operation, char *user_string, status_t (*func_to_assign)(const char *, const char *))
{
	assert(operation);
	assert(user_string);
	assert(func_to_assign);

	operation->str = user_string;
	operation->comper_func = StrCmp;
	operation->action_func = *func_to_assign;

	return SUCCESS;
}

static status_t InitializeProgrem(operation_t *operation)
{
	int index = 0;
	char *user_command[NUM_OF_OPERATIONS] = {
		"-exit\n",
		"-count\n",
		"-remove\n",
		"<",
		"..."
	};
	func_ptr_t action_func[NUM_OF_OPERATIONS] = {
		ExitProgram,
		CountNewLineInFile,
		RemoveFile,
		WriteStringToTopOfFile,
		WriteStringToEndOfFile
	};

	while (NUM_OF_OPERATIONS > index)
	{
		InitializeOperationArray(&operation[index], user_command[index], action_func[index]);
		++index;
	}

	return SUCCESS;
}

static status_t CompareStringAndExecuteAction(operation_t *operation, const char *user_string, const char *file_name)
{
	int index = 0;
	status_t status = DEFAULT;

	while ((NUM_OF_OPERATIONS - 1) > index)
	{
		if (0 == operation[index].comper_func(user_string, operation[index].str))
		{
			status = (operation + index)->action_func(user_string, file_name);
			status = CheckStatusForErrors(status);
		}
		++index;
	}
	if (DEFAULT == status)
	{
		status = operation[index].action_func(user_string, file_name);
	}

	return status;
}

static status_t RunLog(operation_t *operation, const char *file_name)
{
	char *user_string = 0;
	status_t status = SUCCESS;

	if (!(user_string = (char*)malloc(sizeof(char) * STR_LIMIT)))
	{
		return MALLOC_FAIL;
	}
	while (SUCCESS == status || NO_FILE == status)
	{
		if (!(user_string = fgets(user_string, STR_LIMIT, stdin)))
		{
			return GET_STRING_FAIL;
		}
		status = CompareStringAndExecuteAction(operation, user_string, file_name);
	}
	free(user_string);
	user_string = NULL;

	return status;
}

int main(const int ac, const char **av)
{
	operation_t operation[NUM_OF_OPERATIONS];
	status_t status = 0;

	if (2 == ac)
	{
		ExplainProgram();
		InitializeProgrem(operation);
		status = RunLog(operation, av[1]);
	}
	else
	{
		status = NO_ARGUMENT;
	}

	return CheckStatusForErrors(status);
}
