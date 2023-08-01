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
    test_file("u003/fp001/u000_fp001_000.bmp","u003/fp001/u000_fp001_000.bmp");
    test_folder("u003",4);
    return 0;
}
