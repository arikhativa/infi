
#include <assert.h> /*  assert()  */
#include <stddef.h> /*  size_t()  */
#include <stdlib.h> /*  free(), malloc(), calloc()  */
#include <string.h> /*  memcpy(), memset()  */

#include "dhcp.h"

#define MATCH		55		/* '55' is a random number */
#define NO_MATCH	50		/* '50' is a random number */
#define NO_NODE		80		/* '80' is a random number */
#define LIMIT		88		/* '88' is a random number */

/*	Subnet masking in Hex:	*/
#define CLASS_A		(0xff000000)
#define CLASS_B		(0xffff0000)
#define CLASS_C		(0xffffff00)


typedef struct trie_node trie_node_t;

typedef enum child
{
	LEFT = 0,
	RIGHT,
	NUM_OF_CHILDREN	/* The trie tree is based on binary input.  */
} child_t;

typedef enum address_status
{
	FREE = 0,
	OCCUPIED = 3, /* '3' is random. but the value should not be close to FREE	*/
	RESERVED = 4  /* '4' is random. but the value should not be close to FREE	*/
} address_stt;

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


/*	Funcs For Main Functions:	*/
static int AddReservdAddress(dhcp_t *dhcp, unsigned int ip);
static int CheakIPStatusAndMange(trie_node_t *node, unsigned int *ip,
	 int hieght, address_stt status);
static int RecInsert(trie_node_t *node, unsigned int ip, int hieght,
		address_stt status);
static int RecFreeNode(trie_node_t *node, unsigned int ip, int height);
static void RecCount(trie_node_t *node, int height, size_t *count);

/*	Trie Funcs:			*/
static void TrieDestroy(trie_node_t *node);
static trie_node_t *NodeCreate(void);
static void NodeDestroy(trie_node_t *node);
static void UpdataNodeStatus(trie_node_t *node);
static int GetNodeStatus(trie_node_t *node);

/*	IP Manipulation Funcs:		*/
static int GetNetMask(unsigned int *mask, char ch);
static unsigned int GetHieghtFromMask(const unsigned int mask);
static unsigned int
ConverCharPtrToInt(const unsigned char network_ip[IP_NUM_BYTES]);
static void WriteAdress(unsigned char dest[IP_NUM_BYTES], unsigned int src);

/*	Math Funcs:			*/
static size_t Power(size_t num1, size_t num2);


/******************************	API Functions ********************************/

dhcp_t *DHCPCreate(const unsigned char network_ip[IP_NUM_BYTES],
		size_t num_of_occupied_bits)
{
	dhcp_t *dhcp = NULL;
	unsigned int mask = 0;
	unsigned int converted_net_ip = 0;
	unsigned int brodcast_addres = 0;
	int stt = 0;

	assert(network_ip);

	if (GetNetMask(&mask, num_of_occupied_bits))
	{
		return NULL;
	}

	dhcp = (dhcp_t*)calloc(1, sizeof(dhcp_t));
	if (!dhcp)
	{
		return NULL;
	}

	dhcp->root = NodeCreate();

	if (!dhcp->root)
	{
		free(dhcp);
		dhcp = NULL;
		return NULL;
	}

	converted_net_ip = ConverCharPtrToInt((unsigned char *)network_ip);
	converted_net_ip = converted_net_ip & mask;

	memcpy((void*)&dhcp->network_ip, &converted_net_ip, IP_NUM_BYTES);
	memcpy((void*)&dhcp->subnet_mask, &mask, IP_NUM_BYTES);

	brodcast_addres = (mask & converted_net_ip) | ~mask;

	stt = AddReservdAddress(dhcp, (converted_net_ip & mask));	/* The Network adress */
	stt += AddReservdAddress(dhcp, brodcast_addres);			/* The Broadcast addres */
	stt += AddReservdAddress(dhcp, brodcast_addres - 1);		/* The Current divace addres */

	if (stt)
	{
		DHCPDestroy(dhcp);
		dhcp = NULL;
		return NULL;
	}

	return dhcp;
}

void DHCPDestroy(dhcp_t *dhcp)
{
	assert(dhcp);

	TrieDestroy(dhcp->root);

	memset(dhcp, 0, sizeof(dhcp_t));
	free(dhcp);
	dhcp = NULL;
}

