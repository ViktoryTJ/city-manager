#include "operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DISTRICT_PERMISSION 0750
#define REPORT_PERMISSION   0664
#define CFG_PERMISSION      0640
#define LOG_PERMISSION      0644

void district_init(const char *district)
{
    struct stat st;

    // create directory if it doesn't exist
    if (stat(district, &st) != 0)
    {
#ifdef _WIN32
        if (mkdir(district) != 0)
#else
        if (mkdir(district, DISTRICT_PERMISSION) != 0)
#endif
        {
            perror("mkdir failed");
            exit(1);
        }
    }

    char path[512];
    int fd;

    // reports.dat
    snprintf(path, sizeof(path), "%s/reports.dat", district);
    fd = open(path, O_CREAT | O_WRONLY, REPORT_PERMISSION);
    if (fd < 0)
    {
        perror("reports.dat failed");
        exit(1);
    }
    close(fd);

    // district.cfg
    snprintf(path, sizeof(path), "%s/district.cfg", district);
    fd = open(path, O_CREAT | O_WRONLY, CFG_PERMISSION);
    if (fd < 0)
    {
        perror("district.cfg failed");
        exit(1);
    }

    write(fd, "threshold=1\n", 12);
    close(fd);

    // logged_district
    snprintf(path, sizeof(path), "%s/logged_district", district);
    fd = open(path, O_CREAT | O_WRONLY, LOG_PERMISSION);
    if (fd < 0)
    {
        perror("logged_district failed");
        exit(1);
    }
    close(fd);

    printf("District initialized: %s\n", district);
}

void list_reports(const char *district) {
    char path[256];
    snprintf(path, sizeof(path), "%s/reports.dat", district);

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror("reports.dat failed");
        return;
    }
    char buffer[256];
    int bytes;

    while ((bytes = read(fd, buffer, 256)) > 0) {
        write(STDOUT_FILENO, buffer, bytes);
    }
    close(fd);

}