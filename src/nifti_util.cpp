#include "quicknii.h"
int SLICE_INDEX = 0;

win_config get_win_dimension()
{
    nifti_image *nii = nifti_image_read(FILE_PATH, 0);
    // if the nifti file is not read
    if (nii == NULL)
    {
        std::cout << "Error reading nifti file." << std::endl;
        exit(1);
    }

    win_config this_win_config = {.windowHeight = (const int)nii->ny, .windowWidth = (const int)nii->nx};

    return this_win_config;
}

// a function to read a nifti file into nifti_image struct
void read_nifti_file(nifti_image *&nii)
{
    // read the nifti file
    nii = nifti_image_read(FILE_PATH, 1);

    // if the nifti file is not read
    if (nii == NULL)
    {
        std::cout << "Error reading nifti file." << std::endl;
        return;
    }

    // if the nifti file is read
    return;
}

// function use to print header information
void print_nifti_header(nifti_image *nii)
{
    if (nii == NULL)
    {
        std::cout << "Error reading NIfTI image header" << std::endl;
        return;
    }

    // Print the header information
    printf("NIfTI Image Header Information:\n");
    printf("Data type: %d\n", nii->datatype);
    printf("Number of dimensions: %d\n", nii->ndim);
    printf("Dimensions:");
    for (int i = 0; i < nii->ndim; ++i)
    {
        printf(" %d", nii->dim[i]);
    }
    printf("\n");
}

Eigen::MatrixXd loadAndSliceNifti()
{
    nifti_image *nii = nullptr;
    read_nifti_file(nii);

    // Check and handle different data types
    switch (nii->datatype)
    {
    case NIFTI_TYPE_FLOAT32:
        break; // Supported data type
    case NIFTI_TYPE_INT16:
        break; // Supported data type
    case NIFTI_TYPE_UINT16:
        break;
    case NIFTI_TYPE_INT32:
        break; // Supported data type
    case NIFTI_TYPE_UINT8:
        break; // Supported data type
    // Add cases for other data types as needed
    default:
        std::cerr << "Unsupported datatype. Only specific data types are supported." << std::endl;
        nifti_image_free(nii);
        exit(1);
    }


    int DIMX = nii->nx;
    int DIMY = nii->ny;
    int DIMZ = nii->nz;

    if (SLICE_INDEX >= DIMZ) SLICE_INDEX = SLICE_INDEX - DIMZ; // wrapp around if exceed the z-dimension boundary
    if (SLICE_INDEX < 0)  SLICE_INDEX = DIMZ + SLICE_INDEX; // wrapp around if exceed the z-dimension boundary
    
    int sliceIndex = SLICE_INDEX;

    Eigen::MatrixXd sliceMatrix(DIMX, DIMY);

    for (int col = 0; col < DIMX; col++)
    {
        for (int row = 0; row < DIMY; row++)
        {
            int voxelIndex = sliceIndex * DIMX * DIMY + col + DIMX * row;
            switch (nii->datatype)
            {
            case NIFTI_TYPE_FLOAT32:
                sliceMatrix(col, row) = ((float *)nii->data)[voxelIndex];
                break;
            case NIFTI_TYPE_INT16:
                sliceMatrix(col, row) = ((short *)nii->data)[voxelIndex];
                break;
            case NIFTI_TYPE_UINT16:
                sliceMatrix(col, row) = ((unsigned short *)nii->data)[voxelIndex];
                break;
            case NIFTI_TYPE_INT32:
                sliceMatrix(col, row) = ((int *)nii->data)[voxelIndex];
                break;
            case NIFTI_TYPE_UINT8:
                sliceMatrix(col, row) = ((unsigned char *)nii->data)[voxelIndex];
                break;
            // Add cases for other data types as needed
            default:
                std::cout << "Error: type not supported. " << std::endl;
                exit(1);
                break;
            }
        }
    }

    nifti_image_free(nii);

    return sliceMatrix;
}
