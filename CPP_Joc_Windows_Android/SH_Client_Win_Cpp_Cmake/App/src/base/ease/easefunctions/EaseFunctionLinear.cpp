#include "EaseFunctionLinear.h"

float EaseFunctionLinear::easeNone (float t,float b , float c, float d) {
	return c*t/d + b;
}
float EaseFunctionLinear::easeIn (float t,float b , float c, float d) {
	return c*t/d + b;
}
float EaseFunctionLinear::easeOut(float t,float b , float c, float d) {
	return c*t/d + b;
}

float EaseFunctionLinear::easeInOut(float t,float b , float c, float d) {
	return c*t/d + b;
}
