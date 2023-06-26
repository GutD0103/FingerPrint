/*
 * newMath.h
 *
 *  Created on: Jun 22, 2023
 *      Author: ntdat
 */

#ifndef INC_NEWMATH_H_
#define INC_NEWMATH_H_

#include <sys/types.h>
#include <math.h>
#include "../BaseLib/Include/BaseLib.h"
#include "../BaseLib/Include/Memory.h"
#include <stdint.h>
#define PI_FLOAT     3.14159265f
#define PIBY2_FLOAT  1.5707963f
/**
 * Sine calculation using interpolated table lookup.
 * Instead of radiants or degrees we use "turns" here. Means this
 * sine does NOT return one phase for 0 to 2*PI, but for 0 to 1.
 * Input: -1 to 1 as int16 Q15  == -32768 to 32767.
 * Output: -1 to 1 as int16 Q15 == -32768 to 32767.
 *
 * @param int16_t angle Q15
 * @return int16_t Q15
 */
int16_t sin1(int16_t angle);

/**
 * Cosine calculation using interpolated table lookup.
 * Instead of radiants or degrees we use "turns" here. Means this
 * cosine does NOT return one phase for 0 to 2*PI, but for 0 to 1.
 * Input: -1 to 1 as int16 Q15  == -32768 to 32767.
 * Output: -1 to 1 as int16 Q15 == -32768 to 32767.
 *
 * @param int16_t angle Q15
 * @return int16_t Q15
 */
int16_t cos1(int16_t angle);

float atan2_approximation1(float y, float x);
float atan2_approximation2(float y, float x);

#endif /* INC_NEWMATH_H_ */
