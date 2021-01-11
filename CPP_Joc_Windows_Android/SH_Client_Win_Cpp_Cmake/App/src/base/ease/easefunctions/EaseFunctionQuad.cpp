#include "EaseFunctionQuad.h"

float EaseFunctionQuad::easeIn (float t,float b , float c, float d) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return c*(t/=d)*t + b;
#pragma clang diagnostic pop
}
float EaseFunctionQuad::easeOut(float t,float b , float c, float d) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return -c *(t/=d)*(t-2) + b;
#pragma clang diagnostic pop
}

float EaseFunctionQuad::easeInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return ((c/2)*(t*t)) + b;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return -c/2 * (((t-2)*(--t)) - 1) + b;
#pragma clang diagnostic pop
	/*
	originally return -c/2 * (((t-2)*(--t)) - 1) + b;
	
	I've had to swap (--t)*(t-2) due to diffence in behaviour in 
	pre-increment operators between java and c++, after hours
	of joy
	*/
	
}
