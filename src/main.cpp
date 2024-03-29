#include "quicknii.h"
#include "quicknii_cli.h"

char *FILE_PATH;
Eigen::MatrixXd SLICE;

// a function config command line options
void config_cli_options(int argc, char **argv)
{
    if (argc < 2)
    {
        error_msg(ERROR_CLI_NOT_ENOUGH_ARGUMENT);
    }

    for (int i = 1; i < argc; ++i)
    {
        char *arg = argv[i];
        if (std::strcmp(arg, "-h") == 0 || std::strcmp(arg, "--help") == 0)
        {
            error_msg(ERROR_CLI_HELP);
        }
        else
        {
            FILE_PATH = arg;
        }
    }
}

// main function
int main(int argc, char **argv)
{
    // an empty pointer to store file path from command line input
    config_cli_options(argc, argv);

    read_nifti_file();
    loadAndSliceNifti();
    // Initialize GLFW and OpenGL
    initializeGL(argc, argv);
    return 0;
}