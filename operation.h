#ifndef OPERATION_H
#define OPERATION_H

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
void district_init(const char *district);
void add_report(const char *district, const char *role, const char *user);
void list_reports(const char *district, const char *role, const char *user);
void view_report(const char *district, int id, const char *role, const char *user);
void remove_report(const char *district, int id, const char *role, const char *user);
void update_threshold(const char *district, int value, const char *role, const char *user);
void filter_reports(const char *district, const char *role, const char *user, int nconds, char **conds);
void remove_district(const char *district,const char *role, const char *user);

#endif