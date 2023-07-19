#include "Image.h"

//---------------------------------------------------------------

// -------------------------------------------------------

// const char* findNextEdge(Point A, Point B, Point C) {
//     // Tính toán vectơ BA và BC
//     Point BA = {A.x - B.x, A.y - B.y};
//     Point BC = {C.x - B.x, C.y - B.y};

//     // Tính cross product
//     double cross_product = BA.x * BC.y - BA.y * BC.x;

//     // Kiểm tra dấu của cross product
//     if (cross_product > 0) {
//         return "Cạnh tiếp theo theo chiều kim đồng hồ là BC";
//     } else if (cross_product < 0) {
//         return "Cạnh tiếp theo theo chiều kim đồng hồ là AC";
//     } else {
//         return "Tam giác có đỉnh thẳng hàng hoặc cạnh tiếp theo không xác định";
//     }
// }
///


/// @brief The triangle is formed by the three vertices I, J, and K. 
///        This function is used for check if the sides of the triangle follow a clockwise order.

INT8  CheckEdgeClockwise(SpecialPoint *minus ,UINT8 i ,UINT8 j ,UINT8 k){
    int BA_x = minus->minus[i].x - minus->minus[j].x;
    int BA_y = minus->minus[i].y - minus->minus[j].y;
    int BC_x = minus->minus[k].x - minus->minus[j].x;
    int BC_y = minus->minus[k].y - minus->minus[j].y;

    float cross_product = BA_x * BC_y - BA_y * BC_x;
    if(cross_product > 0)
    {
        return 1;
    }
    else if (cross_product < 0)
    {
        return -1;
    }
    return 0;
}

