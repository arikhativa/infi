#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stddef.h>
#include <ctype.h>

#include "color.h"
#include "dhcp.h"

/*
 * 	global
 */

static char g_summery[400][400] = {0};

static int count_test = 1;
static int t_index = 1;
static int g_pass = 0;
static int g_fail = 0;

/*	var per test	*/

static char test_name[40];
static int add_test_name = 1;


typedef void (*convert_t)(char *dest, unsigned char src[4]);

typedef struct trie_node trie_node_t;

typedef enum address_status
{
	FREE = 0,
	OCCUPIED = 3,
	RESERVED = 4 /* '4' is random. but the value should not be close to FREE	*/
} address_stt;

typedef enum child
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN	/* The trie tree is based on binary input.  */
} child_t;

struct trie_node
{
	trie_node_t *child[NUM_OF_CHILDREN];
	address_stt status;
};

struct dhcp
{
	trie_node_t *root;
	const unsigned int network_ip;
	const unsigned int subnet_mask;
};

static void SetTest(int n, char *name)
{
	sprintf(test_name,"\n\n-- "CYAN"(%d) Test %s:\n\n"RESET, n, name);

	add_test_name = 1;

	t_index = 1;
}

static void Pass()
{
	++count_test;
	++g_pass;
	++t_index;
}

static void PrintFail(char *dis, int line, char *value, char *bad_value)
{
	if (add_test_name)
	{
		add_test_name = 0;
		sprintf(g_summery[g_fail],"%s", test_name);
		++g_fail;
	}

	sprintf(g_summery[g_fail],"-- "YELLOW"Discription:\n"RESET"-- %s\n\n",dis);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"In Line "BGREEN "%d "RESET, line);
	++g_fail;
	sprintf(g_summery[g_fail],BBLUE"Value should be "RESET BGREEN"%s\n"RESET, value);
	++g_fail;
	sprintf(g_summery[g_fail],"-- "BBLUE"It is "RESET RED"%s\n\n"RESET, bad_value);
	++g_fail;

	++count_test;
	++t_index;
}

static void PrintSummery(void)
{
	int i = 0;

	--count_test;
	if (g_pass == count_test)
		printf(GREEN"\n-- Pass :) (%d/%d)\n\n"RESET,g_pass, count_test);
	else
	{
		printf(YELLOW"\n-- Not good enough.. (%d/%d)\n\n"RESET,g_pass, count_test);
		for (i = 0; i < g_fail; ++i)
		{
			printf("%s", g_summery[i]);
		}
	}
}

static void ReturnComper(ssize_t num1, ssize_t num2, char *dis, int line)
{
	char value[20] = {0};
	char bad_value[20] = {0};

	if (num1 == num2)
		Pass();
	else
	{
		sprintf(value, "%ld", num1);
		sprintf(bad_value, "%ld", num2);
		PrintFail(dis, line, value, bad_value);
	}
}

static void MemoryComper(void *mem1, void *mem2, size_t n, convert_t conv ,char *dis, int line)
{
	char value[200] = {0};
	char bad_value[200] = {0};

	if (0 == memcmp(mem1, mem2, n))
		Pass();
	else
	{
		conv(value, mem1);
		conv(bad_value, mem2);
		PrintFail(dis, line, value, bad_value);
	}
}


void PrintTree(trie_node_t *root, int space, int dir)
{
	int i = 5;


	if (root == NULL)
		return ;
	space += 5;

	PrintTree(root->child[RIGHT] ,space, RIGHT);

	printf("\n");
	for (; i < space; ++i)
		printf(" ");


	printf("%d", dir);
	printf("(%d)\n", root->status);


	PrintTree(root->child[LEFT], space, LEFT);
}

int ForBits(unsigned int index)
{
	switch(index)
	{
		case 3:
		case 11:
		case 19:
		case 27:
		return 1;
	}
	return 0;
}

int Byte(unsigned int index)
{
	switch(index)
	{
		case 7:
		case 15:
		case 23:
		return 1;
	}
	return 0;
}

void PrintBit(unsigned int number, unsigned int digits)
{
	int i = 0;
	digits = 2147483648;

	printf("-- ");

	while (digits)
	{
		(number & digits) ? printf("1") : printf("0");
		if (ForBits(i))
		{
			printf("-");
		}
		if (Byte(i))
		{
			printf(" ");
		}
		digits >>= 1;
		++i;
	}
	printf("\n");
}

