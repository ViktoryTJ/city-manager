#include "operation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define REPORT_PERMISSION 0664

// -------------------- PERMISSION PRINT --------------------

void print_permissions(mode_t mode)
{
    char p[10];

    p[0] = (mode & S_IRUSR) ? 'r' : '-';
    p[1] = (mode & S_IWUSR) ? 'w' : '-';
    p[2] = (mode & S_IXUSR) ? 'x' : '-';

    p[3] = (mode & S_IRGRP) ? 'r' : '-';
    p[4] = (mode & S_IWGRP) ? 'w' : '-';
    p[5] = (mode & S_IXGRP) ? 'x' : '-';

    p[6] = (mode & S_IROTH) ? 'r' : '-';
    p[7] = (mode & S_IWOTH) ? 'w' : '-';
    p[8] = (mode & S_IXOTH) ? 'x' : '-';

    p[9] = '\0';

    printf("Permissions: %s\n", p);
}

// -------------------- INIT --------------------

void district_init(const char *district)
{
    struct stat st;

    if (stat(district, &st) != 0)
    {
#ifdef _WIN32
        if (mkdir(district) != 0)
#else
        if (mkdir(district, 0750) != 0)
#endif
        {
            perror("mkdir failed");
            exit(1);
        }
    }

    char path[256];
    int fd;

    // -------- reports.dat --------
    snprintf(path, sizeof(path), "%s/reports.dat", district);

    fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0664);
    close(fd);
    chmod(path, 0664);

    // -------- SYMLINK --------
    char linkname[256];
    snprintf(linkname, sizeof(linkname), "active_reports-%s", district);

    unlink(linkname); // remove old link

#ifndef _WIN32
    if (symlink(path, linkname) != 0)
    {
        perror("symlink failed");
    }
#endif

    // -------- district.cfg --------
    snprintf(path, sizeof(path), "%s/district.cfg", district);

    fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0640);
    write(fd, "threshold=1\n", 12);
    close(fd);
    chmod(path, 0640);

    // -------- log file --------
    snprintf(path, sizeof(path), "%s/logged_district", district);

    fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    close(fd);
    chmod(path, 0644);

    printf("District initialized: %s\n", district);
}

// -------------------- LOG --------------------

void log_action(const char *district, const char *role, const char *user, const char *action)
{
    char path[256];
    snprintf(path, sizeof(path), "%s/logged_district", district);

    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd < 0) return;

    char buffer[256];
    time_t now = time(NULL);

    snprintf(buffer, sizeof(buffer), "%ld | %s | %s | %s\n",
             now, role, user, action);

    write(fd, buffer, strlen(buffer));
    close(fd);
}

// -------------------- ADD REPORT --------------------

void add_report(const char *district, const char *role, const char *user)
{
    if (strcmp(role, "inspector") != 0)
    {
        printf("Permission denied\n");
        return;
    }

    char path[256];
    snprintf(path, sizeof(path), "%s/reports.dat", district);

    int fd = open(path, O_WRONLY | O_APPEND);
    if (fd < 0)
    {
        perror("open failed");
        return;
    }

    Report r;
    memset(&r, 0, sizeof(Report));

    r.id = rand() % 100000;
    strcpy(r.inspector, user);

    printf("Latitude: ");
    scanf("%lf", &r.latitude);

    printf("Longitude: ");
    scanf("%lf", &r.longitude);

    printf("Category: ");
    scanf(" %31s", r.category);

    printf("Severity: ");
    scanf("%d", &r.severity);

    printf("Description: ");
    scanf(" %127[^\n]", r.description);

    r.timestamp = time(NULL);

    write(fd, &r, sizeof(Report));
    close(fd);

    printf("Report added!\n");

    log_action(district, role, user, "add_report");
}

// -------------------- LIST --------------------

void list_reports(const char *district, const char *role, const char *user)
{
    char path[256];
    snprintf(path, sizeof(path), "%s/reports.dat", district);

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open failed");
        return;
    }

    struct stat st;

    if (stat(path, &st) == 0)
    {
        print_permissions(st.st_mode);
        printf("File size: %ld bytes\n", st.st_size);
        printf("Last modified: %ld\n", st.st_mtime);
    }

    printf("Reports in %s:\n", district);

    Report r;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report))
    {
        printf("ID: %d | Sev: %d | Cat: %s | Desc: %s\n",
               r.id, r.severity, r.category, r.description);
    }

    close(fd);

    log_action(district, role, user, "list");
}

