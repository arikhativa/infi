
#define _GNU_SOURCE

#include <stdio.h>			/* printf()	*/
#include <stdlib.h>			/* malloc()	*/
#include <unistd.h>			/* ssize_t	*/
#include <sys/syscall.h>	/* ssize_t	*/
#include <dlfcn.h>			/* dlopen(), dlclose, dlsym */
#include <signal.h>			/* sigaction() */
#include <pthread.h>		/* pthread()	*/

#include "write_print.h"	/* PrintArray()	*/
#include "printhey.h"	/* PrintHey()	*/


extern char etext;
extern char edata;
extern char end;

char th1_malloc_addres[100] = {0};
char th2_malloc_addres[100] = {0};

typedef void (*fp_t)(void);

int bss_var_g;
const int bss_cvar_g;
int var_g = 15;
const int cvar_g = 5;
pthread_key_t g_key;

static int SFunc(void)
{
}

int EFunc(int arg)
{
	int stack = 0;

	printf("-- stack EFunc local	%p\n", &stack);
	printf("-- stack EFunc arg	%p\n\n", &arg);
}

static void PP(int i)
{
	int j = 0;
	printf("-- SubFunc1() local	%p\n", &j);
	printf("-- SubFunc1() arg	%p\n", &i);
}

static void Handel1(int i)
{
	int j = 0;
	printf("-- Handel1 local	%p\n", &j);
	printf("-- Hndel1 arg		%p\n", &i);

}

void *Thread_func1(void *a)
{
	int j = 0;
	void *ptr = malloc(200);
	void *ptr2 = malloc(200);

	sprintf(th1_malloc_addres, "-- secound malloc()	%p\n-- first malloc()	%p\n", ptr2, ptr);

	printf("\n-------------------------------------- Stack Thread [1]\n");
	printf("-- Thred%ld() local	%p\n",(size_t)a, &j);
	printf("-- Thred%ld() arg		%p\n",(size_t)a, &a);
	PP(0);

	free (ptr);
	free (ptr2);
	sleep(1);

	return NULL;
}

void *Thread_func2(void *a)
{
	int j = 0;
	void *ptr = malloc(200);
	void *ptr2 = malloc(200);

	sleep(1);

	sprintf(th2_malloc_addres, "-- secound malloc()	%p\n-- first malloc()	%p\n", ptr2, ptr);

	printf("\n-------------------------------------- Stack Thread [2]\n");
	printf("-- Thred%ld() local	%p\n",(size_t)a, &j);
	printf("-- Thred%ld() arg		%p\n",(size_t)a, &a);
	free (ptr);
	free (ptr2);

	return NULL;
}

int main(int ac, char **av, char **env)
{
	static int static_var_g = 15;
	char **run = env;
	char *str_l = "hey";
	char *heap = 0;
	void *handle = 0;
	fp_t func = 0;
	int stack = 0;

	pthread_t thread1 = 0;
	pthread_t thread2 = 0;
	struct sigaction sig1 = {0};

	pthread_key_create(&g_key, NULL);

    handle = dlopen("/usr/lib/libhey.so", RTLD_LAZY);
    func = dlsym(handle, "PrintHey");
    if (!func)
    {
            return -1;
	}

	sig1.sa_handler = Handel1;
	sigaction(SIGUSR1, &sig1, NULL);

	while (*run)
		++run;

	heap = malloc(20);

	printf("-------------------------------------- Env Arguments\n");
	printf("-- env top		%p\n", run);
	printf("-- env start		%p\n\n", env);

	printf("-------------------------------------- Main Stack\n");
	printf("-- main local		%p\n", &stack);
	printf("-- main ac		%p\n", &ac);
	printf("-- main av		%p\n", &av);
	printf("-- main env		%p\n", &env);
	EFunc(88);

	printf("-------------------------------------- Signal Stack\n");
	raise(SIGUSR1);

	printf("\n-------------------------------------- Sheard Objects\n");
	printf("-- PArray() (load)	%p\n", PrintArray);
	printf("-- syscall()		%p\n", syscall);
	printf("-- write()		%p\n", write);
	printf("-- alarm() 		%p\n", alarm);
	printf("-- PrintHey() (run)	%p\n", func);

	pthread_create(&thread1, NULL, &Thread_func1, (void*)1);
	pthread_create(&thread2, NULL, &Thread_func2, (void*)2);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	printf("\n%s", th1_malloc_addres);
	printf("-------------------------------------- Thread[1] heap start\n");
	printf("%s", th2_malloc_addres);
	printf("-------------------------------------- Thread[2] heap start\n");

	printf("----------------------- %p	main heap end (sbrk(0))\n", sbrk(0));
	printf("-- Using malloc()	%p\n", heap);

	printf("----------------------- %p .bss\n", &end);
	printf("-- bss_var_g		%p\n", &bss_var_g);
	printf("-- bss_const_var_g	%p\n", &bss_cvar_g);

	printf("-------------------------------------- .tdata\n");
	printf("-- thread_ket_t		%p\n", &g_key);
	printf("----------------------- %p .edata\n", &edata);
	printf("-- static_var_g		%p\n", &static_var_g);
	printf("-- var_g		%p\n", &var_g);

	printf("-------------------------------------- Read Only\n");
	printf("-- String Literal	%p\n", str_l);
	printf("-- const_var_g		%p\n", &cvar_g);

	printf("----------------------- %p .etext\n", &etext);
	printf("-- Main Func 		%p\n", main);
	printf("-- External Func 	%p\n", EFunc);
	printf("-- Static Func 		%p\n\n", SFunc);

	free(heap);
    dlclose(handle);

	/*printf("\n-- Thread2 tdata.\n%s", g_thread2_text);*/

	return 0;
}
