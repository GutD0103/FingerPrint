#include <stdio.h>
#include <math.h>
#include "../BaseLib/Include/BaseLib.h"
#include "../BaseLib/Include/Memory.h"

#define WidthSquare             4
#define MaxHeight               88
#define MaxWidth                88
#define ImageIsSquare   
#define MaskNumber              32
#define MaxSpecialPoint         500
#define PI                      3.141592653589793

#define MaxTriangle             5000

//image process 
#define m_para                  50
#define v_para                  300
#define threshold_para          125
#define f_para                  0.75 
#define f_para2                 7 //7
#define fi_para                 4 //3

// padding

// #define      left1              5
// #define      top1               5
// #define      right1             5
// #define      bottom1            5
// #define      left2              5
// #define      top2               5
// #define      right2             5
// #define      bottom2            5
// #define      padding            5

#define         left1            10
#define         top1             10
#define         right1           10
#define         bottom1          10
#define         left2            10
#define         top2             10
#define         right2           10
#define         bottom2          10
#define         padding          10
//compare options
#define angleLimit               5
#define distanceLimit            5
#define minuNumberLimit          10

typedef struct {
    UINT16  x;
    UINT16  y;
    float   direct; 
    UINT8 Type; // 1 is terminate, 2 is bifurcation 
} Minutiae;

typedef struct {
    UINT8  Count;
    Minutiae minus[MaxSpecialPoint];

} SpecialPoint;

typedef struct
{
    UINT16  Height;
    UINT16  Width;
    UINT8  data[MaxWidth][MaxHeight];
    float   direct[MaxWidth][MaxHeight]; 
    INT8 isCheck[MaxWidth][MaxHeight];
} Image;

typedef struct {
    float mask[2*WidthSquare + 1][2*WidthSquare + 1];
    UINT8 width;
} MaskFilter;



typedef struct 
{
    /* data */
    float bord1;
    float bord2;
    float bord3;
    int status; // state of the triangle, 1 is confirmed match, 0 is not
    UINT32 id; // each set of 3 special points will have a characteristic ID based on the i j k indices of the for . loop
} GroupDataSpecialPoint;



VOID SetImage(Image	*image,	UINT8 widthSquare);

VOID ToNornal(Image *image,	UINT16 M, UINT16 V);

VOID ToFiltring_Gaussin(Image *image, UINT16 widthSquare);

VOID ToFiltring_GaussinV2(Image *image, UINT16 widthSquare);

VOID ToFiltring_Gabor(Image *image, UINT16 widthSquare);

VOID ToBinary(Image *image,	UINT16 thresh, UINT16 widthSquare);

VOID ToBoneImage(Image *image);

VOID RidgeThin(Image *image);


VOID GetMinutiae 
(
	SpecialPoint *minus, 
	Image *image, 
	UINT8 leftPading,
	UINT8 topPadding,
	UINT8 rightPadding,
	UINT8 bottomPadding
);

GroupDataSpecialPoint* GetTriangle(
    SpecialPoint *ListSpecialPoint,
    UINT16* NumTriangle
);

VOID RidgeThin(Image *image);

INT8 CheckQuality(Image *image);

UINT8 CompairMinutiae(
    GroupDataSpecialPoint *group1,
    GroupDataSpecialPoint *group2,
    UINT16 NumTriangle1,
    UINT16 NumTriangle2,
    SpecialPoint *minus1,
    SpecialPoint *minus2
);

INT8 Segmentation(Image *image);
