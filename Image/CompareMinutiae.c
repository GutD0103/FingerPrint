#include "Image.h"


//-----------------------Compare progress----------------------

Minutiae GetMinutiaeAfterChange(Minutiae m, int deltaX,int deltaY,double angleRotation, int xRoot,int yRoot)
{
    int xCurrent = m.x - xRoot;
    int yCurrent = yRoot - m.y;
    int _x = (int)(xCurrent*cos(angleRotation) - yCurrent*sin(angleRotation)) + deltaX;
    int _y = (int)(xCurrent*sin(angleRotation) + yCurrent*cos(angleRotation)) + deltaY;
    double _direct = m.direct + angleRotation;
    if(_direct>PI)
        _direct = _direct - PI;

    Minutiae result;
    result.x = xRoot + _x;
    result.y = yRoot - _y;
    result.direct = _direct;
    return result;
}

BOOLEAN IsMinutiaeMatching(Minutiae m1 , Minutiae m2 ,Minutiae *minuChanging)
{			
    Minutiae m = GetMinutiaeAfterChange(m2, minuChanging->x,minuChanging->y,minuChanging->direct,128,128);	
    int distance = (int)(sqrt(pow(m.x-m1.x,2) + pow(m.y-m1.y,2)));
    double angleRotate = min(abs(m.direct-m1.direct),PI*2-abs(m.direct-m1.direct));
    if((distance<distanceLimit)&&(angleRotate<angleLimit))
            return true;
        return false;
}

int CountMinuMatching(SpecialPoint *minuSet1,SpecialPoint *minuSet2,Minutiae *minuChanging)
{			 
    int i,j;
    UINT8 mark[minuSet2->Count];
    ZeroMem(mark,minuSet2->Count);
    int count = 0;
    for(i=0;i<minuSet1->Count;i++)
    {
        Minutiae m1 = (Minutiae)minuSet1->minus[i];
        for(j=0;j<minuSet2->Count;j++)
        {					
            if(!mark[j])
            {
                Minutiae m2 = (Minutiae) minuSet2->minus[j];
                if(IsMinutiaeMatching(m1,m2,minuChanging))
                {
                    mark[j] = 1;
                    count++;
                    break;
                }
            }				
        }
    }
    
    // for(i=0;i<minuSet2->Count;i++)
    //     if(mark[i]) count++;
    return count;
}

