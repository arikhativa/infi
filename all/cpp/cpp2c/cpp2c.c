

#include <stdio.h>  /* printf() */
#include <stdlib.h>  /* malloc(), free() */

enum vtable
{
    DTOR = 0,
    DISPLAY,
    WASH
};

typedef void (*fp_t)(void *param);


/******************************************************************* Classes */
typedef struct Public_Transport
{
    fp_t *vptr;
    int m_license_plate;
} Public_Transport;


typedef struct Minibus
{
    Public_Transport base;
    int m_numSeats;
} Minibus;


typedef struct Taxi
{
    Public_Transport base;
    int m_numSeats;
} Taxi;


typedef struct Special_Taxi
{
    Taxi base;
} Special_Taxi;


typedef struct Public_Convoy
{
    Public_Transport base;
    Public_Transport *m_pt1;
    Public_Transport *m_pt2;
    Minibus m_m;
    Taxi m_t;
} Public_Convoy;


/*************************************************************** Declaration */
void Public_Transport_dtor(void *this);
void Public_Transport_Display(void *this);
void Minibus_dtor(void *this);
void Minibus_Display(void *this);
void Minibus_Wash(void *this);
void Taxi_dtor(void *this);
void Taxi_Display(void *this);
void Special_Taxi_dtor(void *this);
void Special_Taxi_Display(void *this);
void Public_Convoy_dtor(void *this);
void Public_Convoy_Display(void *this);
int Max(int a, int b);


/***************************************************************** Global var*/
int s_count = 0;

fp_t Public_Transport_vtable[2] =
    {
        Public_Transport_dtor,
        Public_Transport_Display
    };

fp_t Minibus_vtable[3] =
    {
        Minibus_dtor,
        Minibus_Display,
        Minibus_Wash
    };

fp_t Taxi_vtable[2] =
    {
        Taxi_dtor,
        Taxi_Display
    };

fp_t Special_Taxi_vtable[2] =
    {
        Special_Taxi_dtor,
        Special_Taxi_Display
    };

fp_t Public_Convoy_vtable[2] =
    {
        Public_Convoy_dtor,
        Public_Convoy_Display
    };


/********************************************************** Public_Transport */
void Public_Transport_ctor(Public_Transport *this)
{
    *(fp_t**)this = Public_Transport_vtable;
    this->m_license_plate = ++s_count;

    printf("Public_Transport::Ctor()%d\n", this->m_license_plate);
}

void Public_Transport_dtor(void *this)
{
    --s_count;
    printf("Public_Transport::Dtor()%d\n", ((Public_Transport *)this)->m_license_plate);
}

void Public_Transport_cctor(Public_Transport *this, Public_Transport *other)
{
    (void)other;

    *(fp_t**)this = Public_Transport_vtable;
    this->m_license_plate = ++s_count;

    printf("Public_Transport::CCtor() %d\n", this->m_license_plate);
}

void Public_Transport_Display(void *this)
{
    printf("Public_Transport::Display(): %d\n", ((Public_Transport *)this)->m_license_plate);
}

int GetID(Public_Transport *this)
{
    return this->m_license_plate;
}

static void Public_Transport_PrintCount()
{
    printf("s_count: %d\n", s_count);
}


