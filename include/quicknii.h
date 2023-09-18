#ifndef _quicknii_h_

#define _quicknii_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <Eigen/Dense>

#define GL_SILENCE_DEPRECATION // to silence deprecation warnings
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include "nifti1.h"
#include "nifti1_io.h"

extern char * FILE_PATH;
extern int SLICE_INDEX;
extern nifti_image * NII;

// define a structure that will hold information for width and height
typedef struct win_config
{
    const int windowWidth;
    const int windowHeight;
} win_config; 


// OpenGL texture ID
extern GLuint textureID;

win_config get_win_dimension();
void read_nifti_file(nifti_image *&nii);
void print_nifti_header(nifti_image *nii);
Eigen::MatrixXd loadAndSliceNifti();
void initializeGL(int argc, char** argv, win_config this_win_config);


#endif