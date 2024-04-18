#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// BrainVoyager file formart according to the documentation https://support.brainvoyager.com/brainvoyager/automation-development/84-file-formats

// VMR structure
    // Specifications: 
    //VMR Pre-Data Header
    //BYTES	DATA TYPE	DEFAULT	DESCRIPTION
    //2	unsigned short	4	File version
    //2	unsigned short	256	DimX, dimension of X axis
    //2	unsigned short	256	DimY, dimension of Y axis
    //2	unsigned short	256	DimZ, dimension of Z axis

    //VMR Data
    //Each data element (intensity value) is represented in 1 byte. The data is organized in three loops:
    //DimZ
    //DimY
    //DimX
    //Note that the axes terminology follows the internal BrainVoyager (BV) format. The mapping to Talairach axes is as follows:
    //BV X front -> back = Y in Tal space
    //BV Y top -> bottom = Z in Tal space
    //BV Z left -> right = X in Tal space

    // This file format will not included the post header which is not necessary for the purpose of this project


struct VMR{
    unsigned short version;
    unsigned short DimX;
    unsigned short DimY;
    unsigned short DimZ;
    unsigned char* data;
};

// v16 is basically the same as VMR but with float data 
struct V16{
    unsigned short version;
    unsigned short DimX;
    unsigned short DimY;
    unsigned short DimZ;
    float* data;
};

typedef struct VMR VMR;
typedef struct V16 V16;


// prototypes
VMR read_vmr(const char* filename); 
V16 read_v16(const char* filename);