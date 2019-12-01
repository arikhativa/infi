#include <stdio.h>
#include <unistd.h>

int g_g = 2;

int main(int argc, char **argv, char **envp)
{
  int i = 8;
  int j = 16;
  int k = 32;
  int *p = &j;
  int a = 1;
  int b = 2;
  int c = 3;
  int d = 4;
  int e = 5;
  int f = 6;
  int l = 9;
  int h = 9;
  int m = 10;
  int n = 11;

  FILE *fp;

  if(0 == (fp = fopen("stoopid", "r")))
    {
      printf("well, that didn\'t work!\n");
      return -1;
    }
  
  fscanf(fp, "%p\n", &p);

  if(fclose(fp))
    {
      printf("oh well.");
      return -1;
    }
  
  printf("p points to: %d\n", *p);
  printf(" %d\n", a);
  printf(" %d\n", b);
  printf(" %d\n", c);
  printf(" %d\n", d);
  printf(" %d\n", e);
  printf(" %d\n", f);
  printf(" %d\n", h);
  printf(" %d\n", l);
  printf(" %d\n", m);
  printf(" %d\n", n);

  return 0;
}
