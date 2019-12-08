
#include <stdio.h>

typedef struct Person
{
    int a[100];
    char c;
    int age;
    int b;
} Person_t;

typedef struct List
{
    int a[100];
    char c;
    int age;
    int b;
    Person_t PP[2];
} List_t;

Person_t *ll_find(List_t *l, int key)
{
    return &l->PP[key];
}

void Foo(List_t *ll, int key, int age)
{
    void *vv = ll_find(ll, key);

    Person_t *p = static_cast<Person_t*>(vv);
    p->age = age;
}

int main()
{
    List_t list;

    list.PP[0].age = 5;
    list.PP[1].age = 10;

    Foo(&list, 0, 11);

    printf("-- p1 %d\n", list.PP[0].age);
    printf("-- p1 %d\n", list.PP[1].age);




}
