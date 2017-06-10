#ifndef GOLAY_H_INCLUDED
#define GOLAY_H_INCLUDED
//*****************************************************************************
//
//*****************************************************************************
//
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "inttypes.h"
#include "golay_view.h"

//
//*****************************************************************************
// Define GOLAY DATA
#define X22             0x00400000   /* vector representation of X^{22} */
#define X11             0x00000800   /* vector representation of X^{11} */
#define MASK12          0xfffff800   /* auxiliary vector for testing */
#define GENPOL          0x00000c75   /* generator polinomial, g(x) */

//
//*****************************************************************************
//
#ifdef DEBUG
//
//*****************************************************************************
//
int *a;
int  n, r;
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif
//
//*****************************************************************************
//
// GOLAY Code: funktions and declarations
long pattern;
long decoding_table[2048];
long position[23] = { 0x00000001, 0x00000002, 0x00000004, 0x00000008,
                      0x00000010, 0x00000020, 0x00000040, 0x00000080,
                      0x00000100, 0x00000200, 0x00000400, 0x00000800,
                      0x00001000, 0x00002000, 0x00004000, 0x00008000,
                      0x00010000, 0x00020000, 0x00040000, 0x00080000,
                      0x00100000, 0x00200000, 0x00400000 };
int a[4];										
//
//*****************************************************************************
//

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

unsigned char ts2[128] = {0x4d, 0x46, 0x77, 0x77, 0x44, 0x51, 0x59, 0x4a, 0x4b, 0x6f, 0x5a, 0x49, 0x68, 
													0x76, 0x63, 0x4e, 0x41, 0x51, 0x45, 0x42, 0x42, 0x51, 0x41, 0x44, 0x53, 0x77, 
													0x41, 0x77, 0x53, 0x41, 0x4a, 0x42, 0x41, 0x4b, 0x44, 0x78, 0x64, 0x74, 0x6d, 
													0x6f, 0x73, 0x72, 0x4e, 0x63, 0x6c, 0x4c, 0x4f, 0x6b, 0x57, 0x72, 0x36, 0x74,
													0x4b, 0x57, 0x56, 0x48, 0x67, 0x48, 0x69, 0x4e, 0x73, 0x48, 0x72, 0x2f, 0x49, 
													0x47, 0x54, 0x68, 0x77, 0x4e, 0x4b, 0x79, 0x42, 0x43, 0x78, 0x33, 0x72, 0x67, 
													0x32, 0x61, 0x39, 0x73, 0x36, 0x46, 0x64, 0x38, 0x74, 0x61, 0x4a, 0x2b, 0x58, 
													0x52, 0x42, 0x68, 0x63, 0x4e, 0x6f, 0x49, 0x42, 0x38, 0x43, 0x52, 0x67, 0x66, 
													0x2f, 0x62, 0x72, 0x44, 0x6b, 0x44, 0x65, 0x76, 0x4d, 0x53, 0x5a, 0x39, 0x79, 
													0x72, 0x73, 0x43, 0x41, 0x77, 0x45, 0x41, 0x41, 0x51, 0x3d, 0x3d};
													
