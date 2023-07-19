#include <stdio.h>
#include <math.h>
#include "BaseLib/Include/BaseLib.h"
#include "BaseLib/Include/Memory.h"
#include "Image/Image.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>

VOID display(SpecialPoint *minus){
	printf("\n minus->Count = %d;\n", minus->Count);
	for (int i = 0; i < minus->Count;i++)
	{		
		printf("minus->minus[%d].x = %d;\n", i,minus->minus[i].x);
		printf("minus->minus[%d].y = %d;\n", i,minus->minus[i].y);
		printf("minus->minus[%d].direct = %f;\n",i, minus->minus[i].direct);
	}	
}

VOID saveImageDataToTxt(const Image *image, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("\n -- Cannot Open %s -- \n", filename);
        return;
    }

    for (int i = 0; i < image->Height; i++)
    {
        for (int j = 0; j < image->Width; j++)
        {
            fprintf(file, "%d", image->data[i][j]);
            if (i != image->Height - 1 || j != image->Width - 1)
                fprintf(file, ",");
        }
    }

    fclose(file);
}

void _drawRedCircle(Image *image, int x, int y, int radius, float direction) {

    for (int i = x - radius; i <= x + radius; i++) {
        for (int j = y - radius; j <= y + radius; j++) {
            if (i >= 0 && i < MaxWidth && j >=   0 && j < MaxHeight) {
                int dx = i - x;
                int dy = j - y;
                double distance = sqrt(dx * dx + dy * dy);
                if (distance <= (radius) && distance >= (radius-1)) {
                    int index = i * MaxWidth + j;
                    image->data[i][j] = 125;  // Đặt giá trị mức xám là 255 (màu đỏ)
                }
            }
        }
    }
    float sinn = sin(direction);
    float coss = cos(direction);
    for(int i = 0; i< 8; i++){
        int _y = round(y + i*coss);
        int _x = round(x + i*sinn);
        image->data[_x][_y] = 125;
    }
}

VOID draw(SpecialPoint *minus, Image * image){
    for(int i = 0; i < minus->Count ;i++){
        _drawRedCircle(image,minus->minus[i].x,minus->minus[i].y,3,minus->minus[i].direct);
    }
}
VOID drawDirect(Image *image){
    for(int x = 0; x < MaxHeight; x+= 10)
    {
        for (int y = 0; y < MaxHeight; y++)
        {
            if(image->data[x][y] == 0){
                float sinn = sin(image->direct[x][y]);
                float coss = cos(image->direct[x][y]);
                for(int i = 0; i< 5; i++){
                    int _y = round(y + i*coss);
                    int _x = round(x + i*sinn);
                    image->data[_x][_y] = 125;
                }
            }
        }
        
    }
}


void saveArrayToTextFile(const char* filename, UINT8** array, int width, int height) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fprintf(file, "%d", array[y][x]);

            if (x != width - 1) {
                fprintf(file, ",");
            }
        }
        if (y != height - 1) {
            fprintf(file, " ");
        }
    }

    fclose(file);
}


//--------------------------------------------------
typedef struct {
    unsigned char R;
    unsigned char G;
    unsigned char B;
} Color;

typedef struct {
    int width;
    int height;
} Bitmap;

Bitmap loadBitmapFromFile(const char* filename, unsigned char** imageData) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        exit(1);
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];
    int dataOffset = *(int*)&header[10];

    *imageData = (unsigned char*)malloc(width * height * sizeof(unsigned char));

    fseek(file, dataOffset, SEEK_SET);
    fread(*imageData, sizeof(unsigned char), width * height, file);

    fclose(file);

    Bitmap bmp;
    bmp.width = width;
    bmp.height = height;

    return bmp;
}


VOID DRAW_TOBONE(Image* temp, Image* image1){
    for(int i = 0 ; i < MaxHeight; i++){
        for(int j = 0; j < MaxWidth; j++){
            if(image1->data[i][j] == 0){
                temp->data[i][j] = 200;
            }
        }
    }
}

VOID PreToBone(Image* temp, Image* image1){
    for(int i = 0 ; i < MaxHeight; i++){
        for(int j = 0; j < MaxWidth; j++){
                temp->data[i][j] = image1->data[i][j];
        }
    }
}

