
#include "Image.h"
#include "Memory.h"
#include "newMath.h"
const UINT16 height = MaxHeight;
const UINT16 width = MaxWidth;


INT8 CheckQuality(Image *image)
{
	for(int row = 7; row < MaxHeight - 7; row+=14)
	{
		for(int col = 7; col < MaxWidth - 7; col+=14)
		{
			int SumGrayLevel = 0;
			int count = 0;
			for(int i = row - 7; i < row + 7 + 1; i++)
			{
				for(int j = col - 7; j < col + 7 + 1; j++)
				{
					SumGrayLevel+= image->data[i][j];
					count++;
				}
			}
			float Mean = (float)SumGrayLevel/count;
			//printf("Sum: %d  Count: %d OUTPUT: %f\n",SumGrayLevel, count, Mean);
			if(Mean > 185 || Mean < 50)
			{
				return 0;
			}
		}
	}
	return 1;
}

float TangentDir(Image *image, int x, int y) {
    float A = 0;
    float B = 0;
    float C = 0;
    int size = MaxWidth;

    for (int _x = x - WidthSquare; _x <= x + WidthSquare; _x++) {
        for (int _y = _y - WidthSquare; _y <= y + WidthSquare; _y++) {
            UINT8 *subImage = &(image->data[_x][_y]);
            // double ahk = (-I[h - 1][k - 1] + I[h - 1][k + 1] + I[h + 1][k + 1] - I[h + 1][k - 1]) / 4;
            // double bhk = (-I[h - 1][k - 1] - I[h - 1][k + 1] + I[h + 1][k + 1] + I[h + 1][k - 1]) / 4;
            // Bx = ((*(SubImage + size*2) + 2**(SubImage + size*2 + 1) + *(SubImage + size*2 + 2) - *SubImage - 2**(SubImage + 1) - *(SubImage + 2)));
			// By = ((*(SubImage + 2) + 2**(SubImage + size + 2) + *(SubImage + size*2 + 2) - *SubImage - 2**(SubImage + size) - *(SubImage + size*2)));
				
            float ahk = ( - *(subImage - size - 1) + *(subImage - size + 1) + *(subImage + size + 1) - *(subImage + size - 1)) / 4;
            float bhk = ( - *(subImage - size - 1) - *(subImage - size + 1) + *(subImage + size + 1) + *(subImage + size - 1)) / 4;
            A += ahk*ahk;
            B += bhk*bhk;
            C += ahk * bhk;
        }
    }

    float t1, t2, phit;
    if (C > 0) {
        t1 = 1;
        t2 = (B - A) / (2 * C) - sqrt(pow((B - A) / (2 * C), 2) + 1);
    } else if (C < 0) {
        t1 = 1;
        t2 = (B - A) / (2 * C) + sqrt(pow((B - A) / (2 * C), 2) + 1);
    } else if (A <= B) {
        t1 = 1;
        t2 = 0;
    } else {
        t1 = 0;
        t2 = 1;
    }

    if (t1 == 0) {
        phit = PI / 2;
    } else {
        phit = atan(t2 / t1);
    }

    return phit;
}
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
					UINT8 *SubImage = &(image->data[i ][j ]);
                    //Bx = ((data[i + 2][j] + 2 * data[i + 2][j + 1] + data[i + 2][j + 2] - data[i][j] - 2 * data[i][j + 1] - data[i][j + 2]));
                    //By = ((data[i][j + 2] + 2 * data[i + 1][j + 2] + data[i + 2][j + 2] - data[i][j] - 2 * data[i + 1][j] - data[i + 2][j]));
                   
				    Bx = ((*(SubImage + size*2) + 2**(SubImage + size*2 + 1) + *(SubImage + size*2 + 2) - *SubImage - 2**(SubImage + 1) - *(SubImage + 2)));
					By = ((*(SubImage + 2) + 2**(SubImage + size + 2) + *(SubImage + size*2 + 2) - *SubImage - 2**(SubImage + size) - *(SubImage + size*2)));
					
					Ax += Bx * Bx;
                    Ay += By * By;
                    Axy += Bx * By;
                }
            }

            float direction = PI/2 - 0.5 * atan2_approximation2(2 * Axy, Ax - Ay);
            image->direct[x][y] = direction;
        }
    }
}

//Normalization helper functions

