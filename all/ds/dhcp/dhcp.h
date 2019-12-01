/*
 *  ver 1.3  added comment to Alloc()
 *  ver 1.2  added unsigned to char ip
 *  ver 1.1
*/

#ifndef __DHCP_H__
#define __DHCP_H__

#include <stddef.h> /*size_t*/
#define IP_NUM_BYTES 4

typedef struct dhcp dhcp_t;

typedef enum dhcp_status
{
	SUCCESS,
	FAIL,
    NOT_REQUESTED
}dhcp_status_t;

/*
 *	DHCPCreate: O(1)
 *	    Create the dhcp.
 *
 *	Argumentd:
 *		'network_ip' - the prefix ip.
 *      'num_of_occupied_bits' - num of bits that occupied by the network ip prefix.
 *
 *  Return:
 *      pointer to the managment struct.
 *      'NULL' - in allocation fail.
 *
 */
dhcp_t *DHCPCreate(const unsigned char network_ip[IP_NUM_BYTES], size_t occupied_bits);

/*
 *	DHCPDestroy: O(n)
 *	    Desrtoy the dhcp.
 *
 *	Argumentd:
 *		'dhcp' - pointer to the managment struct.
 */
void DHCPDestroy(dhcp_t *dhcp);


/*
 *	DHCPAlloc: O(log(n))
 *	    Return new IP to the user.
 *
 *	Argumentd:
 *		'dhcp' - pointer to structure.
 *		'return_ip' - pointer to the new ip.
 *		'req_ip - in case the user want a specific ip. (in automatic allocation req = NULL)
 *
 *  Return:
 *      pointer to new ip in user ptr.
 *		SUCCESS - in case the allocation success.
 *      'FAIL' - in allocation fail. or if req_ip is the heightest avilibal.
 *		'NOT_REQUESTED' - in case the user ask specific IP and it didn't available -
 *			returns a diffrent ip and NOT_REQUESTED status.
 */
dhcp_status_t DHCPAllocIP(dhcp_t* dhcp, unsigned char return_ip[IP_NUM_BYTES], const unsigned char req_ip[IP_NUM_BYTES]);

/*
 *	DHCPFree: O(log(n))
 *	    free.
 *
 *	Argumentd:
 *		'dhcp' - pointer to structure.
 *		'ip_to_free' - IP number the user want to free.
 *
 *  Return:
 *		SUCCESS - in case the address is legal to free.
 *      'FAIL' - in illegal address for free.
 */
dhcp_status_t DHCPFreeIP(dhcp_t* dhcp, const unsigned char ip_to_free[IP_NUM_BYTES]);

/*
 *	DHCPCountFree: O(n)
 *	    return the number of free addresses.
 *
 *	Argumentd:
 *		'dhcp' - pointer to structure.
 *
 *  Return:
 *      number of available addresses.
 */
size_t DHCPCountFree(const dhcp_t* dhcp);

#endif /* __DHCP_H__ */