/******************************************************************* Minibus */
void Minibus_ctor(Minibus *this)
{
    Public_Transport_ctor((Public_Transport*)this);

    *(fp_t**)this = Minibus_vtable;

    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void Minibus_cctor(Minibus *this, const Minibus *const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    *(fp_t**)this = Minibus_vtable;

    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void Minibus_dtor(void *this)
{
    printf("Minibus::dtor()\n");

    *(fp_t**)this = Public_Transport_vtable;

    Public_Transport_dtor((Public_Transport*)this);
}

void Minibus_Display(void *this)
{
    printf("Minibus::Display() ID:%d", GetID((Public_Transport*)this));
    printf(" num seats:%d\n", ((Minibus *)this)->m_numSeats);
}

void Minibus_Wash(void *this)
{
    printf("Minibus::Wash() ID:%d\n", GetID((Public_Transport*)this));
}


/********************************************************************** Taxi */
void Taxi_ctor(Taxi *this)
{
    Public_Transport_ctor((Public_Transport*)this);

    *(fp_t**)this = Taxi_vtable;

    printf("Taxi::Ctor()\n");
}

void Taxi_cctor(Taxi *this, const Taxi* const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    *(fp_t**)this = Taxi_vtable;

    printf("Taxi::CCtor()\n");
}

void Taxi_dtor(void *this)
{
    printf("Taxi::dtor()\n");

    *(fp_t**)this = Public_Transport_vtable;

    Public_Transport_dtor((Public_Transport*)this);
}

void Taxi_Display(void *this)
{
    printf("Taxi::Display() ID:%d\n", GetID((Public_Transport*)this));
}

void TaxiDisplay(Taxi* s)
{
	(*(fp_t**)s)[DISPLAY](s);
}


/************************************************************** Special_Taxi */
void Special_Taxi_ctor(Special_Taxi *this)
{
    Taxi_ctor((Taxi*)this);

    *(fp_t**)this = Special_Taxi_vtable;

    printf("Special_Taxi::Ctor()\n");
}

void Special_Taxi_cctor(Special_Taxi *this, const Special_Taxi* const other)
{
    Taxi_cctor((Taxi*)this, (Taxi*)other);

    *(fp_t**)this = Special_Taxi_vtable;

    printf("Special_Taxi::CCtor()\n");
}

void Special_Taxi_dtor(void *this)
{
    printf("Special_Taxi::dtor()\n");

    *(fp_t**)this = Taxi_vtable;

    Taxi_dtor((Taxi*)this);
}

void Special_Taxi_Display(void *this)
{
    printf("Special_Taxi::Display()%d\n", GetID((Public_Transport*)this));
}


/************************************************************* Public_Convoy */
void Public_Convoy_ctor(Public_Convoy *this)
{
    Public_Transport_ctor((Public_Transport*)this);

    *(fp_t**)this = Public_Convoy_vtable;

    this->m_pt1 = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)this->m_pt1);

    this->m_pt2 = malloc(sizeof(Taxi));
    Taxi_ctor((Taxi*)this->m_pt2);

    Minibus_ctor(&this->m_m);
    Taxi_ctor(&this->m_t);
}

void Public_Convoy_cctor(Public_Convoy *this, const Public_Convoy* const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    *(fp_t**)this = Public_Convoy_vtable;

    this->m_pt1 = malloc(sizeof(Minibus));
    Minibus_cctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);

    this->m_pt2 = malloc(sizeof(Taxi));
    Taxi_cctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);

    Minibus_cctor(&this->m_m, &other->m_m);
    Taxi_cctor(&this->m_t, &other->m_t);
}

void Public_Convoy_dtor(void *this)
{
    Public_Convoy* ptr = this;

    (*(fp_t**)((Minibus*)ptr->m_pt1))[DTOR](ptr->m_pt1);
    free(ptr->m_pt1);
    (*(fp_t**)((Taxi*)ptr->m_pt2))[DTOR](ptr->m_pt2);
    free(ptr->m_pt2);

    Taxi_dtor((Taxi*)&ptr->m_t);
    Minibus_dtor((Minibus*)&ptr->m_m);
    
    *(fp_t**)this = Public_Transport_vtable;

    Public_Transport_dtor((Public_Transport*)this);
}

void Public_Convoy_Display(void *this)
{
    Public_Convoy* ptr = this;

    (*(fp_t**)((Minibus*)ptr->m_pt1))[DISPLAY](ptr->m_pt1);
    (*(fp_t**)((Taxi*)ptr->m_pt2))[DISPLAY](ptr->m_pt2);
    Minibus_Display(&ptr->m_m);
    TaxiDisplay(&ptr->m_t);
}


/********************************************************* Static Functions: */
void PrintInfo_Public_Transport(Public_Transport *a)
{
    (*(fp_t**)a)[DISPLAY](a);
}