//----------------------To normal progress----------------------
float GetM(Image *image) {
    UINT32 temp = 0;
    
    for (int j = 0; j < height; j++) 
	{
        for (int i = 0; i < width; i++) 
		{
            temp = temp + image->data[i][j];
        }
    }
    
    return (float)temp / (width * height);
}

float GetV(Image *image, float GetM_value) {
    float temp = 0;
    
    for (int j = 0; j < height; j++) 
	{
        for (int i = 0; i < width; i++) 
		{
            temp += (image->data[i][j] - GetM_value) * (image->data[i][j] - GetM_value);
        }
    }
    
    return (float)temp / (width * height);
}

VOID ToNornal(
		Image *image,
		UINT16 M, 
		UINT16 V
)
{	
//-------------------------------------------------------

	// float m = GetM(image);
	// float v = GetV(image, m);
	// for(int i=0;i<width;i++)
	// {
	// 	for(int j=0;j<height;j++)
	// 	{
	// 		UINT8 tempData = image->data[i][j];
	// 		if(tempData>m)
	// 		{
	// 			image->data[i][j] = (UINT32)(M + sqrt((tempData-m)*(tempData-m)*V/v));
	// 		}
	// 		else
	// 		{
	// 			image->data[i][j] = (UINT32)(M - sqrt((tempData-m)*(tempData-m)*V/v));
	// 		}
	// 	}
	// }
//------------------------------------
	Image tempImage;
    tempImage.Height = image->Height;
    tempImage.Width = image->Width;

    for (int i = 1; i < image->Width - 1; i++) 
	{
        for (int j = 1; j < image->Height - 1; j++) 
		{
            UINT8 neighbors[9];
            int k = 0;

            for (int dx = -1; dx <= 1; dx++) 
			{
                for (int dy = -1; dy <= 1; dy++) 
				{
                    neighbors[k++] = image->data[i + dx][j + dy];
                }
            }

            // Sắp xếp mảng neighbors để lấy giá trị trung vị
            for (int m = 0; m < 9; m++) 
			{
                for (int n = m + 1; n < 9; n++) 
				{
                    if (neighbors[m] > neighbors[n]) 
					{
                        UINT8 temp = neighbors[m];
                        neighbors[m] = neighbors[n];
                        neighbors[n] = temp;
                    }
                }
            }

            tempImage.data[i][j] = neighbors[4]; // Giá trị trung vị
        }
    }

    // Sao chép kết quả vào trường dữ liệu gốc
    for (int i = 1; i < image->Width - 1; i++) 
	{
        for (int j = 1; j < image->Height - 1; j++) 
		{
            image->data[i][j] = tempImage.data[i][j];
        }
    }

}
//---------------------------------------------------------------
VOID Mask_Gabor(MaskFilter *Mask, UINT8 widthSquare,float filterDirect,float f,UINT8 fi)
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
		Mask_Gabor(&Mask[i],WidthSquare,i*PI/MaskNumber,1.0/f_para2,fi_para);
	}
}

VOID ToFiltring_Gabor(Image *image, UINT16 widthSquare){
	float pointValue = 0;
	//GetClock("before GetMaskFilter");
	MaskFilter Mask[MaskNumber];
	CalMaskFilter(Mask);
	for(int x=widthSquare + 1; x < width - widthSquare  - 1; x++){
		for(int y=widthSquare + 1; y < height - widthSquare - 1; y++)
		{			
			UINT8 index = (UINT8) (image->direct[x][y]/(PI/MaskNumber));
			index = (index>(MaskNumber-1)) ? (MaskNumber-1) : index;
			
			for(int i = -widthSquare; i < widthSquare + 1; i++)
			{
				for(int j = -widthSquare; j < widthSquare + 1; j++)
				{
					pointValue += Mask[index].mask[i+widthSquare][j+widthSquare] * image->data[i+x][j+y];					
				}
			}
			if(pointValue<0)
				pointValue = 0;
			if(pointValue>255)
				pointValue = 255;
			image->data[x][y] = (UINT8)(pointValue);
		}
	}		
}
//-----------------------To binary progress----------------------

 VOID MaskGaussin(MaskFilter *Mask, UINT8 widthSquare,float sigma)
{
    float twoSigmaSquared = 2 * sigma * sigma;
    float sum = 0.0;
    
    for(int x = 0; x < 2 * widthSquare + 1; x++) 
	{
        for(int y = 0; y < 2 * widthSquare + 1; y++) 
		{
            float distanceSquared = (x - widthSquare) * (x - widthSquare) + (y - widthSquare) * (y - widthSquare);
            Mask->mask[x][y] = -(1 - distanceSquared / twoSigmaSquared) * exp(-distanceSquared / twoSigmaSquared);
            sum += (Mask->mask[x][y]);
        }
    }
    
    // Normalize the mask
    for(int x = 0; x < 2 * widthSquare + 1; x++) 
	{
        for(int y = 0; y < 2 * widthSquare + 1; y++) 
		{
            Mask->mask[x][y] /= sum;
        }
    }
}


