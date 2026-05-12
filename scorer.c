#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct
{
    int id;
    char inspector[64];
    double latitude;
    double longitude;
    char category[32];
    int severity;
    time_t timestamp;
    char description[128];

} Report;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return 1;
    }

    char path[256];

    sprintf(path, "%s/reports.dat", argv[1]);

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        printf("Could not open district\n");
        return 1;
    }

    Report report;

    int total = 0;

    while (fread(&report, sizeof(Report), 1, file) == 1)
    {
        total += report.severity;
    }

    fclose(file);

    printf("District: %s\n", argv[1]);
    printf("Total workload score: %d\n", total);

    return 0;
}