void PrintInfo_int(Public_Transport *tmp, int i)
{
	Minibus ret;

    (void)i;

    Minibus_ctor(&ret);

	printf("PrintInfo(int i)\n");

    Minibus_Display(&ret);

    Public_Transport_cctor(tmp, (Public_Transport*)&ret);

    Minibus_dtor(&ret);
}

int Max(int a, int b)
{
    return ( ((a) > (b)) ? (a) : (b) );
}

/********************************************************************* Main: */
int main(int argc, char **argv, char **envp)
{
    Minibus m;
    Minibus m2;
    Public_Transport public_transport_tmp;
    Minibus minibus_tmp;
    Taxi taxi_tmp;
    Public_Transport *array[3];
    Public_Transport arr2[3];
    Minibus arr3[4];
    Taxi *arr4;
    int i = 0;
    Special_Taxi st;
    Public_Convoy *ts1 = 0;
    Public_Convoy *ts2 = 0;

    (void)argc;
    (void)argv;
    (void)envp;

    Minibus_ctor(&m);

    PrintInfo_Public_Transport((Public_Transport*)&m);

    PrintInfo_int(&public_transport_tmp, 3);
    Public_Transport_Display(&public_transport_tmp);
    Public_Transport_dtor(&public_transport_tmp);


    array[0] = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)array[0]);
    array[1] = malloc(sizeof(Taxi));
    Taxi_ctor((Taxi*)array[1]);
    array[2] = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)array[2]);


    for (i = 0; i < 3; ++i)
    {
        (*(fp_t**)array[i])[DISPLAY](array[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        (*(fp_t**)array[i])[DTOR](array[i]);
        free(array[i]);
    }

    Minibus_ctor(&minibus_tmp);
    Public_Transport_cctor((Public_Transport*)&arr2[0], (Public_Transport*)&minibus_tmp);
    Minibus_dtor(&minibus_tmp);

    Taxi_ctor(&taxi_tmp);
    Public_Transport_cctor((Public_Transport*)&arr2[1], (Public_Transport*)&taxi_tmp);
    Taxi_dtor(&taxi_tmp);

    Public_Transport_ctor(&arr2[2]);

    for (i = 0; i < 3; ++i)
    {
        (*(fp_t**)(&arr2[i]))[DISPLAY](&arr2[i]);
    }

    Public_Transport_PrintCount();
    Minibus_ctor(&m2);
    Public_Transport_PrintCount();

    Minibus_ctor(&arr3[0]);
    Minibus_ctor(&arr3[1]);
    Minibus_ctor(&arr3[2]);
    Minibus_ctor(&arr3[3]);

    arr4 = malloc(sizeof(Taxi) * 4);
    for (i = 0; i < 4; ++i)
    {
        Taxi_ctor(arr4 + i);
    }
    for (i = 3; i != -1; --i)
    {
        (*(fp_t**)(arr4 + i))[DTOR](arr4 + i);
    }
    free(arr4);

    printf("%d\n", Max(1, 2));
    Special_Taxi_ctor(&st);

    Taxi_cctor(&taxi_tmp, (Taxi*)&st);
    TaxiDisplay(&taxi_tmp);
    Taxi_dtor(&taxi_tmp);

    ts1 = malloc(sizeof(Public_Convoy));
    Public_Convoy_ctor(ts1);

    ts2 = malloc(sizeof(Public_Convoy));
    Public_Convoy_cctor(ts2, ts1);

    (*(fp_t**)ts1)[DISPLAY](ts1);
    (*(fp_t**)ts2)[DISPLAY](ts2);

    (*(fp_t**)ts1)[DTOR](ts1);
    free(ts1);

    (*(fp_t**)ts2)[DISPLAY](ts2);
    (*(fp_t**)ts2)[DTOR](ts2);
    free(ts2);

    Special_Taxi_dtor(&st);
    for (i = 3 ; i != -1 ; --i)
    {
        (*(fp_t**)(&arr3[i]))[DTOR](&arr3[i]);
    }

    Minibus_dtor(&m2);

    Public_Transport_dtor(&arr2[2]);
    Public_Transport_dtor(&arr2[1]);
    Public_Transport_dtor(&arr2[0]);

    Minibus_dtor(&m);

    return 0;
}
