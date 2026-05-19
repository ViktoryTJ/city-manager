#include <stdio.h>
#include <stdlib.h>
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
        printf("Usage: ./scorer <district>\n");
        return 1;
    }

    char path[256];
    snprintf(path, sizeof(path), "%s/reports.dat", argv[1]);

    FILE *file = fopen(path, "rb");
    if (file == NULL)
    {
        printf("Could not open district: %s\n", argv[1]);
        return 1;
    }

    Report report;
    int total_score = 0;
    int report_count = 0;
    int max_severity = 0;

    while (fread(&report, sizeof(Report), 1, file) == 1)
    {
        total_score += report.severity;
        report_count++;

        if (report.severity > max_severity)
        {
            max_severity = report.severity;
        }
    }

    fclose(file);

    printf("District: %s\n", argv[1]);
    printf("Number of reports: %d\n", report_count);
    printf("Total workload score: %d\n", total_score);
    printf("Highest severity: %d\n", max_severity);

    return 0;
}