UINT8 CompairMinutiae(
    GroupDataSpecialPoint *group1,
    GroupDataSpecialPoint *group2,
    UINT16 NumTriangle1,
    UINT16 NumTriangle2,
    SpecialPoint *minus1,
    SpecialPoint *minus2
){
    float _distanceLimit = 2;

    UINT16 counter = 0;
    UINT16 result = 0;


    // printf("So tam giac group1 la: %ld\n", NumTriangle1);
    // printf("So tam giac group2 la: %ld\n", NumTriangle2);



    UINT8 mangIJK[NumTriangle1*3];
    UINT8 mangIJK2[NumTriangle2*3];
    for(int i = 0; i < NumTriangle1; i++ ){
        for(int j = 0; j< NumTriangle2; j++){
            counter+=1;
            if(group2[j].status == 0 )
            {   
                UINT8 flag = 0;

                float SubBord1AndBord1 = fabs ( group1[i].bord1 - group2[j].bord1 );
                float SubBord1AndBord2 = fabs ( group1[i].bord1 - group2[j].bord2 );
                float SubBord1AndBord3 = fabs ( group1[i].bord1 - group2[j].bord3 );

                float SubBord2AndBord1 = fabs ( group1[i].bord2 - group2[j].bord1 );
                float SubBord2AndBord2 = fabs ( group1[i].bord2 - group2[j].bord2 );
                float SubBord2AndBord3 = fabs ( group1[i].bord2 - group2[j].bord3 );

                float SubBord3AndBord1 = fabs ( group1[i].bord3 - group2[j].bord1 );
                float SubBord3AndBord2 = fabs ( group1[i].bord3 - group2[j].bord2 );
                float SubBord3AndBord3 = fabs ( group1[i].bord3 - group2[j].bord3 );

                // printf("---------------%f\n",SubBord1AndBord1);
                // printf("---------------%f\n",SubBord1AndBord2);
                // printf("---------------%f\n",SubBord1AndBord3);
                // printf("---------------%f\n",SubBord2AndBord1);
                // printf("---------------%f\n",SubBord2AndBord2);
                // printf("---------------%f\n",SubBord2AndBord3);
                // printf("---------------%f\n",SubBord3AndBord1);
                // printf("---------------%f\n",SubBord3AndBord2);
                // printf("---------------%f\n",SubBord3AndBord3);

                UINT8 i1 = ((group1[i].id >> 16) & 0xFF);
                UINT8 j1 = ((group1[i].id >> 8)  & 0xFF);
                UINT8 k1 = ((group1[i].id >> 0)  & 0xFF);

                UINT8 i2 = ((group2[j].id >> 16) & 0xFF);
                UINT8 j2 = ((group2[j].id >> 8)  & 0xFF);
                UINT8 k2 = ((group2[j].id >> 0)  & 0xFF);


                if(
                    (minus1->minus[i1].Type == minus2->minus[i2].Type)
                 && (minus1->minus[j1].Type == minus2->minus[j2].Type)
                 && (minus1->minus[k1].Type == minus2->minus[k2].Type)
                 && (SubBord1AndBord1 <= _distanceLimit) && (SubBord2AndBord2 <= _distanceLimit) && (SubBord3AndBord3 <= _distanceLimit)
                ){
                    if(CheckEdgeClockwise(minus1,i1,j1,k1) == CheckEdgeClockwise(minus2,i2,j2,k2))
                    {
                        flag = 1;
                    }
                } else
                if(
                    (minus1->minus[i1].Type == minus2->minus[j2].Type)
                 && (minus1->minus[j1].Type == minus2->minus[i2].Type)
                 && (minus1->minus[k1].Type == minus2->minus[k2].Type)
                 && (SubBord1AndBord1 <= _distanceLimit) && (SubBord2AndBord3 <= _distanceLimit) && (SubBord3AndBord2 <= _distanceLimit)
                ){
                    if(CheckEdgeClockwise(minus1,i1,j1,k1) == CheckEdgeClockwise(minus2,j2,i2,k2))
                    {
                        flag = 1;
                    }
                } else 
                if(
                    (minus1->minus[i1].Type == minus2->minus[k2].Type)
                 && (minus1->minus[j1].Type == minus2->minus[i2].Type)
                 && (minus1->minus[k1].Type == minus2->minus[j2].Type)
                 && (SubBord1AndBord2 <= _distanceLimit) && (SubBord2AndBord3 <= _distanceLimit) && (SubBord3AndBord1 <= _distanceLimit)
                ){
                    if(CheckEdgeClockwise(minus1,i1,j1,k1) == CheckEdgeClockwise(minus2,k2,i2,j2))
                    {
                        flag = 1;
                    }
                } else
                if(
                    (minus1->minus[i1].Type == minus2->minus[i2].Type)
                 && (minus1->minus[j1].Type == minus2->minus[k2].Type)
                 && (minus1->minus[k1].Type == minus2->minus[j2].Type)
                 && (SubBord1AndBord2 <= _distanceLimit) && (SubBord2AndBord1 <= _distanceLimit) && (SubBord3AndBord3 <= _distanceLimit)
                ){
                    if(CheckEdgeClockwise(minus1,i1,j1,k1) == CheckEdgeClockwise(minus2,i2,k2,j2))
                    {
                        flag = 1;
                    }
                } else
                if(
                    (minus1->minus[i1].Type == minus2->minus[k2].Type)
                 && (minus1->minus[j1].Type == minus2->minus[j2].Type)
                 && (minus1->minus[k1].Type == minus2->minus[i2].Type)
                 && (SubBord1AndBord3 <= _distanceLimit) && (SubBord2AndBord2 <= _distanceLimit) && (SubBord3AndBord1 <= _distanceLimit)
                ){
                    if(CheckEdgeClockwise(minus1,i1,j1,k1) == CheckEdgeClockwise(minus2,k2,j2,i2))
                    {
                        flag = 1;
                    }
                } else
                if(
                    (minus1->minus[i1].Type == minus2->minus[j2].Type)
                 && (minus1->minus[j1].Type == minus2->minus[k2].Type)
                 && (minus1->minus[k1].Type == minus2->minus[i2].Type)
                 && (SubBord1AndBord3 <= _distanceLimit) && (SubBord2AndBord1 <= _distanceLimit) && (SubBord3AndBord2 <= _distanceLimit)
                ){
                    if(CheckEdgeClockwise(minus1,i1,j1,k1) == CheckEdgeClockwise(minus2,j2,k2,i2))
                    {
                        flag = 1;
                    }
                }


                if(flag == 1){
                    mangIJK[result*3] = i1;
                    mangIJK[result*3 + 1] = j1;
                    mangIJK[result*3 + 2] = k1;

                    mangIJK2[result*3] = i2;
                    mangIJK2[result*3 + 1] = j2;
                    mangIJK2[result*3 + 2] = k2;

                    result+=1;
                    group2[j].status = 1;
                    // printf("%d-----------%d--------%d\n",i1,j1,k1);
                    // printf("%d-----------%d--------%d\n",i2,j2,k2);
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
        
    INT8 sodiemtrung2=0;

    if(result>0){
        sodiemtrung2 +=1;
    }

    INT8 coincide2 = 0;
    for(int i = 1; i < result*3; i++){
        coincide2 = 0;
        for(int j = i-1; j >= 0; j--){
            if(mangIJK2[i] == mangIJK2[j]){
                coincide2 = 1;
            }
        }
        if(coincide2 == 0){
            sodiemtrung2 +=1;
        }
    }
     UINT8 OUTPUT = (sodiemtrung > sodiemtrung2)?sodiemtrung2:sodiemtrung;

    //  printf("So tam giac trung nhau la: %d\n", result);
    //  printf("So diem trung nhau theo phuong phap 2 la: %d\n", OUTPUT);
     return OUTPUT;
}



