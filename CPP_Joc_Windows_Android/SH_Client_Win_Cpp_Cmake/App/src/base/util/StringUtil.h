#pragma once

#include <base/gh.h>
#include <string>
#include <memory>
#include <base/list/ArrayList.h>
#include <algorithm>

class StringUtil {
private:
	static char buff1[100];
	static const char* VAL_false;
	static const char* VAL_FALSE;
	static const char* VAL_true;
	static const char* VAL_TRUE;

public:
	static std::string EMPTY;
	static std::string empty;

	static int indexOf(char* str, int start, int len, char ch);
	static int indexOf(const std::string& target, const std::string str, const int fromIndex=0);
	static int lastIndexOf(const std::string& target, const std::string str, const unsigned long fromIndex = std::string::npos);

	static int parseInt(char* str, int len);
	static int parseInt(const std::string& str);
	static unsigned int parseUInt(char* str, int len);
	static unsigned int parseUInt(const std::string& str);
	static float parseFloat(char* str, int len);
	static float parseFloat(const std::string& str);
	static bool parseBool(char* str, int len);
	static bool parseBool(const std::string& str);

	static void removeCharacter(std::string& str, const char ch);
	static bool isEqual(const char* s1, const char* s2, const int size);

	// This also includes empty fragments if flag is set.
    // maxLinesCount: > 0 limited lines count.
    static std::shared_ptr<ArrayList<std::string>> split(
        const std::string& s, const std::string delim, const bool includeEmptyParts,
        int maxLinesCount=-1);
    static std::shared_ptr<ArrayList<std::string>> split2(const std::string& target, const std::string delimA, const std::string delimB, const bool includeEmptyParts);
    static std::shared_ptr<ArrayList<std::string>> split3(const std::string& target, const std::string delimA, const std::string delimB, const std::string delimC, const bool includeEmptyParts);
    //insertEmptyEntriesOnDelimRecurence: true; Will insert an empty entry each time the same delim occurs without a string beying inserted.
    // NOTE : Only one empty line is inserted. Consecutive empty lines are ignorred.
    static std::shared_ptr<ArrayList<std::string>> splitToLines(const std::string& target, bool insertEmptyEntriesOnDelimRecurence = false);

    static bool contains(std::string& s, std::string fragment);
    static int indexOf(std::string& s, std::string fragment);

    static std::string substringRange(const std::string& s, int offsetStart, int offsetEnd);
    static std::string substringSegment(const std::string& s, int offsetStart, int len);

    static void parsePropertyValueS(
        std::string& line, std::string propName,
        std::string delim,
        std::string* dest);
    static void parsePropertyValueI(
        std::string& line, std::string propName,
        std::string delim,
        int* dest);
    static void parsePropertyValueI(
        std::string* line, std::string propName,
        std::string delim,
        int* dest);
    static void parsePropertyValueII(
        std::string& line, std::string propName,
        std::string delim, std::string valsDelim,
        int* dest1, int* dest2);
    static void parsePropertyValueIIII(
        std::string& line, std::string propName,
        std::string delim, std::string valsDelim,
        int* dest1, int* dest2, int* dest3, int* dest4);

    static std::string floatToFixedPrecisionString(float val, int decimalsCount);

    // Return occurences count.
	static int replace(std::string& str, std::string pattern, std::string newVal);

	// "paramNameToValidate"
	pub static void validateTuple1(std::string& line, std::string validationParamName);
	// "paramNameToValidate 2";// 2 will be returned as int.
	pub static int getTuple2Value_int(std::string& line, std::string delim, std::string validationParamName);
	pub static float getTuple2Value_float(std::string& line, std::string delim, std::string validationParamName);
	pub static std::string getTuple2Value_string(std::string& line, std::string delim, std::string validationParamName);
};
