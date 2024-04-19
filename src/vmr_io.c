#include "bv_io.h"

// Read function for VMR files
VMR read_vmr(const char* filename){
    VMR vmr_im;
    FILE* file = fopen(filename, "rb");
    if(file == NULL){
        printf("Error: could not open file %s\n", filename);
        exit(1);
    }
    fread(&vmr_im.version, sizeof(unsigned short), 1, file);
    fread(&vmr_im.DimX, sizeof(unsigned short), 1, file);
    fread(&vmr_im.DimY, sizeof(unsigned short), 1, file);
    fread(&vmr_im.DimZ, sizeof(unsigned short), 1, file);
    vmr_im.data = (unsigned char*)malloc(vmr_im.DimX * vmr_im.DimY * vmr_im.DimZ * sizeof(unsigned char));
    fread(vmr_im.data, sizeof(unsigned char), vmr_im.DimX * vmr_im.DimY * vmr_im.DimZ, file);
    fclose(file);
    return vmr_im;
}

void free_vmr(VMR vmr_im){
    free(vmr_im.data);
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


void free_v16(V16 v16){
    free(v16.data);
}
