#include "newPreprocessed.h"
int counter = 0;
float round1(float input){
    return floor(input+0.5);
}
void DrawLine(Image *image, Point A, Point B) {
    if ((A.x >= MaxHeight) || (A.x < 0) || (A.y >= MaxHeight) || (A.y < 0) ||
        (B.x >= MaxHeight) || (B.x < 0) || (B.y >= MaxHeight) || (B.y < 0))
    {
        return;
    }
    
    int dx = abs(B.x - A.x);
    int dy = abs(B.y - A.y);
    int sx = (A.x < B.x) ? 1 : -1;
    int sy = (A.y < B.y) ? 1 : -1;
    int error = dx - dy;

    while (A.x != B.x || A.y != B.y) {
        image->isCheck[A.y][A.x] = 1;
        int e2 = 2 * error;
        if (e2 > -dy) {
            error -= dy;
            A.x += sx;
        }
        if (e2 < dx) {
            error += dx;
            A.y += sy;
        }
    }

    image->isCheck[B.y][B.x] = 1;

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


Point localMax(Image *image, Point point, float direction) {
    float filter[] = { 1.0 / 23, 2.0 / 23, 5.0 / 23, 7.0 / 23, 5.0 / 23, 2.0 / 23, 1.0 / 23 };

    float LM[LengthRigde * 2 + 1];
    UINT8 x0[LengthRigde * 2 + 1];
    UINT8 y0[LengthRigde* 2 + 1];

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

    double LMn[LengthRigde* 2 + 1];
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
    INT8 x1 = round(point.x - HALFLengthRigde * cos(direction + PI / 2));
    INT8 y1 = round(point.y - HALFLengthRigde * sin(direction + PI / 2));

    Point _p1 ;
    _p1.x = x1;
    _p1.y = y1;

    _p1 = localMax(image, _p1, direction);

    INT8 x2 = round(point.x  + HALFLengthRigde * cos(direction + PI / 2));
    INT8 y2 = round(point.y  + HALFLengthRigde * sin(direction + PI / 2));

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

    int _lengthJump = (lengthJump > 0) ? (lengthJump+1) : -(lengthJump+1);
    float dir0 = (lengthJump > 0) ? 0 : PI;
    float direction = TangentDir(image,point) + dir0;
    int esc = 1;
    INT8 type;

    Point nextPoint = point;
    int tempJump = _lengthJump;
    int time = 0;
    
    while (esc) {

        do
        {
            nextPoint.x = (INT8) round(point.x + tempJump * cos(direction));
            nextPoint.y = (INT8) round(point.y + tempJump * sin(direction));
            tempJump++;
        } while ((abs(image->data[nextPoint.y][nextPoint.x] - image->data[point.y][point.x]) <= 10) || (point.x == nextPoint.x && point.y == nextPoint.y));

        direction = TangentDir(image,nextPoint) + dir0;
        nextPoint = localMax(image, nextPoint,direction);
        
        if (nextPoint.x > padding && nextPoint.x < MaxHeight - padding && nextPoint.y > padding && nextPoint.y < MaxHeight - padding) {
            DrawLine(image,point,nextPoint);
            esc = StopCriteria(image,nextPoint,direction,ListMinutiae);
        }else {
            esc = 0;
        }

        point.x = nextPoint.x;
        point.y = nextPoint.y;
        tempJump = _lengthJump;
        
    }
}
INT8 StopCriteria(Image *image, Point point, float direction, SpecialPoint *ListMinutiae) {

        float phinDegrees = direction * 180 / PI;
        int I_threshold = 200;
        int flag = 0; // flag for check this point is a bifurcation
        int output = 1; 
        int left = 6, right = 6, top = 6, bottom = 6;
        float sinn = sin(direction);
        float coss = cos(direction);
        for(int i = 0 ; i < 2  ; i++){
                int x = (INT8) round(point.x + i * coss);
                int y = (INT8) round(point.y + i * sinn);
                if (x < padding || x > MaxHeight - padding || y < padding || y > MaxWidth - padding)
                {
                    break;
                }
                if(x == point.x && y == point.y){
                    continue;
                }
                if(image->isCheck[y][x] == 1){
                    flag = 1;
                }
        }
        if (flag) {
            BOOLEAN label = 1;
            output = 0;
            for (int i = point.y - top; i <= point.y + bottom; i++) {
                for (int j = point.x - left; j <= point.x + right; j++) {
                    if ((image->isCheck[i][j] == 2) || (image->isCheck[i][j] == 3) ||(image->isCheck[i][j] == 8) ||(image->isCheck[i][j] == 9)) {
                        image->isCheck[i][j] = 9; 
                        image->isCheck[point.y][point.x] = 9;
                        deleteMinutiae(ListMinutiae,j,i);
                        label = 0;
                        printf("day ne");
                    }
                }
            }
            if (label){
                image->isCheck[point.y][point.x] = 3;
                addMinutiae(ListMinutiae,point.x,point.y,2,direction);
                printf("%f --%f\n",direction,phinDegrees);
            }
        } else {
            if (image->data[point.x][point.y] > I_threshold && image->data[point.x + 2][point.y] > I_threshold && image->data[point.x - 2][point.y] > I_threshold
                && image->data[point.x][point.y + 2] > I_threshold && image->data[point.x][point.y - 2] > I_threshold) {
                output = 0;
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

        return output;
}
VOID GetMinutiae_v2(Image *image,SpecialPoint *ListMinutiae){

        int step = 2;

        for (int x = top1; x < MaxWidth - bottom1; x +=1) {
            for (int y = left1; y < MaxHeight - right1; y +=1) {

                    Point _ridgeNearest;
                    _ridgeNearest.x = x;
                    _ridgeNearest.y = y;
                    _ridgeNearest = ridgeNearest(image,_ridgeNearest);
                    BOOLEAN label = 1;

                    for (int i = _ridgeNearest.y - 3; i <= _ridgeNearest.y + 3; i++) {
                        for (int j = _ridgeNearest.x - 3; j <= _ridgeNearest.x + 3; j++) {
                            if (image->isCheck[i][j] > 0) {
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