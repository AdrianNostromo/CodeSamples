#include "Math.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <time.h>
#include <base/statics/StaticsInit.h>
#include <base/math/Vector2.h>
#include <base/math/Random.h>
#include <base/exceptions/LogicException.h>
#include <base/MM.h>

std::string Math::SINGLE_CHARACTER_ENCODING = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

bool Math::IsStaticsInitHooked = base::StaticsInit::AddCbGeneral<bool>(0, []() {
    IsStaticsInitHooked = true;

    Math::Rand::InitStatics();
});

void Math::Rand::InitStatics() {
    RNG = new Random(time(NULL));
}

Random* Math::Rand::RNG = nullptr;

float Math::Rand::nextFloat() {
    return RNG->nextFloat();
}

int Math::Rand::nextInt() {
    return RNG->nextInt();
}

int Math::Rand::nextInt(int range) {
    return RNG->nextInt(range);
}

int Math::Rand::nextInt(int start, int end) {
    return RNG->nextInt(start, end);
}

int Math::UlamSpiral::PositionToValueStartsAt1(Vector2Int& _p) {
    // The math works with floats, not ints.
    Vector2 p{ static_cast<float>(_p.x), static_cast<float>(_p.y) };

    float x = abs(p.x);
    float y = abs(p.y);
    bool q = x > y;

    x = q ? x : y;
    y = q ? p.x + p.y : p.x - p.y;
    y = abs(y) + 4.0f * x * x + 1.0f;
    x *= 2.0f;

    int val = q
        ? (p.x > 0. ? y - x - x : y)
        : (p.y > 0. ? y - x : y + x);

    return val;
}

Vector2Int Math::UlamSpiral::ValueToPositionStartsAt1(int val) {
    if (val <= 0) {
        throw LogicException(LOC);
    }

    int root = Math::ceil(Math::sqrt(val));
    int perimeterStartVal = root * root;
    int radius = Math::floor(root / 2.0f);
    int delta = perimeterStartVal - val;
    int edgeLen = root;
    bool isBotRight = (root % 2) == 1 ? true : false;

    int x = -1;
    int y = -1;
    if (isBotRight) {
        // Go left, then go right.
        x = radius;
        y = -radius;

        if (delta < edgeLen) {
            // Go left until destination val.
            x -= delta;
        } else {
            // Go max_left and go up until destination val.
            x -= (edgeLen - 1);
            y += (delta - (edgeLen - 1));
        }
    } else {
        // Go right, then go down.
        x = -(radius - 1);
        y = radius;

        if (delta < edgeLen) {
            // Go right until destination val.
            x += delta;
        } else {
            // Go max_right and go down until destination val.
            x += (edgeLen - 1);
            y -= (delta - (edgeLen - 1));
        }
    }

    return Vector2Int(x, y);
}

const double Math::PI_D = 3.141592653589793238463;
const float Math::PI_F = 3.14159265358979f;
const float Math::PIx2_F = 3.1415927f * 2.0f;

const float Math::FLOAT_ROUNDING_ERROR = 0.000001f; // 32 bits
const int Math::FLOAT_PRECISSION_DECIMALS_COUNT = 7; // 7

const float Math::radiansToDegrees = 180.0f / PI_F;
const float Math::radDeg = radiansToDegrees;

const float Math::degreesToRadians = PI_F / 180.0f;
const float Math::degRad = degreesToRadians;

float Math::round(float value) {
  return std::round(value);
}

float Math::floor(float value) {
  return std::floor(value);
}

float Math::ceil(float value) {
  return std::ceil(value);
}

float Math::tanRad(float angRad) {
    return std::tan(angRad);
}

float Math::acosRad(float value) {
  return std::acos(value);
}

float Math::fmod(float valueA, float valueB) {
  return std::fmod(valueA, valueB);
}

int Math::abs(int value) {
  return std::abs(value);
}

float Math::abs(float value) {
    return std::abs(value);
}

float Math::sqrt(float value) {
  return std::sqrt(value);
}

float Math::powF(float base, float exponent) {
  return std::pow(base, exponent);
}

float Math::sign(float value) {
    return (value >= 0.0f) ? 1.0f : -1.0f;
}

float Math::toRadians(float angdeg) {
    return angdeg / 180.0f * PI_F;
}

float Math::toDegrees(float angrad) {
    return angrad * 180.0f / PI_F;
}

float Math::animateRotationSmallestAngle(float currentAngleD, float destAngleD, float speedDTick, bool* oIsAnimFinished) {
    currentAngleD = normalizeAngleDeg(currentAngleD);
    destAngleD = normalizeAngleDeg(destAngleD);

    bool isAnimFinished = false;

    float deltaZ = closestAngleDeg(
        currentAngleD,
        destAngleD
    );

    float finalRotZ;
    if(abs(deltaZ) <= speedDTick) {
        isAnimFinished = true;

        finalRotZ = destAngleD;
    }else {
        float dir = deltaZ / abs(deltaZ);
        finalRotZ = currentAngleD + speedDTick * dir;
    }

	if (oIsAnimFinished != nullptr) {
		*oIsAnimFinished = isAnimFinished;
	}

    return finalRotZ;
}

float Math::closestAngleDeg(float _startAngDeg, float _endAngDeg) {
    float startAngDeg = normalizeAngleDeg(_startAngDeg);
    float endAngDeg = normalizeAngleDeg(_endAngDeg);

    float a = endAngDeg - startAngDeg;

    a = _customMod((a + 180.0f), 360.0f) - 180.0f;

    return a;
}

