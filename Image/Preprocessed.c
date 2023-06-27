
#include "Image.h"
#include "Memory.h"
#include "newMath.h"
const UINT8 height = MaxHeight;
const UINT8 width = MaxWidth;


VOID SetImage(
    Image* image,
    UINT8 widthSquare
)
{
    UINT8 i, j, x, y;
    int Ax, Ay, Axy;
    int Bx, By;
	int size = MaxWidth;
    if (image->Height <= 0 || image->Width <= 0) {
        printf("Image size error: Too small %d x %d \n", image->Width, image->Height);
        return;
    }

    for (x = widthSquare + 1; x < width - widthSquare - 1; x++) {
        for (y = widthSquare + 1; y < height - widthSquare - 1; y++) {
            Ax = 0;
            Ay = 0;
            Axy = 0;
            Bx = 0;
            By = 0;

            int startX = x - widthSquare;
            int endX = x + widthSquare - 1;
            int startY = y - widthSquare;
            int endY = y + widthSquare - 1;

            for (i = startX; i < endX; i++) {
                for (j = startY; j < endY; j++) {
					UINT8 *a1 = &(image->data[i ][j ]);
                    //Bx = ((data[i + 2][j] + 2 * data[i + 2][j + 1] + data[i + 2][j + 2] - data[i][j] - 2 * data[i][j + 1] - data[i][j + 2]));
                    //By = ((data[i][j + 2] + 2 * data[i + 1][j + 2] + data[i + 2][j + 2] - data[i][j] - 2 * data[i + 1][j] - data[i + 2][j]));
                   
				    Bx = ((*(a1 + size*2) + 2**(a1 + size*2 + 1) + *(a1 + size*2 + 2) - *a1 - 2**(a1 + 1) - *(a1 + 2)));
					By = ((*(a1 + 2) + 2**(a1 + size + 2) + *(a1 + size*2 + 2) - *a1 - 2**(a1 + size) - *(a1 + size*2)));
					
					Ax += Bx * Bx;
                    Ay += By * By;
                    Axy += Bx * By;
                }
            }

            float direction = PI / 2 - 0.5 * atan2_approximation2(2 * Axy, Ax - Ay);
            image->direct[x][y] = direction;
        }
    }
}

//Normalization helper functions

//----------------------To normal progress----------------------
float GetM	(Image *image)
{
	UINT32 temp = 0;

	for(int j=0;j<height;j++){
		for(int i=0;i<width;i++)				
		{
			temp = temp + image->data[i][j];
		}
	}	
	return (float)(temp/(UINT32)(width*height));
}

float GetV(Image *image, float GetM_value)
{
	float temp = 0;
	for(int j=0;j<height;j++)
		for(int i=0;i<width;i++)				
		{
			temp += (image->data[i][j] - GetM_value)*(image->data[i][j] - GetM_value);
		}
	return (float)(temp/(UINT32)(width*height));
}

VOID ToNornal(
		Image *image,
		UINT16 M, 
		UINT16 V
)
{	
	float m = GetM(image);
	float v = GetV(image, m);
	for(int i=0;i<width;i++)			
	{
		for(int j=0;j<height;j++)
		{
			UINT8 tempData = image->data[i][j];
			if(tempData>m)
			{
				image->data[i][j] = (UINT32)(M + sqrt((tempData-m)*(tempData-m)*V/v));
			}
			else
			{
				image->data[i][j] = (UINT32)(M - sqrt((tempData-m)*(tempData-m)*V/v));
			}
		}
	}			
}
//---------------------------------------------------------------


//-----------------------To binary progress----------------------

 VOID MaskGabor(MaskFilter *Mask, UINT8 widthSquare,float filterDirect,float f,UINT8 fi)
{
	float sinn = sin(filterDirect);
	float coss = cos(filterDirect);
	for(int x=0;x<2*widthSquare+1;x++){
		for(int y=0;y<2*widthSquare+1;y++)
		{
			float x1 = sinn*(x-widthSquare) + coss*(y-widthSquare);
			float y1 = sinn*(y-widthSquare) - coss*(x-widthSquare);
			Mask->mask[x][y] = exp(-0.5*(pow(x1,2)/pow(fi,2) + pow(y1,2)/pow(fi,2)))*cos(2*PI*f*x1);				
		}
		// Mask->width = widthSquare*2 + 1;
	}
		
}

