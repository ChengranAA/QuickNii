#include "quicknii.h"
int SLICE_INDEX = 0;

std::string sliceOrderToString(int sliceOrder)
{
    std::string result;

    switch (sliceOrder)
    {
    case 0:
        result = "Slice order unknown";
        break;
    case 1:
        result = "Sequential, increasing";
        break;
    case 2:
        result = "Sequential, decreasing";
        break;
    case 3:
        result = "Interleaved, increasing, starting at the 1st MRI slice";
        break;
    case 4:
        result = "Interleaved, decreasing, starting at the last MRI slice";
        break;
    case 5:
        result = "Interleaved, increasing, starting at the 2nd MRI slice";
        break;
    case 6:
        result = "Interleaved, decreasing, starting at one before the last MRI slice";
        break;
    default:
        result = "Unknown slice order";
        break;
    }

    return result;
}

std::string dataTypeToString(int dataType)
{
    std::string result;

    switch (dataType)
    {
    case 0:
        result = "unknown - 0";
        break;
    case 1:
        result = "bool - 1 bit";
        break;
    case 2:
        result = "unsigned char - 8 bits";
        break;
    case 4:
        result = "signed short - 16 bits";
        break;
    case 8:
        result = "signed int - 32 bits";
        break;
    case 16:
        result = "float - 32 bits";
        break;
    case 32:
        result = "complex - 64 bits";
        break;
    case 64:
        result = "double - 64 bits";
        break;
    case 128:
        result = "rgb - 24 bits";
        break;
    case 255:
        result = "all - 255";
        break;
    case 256:
        result = "signed char - 8 bits";
        break;
    case 512:
        result = "unsigned short - 16 bits";
        break;
    case 768:
        result = "unsigned int - 32 bits";
        break;
    case 1024:
        result = "long long - 64 bits";
        break;
    case 1280:
        result = "unsigned long long - 64 bits";
        break;
    case 1536:
        result = "long double - 128 bits";
        break;
    case 1792:
        result = "double pair - 128 bits";
        break;
    case 2048:
        result = "long double pair - 256 bits";
        break;
    case 2304:
        result = "rgba - 32 bits";
        break;
    default:
        result = "Unknown data type.";
        break;
    }

    return result;
}

std::string orientationTypeToString(int coordinateType)
{
    std::string result;

    switch (coordinateType)
    {
    case 0:
        result = "Unknown";
        break;
    case 1:
        result = "Scanner";
        break;
    case 2:
        result = "Aligned";
        break;
    case 3:
        result = "Talairach";
        break;
    case 4:
        result = "MNI_152";
        break;
    default:
        result = "Illegal code";
        break;
    }

    return result;
}

void readFirst348Bytes(const char *filename, nifti_1_header *ret)
{
    int use_compression = 0; // Default to no compression

    // Check if the file is a zip file by examining its extension
    const char *file_extension = strrchr(filename, '.');
    if (file_extension != NULL && strcmp(file_extension, ".gz") == 0)
    {
        use_compression = 1; // Set to use compression for .gz files
    }

    znzFile file = znzopen(filename, "rb", use_compression ? 1 : 0);
    if (znz_isnull(file))
    {
        printf("Failed to open the file.\n");
        return;
    }

    size_t bytesRead;
    if (use_compression)
    {
        // Use znzread for zip files
        bytesRead = znzread(ret, sizeof(char), sizeof(nifti_1_header), file);
    }
    else
    {
        // Use regular fread for non-zip files
        bytesRead = fread(ret, sizeof(char), sizeof(nifti_1_header), file->nzfptr);
    }

    if (bytesRead < sizeof(nifti_1_header))
    {
        printf("Failed to read the first 384 bytes of the file.\n");
    }

    znzclose(file);
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
        exit(0);
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

    if (SLICE_INDEX >= DIMZ)
        SLICE_INDEX = SLICE_INDEX - DIMZ; // wrapp around if exceed the z-dimension boundary
    if (SLICE_INDEX < 0)
        SLICE_INDEX = DIMZ + SLICE_INDEX; // wrapp around if exceed the z-dimension boundary

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