// RSA private Key (valid)													
unsigned char ts[428] = { 0x4d, 0x49, 0x49, 0x42, 0x4f, 0x77, 0x49, 0x42, 0x41, 0x41, 0x4a, 0x42, 0x41, 0x4b, 0x44, 
													0x78, 0x64, 0x74, 0x6d, 0x6f, 0x73, 0x72, 0x4e, 0x63, 0x6c, 0x4c, 0x4f, 0x6b, 0x57, 0x72, 
													0x36, 0x74, 0x4b, 0x57, 0x56, 0x48, 0x67, 0x48, 0x69, 0x4e, 0x73, 0x48, 0x72, 0x2f, 0x49, 
													0x47, 0x54, 0x68, 0x77, 0x4e, 0x4b, 0x79, 0x42, 0x43, 0x78, 0x33, 0x72, 0x67, 0x32, 0x61, 
													0x39, 0x73, 0x36, 0x46, 0x64, 0x38, 0x74, 0x61, 0x4a, 0x2b, 0x58, 0x52, 0x42, 0x68, 0x63, 
													0x4e, 0x6f, 0x49, 0x42, 0x38, 0x43, 0x52, 0x67, 0x66, 0x2f, 0x62, 0x72, 0x44, 0x6b, 0x44, 
													0x65, 0x76, 0x4d, 0x53, 0x5a, 0x39, 0x79, 0x72, 0x73, 0x43, 0x41, 0x77, 0x45, 0x41, 0x41, 
													0x51, 0x4a, 0x41, 0x61, 0x50, 0x4d, 0x4d, 0x66, 0x59, 0x4c, 0x30, 0x68, 0x4f, 0x47, 0x71,
													0x56, 0x6a, 0x71, 0x6d, 0x73, 0x68, 0x42, 0x5a, 0x48, 0x69, 0x49, 0x79, 0x4e, 0x47, 0x71, 
													0x77, 0x32, 0x6f, 0x50, 0x6a, 0x71, 0x2b, 0x56, 0x4b, 0x79, 0x42, 0x6f, 0x74, 0x66, 0x70, 
													0x44, 0x43, 0x52, 0x79, 0x42, 0x47, 0x79, 0x67, 0x42, 0x36, 0x33, 0x6b, 0x52, 0x34, 0x75, 
													0x43, 0x5a, 0x6b, 0x70, 0x50, 0x6c, 0x77, 0x44, 0x70, 0x34, 0x45, 0x2f, 0x50, 0x54, 0x71, 
													0x62, 0x4f, 0x50, 0x6a, 0x70, 0x53, 0x57, 0x49, 0x30, 0x76, 0x6d, 0x78, 0x41, 0x51, 0x49, 
													0x68, 0x41, 0x4f, 0x47, 0x79, 0x6f, 0x33, 0x53, 0x66, 0x4a, 0x4e, 0x55, 0x4c, 0x63, 0x44, 
													0x4b, 0x62, 0x48, 0x6d, 0x45, 0x4c, 0x44, 0x46, 0x4e, 0x57, 0x44, 0x68, 0x64, 0x67, 0x32, 
													0x39, 0x74, 0x6a, 0x51, 0x67, 0x32, 0x56, 0x35, 0x75, 0x46, 0x32, 0x4e, 0x62, 0x6d, 0x54, 
													0x41, 0x69, 0x45, 0x41, 0x74, 0x6f, 0x30, 0x73, 0x68, 0x64, 0x79, 0x47, 0x52, 0x66, 0x72, 
													0x63, 0x73, 0x4a, 0x64, 0x35, 0x63, 0x65, 0x54, 0x51, 0x39, 0x7a, 0x32, 0x30, 0x70, 0x39, 
													0x6d, 0x64, 0x61, 0x72, 0x47, 0x44, 0x68, 0x52, 0x48, 0x64, 0x67, 0x55, 0x6a, 0x79, 0x51, 
													0x7a, 0x6b, 0x43, 0x49, 0x51, 0x43, 0x50, 0x2b, 0x72, 0x58, 0x72, 0x6e, 0x39, 0x71, 0x50, 
													0x54, 0x6b, 0x79, 0x30, 0x2b, 0x39, 0x2f, 0x56, 0x61, 0x49, 0x62, 0x6b, 0x59, 0x76, 0x46, 
													0x36, 0x57, 0x48, 0x2f, 0x6f, 0x35, 0x4e, 0x6b, 0x75, 0x63, 0x59, 0x4a, 0x6f, 0x4d, 0x6c, 
													0x44, 0x7a, 0x6f, 0x51, 0x49, 0x68, 0x41, 0x49, 0x75, 0x44, 0x58, 0x50, 0x46, 0x34, 0x31, 
													0x63, 0x32, 0x4a, 0x71, 0x54, 0x76, 0x30, 0x70, 0x38, 0x58, 0x5a, 0x58, 0x37, 0x4a, 0x30, 
													0x6f, 0x43, 0x70, 0x66, 0x62, 0x36, 0x54, 0x6f, 0x77, 0x57, 0x44, 0x6a, 0x68, 0x47, 0x75, 
													0x58, 0x2b, 0x6a, 0x35, 0x52, 0x41, 0x69, 0x42, 0x4d, 0x36, 0x44, 0x76, 0x34, 0x63, 0x6b, 
													0x65, 0x44, 0x54, 0x2f, 0x54, 0x7a, 0x59, 0x2b, 0x44, 0x48, 0x5a, 0x42, 0x76, 0x68, 0x63, 
													0x66, 0x38, 0x46, 0x74, 0x70, 0x46, 0x34, 0x41, 0x35, 0x5a, 0x2f, 0x44, 0x42, 0x45, 0x39, 
													0x67, 0x74, 0x62, 0x6d, 0x75, 0x41, 0x3d, 0x3d};
#endif