VOID CalMaskFilter(MaskFilter *Mask){
	
	for(int i=0; i < MaskNumber; i++)
	{
		MaskGabor(&Mask[i],WidthSquare,i*PI/MaskNumber,1.0/f_para,fi_para);
	}
}

VOID ToFiltring(Image *image, UINT16 widthSquare){
	float pointValue = 0;
	//GetClock("before GetMaskFilter");
	MaskFilter Mask[32];
	CalMaskFilter(Mask);
	for(int x=widthSquare + 1; x < width - widthSquare  - 1; x++){
		for(int y=widthSquare + 1; y < height - widthSquare - 1; y++)
		{			
			UINT8 index = (UINT8) (image->direct[x][y]/(PI/MaskNumber));
			index = (index>31) ? 31 : index;
			
			for(int i=0; i < 2*widthSquare + 1; i++)
			{
				for(int j=0; j < 2*widthSquare + 1; j++)
				{
					pointValue += Mask[index].mask[i][j] * image->data[i+x][j+y];					
				}
			}
			if(pointValue<0)
				pointValue = 0;
			if(pointValue>255)
				pointValue = 255;
			image->data[x][y] = (UINT8)(pointValue);
		}
	}	
	// free(Mask);
	//GetClock("after GetMaskFilter");
}		
		

//---------------------------------------------------------------

//-----------------------To binary progress----------------------

float GetF(Image *image, int g){
	int GetT_g = 0;
	int temp_g = 0;
	int temp_255 = 0;
	int x,y;
	for(x=0;x<width;x++){
		for(y=0;y<height;y++){
			if(image->data[x][y]<=g){
				GetT_g++;
				temp_g += image->data[x][y];
			}	
			if(image->data[x][y]<=255){
				temp_255 += image->data[x][y];
			}
		}			
	}
	float GetMbinary_g = temp_g/GetT_g;
	float GetMbinary_255 = temp_255/GetT_g;
	return (float)(GetT_g*(GetMbinary_g - GetMbinary_255)*(GetMbinary_g - GetMbinary_255)) / (float)((width*height)-GetT_g);
}

UINT16 GetThreshold(Image *image)
{
	int threshold = 0;
	float tempF = 0;
	for(int g=125;g<=255;g++){
		float F = GetF(image,g);
		if( F >tempF)
		{
			tempF = F;
			threshold = g;
		}
	}	
	return threshold;		
}

VOID ToBinary(
	Image *image,
	UINT16 thresh,
	UINT16 widthSquare
)
{
	UINT16 thresthold;
	if(thresh > 0 && thresh<255)
		thresthold = thresh;
	else {
		thresthold = GetThreshold(image) - 10;
	}
	//printf("%d ----------- \n",thresthold);
	for(int j = widthSquare + 1;j<height -(widthSquare + 1);j++)			
	{
		for(int i=widthSquare + 1;i<width-(widthSquare + 1);i++)	
		{
			if(image->data[i][j] >= thresthold)
				image->data[i][j] = 255;
			else
				image->data[i][j] = 0;
		}
	}		
}

//---------------------------------------------------------------

//-----------------------To RidgeThin progress----------------------