VOID GetMinutiaeChanging_UseHoughTransform
        (
            SpecialPoint *minuSet1, 
            SpecialPoint *minuSet2,
            Minutiae     *minuResult
        )
{
    //int i;			
	//angles 

    UINT8 xRoot = (UINT8)MaxWidth/2;
    UINT8 yRoot = (UINT8)MaxHeight/2;
    float angleLimitRad = angleLimit*PI/180;

	INT8 angleStart = -30;
	UINT8 angleUnit = 10;
	INT8 angleFinish = 30;
	UINT8 anglesize = (UINT8)((angleFinish-angleStart)/angleUnit) + 1;

  
	INT8 angleSet[anglesize];
	UINT8 cnt = 0;
	INT8 angle = angleStart;
	while(cnt<anglesize)
	{
		angleSet[cnt] = angle*PI/180;
		cnt++;
		angle += angleUnit;
	}
	
    //DELTAXSET
	INT8 deltaXStart = -MaxWidth;
	INT8 deltaXFinish = MaxWidth;
	UINT8 deltaXUnit = 8;
	UINT8 deltaXsize = (UINT8)((deltaXFinish - deltaXStart)/deltaXUnit) + 1;
	INT8 deltaXSet[deltaXsize];
	cnt = 0;
	INT8 deltaX = deltaXStart;
    //DELTAYSET
	INT8 deltaYStart = -MaxHeight;
	INT8 deltaYFinish = MaxHeight;
	UINT8 deltaYUnit = 8;
	UINT8 deltaYsize = (UINT8)((deltaYFinish - deltaYStart)/deltaYUnit) + 1;
	INT8 deltaYSet[deltaYsize];
	
	INT8 deltaY = deltaYStart;
	while(cnt<deltaXsize)
	{
        #ifdef ImageIsSquare
        deltaYSet[cnt] = deltaY;
        #endif
		deltaXSet[cnt] = deltaX;
		cnt++;
        #ifdef ImageIsSquare
        deltaY += deltaYUnit;
        #endif
		deltaX += deltaXUnit;

	}
#ifndef ImageIsSquare	
    cnt = 0;
	while(cnt<deltaYsize)
	{
		deltaYSet[cnt] = deltaY;
		cnt++;
		deltaY += deltaYUnit;
	}
#endif	
    int i,j,k;
 

    UINT8 Arr[deltaXsize][deltaYsize][anglesize];
    Minutiae m1;
    Minutiae m2;
    float _deltaX;
    float _deltaY;	
    ZeroMem(Arr,deltaXsize*deltaYsize*anglesize);
    UINT8 max =0;
    for(i=0;i<minuSet1->Count;i++)
    {
        m1 = (Minutiae)minuSet1->minus[i];
        for(j=0;j<minuSet2->Count;j++)
        {
            m2 = (Minutiae)minuSet2->minus[j];
            for(int angleIndex=0; angleIndex < anglesize; angleIndex++)					
            {
                double tempAngle = abs((m2.direct+angleSet[angleIndex])-m1.direct);
                if((tempAngle < angleLimitRad)||((PI*2 - tempAngle)<angleLimitRad))
                {
                    INT8 c1X = m1.x - xRoot;
                    INT8 c1Y = yRoot - m1.y;
                    
                    INT8 c2X = m2.x - xRoot;
                    INT8 c2Y = yRoot - m2.y;
                
                    _deltaX = c1X - (cos(angleSet[angleIndex])*c2X - sin(angleSet[angleIndex])*c2Y);
                    _deltaY = c1Y - (sin(angleSet[angleIndex])*c2X + cos(angleSet[angleIndex])*c2Y);

                    INT8 deltaXSelect = deltaXSet[0];
                    INT8 deltaXIndexSelect = 0;
                    INT8 deltaYSelect = deltaYSet[0];
                    INT8 deltaYIndexSelect = 0;

                    for(int deltaXIndex=0; deltaXIndex < deltaXsize; deltaXIndex++){
                        if(abs(deltaXSet[deltaXIndex]-_deltaX) < abs(deltaXSelect-_deltaX))
                        {
                            deltaXSelect = deltaXSet[deltaXIndex];
                            deltaXIndexSelect = deltaXIndex;
                        }
                        #ifdef ImageIsSquare
                        if(abs(deltaYSet[deltaXIndex]-_deltaY) < abs(deltaYSelect-_deltaY))
                        {
                            deltaYSelect = deltaYSet[deltaXIndex];
                            deltaYIndexSelect = deltaXIndex;
                        }
                        #endif
                    }							
                    #ifndef ImageIsSquare   
                    for(int deltaYIndex=0 ;deltaYIndex < deltaYsize; deltaYIndex++)								
                        if(abs(deltaYSet[deltaYIndex]-_deltaY) < abs(deltaYSelect-_deltaY))
                        {
                            deltaYSelect = deltaYSet[deltaYIndex];
                            deltaYIndexSelect = deltaYIndex;
                        }
                    #endif
                    Arr[deltaXIndexSelect][deltaYIndexSelect][angleIndex] +=1;
                    if(Arr[deltaXIndexSelect][deltaYIndexSelect][angleIndex] > max){
                        max = Arr[deltaXIndexSelect][deltaYIndexSelect][angleIndex];
                        minuResult->x = deltaXSet[deltaXIndexSelect];
                        minuResult->y = deltaYSet[deltaYIndexSelect];
                        minuResult->direct = angleSet[angleIndex];
                    }
                }
            }
        }
    }
}


UINT8 CompairMinutiae(
    SpecialPoint *minus1,
    SpecialPoint *minus2
)
{
    Minutiae minuResult;
   // GetClock("Start GetMinutiaeChanging_UseHoughTransform");
	GetMinutiaeChanging_UseHoughTransform(minus1,minus2, &minuResult);
    //GetClock("End GetMinutiaeChanging_UseHoughTransform");

    //GetClock("Start CountMinuMatching");
	int count = CountMinuMatching(minus1,minus2,&minuResult);
    //GetClock("end CountMinuMatching");

	// int count = GetMinutiaeChanging_UseHoughTransform(&minus1,&minus2);
    UINT8 minuNumberCompair = (UINT8) (0.5 * min(minus1->Count, minus2->Count));
    // UINT8 minuNumberCompair = (UINT8) round (min(minus1->Count, minus2->Count));
    printf("\nminus1.Count = %d , minus2.Count = %d \n",minus1->Count,minus2->Count);
    printf("count = %d, minuNumberCompair = %d \n", count, minuNumberCompair);
	
    if(count>=minuNumberCompair){
        printf("Hai van tay trung khop\n");
        return 1;
    }else{
        printf("Hai van tay KHONG trung khop\n");
        return 0;
    }		
}

//---------------------------------------------------------------

