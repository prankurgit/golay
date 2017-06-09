#ifndef GOLAY_H_INCLUDED
#define GOLAY_H_INCLUDED

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "inttypes.h"
#include <stdbool.h>
#include "golay_view.h"
using namespace std;

//
//*****************************************************************************
// Define GOLAY DATA
#define X22             0x00400000   /* vector representation of X^{22} */
#define X11             0x00000800   /* vector representation of X^{11} */
#define MASK12          0xfffff800   /* auxiliary vector for testing */
#define GENPOL          0x00000c75   /* generator polinomial, g(x) */

//
//*****************************************************************************
// GOLAY Code: funktions and declarations
long pattern;
long encoding_table[4096];
long position[23] = { 0x00000001, 0x00000002, 0x00000004, 0x00000008,
                      0x00000010, 0x00000020, 0x00000040, 0x00000080,
                      0x00000100, 0x00000200, 0x00000400, 0x00000800,
                      0x00001000, 0x00002000, 0x00004000, 0x00008000,
                      0x00010000, 0x00020000, 0x00040000, 0x00080000,
                      0x00100000, 0x00200000, 0x00400000 };
int a[4];
//*****************************************************************************

struct Item {
    unsigned long offset_begin;
    unsigned long offset_end;
    unsigned long input_length;
    char input_file_name[102];
    char output_file_name[102];
    unsigned long zeros;
    unsigned long ones;
    double entrp;
    double median;
    double average;
    char result[52];
    unsigned int HD_error_pos;
    unsigned int HD_mode;
    bool HW_ENTP_mode;
    //bch data members
    long offSet;
    long input_Key_length;
    char input_Key_name[102];
    char input_PUF_name[102];
    char output_HD_name[102];
    char input_HD_name[102];
    char output_Key_name[102];
    char BCHmode[25];
    int LR;
} ;

//add this info to the helper file for golay decoder
typedef struct{
    int errorCode;
    int Golay_BCH_length;
    int Golay_d_BCH_t;
    int Golay_k_BCH_m;
    int linearRep;
    long puf_offSet_begin;
    long puf_offSet_end;
    long original_filesize;
} Pattern;

void nextcomb(int n, int r, int *a);

#endif
