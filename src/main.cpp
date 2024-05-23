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
    // Define the known extensions
    const char* knownExtensions[] = {"vmr", "v16", "nii"};
    const int numKnownExtensions = sizeof(knownExtensions) / sizeof(knownExtensions[0]);
    
    // Find the last dot in the filename
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename) return "UNKNOWN";
    
    // Handle .gz files
    if (strcmp(dot, ".gz") == 0) {
        // Find the second last dot before .gz
        const char *dot2 = strrchr(filename, '.');
        if (!dot2 || dot2 == filename) return "UNKNOWN";
        // Move dot2 to the second last dot position
        dot2 = strrchr(filename, dot2 - filename - 1);
        if (!dot2 || dot2 == filename) return "UNKNOWN";
        dot = dot2;
    }
    
    // Extract the extension
    const char* extension = dot + 1;
    
    // Check if the extension is one of the known extensions
    for (int i = 0; i < numKnownExtensions; i++) {
        if (strcmp(extension, knownExtensions[i]) == 0) {
            return extension;
        }
    }
    
    return "UNKNOWN";
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
         // bv_image_to_ras(i_nim);
    } else if (strcmp(ext, "nii") == 0 || strcmp(ext, ".nii.gz") == 0) {
         i_nim = wrapper_nifti_image_read(filename);
    } else {
        std::cout << "Error: file format not supported" << std::endl;
        return 1;
    }

    // position all slice indices to the middle of the image
    SAG_SLICE_IDX = i_nim->dim[1]  / 2; 
    COR_SLICE_IDX = i_nim->dim[2]  /2;
    AX_SLICE_IDX = i_nim->dim[3] / 2 ;

    //std::cout << "SLICE: " << SAG_SLICE_IDX << " " << COR_SLICE_IDX << " " << AX_SLICE_IDX<< std::endl;

    // start the GUIs

    if(quickniiGUI(argc, argv, i_nim) != 0){
        return 1;
    }
    
    // free the memory
    internal_image_free(i_nim);
    return 0;
}