// -------------------------------------------------------
UINT8 CompairMinutiae_V3(
    GroupDataSpecialPoint *group1,
    GroupDataSpecialPoint *group2,
    UINT16 NumTriangle1,
    UINT16 NumTriangle2
){
    UINT8 _distanceLimit = 2;


    UINT16 counter = 0;
    UINT16 result = 0;
    printf("So tam giac group1 la: %ld\n", NumTriangle1);
    printf("So tam giac group2 la: %ld\n", NumTriangle2);
    
    UINT8 mangIJK[NumTriangle1*3];
    for(int i = 0; i < NumTriangle1; i++ ){
        for(int j = 0; j< NumTriangle2; j++){
            counter+=1;
            if(group2[j].status == 0){

                if(( fabs(group1[i].bord1 - group2[j].bord1)  <= _distanceLimit  )
                && ( fabs(group1[i].bord2 - group2[j].bord2)  <= _distanceLimit  )
                && ( fabs(group1[i].bord3 - group2[j].bord3)  <= _distanceLimit  )
                ){
                    mangIJK[result*3] = group1[i].id/100;
                    mangIJK[result*3 + 1] = (group1[i].id%100)/10;
                    mangIJK[result*3 + 2] = ((group1[i].id%100)%10);
                    result+=1;
                    group2[j].status = 1;
                    break;
                }

                else if(( fabs(group1[i].bord1 - group2[j].bord1)  <= _distanceLimit  )
                && ( fabs(group1[i].bord2 - group2[j].bord3)  <= _distanceLimit  )
                && ( fabs(group1[i].bord3 - group2[j].bord2)  <= _distanceLimit  )
                ){
                    mangIJK[result*3] = group1[i].id/100;
                    mangIJK[result*3 + 1] = (group1[i].id%100)/10;
                    mangIJK[result*3 + 2] = ((group1[i].id%100)%10);
                    result+=1;
                    group2[j].status = 1;
                    break;
                }
                
                else if(( fabs(group1[i].bord1 - group2[j].bord2)  <= _distanceLimit  )
                && ( fabs(group1[i].bord2 - group2[j].bord1)  <= _distanceLimit  )
                && ( fabs(group1[i].bord3 - group2[j].bord3)  <= _distanceLimit  )
                ){
                    mangIJK[result*3] = group1[i].id/100;
                    mangIJK[result*3 + 1] = (group1[i].id%100)/10;
                    mangIJK[result*3 + 2] = ((group1[i].id%100)%10);
                    result+=1;
                    group2[j].status = 1;
                    break;
                }

                else if(( fabs(group1[i].bord1 - group2[j].bord2)  <= _distanceLimit  )
                && ( fabs(group1[i].bord2 - group2[j].bord3)  <= _distanceLimit  )
                && ( fabs(group1[i].bord3 - group2[j].bord1)  <= _distanceLimit  )
                ){
                    mangIJK[result*3] = group1[i].id/100;
                    mangIJK[result*3 + 1] = (group1[i].id%100)/10;
                    mangIJK[result*3 + 2] = ((group1[i].id%100)%10);
                    result+=1;
                    group2[j].status = 1;
                    break;
                }

                else if(( fabs(group1[i].bord1 - group2[j].bord3)  <= _distanceLimit  )
                && ( fabs(group1[i].bord2 - group2[j].bord1)  <= _distanceLimit  )
                && ( fabs(group1[i].bord3 - group2[j].bord2)  <= _distanceLimit  )
                ){
                    mangIJK[result*3] = group1[i].id/100;
                    mangIJK[result*3 + 1] = (group1[i].id%100)/10;
                    mangIJK[result*3 + 2] = ((group1[i].id%100)%10);
                    result+=1;
                    group2[j].status = 1;
                    break;
                }

                else if(( fabs(group1[i].bord1 - group2[j].bord3)  <= _distanceLimit  )
                && ( fabs(group1[i].bord2 - group2[j].bord2)  <= _distanceLimit  )
                && ( fabs(group1[i].bord3 - group2[j].bord1)  <= _distanceLimit  )
                ){
                    mangIJK[result*3] = group1[i].id/100;
                    mangIJK[result*3 + 1] = (group1[i].id%100)/10;
                    mangIJK[result*3 + 2] = ((group1[i].id%100)%10);
                    result+=1;
                    group2[j].status = 1;
                    break;
                }


            }
        }
    }

    INT8 sodiemtrung=0;

    if(result>0){
        sodiemtrung +=1;
    }

    INT8 coincide = 0;
    for(int i = 1; i < result*3; i++){
        coincide = 0;
        for(int j = i-1; j >= 0; j--){
            if(mangIJK[i] == mangIJK[j]){
                coincide = 1;
            }
        }
        if(coincide == 0){
            sodiemtrung +=1;
        }
    }

     printf("So tam giac trung nhau la: %d\n", result);
     printf("So diem trung nhau theo phuong phap 2 la: %d\n", sodiemtrung);
     return sodiemtrung;
}




