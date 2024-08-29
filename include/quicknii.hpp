// Prevent using deprecated functions
#ifndef IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#endif

// Enable math operators for ImVec
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif


#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "nifti1_io.h"
#include "nifti1_tool.h"
extern "C" {
    #include "bv_io.h"
}
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <filesystem>
#include <cstdlib>
#include <cassert>  
#include <algorithm>
#include <string>
#include <random>
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
// #include <GLES2/gl2.h>
namespace fs = std::__fs::filesystem;

// some global variables
// file extensions
enum file_ext {ext_NII, ext_VMR, ext_V16, UNKNOWN};


// there are current indices for the slices
extern int SAG_SLICE_IDX; 
extern int COR_SLICE_IDX;
extern int AX_SLICE_IDX;

extern int T_INDEX; // this is for the time index in the 4th dimension of the nifti image (not handled yet)


// structs for a slice of a nifti image
typedef struct{
    int width; 
    int height;

    // float* data; // all data are converted to float to be handled by ImPlot
} nii_slice; 

// this is the general internal structure for mri data 
typedef struct{
    file_ext ext;  // file extension
    int dim[5]; // ndim, nx, ny, nz, nt
    float* data;   
} internal_nim;

extern int NII_DIM[3]; // dimensions of the nifti image

// slices in three orientations
extern nii_slice SAG_SLICE;
extern nii_slice COR_SLICE;
extern nii_slice AX_SLICE;

// orientation codes
#define SAGITTAL 1
#define CORONAL 2
#define AXIAL 3

// internal functions

// bv functions
internal_nim *wrapper_bv_image_read(const char *filename, const char *ext);
std::vector<GLuint> bv_image_to_slices_gl(internal_nim* nim);
void bv_image_to_ras(internal_nim* nim);

// nifti functions 
mat44 wrapper_nifti_get_affine(nifti_image* nim);
internal_nim * wrapper_nifti_image_read(const char *filename);
void nifti_image_to_float(nifti_image* nim);
void nifti_image_to_ras(nifti_image* nim);
std::vector<GLuint> nifti_image_to_slices_gl(internal_nim *nim);


// internal image free
void internal_image_free(internal_nim *nim);

// imgui
int quickniiGUI(int argc, char** argv, internal_nim* nim);
bool SliderScalar3D(char const *label, int *pValueX, int *pValueY, int *pValueZ, float const fMinX, float const fMaxX, float const fMinY, float const fMaxY, float const fMinZ, float const fMaxZ, float const fScale);