float Math::_customMod(float val, float mod) {
    return fmod((fmod(val, mod) + mod), mod);
}

//asdC;// Uptimise to use a precompiled sin table. Also used by cos.

float Math::sinDegToRad(float degrees) {
    return std::sin(toRadians(degrees));
}

float Math::asin(float val) {
    return std::asin(val);
}

float Math::asinToDeg(float val) {
    return toDegrees(std::asin(val));
}

float Math::cosDegToRad(float degrees) {
    return std::cos(toRadians(degrees));
}

float Math::sin(float angRad) {
	return std::sin(angRad);
}

float Math::cos(float angRad) {
	return std::cos(angRad);
}

float Math::sinRad(float angRad) {
	return std::sin(angRad);
}

float Math::cosRad(float angRad) {
	return std::cos(angRad);
}

int Math::min(const int valA, const int valB) {
    return std::min(valA, valB);
}

float Math::min(const float valA, const float valB) {
    return std::min(valA, valB);
}

float Math::min(const float valA, const float valB, const float valC, const float valD) {
    return std::min(std::min(valA, valB), std::min(valC, valD));
}

int Math::max(const int valA, const int valB) {
    return std::max(valA, valB);
}

float Math::max(const float valA, const float valB) {
    return std::max(valA, valB);
}

float Math::max(const float valA, const float valB, const float valC, const float valD) {
    return std::max(std::max(valA, valB), std::max(valC, valD));
}

float Math::atan2(const float valY, const float valX) {
    return std::atan2(valY, valX);
}

float Math::atan2Deg(const float valY, const float valX) {
    return toDegrees(std::atan2(valY, valX));
}

// (-180, 180]
float Math::normalizeAngleDeg(float angDeg) {
    float newAngle = angDeg;
    while (newAngle <= -180.0f) {
        newAngle += 360.0f;
    }
    while (newAngle > 180.0f) {
        newAngle -= 360.0f;
    }
    return newAngle;
}

float Math::clamp(float value, float min, float max) {
	if (value < min) {
		return min;
	}
	if (value > max) {
		return max;
	}

    return value;
}

float Math::compare(float a, float b) {
	if (isEqual(a, b)) {
		return 0;
	}

	float ret = a - b;

	return ret;
}

float Math::dist(float aX, float aY, float bX, float bY) {
    float dist = std::sqrt((bX - aX) * (bX - aX) + (bY - aY) * (bY - aY));

    return dist;
}

float Math::dist(float aX, float aY, float aZ, float bX, float bY, float bZ) {
    float dist = std::sqrt((bX - aX) * (bX - aX) + (bY - aY) * (bY - aY) + (bZ - aZ) * (bZ - aZ));

    return dist;
}

float Math::dist3D(Vector3& posA, Vector3& posB) {
	return dist3D(
		posA.x, posA.y, posA.z,
		posB.x, posB.y, posB.z
	);
}

float Math::dist3D(Vector3* posA, Vector3* posB) {
    return dist3D(*posA, *posB);
}

float Math::dist3D(
	float aX, float aY, float aZ,
	float bX, float bY, float bZ
) {
	float dist = (float) Math::sqrt(
        Math::powF(aX - bX, 2)
        + Math::powF(aY - bY, 2)
        + Math::powF(aZ - bZ, 2)
    );

	return dist;
}

std::string Math::intToPaddedString(int value, int paddedStringLen) {
    std::stringstream ss;
    ss << std::setw(paddedStringLen) << std::setfill('0') << value;
    std::string s = ss.str();

    return s;
}

std::string Math::intToPrefixSpacePaddedString(int value, int paddedStringLen) {
    int _v = value;
    int digitsCount = 0;
    if(value < 0) {
        digitsCount++;
    }
    do {
        _v /= 10;
        digitsCount++;
    }while(_v != 0);

    std::stringstream ss;
    while(digitsCount < paddedStringLen) {
        ss << ' ';

        digitsCount++;
    }
    ss << value;

    std::string s = ss.str();
    return s;
}

std::string Math::intToSuffixSpacePaddedString(int value, int paddedStringLen) {
    int _v = value;
    int digitsCount = 0;
    if(value < 0) {
        digitsCount++;
    }
    do {
        _v /= 10;
        digitsCount++;
    }while(_v != 0);

    std::stringstream ss;
    ss << value;
    while(digitsCount < paddedStringLen) {
        ss << ' ';

        digitsCount++;
    }

    std::string s = ss.str();
    return s;
}

float Math::clip(float value, float min, float max) {
	return Math::max(min, Math::min(max, value));
}

std::string Math::toString_1Byte_Base16(int value) {
	std::stringstream stream;
	stream 
		<< std::setfill('0') << std::setw(2)
		<< std::hex << value;

	return stream.str();
}

bool Math::set(float& dest, float val) {
	dest = val;

	return true;
}

char Math::encodeNumberToSingleChar(int val) {
    if (val >= SINGLE_CHARACTER_ENCODING.length()) {
        val = SINGLE_CHARACTER_ENCODING.length() - 1;
    }

    return SINGLE_CHARACTER_ENCODING[val];
}

int Math::cm_to_dm_exact(int cmVal) {
    if ((cmVal % 10) != 0) {
        // The len must be a multiple of 10 cm;
        throw LogicException(LOC);
    }
    int len_dm = cmVal / 10;

    return len_dm;
}

