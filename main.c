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

    for (int i = y - radius; i <= y + radius; i++) {
        for (int j = x - radius; j <= x + radius; j++) {
            if (i >= 0 && i < MaxWidth && j >=   0 && j < MaxHeight) {
                int dx = j - x;
                int dy = i - y;
                double distance = sqrt(dx * dx + dy * dy);
                if (distance <= (radius) && distance >= (radius-1)) {
                    int index = j * MaxWidth + i;
                    image->data[i][j] = 125;  // Đặt giá trị mức xám là 255 (màu đỏ)
                }
            }
        }
    }
    // float sinn = sin(direction);
    // float coss = cos(direction);
    // for(int i = 0; i< 8; i++){
    //     int _y = round(y + i*sinn);
    //     int _x = round(x + i*coss);
    //     image->data[_y][_x] = 125;
    // }
}

VOID draw(SpecialPoint *minus, Image * image){
    for(int i = 0; i < minus->Count ;i++){
        _drawRedCircle(image,minus->minus[i].y,minus->minus[i].x,3,minus->minus[i].direct);
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
        //printf("Failed to open file: %s\n", filename);
        Bitmap bmp;
        bmp.width = 0;
        bmp.height = 0;

        return bmp;
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
VOID t_GetErrorImage(int fileCount,char* SsourceFiles2[150]){
    int less3 = 0;
    int more7 = 0;
    int all   = 0;

    for(int i = 0 ; i < fileCount; i++){
        unsigned char* imageData3;
        const char* sourceFile3 = SsourceFiles2[i];
        Bitmap bmp1 = loadBitmapFromFile(sourceFile3, &imageData3);
        if(bmp1.height==0){
            continue;
        }
        int width = bmp1.width; 
        int height = bmp1.height;
        unsigned char New_009[width][height];
        if(i==0){
            printf("%s\n",sourceFile3);
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                New_009[x][y] = imageData3[y * width + x];
            }
        }
        Image *Image1 = AllocatePool(sizeof(Image));
        ZeroMem(Image1,sizeof(Image));
        Image1->Height = MaxHeight;
        Image1->Width = MaxWidth;
        
        SpecialPoint *minus1 = AllocatePool(sizeof(SpecialPoint));


        CopyMem(Image1->data, New_009, sizeof(New_009));                
        ToNornal(Image1, m_para,v_para);     
        SetImage(Image1, WidthSquare);
        ToFiltring_Gaussin(Image1, WidthSquare); 
        ToFiltring_Gabor(Image1, WidthSquare);
        ToBinary(Image1,threshold_para,WidthSquare);          
        ToBoneImage(Image1);        
        GetMinutiae(minus1, Image1,left1,top1,right1,bottom1);                      
        if(minus1->Count > 6){
            remove(sourceFile3);
            //printf("more 7: %s\n",sourceFile3);
            more7++;
        }
        else if(minus1->Count < 3){
            remove(sourceFile3);
            //printf("less 3: %s\n",sourceFile3);
            less3++;
        }
        if(minus1->Count >= 3 && minus1->Count <= 6){
            all++;
           // printf("OKE: %s\n",sourceFile3);

        }
        free(minus1);
        free(Image1);

    }
    printf("LESS THAN 3: %d-%f\nMORE THAN 6: %d-%f\nError: %d\nALL: %d\n\n",less3,(float)less3/(less3+more7),more7,(float)more7/(less3+more7),less3+more7,all);
}
INT8 isAlready(SpecialPoint *list[10],int count, SpecialPoint *minus){
    
    for(int i = 0; i < count; i++){
        UINT16 NumTriangle1;
        UINT16 NumTriangle2;
        GroupDataSpecialPoint * listTriangle1 = GetTriangle(list[i], &NumTriangle1);
        GroupDataSpecialPoint * listTriangle2 = GetTriangle(minus, &NumTriangle2);   
        float result1 = CompairMinutiae(listTriangle1, listTriangle2, NumTriangle1, NumTriangle2, list[i], minus);
        int minpoint = list[i]->Count < minus->Count ? list[i]->Count : minus->Count;
        float ketqua = (result1/minpoint)*100;
        if(ketqua >= 100){
            free(listTriangle1);
            free(listTriangle2);
            return 1;
        }
    }
    return 0;
}
VOID GetTempalte(int fileCount,char* SsourceFiles2[150],char* template[5]){
    int fileCount3 = 0;
    SpecialPoint *list[10];
    for(int i = 0 ; i < fileCount; i++){
        if(fileCount3 >= 5){
            return;
        }
        unsigned char* imageData3;
        char* sourceFile3 = SsourceFiles2[i];
        Bitmap bmp1 = loadBitmapFromFile(sourceFile3, &imageData3);
        if(bmp1.height==0){
            continue;
        }
        int width = bmp1.width; 
        int height = bmp1.height;
        unsigned char New_009[width][height];

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                New_009[x][y] = imageData3[y * width + x];
            }
        }
        Image *Image1 = AllocatePool(sizeof(Image));
        ZeroMem(Image1,sizeof(Image));
        Image1->Height = MaxHeight;
        Image1->Width = MaxWidth;
        
        SpecialPoint *minus1 = AllocatePool(sizeof(SpecialPoint));


        CopyMem(Image1->data, New_009, sizeof(New_009));                
        ToNornal(Image1, m_para,v_para);     
        SetImage(Image1, WidthSquare);
        ToFiltring_Gaussin(Image1, WidthSquare); 
        ToFiltring_Gabor(Image1, WidthSquare); 
        ToBinary(Image1,threshold_para,WidthSquare);          
        ToBoneImage(Image1);        
        GetMinutiae(minus1, Image1,left1,top1,right1,bottom1);                      
        if(minus1->Count <= 7 && minus1->Count >= 3){
            //template[fileCount3++] = sourceFile3;
            if(!isAlready(list,fileCount3,minus1)){
                template[fileCount3] = sourceFile3;
                list[fileCount3] = minus1;
                fileCount3++;
            }
        }
        free(Image1);
    }
    //printf("%d : 10 \n",fileCount3);
}
int main(){
for(int o = 1 ; o <=4 ; o++){
    if(o == 2){
        continue;
    }
 for(int p = 1 ; p <=4 ; p++){   
    if(p == 2){
        continue;
    }
    int soanhtrung = 0;

    const int MAX_FILES = 150; // Số lượng tệp tin tối đa
    const char* SsourceFiles[MAX_FILES];
    int start = 0;
    int end = 149;
    char prefix[] = "u002/fp003/u000_fp003_";
    sprintf(prefix, "%s%02d%s%02d%s", "u003/fp0", o, "/u000_fp0",o,"_");
    const char* sourceFile1;
    char extension[] = ".bmp";
    
    const int MAX_FILES2 = 150; // Số lượng tệp tin tối đa
    char* SsourceFiles2[MAX_FILES2];
    int start2 = 0;
    int end2 = 149;
    char prefix2[] = "u002/fp010/u000_fp010_";
    sprintf(prefix2, "%s%02d%s%02d%s", "u003/fp0", p, "/u000_fp0",p,"_");
    const char* sourceFile2;
    char extension2[] = ".bmp";

// ...
const int MAX_FILES3 = 5; // Số lượng tệp tin tối đa
char* template[MAX_FILES3];
int fileCount3 = MAX_FILES3;


int fileCount = 0;
int fileCount2 = 0;

for (int i = start; i <= end; i++) {
    char filename[50];
    sprintf(filename, "%s%03d%s", prefix, i, extension);
    SsourceFiles[fileCount] = strdup(filename); // Lưu trữ bản sao của tên tệp tin
    fileCount++;
}

for (int i = start2; i <= end2; i++) {
    char filename[50];
    sprintf(filename, "%s%03d%s", prefix2, i, extension2);
    SsourceFiles2[fileCount2] = strdup(filename); // Lưu trữ bản sao của tên tệp tin
    fileCount2++;
}

//t_GetErrorImage(fileCount2,SsourceFiles2);
GetTempalte(fileCount2,SsourceFiles2,template);
// printf("%s\n",template[9]);
// for(int i = 0; i< 10; i++){
//     printf("%s\n",template[i]);
// }

int all = 0;
 //\-------------------------------------------------------------------------------------------------------/
for (int i = 0; i < fileCount; i++) {
    for(int j = 0; j < fileCount3; j++){
            
            const char* sourceFile2 = template[j];
            const char* sourceFile1 = SsourceFiles[i];
            unsigned char* imageData1;
            unsigned char* imageData2;
            // if(i==0 && j ==0){
                //printf("%s------------%s\n",sourceFile1,sourceFile2);
            // }
            Bitmap bmp1 = loadBitmapFromFile(sourceFile1, &imageData1);
            Bitmap bmp2 = loadBitmapFromFile(sourceFile2, &imageData2);
                if(bmp1.height==0){
                    continue;
                }
                if(bmp2.height==0){
                    continue;
                }
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
        ToNornal(Image1, m_para,v_para);     
        SetImage(Image1, WidthSquare);
        ToFiltring_Gaussin(Image1, WidthSquare); 
        ToFiltring_Gabor(Image1, WidthSquare); 
        ToBinary(Image1,threshold_para,WidthSquare);          
        ToBoneImage(Image1);        
        GetMinutiae(minus1, Image1,left1,top1,right1,bottom1);
        GroupDataSpecialPoint * listTriangle1 = GetTriangle(minus1, &NumTriangle1);                            
        draw(minus1,Image1);
        CopyMem(Image2->data, New_005, sizeof(New_005));                
        ToNornal(Image2, m_para,v_para);                               
        SetImage(Image2, WidthSquare);
        ToFiltring_Gaussin(Image2, WidthSquare);
        ToFiltring_Gabor(Image2, WidthSquare);
        ToBinary(Image2,threshold_para,WidthSquare);
        ToBoneImage(Image2);
        GetMinutiae(minus2, Image2,left2,top2,right2,bottom2);
        GroupDataSpecialPoint * listTriangle2 = GetTriangle(minus2, &NumTriangle2);                           
        draw(minus2,Image2);
        //UINT8 result = CompairMinutiae(minus1, minus2);

        if(minus1->Count < 3 || minus1->Count > 6 || minus2->Count < 3 || minus2->Count > 6 || abs(minus1->Count-minus2->Count) > 2){

            //printf("Van tay khong du du lieu de so sanh\n");

        } else{
           // UINT8 result = CompairMinutiae(minus1, minus2);

            float result1 = CompairMinutiae(listTriangle1, listTriangle2, NumTriangle1, NumTriangle2, minus1, minus2);
            int minpoint = minus1->Count < minus2->Count ? minus1->Count:minus2->Count;
            float ketqua = (result1/minpoint)*100;
            if(ketqua >= 70){
                if(strcmp(prefix2,prefix)!=0){
                    printf("%s------------%s\n",sourceFile1,sourceFile2);
                }
                soanhtrung+=1;
                break;
            }
            // else{
            //     if(strcmp(prefix2,prefix)==0){
            //         printf("%s------------%s\n",sourceFile1,sourceFile2);
            //     }
            // }
        }

        free(minus1);
        free(minus2);
        free(listTriangle1);
        free(listTriangle2);
        free(Image1);
        free(Image2);
        //printf("------------------------------------------------------------------------------------------------------\n");
    }
    if(soanhtrung > 0){
        all++;
    }
    soanhtrung=0;
    }
   printf("%s-%s: %d\n",prefix,prefix2,all);
   all = 0;
}
}
return 0;
}