void RidgeThin(Image *image) {
    int i, j, p, q, r, s, count;
    int thinning = 1;
	int w= image->Width;
	int h = image->Height;
    while (thinning) {
        thinning = 0;
        for (i = 1; i < h - 1; i++) {
            for (j = 1; j < w - 1; j++) {
                if (image->data[i][j] == 255) {
                    p = image->data[i - 1][j - 1];
                    q = image->data[i - 1][j];
                    r = image->data[i - 1][j + 1];
                    s = image->data[i][j - 1];
                    count = p + q + r + s;
                    if (count >= 3*255 && count <= 4*255) {
                        p = image->data[i][j + 1];
                        q = image->data[i + 1][j + 1];
                        r = image->data[i + 1][j];
                        s = image->data[i + 1][j - 1];
                        count = p + q + r + s;
                        if (count >= 2*255 && count <= 3*255) {
                            image->data[i][j] = 0;
                            thinning = 1;
                        }
                    }
                }
            }
        }
    }
}
BOOLEAN IsBoder(Image *image, int x, int y)
{
	if((image->data[x-1][y]==255||image->data[x+1][y]==255||image->data[x][y-1]==255||image->data[x][y+1]==255)&&image->data[x][y]==0)
		return true;
	return false;
}
BOOLEAN IsDeleable(Image *image, int x,int y)
{
	// if((image->data[x-1][y]==255||image->data[x+1][y]==255||image->data[x][y-1]==255||image->data[x][y+1]==255)&&image->data[x][y]==0){
	int p1 = image->data[x-1][y-1];
	int p2 = image->data[x  ][y-1];
	int p3 = image->data[x+1][y-1];
	int p4 = image->data[x-1][y  ];
	int p5 = image->data[x+1][y  ];
	int p6 = image->data[x-1][y+1];
	int p7 = image->data[x  ][y+1];
	int p8 = image->data[x+1][y+1];

	if((p1+p2+p3)==0&&(p6*p7*p8>0)) return true;
	if((p1+p2+p4)==0&&(p5*p7*p8>0)) return true;
	if((p1+p4+p6)==0&&(p3*p5*p8>0)) return true;
	if((p2+p3+p5==0)&&(p4*p6*p7>0)) return true;
	if((p6+p7+p8==0)&&(p1*p2*p3>0)) return true;
	if((p5+p7+p8==0)&&(p1*p2*p4>0)) return true;
	if((p3+p5+p8)==0&&(p1*p4*p6>0)) return true;
	if((p4+p6+p7)==0&&(p2*p3*p5>0)) return true;

	return false;
	// }
}
VOID MakeBone(Image *image)
{
	BOOLEAN isBone = false;
	while(!isBone)
	{
		isBone = true;
		for(int j=1;j<height-1;j++)				
		{
			for(int i=1;i<width-1;i++)	
			{
				if(IsBoder(image, i, j))
				{
					if(IsDeleable(image, i, j))
					{
						image->data[i][j]=255;
						isBone = false;
					}
				}
			}
		}
	}
}
VOID ClearBone(Image *image)
{
	for(int y=1;y<height-1;y++)			
		for(int x=1;x<width-1;x++)	
		{
			if(image->data[x][y]==0)
			{
				if(image->data[x-1][y-1]==0) 
				{
					image->data[x][y-1]=255;
					image->data[x-1][y]=255;
				}
				if(image->data[x+1][y-1]==0)
				{
					image->data[x][y-1]=255;
					image->data[x+1][y] = 255;
				}
				if(image->data[x+1][y+1]==0)
				{
					image->data[x+1][y]=255;
					image->data[x][y+1]=255;
				}
				if(image->data[x-1][y+1]==0)
				{
					image->data[x-1][y]=255;
					image->data[x][y+1]=255;
				}
			}
		}
}

VOID ToBoneImage(Image *image)
{
	MakeBone(image);
	ClearBone(image);
}

//---------------------------------------------------------------


//-----------------------Minutiae finding progress----------------------
//-----------------------Minutiae finding progress----------------------
INT8 IsFeature(Image *image , int x,int y)
{
	if (x > 0 && x < width && y > 0 && y < height && image->data[x][y] == 0)
	{
		int result = image->data[x - 1][ y - 1] + image->data[x - 1][ y] + image->data[x - 1][ y + 1] + image->data[x][ y + 1] + image->data[x + 1][ y + 1] + image->data[x + 1][ y] + image->data[x + 1][ y - 1] + image->data[x][ y - 1];
		if (result == 7 * 255){
			
			return 2;
		} 
		if (result == 5 * 255){

			return 1;
		}
		return 0;
	}
	return 0;
}

