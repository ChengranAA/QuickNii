#include "quicknii.hpp"

int SAG_SLICE_IDX; 
int COR_SLICE_IDX;
int AX_SLICE_IDX;
int T_INDEX;

nii_slice SAG_SLICE;
nii_slice COR_SLICE; 
nii_slice AX_SLICE;

int NII_DIM[3];

// get the file extension
const char *getFileExtension(const char *file)
{
    if (fs::path(file).extension() == ".gz")
    {
        if (fs::path(file).stem().extension() == ".nii") return "nii";
    }
    else if (fs::path(file).extension() == ".nii") return "nii";
    else if (fs::path(file).extension() == ".vmr") return "vmr";
    else if (fs::path(file).extension() == ".v16") return "v16";
    else if (fs::path(file).extension() == ".fmr") return "fmr";
    else if (fs::path(file).extension() == ".vtc") return "vtc";
    else
    {
        assert("File format not supported");
        exit(1);
    }
}


// main entry point
int main(int argc, char** argv)
{
    // load the GUI (for now just treat the first argument as the file name and load it as a nifti image)
    char const* filename = argv[1];

    // check the file extension
    const char *ext = getFileExtension(filename);
    internal_nim *i_nim = nullptr; 

    if(argc == 2){
        if (strcmp(ext, "vmr") == 0 || strcmp(ext, "v16") == 0) {
             i_nim = wrapper_bv_image_read(filename, ext);
             //bv_image_to_ras(i_nim);
        } else if (strcmp(ext, "nii") == 0 || strcmp(ext, "gz") == 0) {
             i_nim = wrapper_nifti_image_read(filename);
        } else {
            std::cout << "Error: file format not supported" << std::endl;
            return 1;
        }
    } else {
        std::cout << "Error: too many arguments" << std::endl;
    }

    // position all slice indices to the middle of the image
    SAG_SLICE_IDX = i_nim->dim[1]  / 2; 
    COR_SLICE_IDX = i_nim->dim[2]  /2;
    AX_SLICE_IDX = i_nim->dim[3] / 2 ;


    // start the GUIs
    if(quickniiGUI(argc, argv, i_nim) != 0){
        return 1;
    }
    
    // free the memory
    internal_image_free(i_nim);
    return 0;
}