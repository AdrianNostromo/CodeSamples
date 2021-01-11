#pragma once

#include <base/gh.h>
#include <base/math/Vector3.h>
#include <base/math/Vector2Int.h>

class Random;

class Math {
	priv static std::string SINGLE_CHARACTER_ENCODING;

	pub class Rand {
		pub static void InitStatics();

		pub static Random* RNG;

		pub static float nextFloat();
		pub static int nextInt();
		/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
		pub static int nextInt(int range);
		/** Returns a random number between start (inclusive) and end (inclusive). */
		pub static int nextInt(int start, int end);

	};
	pub class UlamSpiral {
		pub static int PositionToValueStartsAt1(Vector2Int& p);
		pub static Vector2Int ValueToPositionStartsAt1(int val);

	};

	pub static bool IsStaticsInitHooked;

	pub static const double PI_D;
	pub static const float PI_F;
	pub static const float PIx2_F;

	pub static const float FLOAT_ROUNDING_ERROR; // 32 bits
	pub static const int FLOAT_PRECISSION_DECIMALS_COUNT; // 7 decimal places

	/** multiply by this to convert from radians to degrees */
	pub static const float radiansToDegrees;
	pub static const float radDeg;
	/** multiply by this to convert from degrees to radians */
	pub static const float degreesToRadians;
	pub static const float degRad;

	pub static float round(float value);
	pub static float floor(float value);
	pub static float ceil(float value);

	pub static float tanRad(float angRad);
	pub static float acosRad(float value);

	pub static float fmod(float valueA, float valueB);
	pub static int abs(int value);
	pub static float abs(float value);
	pub static float sqrt(float value);
	pub static float powF(float base, float exponent);

	pub static float sign(float value);

	pub static float toRadians(float angdeg);
	pub static float toDegrees(float angrad);

	pub static float animateRotationSmallestAngle(float currentAngleD, float destAngleD, float speedDTick, bool* oIsAnimFinished=nullptr);
	pub static float closestAngleDeg(float angStartDeg, float angEndDeg);
	pub static float _customMod(float val, float mod);

	pub static float sinDegToRad(float degrees);
	pub static float asin(float val);
	pub static float asinToDeg(float val);
	pub static float cosDegToRad(float degrees);
	
	pub static float sin(float angRad);
	pub static float cos(float angRad);
	pub static float sinRad(float angRad);
	pub static float cosRad(float angRad);

	pub static int min(const int valA, const int valB);
	pub static float min(const float valA, const float valB);
	pub static float min(const float valA, const float valB, const float valC, const float valD);
	pub static int max(const int valA, const int valB);
	pub static float max(const float valA, const float valB);
	pub static float max(const float valA, const float valB, const float valC, const float valD);

	pub static float atan2(const float valY, const float valX);
	pub static float atan2Deg(const float valY, const float valX);

	pub static float normalizeAngleDeg(float angDeg);

	pub static float clamp(float value, float min, float max);

	pub static inline bool isZero(float value) {
		return Math::abs(value) <= FLOAT_ROUNDING_ERROR;
	};

	/** Returns true if a is nearly equal to b. The function uses the default floating error tolerance.
	* @param a the first value.
	* @param b the second value. */
	pub static inline bool isEqual(float a, float b) {
		return Math::abs(a - b) <= FLOAT_ROUNDING_ERROR;
	};
	pub static inline bool isEqual(float a, float b, float roundingError) {
		return Math::abs(a - b) <= roundingError;
	};

	pub static float compare(float a, float b);

	pub static float dist(float aX, float aY, float bX, float bY);
	pub static float dist(float aX, float aY, float aZ, float bX, float bY, float bZ);
	pub static float dist3D(Vector3& posA, Vector3& posB);
	pub static float dist3D(Vector3* posA, Vector3* posB);
	pub static float dist3D(
		float aX, float aY, float aZ,
		float bX, float bY, float bZ
	);

	pub static std::string intToPaddedString(int value, int padding);
	pub static std::string intToPrefixSpacePaddedString(int value, int padding);
	pub static std::string intToSuffixSpacePaddedString(int value, int padding);
    
	pub static float clip(float value, float min, float max);

	pub static std::string toString_1Byte_Base16(int value);

	// This is used to set value in "if" checks.
	pub static bool set(float& dest, float val);

	// This is used to log a radom dungeon in the console and preserve columns alignment.
	pub static char encodeNumberToSingleChar(int val);

	pub static int cm_to_dm_exact(int cmVal);

};
