#include <stdio.h>
#include <stdlib.h>

#define maxFunc(t1, t2) (((t1) > (t2)) ? (t1) : (t2))

static int s_count = 0;

typedef void (*func_ptr)(void* param);
typedef struct p_t Public_Transport;
typedef struct mini Minibus;
typedef struct taxi Taxi;
typedef struct s_taxi Special_Taxi;
typedef struct con Public_Convoy;

void Public_Transport_dtor(void* param);
void Public_Transport_Display(void* param);
void Minibus_dtor(void* param);
void Minibus_Display(void* param);
void Minibus_Wash(void* param);
void Taxi_dtor(void* param);
void Taxi_Display(void* param);
void Special_Taxi_dtor(void* param);
void Special_Taxi_Display(void* param);
void Public_Convoy_dtor(void* param);
void Public_Convoy_Display(void* param);


func_ptr  pt_vtable[2] = {Public_Transport_dtor, Public_Transport_Display};
func_ptr  minibus_vtable[3] = {Minibus_dtor, Minibus_Display, Minibus_Wash};
func_ptr  taxi_vtable[2] = {Taxi_dtor, Taxi_Display};
func_ptr  special_taxi_vtable[2] = {Special_Taxi_dtor, Special_Taxi_Display};
func_ptr  convoy_vtable[2] = {Public_Convoy_dtor, Public_Convoy_Display};

struct p_t
{
    func_ptr* vptr;
    int m_license_plate;
};

Public_Transport* Public_Transport_ctor(Public_Transport* this)
{
    this->vptr = pt_vtable;
    this->m_license_plate = ++s_count;
    printf("Public_Transport::Ctor()%d\n", s_count);
}

void Public_Transport_dtor(void* param)
{
    Public_Transport* this = (Public_Transport*)param;
    --s_count;
    printf("Public_Transport::Dtor()%d\n", this->m_license_plate);
}

void Public_Transport_cctor(Public_Transport* this, Public_Transport* other)
{
    this->vptr = other->vptr;
    this->m_license_plate = ++s_count;
    printf("Public_Transport::CCtor() %d\n", this->m_license_plate);
}

void Public_Transport_Display(void* param)
{
    Public_Transport* this = (Public_Transport*)param;
    printf("Public_Transport::Display(): %d\n", this->m_license_plate);
}

void PrintCount()
{
    printf("s_count: %d\n", s_count);
}

int GetID(Public_Transport* this)
{
    return this->m_license_plate;
}

struct mini
{
    Public_Transport pt;
    int m_numSeats;
};

void Minibus_ctor(Minibus* this)
{
    Public_Transport_ctor(&this->pt);
    this->pt.vptr = minibus_vtable;
    this->m_numSeats = 20;
    printf("Minibus::Ctor()\n");

}

void Minibus_cctor(Minibus* this, Minibus* other)
{
    Public_Transport_cctor(&this->pt, &other->pt);
    this->m_numSeats = other->m_numSeats;
    printf("Minibus::CCtor()\n");
}

void Minibus_dtor(void* param)
{
    Minibus* this = (Minibus*)param;
    printf("Minibus::dtor()\n");
    Public_Transport_dtor(&this->pt);
}

void Minibus_Display(void* param)
{
    Minibus* this = (Minibus*)param;
    printf("Minibus::Display() ID:%d", GetID(&this->pt));
    printf(" num seats:%d\n", this->m_numSeats);
}

void Minibus_Wash(void* param)
{
    Minibus* this = (Minibus*)param;
    printf("Minibus::Wash() ID: %d\n", GetID(&(this->pt)));
}

struct taxi
{
    Public_Transport pt;
    int m_numSeats;
};

void Taxi_ctor(Taxi* this)
{
    Public_Transport_ctor(&this->pt);
    this->pt.vptr = taxi_vtable;
    printf("Taxi::Ctor()\n");
}

void Taxi_cctor(Taxi* this, Taxi* other)
{
    Public_Transport_cctor(&this->pt, &other->pt);
    printf("Taxi::CCtor()\n");
}

void Taxi_dtor(void* param)
{
    Taxi* this = (Taxi*)param;
    printf("Taxi::dtor()\n");
    Public_Transport_dtor(&this->pt);
}


void Taxi_Display(void* param)
{
    Taxi* this = (Taxi*)param;
    printf("Taxi::Display() ID:%d\n", GetID(&this->pt));
}

struct s_taxi
{
    Taxi tax;
};

void Special_Taxi_ctor(Special_Taxi* this)
{
    Taxi_ctor(&this->tax);
    this->tax.pt.vptr = special_taxi_vtable;
    printf("Special_Taxi::Ctor()\n");
}

void Special_Taxi_cctor(Special_Taxi* this, Special_Taxi* other)
{
    Taxi_cctor(&other->tax, &other->tax);
    printf("Special_Taxi::CCtor()\n");
}

void Special_Taxi_dtor(void* param)
{
    Special_Taxi* this = (Special_Taxi*)param;
    printf("Special_Taxi::dtor()\n");
    Taxi_dtor(&this->tax);
}


void Special_Taxi_Display(void* param)
{
    Special_Taxi* this = (Special_Taxi*)param;
    printf("Special_Taxi::Display() ID:%d\n", GetID(&this->tax.pt));
}

struct con
{
    Public_Transport pt;
    Public_Transport *m_pt1;
    Public_Transport *m_pt2;
    Minibus m_m;
    Taxi m_t;

};

void Public_Convoy_ctor(Public_Convoy* this)
{
    Public_Transport_ctor(&this->pt);
    this->pt.vptr = convoy_vtable;
    this->m_pt1 = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)this->m_pt1);
    this->m_pt2 = malloc(sizeof(Taxi));
    Taxi_ctor((Taxi*)this->m_pt2);
    Minibus_ctor(&this->m_m);
    Taxi_ctor(&this->m_t);
}

