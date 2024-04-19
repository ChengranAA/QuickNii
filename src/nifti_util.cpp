#include "quicknii.hpp"

// wrapper for nifti_read_header
nifti_1_header *wrapper_nifti_read_header(const char *filename)
{
    nifti_1_header *header;
    header = nifti_read_header(filename, nullptr, 1);
    return header;
}

// wrapper for nifti_image_read
nifti_image *wrapper_nifti_image_read(const char *filename)
{
    nifti_image *nim;
    nim = nifti_image_read(filename, 1);
    NII_DIM[0] = nim->nx;
    NII_DIM[1] = nim->ny;
    NII_DIM[2] = nim->nz;
    return nim;
}

// wrapper for nifti_image_free
void wrapper_nifti_image_free(nifti_image *nim)
{
    nifti_image_free(nim);
}

// wrapper to get the affine matrix
mat44 wrapper_nifti_get_affine(nifti_image *nim)
{
    return nim->qto_xyz;
}

// Function to calculate the multiplication of two 4x4 matrices
mat44 nifti_mat44_mul(mat44 A, mat44 B)
{
    mat44 C;
    int i, j, k;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
        {
            C.m[i][j] = 0.0;
            for (k = 0; k < 4; k++)
                C.m[i][j] += A.m[i][k] * B.m[k][j];
        }
    return C;
}

// Function to change the data store in the nifti_image to float
// this make it easier to handle the data later since I will have to implement typing each time for different data type
// this function will free the original void* data store and replace it with a float* data store (it will still be a void* but I will cast it to float* later)
void nifti_image_to_float(nifti_image *nim)
{
    // Only convert if the data is not already a float and size is smaller than or equal to 4 bytes (32 bits)
    if (nim->datatype != DT_FLOAT32 && nim->nbyper <= 4)
    {
        float *float_data = (float *)calloc(nim->nvox, sizeof(float));
        if (float_data == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for float data.\n");
            exit(EXIT_FAILURE);
        }

        switch (nim->datatype)
        {
        case DT_UINT8:
        {
            unsigned char *int_data = (unsigned char *)nim->data;
            for (int i = 0; i < nim->nvox; i++)
            {
                float_data[i] = (float)int_data[i];
            }
            break;
        }

        case DT_UINT16:
        {
            unsigned short *int_data = (unsigned short *)nim->data;
            for (int i = 0; i < nim->nvox; i++)
            {
                float_data[i] = (float)int_data[i];
            }
            break;
        }

        case DT_UINT32:
        {
            unsigned int *int_data = (unsigned int *)nim->data;
            for (int i = 0; i < nim->nvox; i++)
            {
                float_data[i] = (float)int_data[i];
            }
            break;
        }

        case DT_INT8:
        {
            char *int_data = (char *)nim->data;
            for (int i = 0; i < nim->nvox; i++)
            {
                float_data[i] = (float)int_data[i];
            }
            break;
        }

        case DT_INT16:
        {
            short *int_data = (short *)nim->data;
            for (int i = 0; i < nim->nvox; i++)
            {
                float_data[i] = (float)int_data[i];
            }
            break;
        }
        case DT_INT32:
        {
            int *int_data = (int *)nim->data;
            for (int i = 0; i < nim->nvox; i++)
            {
                float_data[i] = (float)int_data[i];
            }
            break;
        }
        default:
            fprintf(stderr, "Unsupported data type for conversion.\n");
            free(float_data);
            exit(EXIT_FAILURE);
        }

        free(nim->data);
        nim->data = float_data;
        nim->datatype = DT_FLOAT32;  // Update the datatype to float
        nim->nbyper = sizeof(float); // Update the number of bytes per voxel to 4
    }
    else if (nim->datatype == DT_FLOAT32)
    {
        return;
    }
}

// Function to change the data store in the nifti_image to RAS orientation
// this function will free the original void* data store and replace it with a float* data store (it will still be a void* but I will cast it to float* later)
// this float* data store will be in RAS orientation by taking the nitifti_image affine matrix and multiply it with the voxel index to get the RAS index
// and then the data will be reshaped to the RAS orientation
void nifti_image_to_ras(nifti_image *nim)
{
    // Only convert if the data is not already in RAS orientation
    if (nim->sform_code != NIFTI_XFORM_UNKNOWN)
    {
        float *float_data = (float *)calloc(nim->nvox, sizeof(float)); // memory to store the data in RAS orientation
        if (float_data == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for float data.\n");
            exit(EXIT_FAILURE);
        }

        mat44 affine = nim->qto_xyz; // get the affine matrix
        mat44 inverse_affine = nifti_mat44_inverse(affine);

        for (int i = 0; i < nim->nx; i++)
        {
            for (int j = 0; j < nim->ny; j++)
            {
                for (int k = 0; k < nim->nz; k++)
                {

                    mat44 index = {(float)i, (float)j, (float)k, 1.0f}; // voxel index
                    mat44 ras = nifti_mat44_mul(inverse_affine, index); // logic: RAS = inverse_affine * index  the turn the index will be mat44(x, y, z, 1))

                    // rearrange the data to RAS orientation
                    int index_ras = (int)ras.m[0][0] + (int)ras.m[1][0] * nim->nx + (int)ras.m[2][0] * nim->nx * nim->ny;
                    int index_voxel = i + j * nim->nx + k * nim->nx * nim->ny;
                    if (index_ras < nim->nvox && index_ras >= 0)
                    {
                        float_data[index_ras] = ((float *)nim->data)[index_voxel];
                    }
                }
            }
        }

        free(nim->data);
        nim->data = float_data;
    }
    else
    {
        return;
    }
}

