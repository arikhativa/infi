#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "uid.h"


static int count_test = 1;
static int t_index = 1;
static int pass = 0;

static int boly = 1;
static int num = 1;
static char *name;


typedef enum status
{
	FAIL = -1,
	SUCCESS = 0
} status_t;

static void SetTest(int n, char *str)
{
	num = n;
	name = str;
	boly = 0;
	t_index = 1;
}

static void Pass()
{
	++count_test;
	++pass;
	++t_index;
}

static void PrintFail(char *str)
{
	if (!boly)
	{
		printf(CYAN"\n-- (%d) Test %s:\n\n"RESET, num, name);
		++boly;
	}
	printf("-- Test"GRAY" %d"RESET RED" - Fail :( "RESET, t_index);
	printf("%s\n", str);
	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	--count_test;
	if (pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,pass, count_test);
	else
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,pass, count_test);
}
/*
static void MemoryComper(void *mem1, void *mem2, size_t n)
{
	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
		PrintFail(" -- Diffrence in memory.");
}
*/
static void ReturnComper(size_t num1, size_t num2)
{
	if (num1 == num2)
		Pass();
	else
		PrintFail(" -- Diffrence in return value.");
}

void PrintUid(unique_id_t uid)
{
	printf("-- PID  == %u\n", uid.pid);
	printf("-- NUM  == %lu\n", uid.counter);
	printf("-- TIME == %s\n", ctime(&uid.time));
}

static void TestUidCreate()
{
	unique_id_t uid;

	SetTest(1, "(white) UID Create");

	uid = UidCreate();
	ReturnComper(0, uid.counter);
	uid = UidCreate();
	ReturnComper(1, uid.counter);
	uid = UidCreate();
	uid = UidCreate();
	ReturnComper(3, uid.counter);
}

static void TestIsSame()
{
	unique_id_t uid1;
	unique_id_t uid2;

	SetTest(2, "UID IsSame");

	uid1 = UidCreate();
	uid2 = uid1;
	ReturnComper(1, UidIsSame(uid1, uid2));
}

static void TestBad()
{
	unique_id_t uid1;

	SetTest(3, "UID Bad");

	uid1 = UidCreate();
	ReturnComper(0, UidIsBad(uid1));
	ReturnComper(1, UidIsBad(bad_uid));
}

int main(int ac, char **av)
{
	int c = 0;


	if (2 <= ac)
	{
		c = atoi(av[1]);
	}
	switch (c)
	{
		case 1:
			TestUidCreate();
			break ;
		case 2:
			TestIsSame();
			break ;
		case 3:
			TestBad();
			break ;

		default:
			TestUidCreate();
			TestIsSame();
			TestBad();
	}
	PrintSummery();

	return 0;
}
