#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "operation.h"




int main(int argc, char *argv[])
{
    srand(time(NULL));
    char role[20] = "";
    char user[64] = "";
    char command[32] = "";
    char district[64] = "";

    int id = 0;
    int value = 0;

    char *conds[10];
    int nconds = 0;

    // -------------------- PARSE --------------------
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--role") == 0 && i + 1 < argc)
        {
            strcpy(role, argv[++i]);
        }
        else if (strcmp(argv[i], "--user") == 0 && i + 1 < argc)
        {
            strcpy(user, argv[++i]);
        }
        else if (strcmp(argv[i], "--add") == 0 && i + 1 < argc)
        {
            strcpy(command, "add");
            strcpy(district, argv[++i]);
        }
        else if (strcmp(argv[i], "--list") == 0 && i + 1 < argc)
        {
            strcpy(command, "list");
            strcpy(district, argv[++i]);
        }
        else if (strcmp(argv[i], "--view") == 0 && i + 2 < argc)
        {
            strcpy(command, "view");
            strcpy(district, argv[++i]);
            id = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--add_report") == 0 && i + 1 < argc)
        {
            strcpy(command, "add_report");
            strcpy(district, argv[++i]);
        }
        else if (strcmp(argv[i], "--remove_report") == 0 && i + 2 < argc)
        {
            strcpy(command, "remove_report");
            strcpy(district, argv[++i]);
            id = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--update_threshold") == 0 && i + 2 < argc)
        {
            strcpy(command, "update_threshold");
            strcpy(district, argv[++i]);
            value = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--filter") == 0 && i + 1 < argc)
        {
            strcpy(command, "filter");
            strcpy(district, argv[++i]);

            // rest are conditions
            while (i + 1 < argc)
            {
                conds[nconds++] = argv[++i];
            }
        }
        else if (strcmp(argv[i], "--remove_district") == 0 && i + 1 < argc)
        {
            strcpy(command, "remove_district");
            strcpy(district, argv[++i]);
        }
    }

    // -------------------- BASIC CHECK --------------------
    if (strlen(role) == 0 || strlen(user) == 0)
    {
        printf("Missing --role or --user\n");
        return 0;
    }

    // -------------------- EXECUTE --------------------
    if (strcmp(command, "add") == 0)
    {
        // reuse your district_init if you have it
        district_init(district);
    }
    else if (strcmp(command, "list") == 0)
    {
        list_reports(district, role, user);
    }
    else if (strcmp(command, "view") == 0)
    {
        view_report(district, id, role, user);
    }
    else if (strcmp(command, "add_report") == 0)
    {
        add_report(district, role, user);
    }
    else if (strcmp(command, "remove_report") == 0)
    {
        remove_report(district, id, role, user);
    }
    else if (strcmp(command, "update_threshold") == 0)
    {
        update_threshold(district, value, role, user);
    }
    else if (strcmp(command, "filter") == 0)
    {
        filter_reports(district, role, user, nconds, conds);
    }
    else if (strcmp(command, "remove_district") == 0)
    {
        remove_district(district,role,user);
    }
    else
    {
        printf("Invalid command\n");
    }

    return 0;
}