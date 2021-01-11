#include "EaseFunctionCubicBezier.h"

float EaseFunctionCubicBezier::easeCubicBezier(float startX, float startY, float endX, float endY, float time) {
    float cY = 3 * startY;
    float bY = 3 * (endY - startY) - cY;
    float aY = 1 - cY - bY;
    return time * (cY + time * (bY + time * aY));
}
