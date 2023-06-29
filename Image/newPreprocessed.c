#include "newPreprocessed.h"
int counter = 0;
float round1(float input){
    return floor(input+0.5);
}
VOID addMinutiae(SpecialPoint *ListMinutae,UINT8 x, UINT8 y, INT8 type, float direction){
    INT8 cnt = ListMinutae->Count;
    if(cnt < MaxSpecialPoint){
        ListMinutae->minus[cnt].direct = direction;
		ListMinutae->minus[cnt].x = x;
		ListMinutae->minus[cnt].y = y;
		ListMinutae->minus[cnt].Type = type;
        ListMinutae->Count++;
    }
}
VOID deleteMinutiae(SpecialPoint *ListMinutae,UINT8 x, UINT8 y){
    if(ListMinutae->Count > 0){
        for(int index = 0; index < ListMinutae -> Count; index++){
            if(ListMinutae->minus[index].x == x && ListMinutae->minus[index].y == y){
                for(int a = index; a < ListMinutae->Count ; a++)//dich chuyen mang
				{
				 	ListMinutae->minus[a] = ListMinutae->minus[a+1];        
				}
				ListMinutae->Count--;
                break;
            }
        }
    }
}
float TangentDir(Image *image, Point point) {
    float A = 0;
    float B = 0;
    float C = 0;
    int size = MaxWidth;

    for (int y = point.y - WidthSquare; y <= point.y + WidthSquare; y++) {
        for (int x = point.x - WidthSquare; x <= point.x + WidthSquare; x++) {
            UINT8 *subImage = &(image->data[y ][x ]);
            // double ahk = (-I[h - 1][k - 1] + I[h - 1][k + 1] + I[h + 1][k + 1] - I[h + 1][k - 1]) / 4;
            // double bhk = (-I[h - 1][k - 1] - I[h - 1][k + 1] + I[h + 1][k + 1] + I[h + 1][k - 1]) / 4;
				
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


Point localMax(Image *image, Point point, float direction) {
    float filter[] = { 1.0 / 23, 2.0 / 23, 5.0 / 23, 7.0 / 23, 5.0 / 23, 2.0 / 23, 1.0 / 23 };

    float LM[LengthRigde * 2 + 1];
    UINT8 x0[LengthRigde * 2 + 1];
    UINT8 y0[LengthRigde * 2 + 1];

    Point result;
    result.grayValue = 255;

    for (int cnt = -LengthRigde; cnt <= LengthRigde; cnt++) {
        INT8 x = (INT8)(point.x + cnt * cos(direction + PI / 2));
        INT8 y = (INT8)(point.y + cnt * sin(direction + PI / 2));
        INT8 xPrev = (INT8)(x - cos(direction));
        INT8 yPrev = (INT8)(y - sin(direction));
        INT8 xNext = (INT8)(x + cos(direction));
        INT8 yNext = (INT8)(y + sin(direction));

        double Io = (image->data[yPrev][xPrev] + image->data[y][x] + image->data[yNext][xNext]) / 3.0;
        LM[cnt + LengthRigde] = Io;
        x0[cnt + LengthRigde] = x;
        y0[cnt + LengthRigde] = y;
    }

    double LMn[LengthRigde * 2 + 1];
    for (int x = 4; x <= 2 * LengthRigde - 2; x++) {
        LMn[x] = LM[x - 3] * filter[0] + LM[x - 2] * filter[1] + LM[x - 1] * filter[2] + LM[x] * filter[3] +
                 LM[x + 1] * filter[4] + LM[x + 2] * filter[5] + LM[x + 3] * filter[6];

        if (result.grayValue > LMn[x]) {
            result.x = x0[x];
            result.y = y0[x];
            result.grayValue = (UINT8)LMn[x];
        }
    }
    

    return result;
}

Point ridgeNearest(Image *image, Point point) {


    double direction = TangentDir(image,point);
    INT8 x1 = round(point.x - (LengthRigde - 3) * cos(direction + PI / 2));
    INT8 y1 = round(point.y - (LengthRigde - 3) * sin(direction + PI / 2));

    Point _p1 ;
    _p1.x = x1;
    _p1.y = y1;

    _p1 = localMax(image, _p1, direction);

    INT8 x2 = round(point.x  + (LengthRigde - 3) * cos(direction + PI / 2));
    INT8 y2 = round(point.y  + (LengthRigde - 3) * sin(direction + PI / 2));

    Point _p2;
    _p2.x=x2;
    _p2.y=y2;

    _p2 = localMax(image, _p2 , direction);

    int d1 = (_p1.x - point.x) * (_p1.x - point.x) + (_p1.y - point.y) * (_p1.y - point.y);
    int d2 = (_p2.x - point.x) * (_p2.x - point.x) + (_p2.y - point.y) * (_p2.y - point.y);

                        
    if (d1 < d2) {

        return _p1;
    }

    return _p2;
}

/// @brief This function is used for following the ridge 
/// @param image  The main image
/// @param point Start point
VOID RidgeFollowing(Image *image, Point point, INT8 lengthJump, SpecialPoint *ListMinutiae) {

    int _lengthJump = (lengthJump > 0) ? lengthJump : -lengthJump;
    float dir0 = (lengthJump > 0) ? 0 : PI;
    float direction = TangentDir(image,point) + dir0;
    BOOLEAN esc = 1;
    INT8 type;

    Point nextPoint;
    while (esc) {

        nextPoint.x = (INT8) round(point.x + _lengthJump * cos(direction));
        nextPoint.y = (INT8) round(point.y + _lengthJump * sin(direction));
        direction = TangentDir(image,nextPoint) + dir0;

        nextPoint = localMax(image, nextPoint,direction);

        if (nextPoint.x > padding && nextPoint.x < MaxHeight - padding && nextPoint.y > padding && nextPoint.y < MaxHeight - padding) {

            if (nextPoint.x == point.x && nextPoint.y == point.y) {                
                return;
            } else {
                
                type = StopCriteria(image,nextPoint,direction,ListMinutiae);
                if(type != 1){
                    esc = 0;  
                }
            }
        }else {
            esc = 0;
        }

        point.x = nextPoint.x;
        point.y = nextPoint.y;
        
    }
}
INT8 StopCriteria(Image *image, Point point, float direction, SpecialPoint *ListMinutiae) {

        double phinDegrees = direction * 180 / PI;
        int I_threshold = 200;
        int flag = 0; // flag for check this point is a bifurcation
        int output; 
        int left = 6, right = 6, top = 6, bottom = 6;
        float sinn = sin(direction);
        float coss = cos(direction);
        for(int i = 1 ; i < LengthJump ; i++){
                int x = (INT8) round(point.x + i * coss);
                int y = (INT8) round(point.y + i * sinn);
                if (x < padding || x > MaxHeight - padding || y < padding || y > MaxWidth - padding)
                {
                    break;
                }
                
                if(image->isCheck[y][x] == 1){
                    flag = 1;
                }
        }
        

        if (flag) {
            BOOLEAN label = 1;
            for (int i = point.y - top; i <= point.y + bottom; i++) {
                for (int j = point.x - left; j <= point.x + right; j++) {
                    if ((image->isCheck[i][j] == 2) || (image->isCheck[i][j] == 3) ||(image->isCheck[i][j] == 8) ||(image->isCheck[i][j] == 9)) {
                        image->isCheck[i][j] = 9; 
                        image->isCheck[point.y][point.x] = 9;
                        deleteMinutiae(ListMinutiae,j,i);
                        label = 0;
                    }
                }
            }
            if (label){
                image->isCheck[point.y][point.x] = 3;
                addMinutiae(ListMinutiae,point.x,point.y,2,direction);
            }
        } else {

            for (int i = point.y - 1; i <= point.y + 1; i++) {
                for (int j = point.x - 1; j <= point.x + 1; j++) {
                    image->isCheck[i][j] = 1;
                }
            }
            if (image->data[point.x][point.y] > I_threshold && image->data[point.x + 2][point.y] > I_threshold && image->data[point.x - 2][point.y] > I_threshold
                && image->data[point.x][point.y + 2] > I_threshold && image->data[point.x][point.y - 2] > I_threshold) {
                int label = 1;
                for (int i = point.y - top; i <= point.y + bottom; i++) {
                    for (int j = point.x - left; j <= point.x + right; j++) {
                        if ((image->isCheck[i][j] == 2) || (image->isCheck[i][j] == 3) || (image->isCheck[i][j] == 8) ||(image->isCheck[i][j] == 9)) {
                            image->isCheck[i][j] = 8;
                            image->isCheck[point.y][point.x] = 8;
                            deleteMinutiae(ListMinutiae,j,i);
                            label = 0;
                        }
                    }
                }
                if (label)
                    image->isCheck[point.y][point.x] = 2;
                    addMinutiae(ListMinutiae,point.x,point.y,1,direction);
            }

        }

        return image->isCheck[point.y][point.x];
}
VOID GetMinutiae_v2(Image *image,SpecialPoint *ListMinutiae){

        int step = 2;

        for (int x = top1; x < MaxWidth - bottom1; x +=4) {
            for (int y = left1; y < MaxHeight - right1; y +=4) {

                    Point _ridgeNearest;
                    _ridgeNearest.x = x;
                    _ridgeNearest.y = y;
                    _ridgeNearest = ridgeNearest(image,_ridgeNearest);
                    BOOLEAN label = 1;

                    for (int i = _ridgeNearest.y - 4; i <= _ridgeNearest.y + 4; i++) {
                        for (int j = _ridgeNearest.x - 4; j <= _ridgeNearest.x + 4; j++) {
                            if (image->isCheck[i][j] == 1) {
                                label = 0;
                                i = 100;
                                j = 100;
                            }
                        }
                    }

                    if (label) {
                       RidgeFollowing(image, _ridgeNearest, LengthJump , ListMinutiae);
                       RidgeFollowing(image, _ridgeNearest, - LengthJump , ListMinutiae);
                    }
            }
        }
}