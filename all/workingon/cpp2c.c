

#include <stdio.h>  /* printf() */
#include <stdlib.h>  /* malloc(), free() */

enum vtable
{
    DTOR = 0,
    DISPLAY,
    WASH
};

typedef void (*fp_t)(void *param);

fp_t Public_Transport_vtable[2] = {0};
fp_t Minibus_vtable[3] = {0};
fp_t Taxi_vtable[2] = {0};


int s_count = 0;
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

/********************************************************** Public_Transport */
void Public_Transport_ctor(Public_Transport *this)
{
    this->vptr = Public_Transport_vtable;
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

    this->vptr = Public_Transport_vtable;
    this->m_license_plate = ++s_count;

    printf("Public_Transport::CCtor()%d\n", this->m_license_plate);
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

    this->base.vptr = Minibus_vtable;

    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");
}

void Minibus_cctor(Minibus *this, const Minibus *const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    this->base.vptr = Minibus_vtable;

    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void Minibus_dtor(void *this)
{
    printf("Minibus::Dtor()\n");
    Public_Transport_dtor((Public_Transport*)this);
}

void Minibus_Display(void *this)
{
    printf("Minibus::Display() ID: %d", GetID((Public_Transport*)this));
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

    this->base.vptr = Taxi_vtable;

    printf("Taxi::Ctor()\n");
}

void Taxi_cctor(Taxi *this, const Taxi* const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    this->base.vptr = Taxi_vtable;

    printf("Taxi::CCtor()\n");
}

void Taxi_dtor(void *this)
{
    printf("Taxi::dtor()\n");
    Public_Transport_dtor((Public_Transport*)this);
}

void Taxi_Display(void *this)
{
    printf("Taxi::Display()%d\n", GetID((Public_Transport*)this));
}


/******************************************************************** Vtable */
static void CreateVtable()
{
    Public_Transport_vtable[DTOR] = Public_Transport_dtor;
    Public_Transport_vtable[DISPLAY] = Public_Transport_Display;

    Minibus_vtable[DTOR] = Minibus_dtor;
    Minibus_vtable[DISPLAY] = Minibus_Display;
    Minibus_vtable[WASH] = Minibus_Wash;

    Taxi_vtable[DTOR] = Taxi_dtor;
    Taxi_vtable[DISPLAY] = Taxi_Display;
}


/********************************************************* Static Functions: */
void PrintInfo_Public_Transport(Public_Transport *a)
{
    a->vptr[DISPLAY](a);
}

void PrintInfo_int(Public_Transport *tmp, int i)
{
	Minibus ret;

    (void)i;

    Minibus_ctor(&ret);

	printf("PrintInfo(int i)\n");

    ret.base.vptr[DISPLAY](&ret);

    Public_Transport_cctor(tmp, (Public_Transport*)&ret);
    ret.base.vptr[DTOR](&ret);
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
    Taxi *arr4[4];
    int i = 0;

    (void)argc;
    (void)argv;
    (void)envp;

    CreateVtable();

    Minibus_ctor(&m);

    PrintInfo_Public_Transport((Public_Transport*)&m);

    PrintInfo_int(&public_transport_tmp, 3);
    public_transport_tmp.vptr[DISPLAY](&public_transport_tmp);
    public_transport_tmp.vptr[DTOR](&public_transport_tmp);

    array[0] = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)array[0]);
    array[1] = malloc(sizeof(Taxi));
    Taxi_ctor((Taxi*)array[1]);
    array[2] = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)array[2]);


    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr[DISPLAY](array[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr[DTOR](array[i]);
    }

    Minibus_ctor(&minibus_tmp);
    Public_Transport_cctor((Public_Transport*)&arr2[0], (Public_Transport*)&minibus_tmp);
    minibus_tmp.base.vptr[DTOR](&minibus_tmp);

    Taxi_ctor(&taxi_tmp);
    Public_Transport_cctor((Public_Transport*)&arr2[1], (Public_Transport*)&taxi_tmp);
    taxi_tmp.base.vptr[DTOR](&taxi_tmp);

    Public_Transport_ctor(&arr2[2]);

    for (i = 0; i < 3; ++i)
    {
        arr2[i].vptr[DISPLAY](&arr2[i]);
    }

    Public_Transport_PrintCount();
    Minibus_ctor(&m2);
    Public_Transport_PrintCount();


    printf("\n\n\n");
    Minibus_ctor(&arr3[0]);
    Minibus_ctor(&arr3[1]);
    Minibus_ctor(&arr3[2]);
    Minibus_ctor(&arr3[3]);

    arr4[0] = malloc(sizeof(Taxi));
    Taxi_ctor(arr4[0]);
    arr4[1] = malloc(sizeof(Taxi));
    Taxi_ctor(arr4[1]);
    arr4[2] = malloc(sizeof(Taxi));
    Taxi_ctor(arr4[2]);
    arr4[3] = malloc(sizeof(Taxi));
    Taxi_ctor(arr4[3]);

    Taxi_dtor(arr4[3]);
    free(arr4[3]);
    Taxi_dtor(arr4[2]);
    free(arr4[2]);
    Taxi_dtor(arr4[1]);
    free(arr4[1]);
    Taxi_dtor(arr4[0]);
    free(arr4[0]);



    printf("\n\n\n");

    m.base.vptr[DTOR](&m);
    return 0;
}
