#ifndef USER_H
#define USER_H

typedef enum
{
    INSPECTOR,
    MANAGER
} userType;

typedef struct
{
    userType role;
    char name[64];
} user_t;

#endif