void PrintIP(const unsigned char *ip)
{
	size_t i = 0;

	printf("-- ip ");

	for (; i < 4; ++i)
	{
		printf("%d", ip[i]);
		if (i + 1 < 4)
		{
			printf(".");
		}
	}
	printf("\n");
}

void Convert(char *dest, unsigned char src[4])
{
	int i = 0;

	while (i < 4)
	{
		sprintf(dest, "%u", src[i]);
		do
		{
			src[i] /= 10;
			++dest;
		} while (src[i]);
		++i;
		if (i < 4)
		{
			*dest = '.';
		}
		++dest;
	}

	*dest = '\0';
}

void CreateIP(unsigned char ip[4], char *str)
{
	unsigned char tmp = 0;
	int i = 0;

	if (!str)
	{
		return ;
	}

	while (*str)
	{
		tmp = 0;

		while(*str && *str != '.')
		{
			tmp = (tmp * 10) + (*str - '0');
			++str;
		}

		if (i < 4)
		{
			ip[i] = tmp;
			++i;
		}
		if (*str == '.')
		{
			++str;
		}
	}
}

void TestCreateAndDestroy()
{
	dhcp_t *dhcp = NULL;
	unsigned char net_ip[4];
	char mask = 24;

	SetTest(1, "Create && Destroy - run with vlg");

	CreateIP(net_ip, "100.010.005.005");

	dhcp = DHCPCreate(net_ip, mask);

	DHCPDestroy(dhcp);
}

static void MiniAlloc(char *net_ip, char **ret_ip, int ret,  char *dis)
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_ret_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	int real_ret = 0;
	int i = 0;

	CreateIP(mini_net_ip, net_ip);

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		CreateIP(mini_ret_ip, ret_ip[i]);

		real_ret = DHCPAllocIP(dhcp, real_ret_ip, 0);

/*		PrintTree(dhcp->root, 0,3);*/
		ReturnComper(ret, real_ret, dis, __LINE__);
		MemoryComper(mini_ret_ip, real_ret_ip, sizeof(int), Convert, dis, __LINE__ + i);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
}

static void MiniAlloc1(char *net_ip, char *req_ip, char **ret_ip, int ret,  char *dis)
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_req_ip[4] = {0};
	unsigned char mini_ret_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	int real_ret = 0;
	int i = 0;

	CreateIP(mini_net_ip, net_ip);
	CreateIP(mini_req_ip, req_ip);

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		CreateIP(mini_ret_ip, ret_ip[i]);

		real_ret = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

/*		PrintTree(dhcp->root, 0,3);*/
		ReturnComper(ret, real_ret, dis, __LINE__);
		MemoryComper(mini_ret_ip, real_ret_ip, sizeof(int), Convert, dis, __LINE__ + i);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
}

static void MiniAlloc2(char *net_ip, char **ret_ip, int ret,  char *dis)
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_req_ip[4] = {0};
	unsigned char mini_ret_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	int real_ret = 0;
	int i = 0;

	CreateIP(mini_net_ip, net_ip);

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		CreateIP(mini_ret_ip, ret_ip[i]);

		CreateIP(mini_req_ip, "255.000.000.005");
		real_ret = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

		CreateIP(mini_req_ip, "255.000.000.006");
		real_ret = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

		CreateIP(mini_req_ip, "255.000.000.007");
		real_ret = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

		CreateIP(mini_req_ip, "255.000.000.005");
		real_ret = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

/*		PrintTree(dhcp->root, 0,3);*/



		ReturnComper(ret, real_ret, dis, __LINE__);
		MemoryComper(mini_ret_ip, real_ret_ip, sizeof(int), Convert, dis, __LINE__ + i);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
}

