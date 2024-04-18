#include "bv_io.h"

// Read function for VMR files
VMR read_vmr(const char* filename){
    VMR vmr;
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        printf("Error: could not open file %s\n", filename);
        exit(1);
    }
    fread(&vmr.version, sizeof(unsigned short), 1, file);
    fread(&vmr.DimX, sizeof(unsigned short), 1, file);
    fread(&vmr.DimY, sizeof(unsigned short), 1, file);
    fread(&vmr.DimZ, sizeof(unsigned short), 1, file);
    vmr.data = (unsigned char*)malloc(vmr.DimX * vmr.DimY * vmr.DimZ * sizeof(unsigned char));
    fread(vmr.data, sizeof(unsigned char), vmr.DimX * vmr.DimY * vmr.DimZ, file);
    fclose(file);
    return vmr;
}

// Read function for V16 files
V16 read_v16(const char* filename){
    V16 v16;
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        printf("Error: could not open file %s\n", filename);
        exit(1);
    }
    fread(&v16.version, sizeof(unsigned short), 1, file);
    fread(&v16.DimX, sizeof(unsigned short), 1, file);
    fread(&v16.DimY, sizeof(unsigned short), 1, file);
    fread(&v16.DimZ, sizeof(unsigned short), 1, file);
    v16.data = (float*)malloc(v16.DimX * v16.DimY * v16.DimZ * sizeof(float));
    fread(v16.data, sizeof(float), v16.DimX * v16.DimY * v16.DimZ, file);
    fclose(file);
    return v16;
}