// -------------------- VIEW --------------------

void view_report(const char *district, int id, const char *role, const char *user)
{
    char path[256];
    snprintf(path, sizeof(path), "%s/reports.dat", district);

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open failed");
        return;
    }

    Report r;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report))
    {
        if (r.id == id)
        {
            printf("ID: %d\nInspector: %s\nLat: %.2lf\nLon: %.2lf\nCat: %s\nSev: %d\nDesc: %s\n",
                   r.id, r.inspector, r.latitude, r.longitude,
                   r.category, r.severity, r.description);
            close(fd);
            return;
        }
    }

    printf("Report not found\n");
    close(fd);
}

// -------------------- REMOVE --------------------

void remove_report(const char *district, int id, const char *role, const char *user)
{
    if (strcmp(role, "manager") != 0)
    {
        printf("Permission denied\n");
        return;
    }

    char path[256], temp[256];
    snprintf(path, sizeof(path), "%s/reports.dat", district);
    snprintf(temp, sizeof(temp), "%s/temp.dat", district);

    struct stat st;

    if (stat(path, &st) == 0)
    {
        if (!(st.st_mode & S_IWUSR))
        {
            printf("No write permission\n");
            return;
        }
    }

    int fd = open(path, O_RDONLY);
    int fd2 = open(temp, O_CREAT | O_WRONLY | O_TRUNC, REPORT_PERMISSION);

    if (fd < 0 || fd2 < 0)
    {
        perror("file error");
        return;
    }

    Report r;
    int found = 0;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report))
    {
        if (r.id == id)
        {
            found = 1;
            continue;
        }
        write(fd2, &r, sizeof(Report));
    }

    close(fd);
    close(fd2);

    remove(path);
    rename(temp, path);

    if (found)
        printf("Report removed\n");
    else
        printf("Not found\n");

    log_action(district, role, user, "remove_report");
}

void remove_district(const char *district, const char *role, const char *user)
{
    if (strcmp(role, "manager") != 0)
    {
        printf("Permission denied\n");
        return;
    }
    pid_t pid = fork();
    if (pid <0)
    {
        perror("fork failed");
        return;
    }
    if (pid == 0)
    {
        execlp("rm" , "rm", "-rf" , district,NULL);
        perror("exec failed");
        exit(1);
    }
    else {
        wait(NULL);
        char linkname[128];
        snprintf(linkname, sizeof(linkname), "active_reports-%s", district);
        unlink(linkname);
        printf("District removed: %s\n", district );
    }

}

// -------------------- UPDATE THRESHOLD --------------------

void update_threshold(const char *district, int value, const char *role, const char *user)
{
    if (strcmp(role, "manager") != 0)
    {
        printf("Permission denied\n");
        return;
    }

    char path[256];
    snprintf(path, sizeof(path), "%s/district.cfg", district);

    struct stat st;

    if (stat(path, &st) == 0)
    {
        if (!(st.st_mode & S_IWUSR))
        {
            printf("No write permission\n");
            return;
        }
    }

    int fd = open(path, O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        perror("open failed");
        return;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "threshold=%d\n", value);
    write(fd, buffer, strlen(buffer));

    close(fd);

    printf("Threshold updated\n");

    log_action(district, role, user, "update_threshold");
}

// -------------------- FILTER --------------------

void filter_reports(const char *district, const char *role, const char *user, int nconds, char **conds)
{
    char path[256];
    snprintf(path, sizeof(path), "%s/reports.dat", district);

    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open failed");
        return;
    }

    Report r;

    while (read(fd, &r, sizeof(Report)) == sizeof(Report))
    {
        int ok = 1;

        for (int i = 0; i < nconds; i++)
        {
            if (strncmp(conds[i], "severity>=", 10) == 0)
            {
                int val = atoi(conds[i] + 10);
                if (r.severity < val) ok = 0;
            }
            else if (strncmp(conds[i], "category==", 10) == 0)
            {
                if (strcmp(r.category, conds[i] + 10) != 0) ok = 0;
            }
        }

        if (ok)
        {
            printf("ID: %d | Sev: %d | Cat: %s | Desc: %s\n",
                   r.id, r.severity, r.category, r.description);
        }
    }

    close(fd);

    log_action(district, role, user, "filter");
}