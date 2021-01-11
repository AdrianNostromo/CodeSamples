#include "EaseFunctionCirc.h"

float EaseFunctionCirc::easeIn (float t,float b , float c, float d) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return -c * (sqrt(1 - (t/=d)*t) - 1) + b;
#pragma clang diagnostic pop
}
float EaseFunctionCirc::easeOut(float t,float b , float c, float d) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return c * sqrt(1 - (t=t/d-1)*t) + b;
#pragma clang diagnostic pop
}

float EaseFunctionCirc::easeInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return -c/2 * (sqrt(1 - t*t) - 1) + b;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return c/2 * (sqrt(1 - t*(t-=2)) + 1) + b;
#pragma clang diagnostic pop
}