void Public_Convoy_cctor(Public_Convoy* this, Public_Convoy* other)
{
    Public_Transport_cctor(&this->pt, &other->pt);
    this->m_pt1 = malloc(sizeof(Minibus));
    Minibus_cctor((Minibus*)this->m_pt1, (Minibus*)other->m_pt1);
    this->m_pt2 = malloc(sizeof(Taxi));
    Taxi_cctor((Taxi*)this->m_pt2, (Taxi*)other->m_pt2);
    Minibus_cctor(&this->m_m, &other->m_m);
    Taxi_cctor(&this->m_t, &other->m_t);
}

void Public_Convoy_dtor(void* param)
{
    Public_Convoy* this = (Public_Convoy*)param;
    this->m_pt1->vptr[0](this->m_pt1);
    free(this->m_pt1);
    this->m_pt2->vptr[0](this->m_pt2);
    free(this->m_pt2);
    Taxi_dtor(&this->m_t);
    Minibus_dtor(&this->m_m);
    Public_Transport_dtor(&this->pt);
}


void Public_Convoy_Display(void* param)
{
    Public_Convoy* this = (Public_Convoy*)param;
    this->m_pt1->vptr[1](this->m_pt1);
    this->m_pt2->vptr[1](this->m_pt2);
    Minibus_Display(&this->m_m);
    Taxi_Display(&this->m_t);
}

void PrintInfo_P_T_ref(Public_Transport* a)
{
    a->vptr[1](a);
}

void PrintInfo(void)
{
    PrintCount();
}

void PrintInfo_int(Public_Transport* tmp, int i)
{
    (void)i;
    Minibus ret;
    Minibus_ctor(&ret);
    printf("PrintInfo(int i)\n");
    Minibus_Display(&ret);
    Public_Transport_cctor(tmp, (Public_Transport*)&ret);
    Minibus_dtor(&ret);
}

void TaxiDisplay(Taxi* s)
{
    Taxi_Display(s);
}

int main()
{
    Minibus m;

    Minibus_ctor(&m);

    PrintInfo_P_T_ref((Public_Transport*)&m);

    Public_Transport tmp;

    PrintInfo_int(&tmp, 3);
    Public_Transport_Display(&tmp);
    Public_Transport_dtor(&tmp);

    Public_Transport *array[3];

    array[0] = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)array[0]);
    array[1] = malloc(sizeof(Taxi));
    Taxi_ctor((Taxi*)array[1]);
    array[2] = malloc(sizeof(Minibus));
    Minibus_ctor((Minibus*)array[2]);

    for(int i = 0; i < 3; ++i)
    {
        array[i]->vptr[1](array[i]);
    }
    for (int i = 0; i < 3; ++i)
    {
        array[i]->vptr[0](array[i]);
        free(array[i]);
    }

    Public_Transport arr2[3];
    Minibus tmp1;

    Minibus_ctor(&tmp1);
    Public_Transport_cctor(&arr2[0], (Public_Transport*)&tmp1);
    Minibus_dtor(&tmp1);
    Taxi tmp2;
    Taxi_ctor(&tmp2);
    Public_Transport_cctor(&arr2[1], (Public_Transport*)&tmp2);
    Taxi_dtor(&tmp2);
    Public_Transport tmp3;
    Public_Transport_ctor(&arr2[2]);

    for(int i = 0; i < 3; ++i)
    {
        // virtual
        Public_Transport_Display(&arr2[i]);
    }

    PrintCount();
    Minibus m2;
    Minibus_ctor(&m2);
    PrintCount();

    Minibus arr3[4];
    Minibus_ctor(&arr3[0]);
    Minibus_ctor(&arr3[1]);
    Minibus_ctor(&arr3[2]);
    Minibus_ctor(&arr3[3]);

    Taxi* arr4 = malloc(4 * sizeof(Taxi));
    Taxi_ctor(arr4);
    Taxi_ctor(arr4 + 1);
    Taxi_ctor(arr4 + 2);
    Taxi_ctor(arr4 + 3);

    Taxi_dtor(arr4 + 3);
    Taxi_dtor(arr4 + 2);
    Taxi_dtor(arr4 + 1);
    Taxi_dtor(arr4);
    free(arr4);

    printf("%d\n", maxFunc(1,2));

    Special_Taxi st;
    Special_Taxi_ctor(&st);
    Taxi tmp4;
    Taxi_cctor(&tmp4, (Taxi*)&st);
    TaxiDisplay(&tmp4);
    Taxi_dtor(&tmp4);

    Public_Convoy *ts1 = malloc(sizeof(Public_Convoy));
    Public_Convoy_ctor(ts1);
    Public_Convoy *ts2 = malloc(sizeof(Public_Convoy));
    Public_Convoy_cctor(ts2, ts1);
    // virtual
    Public_Convoy_Display(ts1);
    Public_Convoy_Display(ts2);
    Public_Convoy_dtor(ts1);
    free(ts1);
    Public_Convoy_Display(ts2);
    Public_Convoy_dtor(ts2);
    free(ts2);
    // end virtual 


    Special_Taxi_dtor(&st);
    Minibus_dtor(&arr3[3]);
    Minibus_dtor(&arr3[2]);
    Minibus_dtor(&arr3[1]);
    Minibus_dtor(&arr3[0]);
    Minibus_dtor(&m2);
    Public_Transport_dtor(&arr2[2]);
    Public_Transport_dtor(&arr2[1]);
    Public_Transport_dtor(&arr2[0]);
    Minibus_dtor(&m);
    return 0;
}
