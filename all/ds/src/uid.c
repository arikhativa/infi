
#include <time.h>	/* time() */
#include <sys/types.h>	/* getpid() */
#include <unistd.h>	/* getpid() */
#include <stddef.h>	/* size_t */

#include "uid.h"

const unique_id_t bad_uid = { 0 };

unique_id_t UidCreate()
{
	unique_id_t ret = { 0 };
	static size_t counter = 0;

	ret.time = time(NULL);
	ret.pid = getpid();
	ret.counter = counter;

	++counter;

	return ret;
}

int UidIsSame(const unique_id_t uid1, const unique_id_t uid2)
{
	int ret = 0;

	ret += (uid1.pid == uid2.pid);
	ret += (uid1.counter == uid2.counter);
	ret += (uid1.time == uid2.time);

	return (ret == 3);
}

int UidIsBad(const unique_id_t uid)
{
	return UidIsSame(uid, bad_uid);
}
