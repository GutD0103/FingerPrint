#include <stdio.h>
#include <math.h>
#include "BaseLib/Include/BaseLib.h"
#include "BaseLib/Include/Memory.h"
#include "Image/Image.h"
#include "Image/test.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // test_file("u002/fp009/u000_fp009_060.bmp","u002/fp003/u000_fp003_002.bmp");
    // test_folder("u002",10);
    test_segmentation("u002",10);
    return 0;
}
