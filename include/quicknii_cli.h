#ifndef _quicknii_err_h_
#define _quicknii_err_h_

#include "quicknii.h"
// ERRORS

#define ERROR_CLI_HELP "Usage: quicknii <file path>."
#define ERROR_CLI_INVALID_ARGUMENT "Error: Invalid option."
#define ERROR_CLI_NOT_ENOUGH_ARGUMENT "Error: Not enought argument."

// Error handle function
void error_msg(std::string msg);

void error_msg(std::string msg){
    std::cout << msg << std::endl;
    std::exit(1);
}

#endif 