dhcp_status_t DHCPAllocIP(dhcp_t* dhcp, unsigned char return_ip[IP_NUM_BYTES],
		const unsigned char req_ip[IP_NUM_BYTES])
{
	unsigned int converted_req_ip = 0;
	unsigned int converted_ip = 0;
	int height = 0;
	int stt = 0;

	assert(dhcp);
	assert(return_ip);

	/* If req_ip is NULL. We start searching from the network_ip.	*/
	if (req_ip)
	{
		converted_req_ip = ConverCharPtrToInt(req_ip);
	}

	converted_req_ip = converted_req_ip & ~dhcp->subnet_mask;
	converted_req_ip = converted_req_ip | dhcp->network_ip;
	converted_ip = converted_req_ip;

	height = GetHieghtFromMask(dhcp->subnet_mask);

	stt = CheakIPStatusAndMange(dhcp->root,
		 &converted_ip, height, OCCUPIED);
	if (stt)
	{
		return FAIL;
	}

	WriteAdress(return_ip, converted_ip);

	if (converted_ip == converted_req_ip || !req_ip)
	{
		return SUCCESS;
	}

	return NOT_REQUESTED;
}

dhcp_status_t
DHCPFreeIP(dhcp_t* dhcp, const unsigned char ip_to_free[IP_NUM_BYTES])
{
	unsigned int ip = 0;
	int stt = 0;
	int height = 0;

	assert(dhcp);

	ip = ConverCharPtrToInt(ip_to_free);
	height = GetHieghtFromMask(dhcp->subnet_mask);

	stt = RecFreeNode(dhcp->root, ip, height);

	return stt;
}

size_t DHCPCountFree(const dhcp_t *dhcp)
{
	int height = 0;
	size_t count = 0;

	assert(dhcp);

	height = GetHieghtFromMask(dhcp->subnet_mask);

	RecCount(dhcp->root, height, &count);

	return count;
}


/************************ Funcs For Main Functions ***************************/

static int AddReservdAddress(dhcp_t *dhcp, unsigned int ip)
{
	unsigned int hieght = 0;
	int stt = 0;

	assert(dhcp);

	hieght = GetHieghtFromMask(dhcp->subnet_mask);

	stt = CheakIPStatusAndMange(dhcp->root,
		 &ip, hieght, RESERVED);
	if (stt)
	{
		return FAIL;
	}

	return SUCCESS;
}

static int CheakIPStatusAndMange(
	trie_node_t *node, unsigned int *ip, int hieght, address_stt status)
{
	int stt = 0;
	unsigned int input = 0;

	if (!node)
	{
		return NO_NODE;
	}

	/* we use '<=' and not just '==' becuse RESERVED is also a status,
	   and it's value is greter then OCCUPIED.		*/
	if (OCCUPIED <= node->status)
	{
		return OCCUPIED;
	}

	input = !!(*ip & (1 << (hieght - 1)));

	stt = CheakIPStatusAndMange(node->child[input], ip, hieght - 1, status);

	if (NO_NODE == stt)
	{
		if (RecInsert(node, *ip, hieght, status))
		{
			return FAIL;
		}

		UpdataNodeStatus(node);
		return SUCCESS;
	}

	if (OCCUPIED == stt)
	{
		if (RIGHT == input)
		{
			return OCCUPIED;
		}

		/* We set the bit to '1'.
		   then we set every other bit to the right of it to '0'.
		   This get us to the next avilibal ip.				*/
		*ip = *ip | (1 << (hieght -1));
		*ip = *ip & (~0 << (hieght -1));

		input = !!(*ip & (1 << (hieght - 1)));

		stt = CheakIPStatusAndMange(node->child[input], ip, hieght - 1, status);
	}

	UpdataNodeStatus(node);

	return stt;
}

static int RecInsert(trie_node_t *node, unsigned int ip,
		int hieght, address_stt status)
{
	int stt = 0;
	unsigned int input = 0;

	if (0 == hieght)
	{
		node->status = status;
		return SUCCESS;
	}

	input = !!(ip & (1 << (hieght - 1)));

	if (!(node->child[input]))
	{
		node->child[input] = NodeCreate();
		if (!(node->child[input]))
		{
			return FAIL;
		}
	}

	stt = RecInsert(node->child[input], ip, hieght - 1, status);

	if (SUCCESS == stt)
	{
		UpdataNodeStatus(node);
	}

	return stt;
}

