

#include <stdio.h>  /* printf() */
#include <stdlib.h>  /* malloc(), free() */

#define Max(a, b) (((a) > (b)) ? (a) : (b))

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


/***************************************************************** Global var*/
int s_count = 0;

fp_t Public_Transport_vtable[2] = {0};
fp_t Minibus_vtable[3] = {0};
fp_t Taxi_vtable[2] = {0};
fp_t Special_Taxi_vtable[2] = {0};
fp_t Public_Convoy_vtable[2] = {0};


/********************************************************** Public_Transport */
void Public_Transport_ctor(Public_Transport *this)
{
    this->vptr = Public_Transport_vtable;
    this->m_license_plate = ++s_count;

}

void Public_Transport_dtor(void *this)
{
    --s_count;
}

void Public_Transport_cctor(Public_Transport *this, Public_Transport *other)
{
    (void)other;

    this->vptr = Public_Transport_vtable;
    this->m_license_plate = ++s_count;

}

void Public_Transport_Display(void *this)
{
}

int GetID(Public_Transport *this)
{
    return this->m_license_plate;
}

static void Public_Transport_PrintCount()
{
}


/******************************************************************* Minibus */
void Minibus_ctor(Minibus *this)
{
    Public_Transport_ctor((Public_Transport*)this);

    this->base.vptr = Minibus_vtable;

    this->m_numSeats = 20;
}

void Minibus_cctor(Minibus *this, const Minibus *const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    this->base.vptr = Minibus_vtable;

    this->m_numSeats = other->m_numSeats;
}

void Minibus_dtor(void *this)
{
    Public_Transport_dtor((Public_Transport*)this);
}

void Minibus_Display(void *this)
{
    GetID((Public_Transport*)this);
}

void Minibus_Wash(void *this)
{
    GetID((Public_Transport*)this);
}


/********************************************************************** Taxi */
void Taxi_ctor(Taxi *this)
{
    Public_Transport_ctor((Public_Transport*)this);

    this->base.vptr = Taxi_vtable;

}

void Taxi_cctor(Taxi *this, const Taxi* const other)
{
    Public_Transport_cctor((Public_Transport*)this, (Public_Transport*)other);

    this->base.vptr = Taxi_vtable;

}

void Taxi_dtor(void *this)
{
    Public_Transport_dtor((Public_Transport*)this);
}

void Taxi_Display(void *this)
{
    GetID((Public_Transport*)this);
}

void TaxiDisplay(Taxi* s)
{
	s->base.vptr[DISPLAY](s);
}


/************************************************************** Special_Taxi */
void Special_Taxi_ctor(Special_Taxi *this)
{
    Taxi_ctor((Taxi*)this);

    this->base.base.vptr = Special_Taxi_vtable;

}

void Special_Taxi_cctor(Special_Taxi *this, const Special_Taxi* const other)
{
    Taxi_cctor((Taxi*)this, (Taxi*)other);

    this->base.base.vptr = Special_Taxi_vtable;

}

void Special_Taxi_dtor(void *this)
{
    Taxi_dtor((Taxi*)this);
}

void Special_Taxi_Display(void *this)
{
    GetID((Public_Transport*)this);
}


/************************************************************* Public_Convoy */
void Public_Convoy_ctor(Public_Convoy *this)
{
    Public_Transport_ctor((Public_Transport*)this);

    this->base.vptr = Public_Convoy_vtable;

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

    this->base.vptr = Public_Convoy_vtable;

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

    Minibus_dtor((Minibus*)ptr->m_pt1);
    Taxi_dtor((Taxi*)ptr->m_pt2);

    Taxi_dtor((Taxi*)&ptr->m_t);
    Minibus_dtor((Minibus*)&ptr->m_m);

    Public_Transport_dtor((Public_Transport*)this);
}

void Public_Convoy_Display(void *this)
{
    Public_Convoy* ptr = this;

    ((Minibus*)ptr->m_pt1)->base.vptr[DISPLAY](ptr->m_pt1);
    ((Taxi*)ptr->m_pt2)->base.vptr[DISPLAY](ptr->m_pt2);
    ptr->m_m.base.vptr[DISPLAY](&ptr->m_m);
    ptr->m_t.base.vptr[DISPLAY](&ptr->m_t);
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

    Special_Taxi_vtable[DTOR] = Special_Taxi_dtor;
    Special_Taxi_vtable[DISPLAY] = Special_Taxi_Display;

    Public_Convoy_vtable[DTOR] = Public_Convoy_dtor;
    Public_Convoy_vtable[DISPLAY] = Public_Convoy_Display;
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
    Taxi *arr4;
    int i = 0;
    Special_Taxi st;
    Public_Convoy *ts1 = 0;
    Public_Convoy *ts2 = 0;

    (void)argc;
    (void)argv;
    (void)envp;

    CreateVtable();

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
        array[i]->vptr[DISPLAY](array[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        array[i]->vptr[DTOR](array[i]);
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
        arr2[i].vptr[DISPLAY](&arr2[i]);
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
        Taxi_dtor(arr4 + i);
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

    ts1->base.vptr[DISPLAY](ts1);
    ts2->base.vptr[DISPLAY](ts2);

    ts1->base.vptr[DTOR](ts1);

    ts2->base.vptr[DISPLAY](ts2);
    ts2->base.vptr[DTOR](ts2);

    Special_Taxi_dtor(&st);
    for (i = 3 ; i != -1 ; --i)
    {
        arr3[i].base.vptr[DTOR](&arr3[i]);
    }

    Minibus_dtor(&m2);

    arr2[2].vptr[DTOR](&arr2[2]);
    arr2[1].vptr[DTOR](&arr2[1]);
    arr2[0].vptr[DTOR](&arr2[0]);

    Minibus_dtor(&m);

    return 0;
}