void TestAlloc()
{
	char net_ip1[] = "015.007.005.045";
	char *ret_ip1[] = {"015.007.005.001", "015.007.000.001", "015.000.000.001"};
	int ret1 = 0	;
	char dis1[] = "requesting NULL ip. cheking the return value. and the return ip addres. for all classes";

	char net_ip2[] = "255.255.255.045";
	char req_ip2[] = "255.200.100.005";
	char *ret_ip2[] = {"255.255.255.005", "255.255.100.005", "255.200.100.005"};
	int ret2 = 0;
	char dis2[] = "requesting valid ip. cheking the return value. and the return ip addres. for all classes";

	char net_ip3[] = "005.255.255.045";
	char req_ip3[] = "005.255.255.254";
	char *ret_ip3[] = {"000.000.000.000", "000.000.000.000", "000.000.000.000"};
	int ret3 = 1;
	char dis3[] = "requesting RESERVED ip. cheking the return value. and the return ip addres. for all classes";

	char net_ip4[] = "255.255.255.045";
	char req_ip4[] = "005.006.200.001";
	char *ret_ip4[] = {"255.255.255.001", "255.255.200.001", "255.006.200.001"};
	int ret4 = 0;
	char dis4[] = "requesting valid ip but with bad net_work_ip. cheking the return value. and the return ip addres. for all classes";

	char net_ip5[] = "255.255.255.045";
	char *ret_ip5[] = {"255.255.255.008", "255.255.000.008", "255.000.000.008"};
	int ret5 = 2;
	char dis5[] = "requesting OCCUPIED ip. expecting the next vaild. cheking the return value. and the return ip addres. for all classes";

	SetTest(2, "Alloc IP");

	MiniAlloc(net_ip1, ret_ip1, ret1, dis1);
	MiniAlloc1(net_ip2, req_ip2, ret_ip2, ret2, dis2);
	MiniAlloc1(net_ip3, req_ip3, ret_ip3, ret3, dis3);
	MiniAlloc1(net_ip4, req_ip4, ret_ip4, ret4, dis4);
	MiniAlloc2(net_ip5, ret_ip5, ret5, dis5);
}

static void MiniFree(char *net_ip, char *req_ip, char **ret_ip, int *ret,  char *dis)
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_req_ip[4] = {0};
	unsigned char mini_ret_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	int real_ret1 = 0;
	int real_ret2 = 0;
	int i = 0;

	CreateIP(mini_net_ip, net_ip);
	CreateIP(mini_req_ip, req_ip);

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		CreateIP(mini_ret_ip, ret_ip[i]);

		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		real_ret1 = DHCPFreeIP(dhcp, mini_req_ip);
		real_ret2 = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

	/*PrintTree(dhcp->root, 0,3);*/
		ReturnComper(ret[0], real_ret1, dis, __LINE__);
		ReturnComper(ret[1], real_ret2, dis, __LINE__);
		MemoryComper(mini_ret_ip, real_ret_ip, sizeof(int), Convert, dis, __LINE__ + i);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
}

static void MiniFree1(char *net_ip, char *req_ip, char **ret_ip, int *ret,  char *dis)
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_req_ip[4] = {0};
	unsigned char mini_ret_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	int real_ret1 = 0;
	int real_ret2 = 0;
	int i = 0;

	CreateIP(mini_net_ip, net_ip);
	CreateIP(mini_req_ip, req_ip);

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		CreateIP(mini_ret_ip, ret_ip[i]);

		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		real_ret1 = DHCPFreeIP(dhcp, mini_req_ip);
		real_ret2 = DHCPFreeIP(dhcp, mini_req_ip);

	/*PrintTree(dhcp->root, 0,3);*/
		ReturnComper(ret[0], real_ret1, dis, __LINE__);
		ReturnComper(ret[1], real_ret2, dis, __LINE__);
		MemoryComper(mini_ret_ip, real_ret_ip, sizeof(int), Convert, dis, __LINE__ + i);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
}

static void MiniFree2(char *net_ip, char *req_ip, char **ret_ip, int *ret,  char *dis)
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_req_ip[4] = {0};
	unsigned char mini_ret_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	int real_ret1 = 0;
	int real_ret2 = 0;
	int i = 0;

	CreateIP(mini_net_ip, net_ip);

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		CreateIP(mini_ret_ip, ret_ip[i]);

		CreateIP(mini_req_ip, "255.255.255.001");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.002");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.003");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.004");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, req_ip);
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.006");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.007");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.008");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

		CreateIP(mini_req_ip, req_ip);
		real_ret1 = DHCPFreeIP(dhcp, mini_req_ip);
		real_ret2 = DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

