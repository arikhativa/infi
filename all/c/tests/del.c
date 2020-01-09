
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void PrintTabs(size_t tabs)
{
    while (tabs)
    {
        printf("\t");
        --tabs;
    }
}

void RecTree(size_t tabs, char *file_name)
{
    DIR* hundel = NULL;
    struct dirent *entry = NULL;

    hundel = opendir(file_name);

    // printf("(%s)\n", file_name);
    // printf("(%p)\n", hundel);
    // printf("(%s)\n", entry->d_name);

    if (hundel != NULL)
    {
        entry = readdir(hundel);
    }

    while (entry != NULL)
    {
        // if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            PrintTabs(tabs);
            printf("%s\n", entry->d_name);

            // if (DT_DIR == entry->d_type)
            // {
            //     RecTree(tabs + 1, entry->d_name);
            // }
        }

        entry = readdir(hundel);
    }
    printf("\n");

    if (hundel != NULL)
    {
        closedir(hundel);
    }

}

int main()
{
    // char tabs[] = "\t - ";
    RecTree(1, ".");

    return 0;
}
