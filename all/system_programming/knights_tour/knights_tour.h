
#ifndef __KNIGHT_TOUR_H__
#define __KNIGHT_TOUR_H__


typedef enum KTstatus
{
    SUCCESS,
    FAIL,
    TIME_FAIL
}KTstatus_t;


/*
 *	KnightTour: O(2^n)
 *	   Find path to Knight Tour problem.
 *
 *	Argumentd:
 *		'x' - index to lines.
 *      'y' - index to columns.
 *
 *  Return:
 *		'SUCCESS' - success. In addition the function prints the path as a matrix.
 *      'FAIL' - fail.
 *      'TIME_FAIL' - didnt find path within 5 minuts.
 *
 */
KTstatus_t KnightTour(int x, int y);

#endif /* __KNIGHT_TOUR_H__ */