/*		PrintTree(dhcp->root, 0,3);*/

		ReturnComper(ret[0], real_ret1, dis, __LINE__);
		ReturnComper(ret[1], real_ret2, dis, __LINE__);
		MemoryComper(mini_ret_ip, real_ret_ip, sizeof(int), Convert, dis, __LINE__ + i);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
}

void TestFree()
{
	char net_ip1[] = "015.007.005.045";
	char *req_ip1 = "015.057.055.001";
	char *ret_ip1[] = {"015.007.005.001", "015.007.055.001", "015.057.055.001"};
	int ret1[2] = {0, 0};
	char dis1[] = "tring to free valid ip. cheking the return value. and the return ip addres. for all classes";

	char net_ip2[] = "255.255.255.045";
	char req_ip2[] = "255.255.255.255";
	char *ret_ip2[] = {"000.000.000.000", "000.000.000.000", "000.000.000.000"};
	int ret2[2] = {1, 1};
	char dis2[] = "tring to free invalid ip. cheking the return value. and the return ip addres. for all classes";

	char net_ip3[] = "005.255.255.045";
	char req_ip3[] = "005.005.004.002";
	char *ret_ip3[] = {"005.255.255.002", "005.255.004.002", "005.005.004.002"};
	int ret3[2] = {0, 1};
	char dis3[] = "tring to double free. cheking the return value. for all classes.";

	char net_ip4[] = "255.255.255.045";
	char req_ip4[] = "255.255.255.005";
	char *ret_ip4[] = {"255.255.255.005", "255.255.255.005", "255.255.255.005"};
	int ret4[2] = {0, 0};
	char dis4[] = "adding 8 ips. then free ip 005, and allocing again ip 005. cheking the return value. and the return ip addres. for all classes";

	SetTest(3, "Free IP");

	MiniFree(net_ip1, req_ip1, ret_ip1, ret1, dis1);
	MiniFree(net_ip2, req_ip2, ret_ip2, ret2, dis2);
	MiniFree1(net_ip3, req_ip3, ret_ip3, ret3, dis3);
	MiniFree2(net_ip4, req_ip4, ret_ip4, ret4, dis4);
}

static void TestCount()
{
	dhcp_t *dhcp = NULL;
	unsigned char mini_net_ip[4] = {0};
	unsigned char mini_req_ip[4] = {0};
	unsigned char real_ret_ip[4] = {0};
	char mask = 24;
	size_t ret1[3] = {256 - 3, 65536 - 3, 16777216 - 3};
	size_t ret2[3] = {256 - 14, 65536 - 14, 16777216 - 14};
	size_t real_ret1 = 0;
	size_t real_ret2 = 0;
	char dis1[] = "using DCHPCount right after Create and cheaking the return value.";
	char dis2[] = "Allocated 15 random address. ond of them is free. using count and cheaking the return value.";
	int i = 0;

	SetTest(4, "Count");

	CreateIP(mini_net_ip, "255.000.000.000");

	while (i < 3)
	{
		dhcp = DHCPCreate(mini_net_ip, mask);

		real_ret1 = DHCPCountFree(dhcp);

		CreateIP(mini_req_ip, "255.255.255.001");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.002");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.003");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.004");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.006");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.007");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.008");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);

		CreateIP(mini_req_ip, "255.255.255.010");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.055");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.100");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.150");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		CreateIP(mini_req_ip, "255.255.255.180");
		DHCPAllocIP(dhcp, real_ret_ip, mini_req_ip);
		DHCPFreeIP(dhcp, mini_req_ip);


		/*PrintTree(dhcp->root, 0,3);*/

		real_ret2 = DHCPCountFree(dhcp);

		ReturnComper(ret1[i], real_ret1, dis1, __LINE__);
		ReturnComper(ret2[i], real_ret2, dis2, __LINE__);

		DHCPDestroy(dhcp);
		++i;
		mask -= 8;
	}
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
			TestCreateAndDestroy();
			break ;

		case 2:
			TestAlloc();
			break ;

		case 3:
			TestFree();
			break ;

		case 4:
		TestCount();
			break ;

		case 5:
			break ;
		case 6:
			break ;
		case 7:
			break ;
		case 8:
			break ;
		case 9:
			break ;
		default:
		TestCreateAndDestroy();
		TestAlloc();
		TestFree();
		TestCount();
	}
	PrintSummery();

	return 0;
}
