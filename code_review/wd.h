
/*
    ver 1.1 - It Has Bigan!
*/

#ifndef __WATCHDOG_FUNCS_H__
#define __WATCHDOG_FUNCS_H__

/* DELLLLLLLLLLLLLLLL THIS
    files names:
    watchdog.out
    watchdog_funcs.c
    watchdog_funcs.h
*/

/*
    General instructions:
        watchdog.out must be in the same diractory as
        the user's program.

        The user's program must have it's own char *argv[].

        The user's program must not use SIGUSR1 and SIGUSR2.

        'watchdog.out' will check the user's program every 5 seconds.
*/

/*
    WDStart():
        Run the watchdog.out that will watch the caller.
        if the caller had stoped running, watchdog.out will
        run it again.

    Arguments:
        'argv' - needs to be the caller's argv.

    Return Value:
        '0' - On success.
        '-1' - On failure.
*/
int WDStart(char *argv[]);

/*
    WDStop():
        Terminate watchdog.out.
*/
void WDStop(void);

#endif /* __WATCHDOG_FUNCS_H__ */
