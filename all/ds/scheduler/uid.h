
/*
 *  ver 1.3 Added <sys/types.h>
 *  ver 1.2 Changed Struct uid, syntacs problems..
 *  ver 1.1
 */

#ifndef __UID_H__
#define __UID_H__

#include <time.h> /* time_t */
#include <stddef.h> /* size_t */
#include <unistd.h> /* pid_t */
#include <sys/types.h> /* pid_t */


typedef struct uid unique_id_t;

struct uid
{
	pid_t pid;
	size_t counter;
	time_t time;
};

/*
 * 	FILE NAMES:
 * 	uid.c	uid.h	uid_test.c
 *
 * 	IN THE .C FILE:
 *
 *	const unique_id_t bad_uid = { 0 };
 */

/*
 * 	'bad_uid' - A global var represent a problem with the UID.
 * 	It's values are set to 0;
 */
	extern const unique_id_t bad_uid;


/*	
 *	UidCreat()		O(1)
 *	Create a UID (with no allocation). 
 *
 *	Return:
 *		On Seccess - The new UID.
 *		On Failure - Bad UID ('uid_bad').
 */
	unique_id_t UidCreate(void);

/*
 *	UidIsSame():             O(1)
 *	Comparing between two uid's.
 *
 *	Arguments:
 *		'uid1' - uid to compare.
 *		'uid2' - uid to compare.
 *
 *	Return:
 *		Equal = 1, Otherwise = 0.
 */
	int UidIsSame(const unique_id_t uid1, const unique_id_t uid2);

/*
 *	IidIsBad():             O(1)
 *	Comparing a given uid to bad uid.
 *
 *	Arguments:
 *		'uid' - uid to compare.
 *
 *	Return:
 *		Equal = 1, Otherwise = 0.
 */
	int UidIsBad(const unique_id_t uid);

#endif /* __UID_H__ */

