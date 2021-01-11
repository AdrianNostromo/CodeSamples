#include "EaseFunctionQuint.h"

float EaseFunctionQuint::easeIn (float t,float b , float c, float d) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return c*(t/=d)*t*t*t*t + b;
#pragma clang diagnostic pop
}
float EaseFunctionQuint::easeOut(float t,float b , float c, float d) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return c*((t=t/d-1)*t*t*t*t + 1) + b;
#pragma clang diagnostic pop
}

float EaseFunctionQuint::easeInOut(float t,float b , float c, float d) {
	if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsequenced"
	return c/2*((t-=2)*t*t*t*t + 2) + b;
#pragma clang diagnostic pop
}