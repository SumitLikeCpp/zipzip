/*zipzip.h*/

#pragma once
#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

union u_entry;
 typedef union u_entry entry;
enum e_sortres;
 typedef enum e_sortres sortres;
union u_list;
 typedef union u_list List;

#define Blocksize       0xffff
#define Maxcap          ((0xffffffff/8)-Blocksize-1)

typedef unsigned char int8;
typedef unsigned short int int16;
typedef unsigned int int32;
typedef unsigned long long int int64;

typedef sortres (*sortfunc)(entry,entry);

#define $c (char *)
#define $i (int)
#define $v (void *)
#define $1 (int8 *)
#define $2 (int16)
#define $4 (int32)
#define $8 (int64)

#define alloc(x)        malloc($i (x))
#define destroy(x)      free((x))
#define ralloc(x,y)     realloc((x),$i (y))
#define show(x)     _Generic((x), \
    amtentry:       showamtentry, \
    amtlist *:      showamtlist) \
    ($1 (#x), (x))

#define swap(x,y)     _Generic((x), \
    amtentry*:       amtswap) \
    ((x), (y))

struct s_amtentry{
    int32 block;
    int32 amt;
};

typedef struct s_amtentry amtentry;

struct s_amtlist{
    int32 capacity;
    int32 length;
    amtentry data[];
};

typedef struct s_amtlist amtlist;

union u_entry{
    amtentry amt;
};

union u_list{
    amtlist *amt;
};

enum e_sortres{
    MoreThan,
    Equal,
    LessThan
};

// constructors
amtlist *mkamtlist(void);

void showamtlist(int8*,amtlist*);
void showamtentry(int8 *, amtentry);

bool amtadd(amtlist**,amtentry);
bool increase(amtlist**);
void amtswap(amtentry*,amtentry*);
bool zipsort(List*,int32,sortfunc);

void zero(int8*,int32);
int main(void);