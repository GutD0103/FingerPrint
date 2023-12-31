#ifndef INC_NEW_PREPROCESSED_H_
#define INC_NEW_PREPROCESSED_H_

#include "Image.h"
#include "Memory.h"
#include "newMath.h"

#define     LengthRigde             6
#define     HALFLengthRigde         3
#define     LengthJump              1

typedef struct 
{
    UINT8 x; 
    UINT8 y;
    UINT8 grayValue;
}Point;


VOID        GetMinutiae_v2(Image *image,SpecialPoint *ListMinutiae);
VOID        RidgeFollowing(Image *image, Point point, INT8 lengthJump,SpecialPoint *ListMinutiae); 
INT8        StopCriteria(Image *image, Point point, float phin,SpecialPoint *ListMinutiae);
Point       localMax(Image *image, Point point, float direction);
Point       ridgeNearest(Image *image, Point point); 
float       TangentDir(Image *image, Point point);

#endif /* INC_NEW_PREPROCESSED_H_ */