VOID GetMinutiae 
(
	SpecialPoint *minus, 
	Image *image, 
	UINT8 leftPading,
	UINT8 topPadding,
	UINT8 rightPadding,
	UINT8 bottomPadding
)
{
	int cnt = 0;
	for(int x=leftPading;x<width-rightPadding;x++)
		for(int y=topPadding;y<height-bottomPadding;y++)
		{
			int temp = IsFeature(image,x,y);
			if(temp){
				minus->minus[cnt].direct = image->direct[x][y];
				minus->minus[cnt].x = x;
				minus->minus[cnt].y = y;
				minus->minus[cnt].Type = temp;
				cnt++;				
			}
		}
	minus->Count = cnt;
	for(int i = 0; i < minus ->Count; i++){

	}
	for(int i=0 ; i < minus->Count-1 ;i++)
	{
		for(int j=i+1; j <minus->Count;j++)
		{
			UINT32 Distance = (UINT32)(sqrt(pow(minus->minus[i].x-minus->minus[j].x,2) + pow(minus->minus[i].y-minus->minus[j].y,2)));
			if(Distance <= 2){
								//minus1.RemoveAt(j);
				for(int b = j; b < minus->Count ; b++)//dich chuyen mang
				{
					minus->minus[b] = minus->minus[b+1];        
				}
				minus->Count--;
				// minus1.RemoveAt(i);
				for(int a = i; a < minus->Count ; a++)//dich chuyen mang
				{
				 	minus->minus[a] = minus->minus[a+1];        
				}
				minus->Count--;
				if(i>0)
				{
					
					j = j-2;							
					i--;
					break;
				}
				else
				{
					if(j>1)
						j--;
					break;
				}	
			}
			else if((Distance > 2 && Distance < 6 && minus->minus[i].Type == minus->minus[j].Type)  || (Distance > 2 && Distance<10 && minus->minus[i].Type != minus->minus[j].Type))
			{
				//minus1.RemoveAt(j);
				for(int b = j; b < minus->Count ; b++)//dich chuyen mang
				{
					minus->minus[b] = minus->minus[b+1];        
				}
				minus->Count--;
				//minus1.RemoveAt(i);
				for(int a = i; a < minus->Count ; a++)//dich chuyen mang
				{
					minus->minus[a] = minus->minus[a+1];        
				}
				minus->Count--;
				if(i>0)
				{
					
					j = j-2;							
					i--;
					break;
				}
				else
				{
					if(j>1)
						j--;
					break;
				}	
			}
		}
	}
}
//---------------------------------------------------------------
unsigned long long factorial(unsigned int num) {
    if (num == 0)
        return 1;
    else
        return num * factorial(num - 1);
}
GroupDataSpecialPoint* GetTriangle(SpecialPoint *ListSpecialPoint, GroupDataSpecialPoint *ListTriangle){
	UINT8 CountSpecialPoint = ListSpecialPoint->Count;
	int NumElement = factorial(CountSpecialPoint) / (6 * factorial(CountSpecialPoint - 3));   
	ListTriangle = (GroupDataSpecialPoint*)malloc(NumElement * sizeof(GroupDataSpecialPoint));
	int counter = 0;
	float dx1;
    float dy1;
    float dx2;
    float dy2;
    float dx3;
    float dy3;

	for(int i = 0; i < CountSpecialPoint -2; i++ ){

        for(int j = i+1; j< CountSpecialPoint - 1; j++){
            
            for(int k = j+1; k < CountSpecialPoint ; k++){

                dx1 = ListSpecialPoint->minus[j].x - ListSpecialPoint->minus[i].x;
                dy1 = ListSpecialPoint->minus[j].y - ListSpecialPoint->minus[i].y;

                dx2 = ListSpecialPoint->minus[k].x - ListSpecialPoint->minus[i].x;
                dy2 = ListSpecialPoint->minus[k].y - ListSpecialPoint->minus[i].y;

                dx3 = ListSpecialPoint->minus[k].x - ListSpecialPoint->minus[j].x;
                dy3 = ListSpecialPoint->minus[k].y - ListSpecialPoint->minus[j].y;


                ListTriangle[counter].bord1 = sqrt(dx1*dx1 + dy1*dy1);
                ListTriangle[counter].bord2 = sqrt(dx2*dx2 + dy2*dy2);
                ListTriangle[counter].bord3 = sqrt(dx3*dx3 + dy3*dy3);

                ListTriangle[counter].status = 0;
                ListTriangle[counter].id = i*100 + j*10 + k;
                counter+=1;
            }
        }
    }
	ListTriangle->numberOfTriangle = counter;                       printf("%d+----------------\n",ListTriangle->numberOfTriangle);
	return ListTriangle;
}