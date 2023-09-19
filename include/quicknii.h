#ifndef _quicknii_h_

#define _quicknii_h_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <zlib.h>
#include <Eigen/Dense>

#define GL_SILENCE_DEPRECATION // to silence deprecation warnings
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#include "nifti1.h"
#include "nifti1_io.h"
#include "znzlib.h"

extern char *FILE_PATH;
extern int SLICE_INDEX;
extern Eigen::MatrixXd SLICE;
extern nifti_image *NII;
extern double rotationAngle;
extern nifti_1_header NII_HEADER;

// OpenGL texture ID
extern GLuint textureID;

void read_nifti_file();
void loadAndSliceNifti();
void initializeGL(int argc, char **argv);
void print_nifti_header(nifti_image *nii);
std::string orientationTypeToString(int coordinateType);
std::string dataTypeToString(int dataType);
std::string sliceOrderToString(int sliceOrder);

#endif