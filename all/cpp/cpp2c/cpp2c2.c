

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
typedef struct Animal
{
    fp_t *vptr;
    int m_id;
} Animal;


typedef struct Centipede
{
    Animal base;
    int m_numLegs;
} Centipede;


typedef struct Squirl
{
    Animal base;
} Squirl;


typedef struct FlyingSquirl
{
    Squirl base;
} FlyingSquirl;


typedef struct LegendaryAnimal
{
    Animal base;
    Animal *m_pt1;
    Animal *m_pt2;
    Centipede m_m;
    Squirl m_t;
} LegendaryAnimal;


/*************************************************************** Declaration */
void Animal_dtor(void *this);
void Animal_Display(void *this);
void Centipede_dtor(void *this);
void Centipede_Display(void *this);
void Centipede_BuyShues(void *this);
void Squirl_dtor(void *this);
void Squirl_Display(void *this);
void Special_Squirl_dtor(void *this);
void Special_Squirl_Display(void *this);
void LegendaryAnimal_dtor(void *this);
void LegendaryAnimal_Display(void *this);
int Max(int a, int b);


/***************************************************************** Global var*/
int s_count = 0;

fp_t Animal_vtable[2] =
    {
        Animal_dtor,
        Animal_Display
    };

fp_t Centipede_vtable[3] =
    {
        Centipede_dtor,
        Centipede_Display,
        Centipede_BuyShues
    };

fp_t Squirl_vtable[2] =
    {
        Squirl_dtor,
        Squirl_Display
    };

fp_t Special_Squirl_vtable[2] =
    {
        Special_Squirl_dtor,
        Special_Squirl_Display
    };

fp_t LegendaryAnimal_vtable[2] =
    {
        LegendaryAnimal_dtor,
        LegendaryAnimal_Display
    };


/********************************************************** Animal */
void Animal_ctor(Animal *this)
{
    *(fp_t**)this = Animal_vtable;
    this->m_id = ++s_count;

    printf("Animal::Ctor()%d\n", this->m_id);
}

void Animal_dtor(void *this)
{
    --s_count;
    printf("Animal::Dtor()%d\n", ((Animal *)this)->m_id);
}

void Animal_cctor(Animal *this, Animal *other)
{
    (void)other;

    *(fp_t**)this = Animal_vtable;
    this->m_id = ++s_count;

    printf("Animal::CCtor() %d\n", this->m_id);
}

void Animal_Display(void *this)
{
    printf("Animal::Display() : %d\n", ((Animal *)this)->m_id);
}

int GetID(Animal *this)
{
    return this->m_id;
}

static void Animal_PrintCount()
{
    printf("s_count: %d\n", s_count);
}


/******************************************************************* Centipede */
void Centipede_ctor(Centipede *this)
{
    Animal_ctor((Animal*)this);

    *(fp_t**)this = Centipede_vtable;

    this->m_numLegs = 46;
    printf("Centipede::Ctor()\n");
}

void Centipede_cctor(Centipede *this, const Centipede *const other)
{
    Animal_cctor((Animal*)this, (Animal*)other);

    *(fp_t**)this = Centipede_vtable;

    this->m_numLegs = other->m_numLegs;
    printf("Centipede::CCtor()\n");
}

void Centipede_dtor(void *this)
{
    printf("Centipede::dtor()\n");

    *(fp_t**)this = Animal_vtable;

    Animal_dtor((Animal*)this);
}

void Centipede_Display(void *this)
{
    printf("Centipede::Display() ID:%d", GetID((Animal*)this));
    printf("  num legs:%d\n", ((Centipede *)this)->m_numLegs);
}

void Centipede_BuyShues(void *this)
{
    printf("Centipede::BuyShues() ID:%d\n", GetID((Animal*)this));
}


/********************************************************************** Squirl */
void Squirl_ctor(Squirl *this)
{
    Animal_ctor((Animal*)this);

    *(fp_t**)this = Squirl_vtable;

    printf("Squirl::Ctor()\n");
}

void Squirl_cctor(Squirl *this, const Squirl* const other)
{
    Animal_cctor((Animal*)this, (Animal*)other);

    *(fp_t**)this = Squirl_vtable;

    printf("Squirl::CCtor()\n");
}

