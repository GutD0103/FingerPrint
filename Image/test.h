#ifndef INC_TEST_H_
#define INC_TEST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Image.h"
#include "../BaseLib/Include/BaseLib.h"
#include "../BaseLib/Include/Memory.h"

int test_file(char* sourceFile1, char* sourceFile2);
int test_folder(char* sourceFolder, int num_of_folders);

#endif /* INC_TEST_H_ */