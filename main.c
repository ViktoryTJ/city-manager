#include <stdio.h>
#include <string.h>

#include "operation.h"

int main(int argc, char *argv[])
{
    char command[20] = "";
    char district[64] = "";

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--add") == 0 && i + 1 < argc)
        {
            strcpy(command, "add");
            strcpy(district, argv[++i]);
        }
        else if (strcmp(argv[i], "--list") == 0 && i + 1 < argc) {
            strcpy(command, "list");
            strcpy(district, argv[++i]);
        }
    }

    if (strcmp(command, "add") == 0)
    {
        district_init(district);
    }
    else if (strcmp(command, "list") == 0) {
        list_reports(district);
    }
    else
    {
        printf("Invalid command\n");
    }

    return 0;
}