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
#include "bv_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <cassert>  
#include <algorithm>
#include <string>
#include <random>
#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h> 
// #include <GLES2/gl2.h>

// some global variables
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

// nifti functions 
mat44 wrapper_nifti_get_affine(nifti_image* nim);
nifti_image* wrapper_nifti_image_read(const char* filename);
void wrapper_nifti_image_free(nifti_image* nim);
void nifti_image_to_float(nifti_image* nim);
void nifti_image_to_ras(nifti_image* nim);
std::vector<GLuint> nifti_image_to_slices_gl(nifti_image *nim);

// imgui
int quickniiGUI(int argc, char** argv, nifti_image* nim);
bool SliderScalar3D(char const *label, int *pValueX, int *pValueY, int *pValueZ, float const fMinX, float const fMaxX, float const fMinY, float const fMaxY, float const fMinZ, float const fMaxZ, float const fScale /*= 1.0f*/);