// Function to slice the nifti image
// it will slice the nifti image in three orientations: sagittal, coronal, and axial according to global variables SAG_SLICE_IDX, COR_SLICE_IDX, AX_SLICE_IDX
// Sagittall will have nim->dim[2] as the width,  nim->dim[3] as the height and nim->dim[1] as the number of slices
// Coronal will have nim->dim[1] as the width,  nim->dim[3] as the height and nim->dim[2] as the number of slices
// Axial will have nim->dim[1] as the width,  nim->dim[2] as the height and nim->dim[3] as the number of slices
// the nii_slice have four fields: width, height, index, data
// the first three fields are the width, height, index of the slice will unambiously tell us the size of the data field calculated as width*height*sizeof(float)

// this function will also take the slice to a opengl texture

// helper function to normalize the data with a pointer to the data and the size of the data
void normalize_data(float *data, int size)
{
    float min = data[0];
    float max = data[0];
    for (int i = 0; i < size; i++)
    {
        if (data[i] < min)
        {
            min = data[i];
        }
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    for (int i = 0; i < size; i++)
    {
        data[i] = (data[i] - min) / (max - min);
    }
}

// helper function to flip the data in the y axis
void flip_data(float *data, int width, int height)
{
    float *temp = (float *)malloc(width * sizeof(float));
    if (!temp)
    {
        printf("Failed to allocate memory for temp\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < height / 2; i++)
    {
        memcpy(temp, data + i * width, width * sizeof(float));
        memcpy(data + i * width, data + (height - i - 1) * width, width * sizeof(float));
        memcpy(data + (height - i - 1) * width, temp, width * sizeof(float));
    }

    free(temp);
}


std::vector<GLuint> nifti_image_to_slices_gl(nifti_image *nim)
{
    // Safety check for nim
    if (!nim || !nim->data)
    {
        printf("Invalid nifti_image or data is NULL\n");
        exit(EXIT_FAILURE);
    }

    // Set the width and height of the slices
    SAG_SLICE.width = nim->dim[2];
    SAG_SLICE.height = nim->dim[3];

    COR_SLICE.width = nim->dim[1];
    COR_SLICE.height = nim->dim[3];

    AX_SLICE.width = nim->dim[1];
    AX_SLICE.height = nim->dim[2];

    // Memory allocation for placeholders
    float *ax_placeholder = (float *)calloc(1, AX_SLICE.width * AX_SLICE.height * sizeof(float));
    float *cor_placeholder = (float *)malloc(COR_SLICE.width * COR_SLICE.height * sizeof(float));
    float *sag_placeholder = (float *)malloc(SAG_SLICE.width * SAG_SLICE.height * sizeof(float));

    if (!ax_placeholder || !cor_placeholder || !sag_placeholder)
    {
        printf("Failed to allocate memory for one or more slices\n");
        free(ax_placeholder); // Safe to call free on NULL
        free(cor_placeholder);
        free(sag_placeholder);
        exit(EXIT_FAILURE);
    }

    // Copy the data to the placeholders
    // note: the data is fortran ordered, so the data is stored in the following way: data[x + y * width + z * width * height]
    
    memcpy(ax_placeholder, (float *)nim->data + AX_SLICE_IDX * AX_SLICE.width * AX_SLICE.height, AX_SLICE.width * AX_SLICE.height * sizeof(float));

    // Coronal slice
    for (int z = 0; z < COR_SLICE.height; z++)
    {
        for (int x = 0; x < COR_SLICE.width; x++)
        {
            int index = x + (COR_SLICE_IDX * nim->dim[1]) + (z * nim->dim[1] * nim->dim[2]);
            cor_placeholder[x + z * COR_SLICE.width] = ((float *)nim->data)[index];
        }
    }

    // Sagittal slice
   for (int z = 0; z < SAG_SLICE.height; z++)
{
    for (int y = 0; y < SAG_SLICE.width; y++)
    {
        int index = (SAG_SLICE_IDX) + (y * nim->dim[1]) + (z * nim->dim[1] * nim->dim[2]);
        sag_placeholder[y + z * SAG_SLICE.width] = ((float *)nim->data)[index];
    }
}


    // normalize the data
    normalize_data(ax_placeholder, AX_SLICE.width * AX_SLICE.height);
    flip_data(ax_placeholder, AX_SLICE.width, AX_SLICE.height);

    normalize_data(cor_placeholder, COR_SLICE.width * COR_SLICE.height);
    flip_data(cor_placeholder, COR_SLICE.width, COR_SLICE.height);

    normalize_data(sag_placeholder, SAG_SLICE.width * SAG_SLICE.height);
    flip_data(sag_placeholder, SAG_SLICE.width, SAG_SLICE.height);


    std::vector<GLuint> sliceTexture(3);
    glGenTextures(3, &sliceTexture[0]);

    // for binding the texture
    for (int i = 0; i < 3; i++)
    {
        glBindTexture(GL_TEXTURE_2D, sliceTexture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        // swizzle mask to only use the red channel
        GLint swizzleMask[] = {GL_RED, GL_RED, GL_RED, GL_ONE};
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    }

    // Sagittal slice
    glBindTexture(GL_TEXTURE_2D, sliceTexture[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, SAG_SLICE.width, SAG_SLICE.height, 0, GL_RED, GL_FLOAT, sag_placeholder);

    // Coronal slice
    glBindTexture(GL_TEXTURE_2D, sliceTexture[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, COR_SLICE.width, COR_SLICE.height, 0, GL_RED, GL_FLOAT, cor_placeholder);

    // Axial slice
    glBindTexture(GL_TEXTURE_2D, sliceTexture[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, AX_SLICE.width, AX_SLICE.height, 0, GL_RED, GL_FLOAT, ax_placeholder);


    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);


    // Example cleanup
    free(ax_placeholder);
    free(cor_placeholder);
    free(sag_placeholder);

    return sliceTexture;
}