VOID ToFiltring_Gaussin(Image *image, UINT16 widthSquare)
{
	float pointValue = 0;
	MaskFilter Mask;
	MaskGaussin(&Mask,WidthSquare,1.0/f_para);
	for(int x=widthSquare + 1; x < width - widthSquare  - 1; x++)
	{
		for(int y=widthSquare + 1; y < height - widthSquare - 1; y++)
		{			
			
			for(int i= - widthSquare ; i < widthSquare + 1; i++)
			{
				for(int j= - widthSquare; j < widthSquare +1; j++)
				{
					pointValue += Mask.mask[i+widthSquare][j+widthSquare] * image->data[i+x][j+y];					
				}
			}
			
			if(pointValue<0)
				pointValue = 0;
			if(pointValue>255)
				pointValue = 255;

			image->data[x][y] = (UINT8)(pointValue);
		}
	}	
}		
VOID ToFiltring_GaussinV2(Image *image, UINT16 widthSquare)
{
	float pointValue = 0;
	MaskFilter Mask;
	MaskGaussin(&Mask,WidthSquare,1.0/f_para);
	for(int x=widthSquare + 1; x < width - widthSquare  - 1; x++)
	{
		for(int y=widthSquare + 1; y < height - widthSquare - 1; y++)
		{			
			
			for(int i= 0 ; i < 2*widthSquare + 1; i++)
			{
				for(int j= 0; j < 2*widthSquare +1; j++)
				{
					pointValue += Mask.mask[i][j] * image->data[i+x][j+y];					
				}
			}
			
			if(pointValue<0)
				pointValue = 0;
			if(pointValue>255)
				pointValue = 255;

			image->data[x][y] = (UINT8)(pointValue);
		}
	}	
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

void RidgeThin(Image *image) 
{
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
BOOLEAN IsBoder(Image *image, int x, int y, UINT8 isDeleable[][MaxHeight])
{
	int p1 = (isDeleable[x-1][y-1]) ? 255 : image->data[x-1][y-1];
	int p2 = (isDeleable[x  ][y-1]) ? 255 : image->data[x  ][y-1];
	int p3 = (isDeleable[x+1][y-1]) ? 255 : image->data[x+1][y-1];
	int p4 = (isDeleable[x-1][y  ]) ? 255 : image->data[x-1][y  ];
	int p5 = (isDeleable[x+1][y  ]) ? 255 : image->data[x+1][y  ];
	int p6 = (isDeleable[x-1][y+1]) ? 255 : image->data[x-1][y+1];
	int p7 = (isDeleable[x  ][y+1]) ? 255 : image->data[x  ][y+1];
	int p8 = (isDeleable[x+1][y+1]) ? 255 : image->data[x+1][y+1];

	int p = p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;

	if((p>= 255*3 && p<=255*6)&&(image->data[x][y]==0)&&
		((image->data[x+1][y]==255||image->data[x][y+1]==255||image->data[x-1][y]==255)
		&&(image->data[x][y+1]==255||image->data[x-1][y]==255||image->data[x][y-1]==255))
		)
		// ||(image->data[x+1][y]==255||image->data[x][y+1]==255||image->data[x][y-1]==255)
		// &&(image->data[x+1][y]==255||image->data[x-1][y]==255||image->data[x][y-1]==255)
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

		if((p1+p2+p3)==0&&(p6*p7*p8)>0) return true;
	if((p1+p2+p4)==0&&(p5*p7*p8)>0) return true;
		if((p1+p4+p6)==0&&(p3*p5*p8)>0) return true;
	if((p3+p2+p5)==0&&(p4*p7*p6)>0) return true;
		if((p6+p7+p8)==0&&(p1*p2*p3)>0) return true;
	if((p8+p5+p7)==0&&(p2*p4*p1)>0) return true;
		if((p3+p5+p8)==0&&(p1*p4*p6)>0) return true;
	if((p6+p4+p7)==0&&(p2*p5*p3)>0) return true;

	return false;
	// }
}
VOID MakeBone(Image *image)
{
	BOOLEAN isBone = false;
	UINT8 temp[MaxHeight][MaxWidth];
	ZeroMem(temp,sizeof(temp));
	while(!isBone)
	// for(int count = 0; count < 20; count++)
	{
		isBone = true;
		for(int j=1;j<height-1;j++)				
		{
			for(int i=1;i<width-1;i++)	
			{
				if(IsBoder(image, i, j, temp))
				{
					if(IsDeleable(image, i, j))
					{
						temp[i][j]=1;
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
	{	
		for(int x=1;x<width-1;x++)	
		{
			if(image->data[x][y]==0)
			{
				int p1 = image->data[x-1][y-1];
				int p2 = image->data[x  ][y-1];
				int p3 = image->data[x+1][y-1];
				int p4 = image->data[x-1][y  ];
				int p5 = image->data[x+1][y  ];
				int p6 = image->data[x-1][y+1];
				int p7 = image->data[x  ][y+1];
				int p8 = image->data[x+1][y+1];

				if((p2+p4)==0&&(p5*p7*p8)>0)
				{
					image->data[x][y] = 255;
				}
				if((p2+p5)==0&&(p4*p7*p6)>0)
				{
					image->data[x][y] = 255;
				}
				if((p5+p7)==0&&(p2*p4*p1)>0)
				{
					image->data[x][y] = 255;
				}
				if((p4+p7)==0&&(p2*p5*p3)>0)
				{
					image->data[x][y] = 255;
				}
				if(((p2+p5+p7) == 0) || ((p5+p7+p4) == 0) || ((p7+p4+p2) == 0) ||((p4+p2+p5) == 0))
				{
					image->data[x][y] = 255;
				}
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
		if (result == 7 * 255)
		{	
			return 1;
		} 
		if (result == 5 * 255)
		{
			return 2;
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
	{
		for(int y=topPadding;y<height-bottomPadding;y++)
		{
			int temp = IsFeature(image,x,y);
			if(temp)
			{
				minus->minus[cnt].direct = image->direct[x][y];
				minus->minus[cnt].x = x;
				minus->minus[cnt].y = y;
				minus->minus[cnt].Type = temp;
				cnt++;				
			}
		}
	}
	minus->Count = cnt;

	// for(int i = 0; i < minus->Count; i++)
	// {
	// 	int widthsquare1 = 3;
	// 	int left = minus->minus[i].x - widthsquare1;
	// 	int right = minus->minus[i].x + widthsquare1;
	// 	int top = minus->minus[i].y + widthsquare1;
	// 	int bottom = minus->minus[i].y - widthsquare1;

	// 	int ZeroTurnOne = 0;

	// 	while (left <= right)
	// 	{
	// 		if(image->data[left][minus->minus[i].y+widthsquare1] == 0 && image->data[left+1][minus->minus[i].y+widthsquare1] == 255)
	// 		{
	// 			ZeroTurnOne++;
	// 		}
	// 		if(image->data[left][minus->minus[i].y-widthsquare1] == 0 && image->data[left+1][minus->minus[i].y-widthsquare1] == 255)
	// 		{
	// 			ZeroTurnOne++;
	// 		}
	// 		left++;
	// 	}
	// 	while (bottom <= top)
	// 	{
	// 		if(image->data[minus->minus[i].x+widthsquare1][bottom] == 0 && image->data[minus->minus[i].x+widthsquare1][bottom+1] == 255)
	// 		{
	// 			ZeroTurnOne++;
	// 		}
	// 		if(image->data[minus->minus[i].x-widthsquare1][bottom] == 0 && image->data[minus->minus[i].x-widthsquare1][bottom+1] == 255)
	// 		{
	// 			ZeroTurnOne++;
	// 		}
	// 		bottom++;
	// 	}
		
	// 	if(minus->minus[i].Type == 1 && ZeroTurnOne != 1)
	// 	{
	// 		for(int a = i; a < minus->Count ; a++)
	// 		{
	// 		 	minus->minus[a] = minus->minus[a+1];        
	// 		}
	// 		minus->Count--;
	// 		i--;
	// 	}
	// 	else if(minus->minus[i].Type == 2 && ZeroTurnOne != 3)
	// 	{
	// 		for(int a = i; a < minus->Count ; a++)
	// 		{
	// 		 	minus->minus[a] = minus->minus[a+1];        
	// 		}
	// 		minus->Count--;
	// 		i--;
	// 	}
	// }
	for(int i=0 ; i < minus->Count-1 ;i++)
	{
		for(int j=i+1; j <minus->Count;j++)
		{
			UINT32 Distance = (UINT32)(sqrt(pow(minus->minus[i].x-minus->minus[j].x,2) + pow(minus->minus[i].y-minus->minus[j].y,2)));
			if(Distance <= 2)
			{
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
			else if((Distance > 2 && Distance < 6 && minus->minus[i].Type == minus->minus[j].Type && minus->minus[j].Type == 1))
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
			else if((Distance > 2 && Distance<6 && minus->minus[i].Type != minus->minus[j].Type) || (Distance > 2 && Distance<6 && minus->minus[i].Type == minus->minus[j].Type))
			{
				
				//minus1.RemoveAt(j);
				for(int b = j; b < minus->Count ; b++)//dich chuyen mang
				{
					minus->minus[b] = minus->minus[b+1];        
				}
				minus->Count--;
				//minus1.RemoveAt(i);
				// for(int a = i; a < minus->Count ; a++)//dich chuyen mang
				// {
				// 	minus->minus[a] = minus->minus[a+1];        
				// }
				// minus->Count--;
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
UINT64 factorial(UINT64 num) 
{
    if (num == 0)
        return 1;
    else
        return num * factorial(num - 1);
}

GroupDataSpecialPoint* GetTriangle(SpecialPoint *ListSpecialPoint, UINT16* NumTriangle)
{
	if(ListSpecialPoint->Count < 3 || ListSpecialPoint->Count > 15 ){
		//printf("Num of minutiae less than 3 or more than 15");
		return  NULL;
	}
	UINT16 CountSpecialPoint = ListSpecialPoint->Count;
	UINT16 NumElement = factorial(CountSpecialPoint) / (6 * factorial(CountSpecialPoint - 3));   
	GroupDataSpecialPoint *ListTriangle = (GroupDataSpecialPoint*)malloc(NumElement * sizeof(GroupDataSpecialPoint));
	UINT16 counter = 0;
	INT8 dx1;
    INT8 dy1;
    INT8 dx2;
    INT8 dy2;
    INT8 dx3;
    INT8 dy3;
      
	for(UINT8 i = 0; i < CountSpecialPoint -2; i++ ){

        for(UINT8 j = i+1; j< CountSpecialPoint - 1; j++){
            
            for(UINT8 k = j+1; k < CountSpecialPoint ; k++){
                dx1 = ListSpecialPoint->minus[i].x - ListSpecialPoint->minus[j].x;
                dy1 = ListSpecialPoint->minus[i].y - ListSpecialPoint->minus[j].y;

                dx2 = ListSpecialPoint->minus[j].x - ListSpecialPoint->minus[k].x;
                dy2 = ListSpecialPoint->minus[j].y - ListSpecialPoint->minus[k].y;

                dx3 = ListSpecialPoint->minus[k].x - ListSpecialPoint->minus[i].x;
                dy3 = ListSpecialPoint->minus[k].y - ListSpecialPoint->minus[i].y;


                ListTriangle[counter].bord1 = sqrt(dx1*dx1 + dy1*dy1);
                ListTriangle[counter].bord2 = sqrt(dx2*dx2 + dy2*dy2);
                ListTriangle[counter].bord3 = sqrt(dx3*dx3 + dy3*dy3);

                ListTriangle[counter].status = 0;
                ListTriangle[counter].id = 0;
				ListTriangle[counter].id =  ListTriangle[counter].id | i;
				ListTriangle[counter].id = (ListTriangle[counter].id <<8) | j;
				ListTriangle[counter].id = (ListTriangle[counter].id <<8) | k;
                counter+=1;
            }
        }
    }
	*NumTriangle = NumElement;     
	return ListTriangle;
}