int main(){

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    Image *temp = AllocatePool(sizeof(Image));
    temp->Height = MaxHeight;
    temp->Width = MaxWidth;

    const char* sourceFile1 = "u003/fp004/u000_fp004_114.bmp";
    const char* sourceFile2 = "u003/fp004/u000_fp004_048.bmp";
    unsigned char* imageData1;
    unsigned char* imageData2;
    Bitmap bmp1 = loadBitmapFromFile(sourceFile1, &imageData1);
    Bitmap bmp2 = loadBitmapFromFile(sourceFile2, &imageData2);

    // Process the bitmaps as needed

    // Example: Copy the image data to the unsigned char arrays
    int width = bmp1.width;  // Assuming both bitmaps have the same width
    int height = bmp1.height;

    unsigned char New_009[width][height];
    unsigned char New_005[width][height];

    // Copy imageData1 to New_009
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            New_009[x][y] = imageData1[y * width + x];
        }
    }

    // Copy imageData2 to New_005
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            New_005[x][y] = imageData2[y * width + x];
        }
    }
//----------------------------------------------------------------------
  Image *Image1 = AllocatePool(sizeof(Image));
  Image *Image2 = AllocatePool(sizeof(Image));
   
  ZeroMem(Image1,sizeof(Image));
  ZeroMem(Image2,sizeof(Image));

  Image1->Height = MaxHeight;
  Image1->Width = MaxWidth;
  Image2->Height = MaxHeight;
  Image2->Width = MaxWidth;
 
  UINT16 NumTriangle1;
  UINT16 NumTriangle2;
 
  
  SpecialPoint *minus1 = AllocatePool(sizeof(SpecialPoint));
  SpecialPoint *minus2 = AllocatePool(sizeof(SpecialPoint));

  CopyMem(Image1->data, New_009, sizeof(New_009));
  CopyMem(Image2->data, New_005, sizeof(New_005));      

// if(!CheckQuality(Image1)){
//     printf("%s\n",sourceFile1);
// }
// printf("-------------------\n");
// if(!CheckQuality(Image2)){
//     printf("%s\n",sourceFile2);
// }

  ToNornal(Image1, m_para,v_para);
  SetImage(Image1, WidthSquare);
                                                            saveImageDataToTxt(Image1,"output.txt");
  ToFiltring_Gaussin(Image1, WidthSquare);
  ToFiltring_Gabor(Image1, WidthSquare);
  ToBinary(Image1,threshold_para,WidthSquare);
                                                            //PreToBone(temp,Image1);
  ToBoneImage(Image1);
                                                            //drawDirect(Image1);
  GetMinutiae(minus1, Image1,left1,top1,right1,bottom1);
                                                            draw(minus1,Image1);
                                                            saveImageDataToTxt(Image1,"output1.txt");
                                                            //DRAW_TOBONE(temp,Image1);
  GroupDataSpecialPoint * listTriangle1 = GetTriangle(minus1, &NumTriangle1); 
  if(!listTriangle1){
    
  }    

  ToNornal(Image2, m_para,v_para);                               
  SetImage(Image2, WidthSquare);
                                                            saveImageDataToTxt(Image2,"output2.txt");
  ToFiltring_Gaussin(Image2, WidthSquare);
  ToFiltring_Gabor(Image2, WidthSquare);
  ToBinary(Image2,threshold_para,WidthSquare);
  ToBoneImage(Image2);
  GetMinutiae(minus2, Image2,left2,top2,right2,bottom2);
                                                            draw(minus2,Image2);
                                                            saveImageDataToTxt(Image2,"output3.txt");
                                                            //drawDirect(Image2);  
  GroupDataSpecialPoint * listTriangle2 = GetTriangle(minus2, &NumTriangle2);    
  if(!listTriangle2){
    return 0;
  }                        

  //UINT8 result = CompairMinutiae(minus1, minus2);
  if(minus1->Count < 3 || minus2->Count < 3 ||minus1->Count > 6 || minus2->Count > 6){

    printf("Van tay khong du du lieu de so sanh");

  } else{
      UINT8 result1 = CompairMinutiae(listTriangle1, listTriangle2, NumTriangle1, NumTriangle2, minus1, minus2);
  }

  free(minus1);
  free(minus2);
//   free(listTriangle1);
//   free(listTriangle2);
  return 0;
}