void Squirl_dtor(void *this)
{
    printf("Squirl::dtor()\n");

    *(fp_t**)this = Animal_vtable;

    Animal_dtor((Animal*)this);
}

void Squirl_Display(void *this)
{
    printf("Squirl::Display() ID:%d\n", GetID((Animal*)this));
}

void SquirlDisplay(Squirl* s)
{
	(*(fp_t**)s)[DISPLAY](s);
}


/************************************************************** FlyingSquirl */
void Special_Squirl_ctor(FlyingSquirl *this)
{
    Squirl_ctor((Squirl*)this);

    *(fp_t**)this = Special_Squirl_vtable;

    printf("FlyingSquirl::Ctor()\n");
}

void Special_Squirl_cctor(FlyingSquirl *this, const FlyingSquirl* const other)
{
    Squirl_cctor((Squirl*)this, (Squirl*)other);

    *(fp_t**)this = Special_Squirl_vtable;

    printf("FlyingSquirl::CCtor()\n");
}

void Special_Squirl_dtor(void *this)
{
    printf("FlyingSquirl::dtor()\n");

    *(fp_t**)this = Squirl_vtable;

    Squirl_dtor((Squirl*)this);
}

void Special_Squirl_Display(void *this)
{
    printf("FlyingSquirl::Display()%d\n", GetID((Animal*)this));
}


/************************************************************* LegendaryAnimal */
void LegendaryAnimal_ctor(LegendaryAnimal *this)
{
    Animal_ctor((Animal*)this);

    *(fp_t**)this = LegendaryAnimal_vtable;

    this->m_pt1 = malloc(sizeof(Centipede));
    Centipede_ctor((Centipede*)this->m_pt1);

    this->m_pt2 = malloc(sizeof(Squirl));
    Squirl_ctor((Squirl*)this->m_pt2);

    Centipede_ctor(&this->m_m);
    Squirl_ctor(&this->m_t);
}

void LegendaryAnimal_cctor(LegendaryAnimal *this, const LegendaryAnimal* const other)
{
    Animal_cctor((Animal*)this, (Animal*)other);

    *(fp_t**)this = LegendaryAnimal_vtable;

    this->m_pt1 = malloc(sizeof(Centipede));
    Centipede_cctor((Centipede*)this->m_pt1, (Centipede*)other->m_pt1);

    this->m_pt2 = malloc(sizeof(Squirl));
    Squirl_cctor((Squirl*)this->m_pt2, (Squirl*)other->m_pt2);

    Centipede_cctor(&this->m_m, &other->m_m);
    Squirl_cctor(&this->m_t, &other->m_t);
}

void LegendaryAnimal_dtor(void *this)
{
    LegendaryAnimal* ptr = this;

    (*(fp_t**)((Centipede*)ptr->m_pt1))[DTOR](ptr->m_pt1);
    free(ptr->m_pt1);
    (*(fp_t**)((Squirl*)ptr->m_pt2))[DTOR](ptr->m_pt2);
    free(ptr->m_pt2);

    Squirl_dtor((Squirl*)&ptr->m_t);
    Centipede_dtor((Centipede*)&ptr->m_m);

    *(fp_t**)this = Animal_vtable;

    Animal_dtor((Animal*)this);
}

void LegendaryAnimal_Display(void *this)
{
    LegendaryAnimal* ptr = this;

    (*(fp_t**)((Centipede*)ptr->m_pt1))[DISPLAY](ptr->m_pt1);
    (*(fp_t**)((Squirl*)ptr->m_pt2))[DISPLAY](ptr->m_pt2);
    Centipede_Display(&ptr->m_m);
    SquirlDisplay(&ptr->m_t);
}


/********************************************************* Static Functions: */
void PrintInfo_Animal(Animal *a)
{
    (*(fp_t**)a)[DISPLAY](a);
}

void PrintInfo_int(Animal *tmp, int i)
{
	Centipede ret;

    (void)i;

    Centipede_ctor(&ret);

	printf("Foo(int i)\n");

    Centipede_Display(&ret);

    Animal_cctor(tmp, (Animal*)&ret);

    Centipede_dtor(&ret);
}

