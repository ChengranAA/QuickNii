#include "quicknii.hpp"

int SAG_SLICE_IDX; 
int COR_SLICE_IDX;
int AX_SLICE_IDX;
int T_INDEX;

nii_slice SAG_SLICE;
nii_slice COR_SLICE; 
nii_slice AX_SLICE;

int NII_DIM[3];

// main entry point
int main(int argc, char** argv)
{
    // load the GUI (for now just treat the first argument as the file name and load it as a nifti image)
    char const* filename = argv[1];
    nifti_image* nim = wrapper_nifti_image_read(filename);
    nifti_image_to_float(nim); // from this point on, the data is float

    // position all slice indices to the middle of the image
    SAG_SLICE_IDX = nim->dim[1] / 2; 
    COR_SLICE_IDX = nim->dim[2] / 2;
    AX_SLICE_IDX = nim->dim[3] / 2;

    // slice the nifti image
    // nifti_image_to_slices_gl(nim);

    // start the GUIs

    if(quickniiGUI(argc, argv, nim) != 0){
        return 1;
    }
    
    wrapper_nifti_image_free(nim);
    return 0;
}