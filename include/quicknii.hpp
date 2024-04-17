#ifndef IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#define IMPLOT_DISABLE_OBSOLETE_FUNCTIONS
#endif

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "nifti1_io.h"
#include <stdio.h>
#include <iostream> 
#include <cassert>  
#include <algorithm>
#include <string>
#include <random>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h> 


bool slider3d(const char* label);