int Max(int a, int b)
{
    return ( ((a) > (b)) ? (a) : (b) );
}

/********************************************************************* Main: */
int main(int argc, char **argv, char **envp)
{
    Centipede m;
    Centipede m2;
    Animal Animal_tmp;
    Centipede Centipede_tmp;
    Squirl Squirl_tmp;
    Animal *array[3];
    Animal arr2[3];
    Centipede arr3[4];
    Squirl *arr4;
    int i = 0;
    FlyingSquirl st;
    LegendaryAnimal *ts1 = 0;
    LegendaryAnimal *ts2 = 0;

    (void)argc;
    (void)argv;
    (void)envp;

    Centipede_ctor(&m);

    PrintInfo_Animal((Animal*)&m);

    PrintInfo_int(&Animal_tmp, 3);
    Animal_Display(&Animal_tmp);
    Animal_dtor(&Animal_tmp);


    array[0] = malloc(sizeof(Centipede));
    Centipede_ctor((Centipede*)array[0]);
    array[1] = malloc(sizeof(Squirl));
    Squirl_ctor((Squirl*)array[1]);
    array[2] = malloc(sizeof(Centipede));
    Centipede_ctor((Centipede*)array[2]);


    for (i = 0; i < 3; ++i)
    {
        (*(fp_t**)array[i])[DISPLAY](array[i]);
    }
    for (i = 0; i < 3; ++i)
    {
        (*(fp_t**)array[i])[DTOR](array[i]);
        free(array[i]);
    }

    Centipede_ctor(&Centipede_tmp);
    Animal_cctor((Animal*)&arr2[0], (Animal*)&Centipede_tmp);
    Centipede_dtor(&Centipede_tmp);

    Squirl_ctor(&Squirl_tmp);
    Animal_cctor((Animal*)&arr2[1], (Animal*)&Squirl_tmp);
    Squirl_dtor(&Squirl_tmp);

    Animal_ctor(&arr2[2]);

    for (i = 0; i < 3; ++i)
    {
        (*(fp_t**)(&arr2[i]))[DISPLAY](&arr2[i]);
    }

    Animal_PrintCount();
    Centipede_ctor(&m2);
    Animal_PrintCount();

    Centipede_ctor(&arr3[0]);
    Centipede_ctor(&arr3[1]);
    Centipede_ctor(&arr3[2]);
    Centipede_ctor(&arr3[3]);

    arr4 = malloc(sizeof(Squirl) * 4);
    for (i = 0; i < 4; ++i)
    {
        Squirl_ctor(arr4 + i);
    }
    for (i = 3; i != -1; --i)
    {
        (*(fp_t**)(arr4 + i))[DTOR](arr4 + i);
    }
    free(arr4);

    printf("%d\n", Max(1, 2));
    Special_Squirl_ctor(&st);

    Squirl_cctor(&Squirl_tmp, (Squirl*)&st);
    SquirlDisplay(&Squirl_tmp);
    Squirl_dtor(&Squirl_tmp);

    ts1 = malloc(sizeof(LegendaryAnimal));
    LegendaryAnimal_ctor(ts1);
    ts2 = malloc(sizeof(LegendaryAnimal));
    LegendaryAnimal_cctor(ts2, ts1);

    (*(fp_t**)ts1)[DISPLAY](ts1);
    (*(fp_t**)ts2)[DISPLAY](ts2);

    (*(fp_t**)ts1)[DTOR](ts1);
    free(ts1);

    (*(fp_t**)ts2)[DISPLAY](ts2);
    (*(fp_t**)ts2)[DTOR](ts2);
    free(ts2);

    Special_Squirl_dtor(&st);
    for (i = 3 ; i != -1 ; --i)
    {
        (*(fp_t**)(&arr3[i]))[DTOR](&arr3[i]);
    }

    Centipede_dtor(&m2);

    Animal_dtor(&arr2[2]);
    Animal_dtor(&arr2[1]);
    Animal_dtor(&arr2[0]);

    Centipede_dtor(&m);

    return 0;
}