static int RecFreeNode(trie_node_t *node, unsigned int ip, int height)
{
	int input = 0;
	int stt = 0;

	if (!node)
	{
		return FAIL;
	}

	if (0 == height)
	{
		if (OCCUPIED == node->status)
		{
			node->status = FREE;
			return SUCCESS;
		}
		else
		{
			return FAIL;
		}
	}

	input = !!(ip & (1 << (height - 1)));

	stt = RecFreeNode(node->child[input], ip, height - 1);

	if (SUCCESS == stt)
	{
		UpdataNodeStatus(node);
	}

	return stt;
}

static void RecCount(trie_node_t *node, int height, size_t *count)
{
	if (!node)
	{
		*count += Power(2, height);
		return ;
	}

	if (0 == height && FREE == node->status)
	{
		++*count;
		return ;
	}

	if (OCCUPIED <= node->status)
	{
		return ;
	}

	RecCount(node->child[LEFT], height - 1, count);
	RecCount(node->child[RIGHT], height - 1, count);
}


/******************************	Trie Funcs ***********************************/

static void TrieDestroy(trie_node_t *node)
{
	if (!node)
	{
		return ;
	}

	TrieDestroy(node->child[LEFT]);
	TrieDestroy(node->child[RIGHT]);

	NodeDestroy(node);
	node = NULL;
}

static trie_node_t *NodeCreate(void)
{
	trie_node_t *new = NULL;

	/* We use calloc() so the node's status is set to FREE, and childs to NULL */
	new = (trie_node_t*)calloc(1, sizeof(trie_node_t));
	if (!new)
	{
		return NULL;
	}

	return new;
}

static void NodeDestroy(trie_node_t *node)
{
	assert(node);

	memset(node, 0, sizeof(trie_node_t));
	free(node);
	node = NULL;
}

static void UpdataNodeStatus(trie_node_t *node)
{
	int stt = 0;

	assert(node);

	stt = GetNodeStatus(node->child[LEFT]);
	stt += GetNodeStatus(node->child[RIGHT]);

	if (5 <= stt) /* '5' means the two children are OCCUPIED/RESERVED so the parent should be as well */
	{
		node->status = OCCUPIED;
	}
	else
	{
		node->status = FREE;
	}
}

static int GetNodeStatus(trie_node_t *node)
{
	if (!node)
	{
		return FREE;
	}

	return node->status;
}


/**************************** IP Manipulation Funcs: *************************/

static int GetNetMask(unsigned int *mask, char ch)
{
	switch (ch)
	{
		case 8:
			*mask = CLASS_A;
			return 0;

		case 16:
			*mask = CLASS_B;
			return 0;

		case 24:
			*mask = CLASS_C;
			return 0;
	}
	return 1;
}

static unsigned int GetHieghtFromMask(const unsigned int mask)
{
	switch (mask)
	{
		case CLASS_A:
			return 24;

		case CLASS_B:
			return 16;

		case CLASS_C:
			return 8;
	}

	return 0;
}

static unsigned int
ConverCharPtrToInt(const unsigned char network_ip[IP_NUM_BYTES])
{
	unsigned int ret = 0;
	unsigned int tmp = 0;
	int i = 0;

	for (i = 0; i < 4; ++i)
	{
		tmp = network_ip[i];
		ret = ret << 8;
		ret = ret | tmp;
	}

	return ret;
}

static void WriteAdress(unsigned char dest[IP_NUM_BYTES], unsigned int src)
{
	unsigned char tmp = 0;
	int i = 0;

	assert(dest);

	for (i = 3; -1 < i ; --i)
	{
		tmp = (char)src;
		src = src >> 8;
		dest[i] = tmp;
	}
}


/****************************** Math Funcs ***********************************/

static size_t Power(size_t num1, size_t num2)
{
	size_t origin = num1;

	if (0 == num2)
	{
		num1 = 1;
	}
	while (1 < num2)
	{
		num1 *= origin;
		--num2;
	}

	return num1;
}
