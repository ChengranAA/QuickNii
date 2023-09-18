#include "quicknii.h"
int SLICE_INDEX = 0;

void readFirst348Bytes(const char *filename, nifti_1_header * ret)
{
    FILE *file = fopen(filename, "rb"); // Open the file in binary mode for reading
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }

    size_t bytesRead = fread(ret, sizeof(char), sizeof(nifti_1_header), file);
    if (bytesRead < sizeof(nifti_1_header))
    {
        printf("Failed to read the first 384 bytes of the file.\n");
        fclose(file);
        return;
    }

    fclose(file);
}


// a function to read a nifti file into nifti_image struct
void read_nifti_file()
{
    // read the nifti file
    NII = nifti_image_read(FILE_PATH, 1);
    readFirst348Bytes(FILE_PATH, &NII_HEADER);

    // if the nifti file is not read
    if (NII == NULL)
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

void loadAndSliceNifti()
{
    // Check and handle different data types
    switch (NII->datatype)
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
        nifti_image_free(NII);
        exit(1);
    }


    int DIMX = NII->nx;
    int DIMY = NII->ny;
    int DIMZ = NII->nz;

    if (SLICE_INDEX >= DIMZ) SLICE_INDEX = SLICE_INDEX - DIMZ; // wrapp around if exceed the z-dimension boundary
    if (SLICE_INDEX < 0)  SLICE_INDEX = DIMZ + SLICE_INDEX; // wrapp around if exceed the z-dimension boundary
    
    int sliceIndex = SLICE_INDEX;

    Eigen::MatrixXd sliceMatrix(DIMX, DIMY);

    for (int col = 0; col < DIMX; col++)
    {
        for (int row = 0; row < DIMY; row++)
        {
            int voxelIndex = sliceIndex * DIMX * DIMY + col + DIMX * row;
            switch (NII->datatype)
            {
            case NIFTI_TYPE_FLOAT32:
                sliceMatrix(col, row) = ((float *)NII->data)[voxelIndex];
                break;
            case NIFTI_TYPE_INT16:
                sliceMatrix(col, row) = ((short *)NII->data)[voxelIndex];
                break;
            case NIFTI_TYPE_UINT16:
                sliceMatrix(col, row) = ((unsigned short *)NII->data)[voxelIndex];
                break;
            case NIFTI_TYPE_INT32:
                sliceMatrix(col, row) = ((int *)NII->data)[voxelIndex];
                break;
            case NIFTI_TYPE_UINT8:
                sliceMatrix(col, row) = ((unsigned char *)NII->data)[voxelIndex];
                break;
            // Add cases for other data types as needed
            default:
                std::cout << "Error: type not supported. " << std::endl;
                exit(1);
                break;
            }
        }
    }

    SLICE = sliceMatrix;
}
