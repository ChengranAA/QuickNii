#include "quicknii.hpp"

int SAG_SLICE_IDX; 
int COR_SLICE_IDX;
int AX_SLICE_IDX;
int T_INDEX;

nii_slice SAG_SLICE;
nii_slice COR_SLICE; 
nii_slice AX_SLICE;

int NII_DIM[3];

// current implementation of the file extension checker (better thing needed) 
const char* getFileExtension(const char* filename) {
    const char* lastDot = strrchr(filename, '.');
    if (!lastDot || lastDot == filename) return ""; // Check if no dot or dot is the first character.

    // Handle compressed file extensions like .tar.gz
    const char* previousDot = NULL;
    if (lastDot != filename) {
        previousDot = lastDot - 1;
        // Search backwards for the next dot before the last dot
        while (previousDot > filename && *previousDot != '.') {
            previousDot--;
        }
        // Check if the dot found is part of the extension (like .tar in .tar.gz)
        if (*previousDot != '.' || previousDot == filename) {
            previousDot = lastDot; // No valid second dot found, use the last dot
        }
    }

    // If the extension has only one dot, return the extension without the dot
    if (previousDot == lastDot) {
        previousDot++;
    }

    return previousDot; // Return the extension starting from the first relevant dot
}

// main entry point
int main(int argc, char** argv)
{
    // load the GUI (for now just treat the first argument as the file name and load it as a nifti image)
    char const* filename = argv[1];

    // check the file extension
    const char *ext = getFileExtension(filename);
    internal_nim * i_nim; 

    if (strcmp(ext, "vmr") == 0 || strcmp(ext, "v16") == 0) {
         i_nim = wrapper_bv_image_read(filename, ext);
    } else if (strcmp(ext, "nii") == 0 || strcmp(ext, ".nii.gz") == 0) {
         i_nim = wrapper_nifti_image_read(filename);
    } else {
        std::cout << "Error: file format not supported" << std::endl;
        return 1;
    }

    // position all slice indices to the middle of the image
    SAG_SLICE_IDX = i_nim->dim[1] / 2; 
    COR_SLICE_IDX = i_nim->dim[2] / 2;
    AX_SLICE_IDX = i_nim->dim[3] / 2;

    //std::cout << "SLICE: " << SAG_SLICE_IDX << " " << COR_SLICE_IDX << " " << AX_SLICE_IDX<< std::endl;

    // start the GUIs

    if(quickniiGUI(argc, argv, i_nim) != 0){
        return 1;
    }
    
    // free the memory
    internal_image_free(i_nim);
    return 0;
}