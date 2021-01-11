#include "StringUtil.h"
#include <cstring>
#include <iterator>
#include <base/list/ArrayList.h>
#include <base/exceptions/LogicException.h>
#include <base/exceptions/DataFormatException.h>
#include <algorithm>
#include <string>
#include <iomanip>
#include <sstream>

char StringUtil::buff1[100];
const char* StringUtil::VAL_false = "false";
const char* StringUtil::VAL_FALSE = "FALSE";
const char* StringUtil::VAL_true = "true";
const char* StringUtil::VAL_TRUE = "TRUE";

std::string StringUtil::EMPTY = "";
std::string StringUtil::empty = "";

int StringUtil::indexOf(char* str, int start, int len, char ch) {
	char* pch = (char*)memchr(&str[start], ch, len - start);
	if (pch == nullptr) {
		return -1;
	}

	return pch - str;
}

int StringUtil::indexOf(const std::string& target, const std::string str, const int fromIndex) {
    std::string::size_type loc = target.find(str, (unsigned long)fromIndex);
    int r;
    if(loc != std::string::npos) {
        r = (int)loc;
    } else {
        r = -1;
    }

    return r;
}

int StringUtil::lastIndexOf(const std::string& target, const std::string str, const unsigned long fromIndex) {
    std::string::size_type loc = target.find_last_of(str, fromIndex);
    int r;
    if(loc != std::string::npos) {
        r = (int)loc;
    } else {
        r = -1;
    }

    return r;
}

int StringUtil::parseInt(char* str, const int len) {
	memcpy(buff1, str, len);
	buff1[len] = '\0';

	int ret = strtol(buff1, nullptr, 10);

	return ret;
}

int StringUtil::parseInt(const std::string& str) {
	if (str.length() == 0) {
		throw DataFormatException();
	}

	int ret = std::stoi(str, 0, 10);

	return ret;
}

unsigned int StringUtil::parseUInt(char* str, const int len) {
	memcpy(buff1, str, len);
	buff1[len] = '\0';

	int ret = strtoul(buff1, nullptr, 10);

	return ret;
}

unsigned int StringUtil::parseUInt(const std::string& str) {
	if (str.length() == 0) {
		throw DataFormatException();
	}

	int ret = std::stoul(str, 0, 10);

	return ret;
}

float StringUtil::parseFloat(char* str, const int len) {
    memcpy(buff1, str, len);
    buff1[len] = '\0';

    float ret = strtof(buff1, nullptr);

    return ret;
}

float StringUtil::parseFloat(const std::string& str) {
    float ret = std::stof(str, 0);

    return ret;
}

bool StringUtil::parseBool(char* str, const int len) {
	if (len == 5 && (isEqual(str, VAL_false, len) || isEqual(str, VAL_FALSE, len))) {
		return false;
	} else if (len == 4 && (isEqual(str, VAL_true, len) || isEqual(str, VAL_TRUE, len))) {
		return true;
	}

    throw LogicException(LOC);
}

bool StringUtil::parseBool(const std::string& str) {
	if (str == "false" || str == "FALSE") {
		return false;
	} else if (str == "true" || str == "TRUE") {
		return true;
	}

    throw LogicException(LOC);
}

void StringUtil::removeCharacter(std::string& str, const char ch) {
    str.erase(std::remove(str.begin(), str.end(), ch), str.end());
}

bool StringUtil::isEqual(const char* s1, const char* s2, const int size) {
	if (memcmp(s1, s2, size) == 0) {
		return true;
	}

	return false;
}

std::shared_ptr<ArrayList<std::string>> StringUtil::split(
    const std::string& target, const std::string delim, const bool includeEmptyParts,
    int maxLinesCount)
{
    int delimLen = (int)delim.size();

    std::shared_ptr<ArrayList<std::string>> r = std::make_shared<ArrayList<std::string>>();

    bool isPostPart = true;
    int cOffset = 0;
    while(cOffset < (int)target.size()) {
        int end = 0;
        if (maxLinesCount > 0 && r->count() + 1 >= maxLinesCount) {
            end = (int)target.size() - 1;
        } else {
            end = indexOf(target, delim, cOffset);
            if (end < 0) {
                end = (int)target.size();
                isPostPart = false;
            }
            end -= 1;
        }

        if(end >= cOffset) {
            std::string s = substringRange(target, cOffset, end);

            if (includeEmptyParts || !s.empty()) {
                r->appendReference(s);
            }

            cOffset = end + 1 + delimLen;
        }else {
            // Zero length part occured.
            if(includeEmptyParts) {
                std::string s = "";
                r->appendReference(s);
            }

            cOffset += delimLen;
        }
    }

    if(isPostPart && includeEmptyParts) {
        std::string s = "";
        r->appendReference(s);
    }

    return r;
}

std::shared_ptr<ArrayList<std::string>> StringUtil::split2(const std::string& target, const std::string delimA, const std::string delimB, const bool includeEmptyParts) {
    int delimALen = (int)delimA.size();
    int delimBLen = (int)delimB.size();

    std::shared_ptr<ArrayList<std::string>> r = std::make_shared<ArrayList<std::string>>();

    bool isPostPart = true;
    int cOffset = 0;
    while(cOffset < (int)target.size()) {
        int cDelimLen;
        int end;

        int endA = indexOf(target, delimA, cOffset);
        int endB = indexOf(target, delimB, cOffset);
        if(endA >= 0 && (endB < 0 || endA < endB)) {
            end = endA;
            cDelimLen = delimALen;
        }else if(endB >= 0 && (endA < 0 || endB < endA)) {
            end = endB;
            cDelimLen = delimBLen;
        }else {
            end = (int)target.size();
            cDelimLen = 0;
            isPostPart = false;
        }
        end -= 1;

        if(end >= cOffset) {
            std::string s = substringRange(target, cOffset, end);

            if (!s.empty()) {
                r->appendDirect(s);
            }

            cOffset = end + 1 + cDelimLen;
        }else {
            // Zero length part occured.
            if(includeEmptyParts) {
                std::string s = "";
                r->appendReference(s);
            }

            cOffset += cDelimLen;
        }
    }

    if(isPostPart && includeEmptyParts) {
        std::string s = "";
        r->appendReference(s);
    }

    return r;
}

std::shared_ptr<ArrayList<std::string>> StringUtil::split3(const std::string& target, const std::string delimA, const std::string delimB, const std::string delimC, const bool includeEmptyParts) {
    int delimALen = (int)delimA.size();
    int delimBLen = (int)delimB.size();
    int delimCLen = (int)delimC.size();

    std::shared_ptr<ArrayList<std::string>> r = std::make_shared<ArrayList<std::string>>();

    bool isPostPart = true;
    int cOffset = 0;
    while(cOffset < (int)target.size()) {
        int cDelimLen;
        int end;

        int endA = indexOf(target, delimA, cOffset);
        int endB = indexOf(target, delimB, cOffset);
        int endC = indexOf(target, delimC, cOffset);
        if(endA >= 0 && (endB < 0 || endA < endB) && (endC < 0 || endA < endC)) {
            end = endA;
            cDelimLen = delimALen;
        }else if(endB >= 0 && (endA < 0 || endB < endA) && (endC < 0 || endB < endC)) {
            end = endB;
            cDelimLen = delimBLen;
        }else if(endC >= 0 && (endA < 0 || endC < endA) && (endB < 0 || endC < endB)) {
            end = endC;
            cDelimLen = delimCLen;
        }else {
            end = (int)target.size();
            cDelimLen = 0;
            isPostPart = false;
        }
        end -= 1;

        if(end >= cOffset) {
            std::string s = substringRange(target, cOffset, end);

            if (!s.empty()) {
                r->appendDirect(s);
            }

            cOffset = end + 1 + cDelimLen;
        }else {
            // Zero length part occured.
            if(includeEmptyParts) {
                std::string s = "";
                r->appendReference(s);
            }

            cOffset += cDelimLen;
        }
    }

    if(isPostPart && includeEmptyParts) {
        std::string s = "";
        r->appendReference(s);
    }

    return r;
}

std::shared_ptr<ArrayList<std::string>> StringUtil::splitToLines(const std::string& target, bool insertEmptyEntriesOnDelimRecurence) {
    const std::string delimA = "\r";
    const std::string delimB = "\n";

    int delimALen = (int)delimA.size();
    int delimBLen = (int)delimB.size();

    bool isDelimChargedA = false;
    bool isDelimChargedB = false;
	bool isFirstDelimFound = false;

    std::shared_ptr<ArrayList<std::string>> r = std::make_shared<ArrayList<std::string>>();

    int cOffset = 0;
    while(cOffset < (int)target.size()) {
        int cDelimLen;
        int end;
        bool* isDelimCharged;

        int endA = indexOf(target, delimA, cOffset);
        int endB = indexOf(target, delimB, cOffset);
        if(endA >= 0 && (endB < 0 || endA < endB)) {
            end = endA;
            cDelimLen = delimALen;
            isDelimCharged = &isDelimChargedA;

			if (!isFirstDelimFound) {
				isFirstDelimFound = true;

				isDelimChargedA = true;
			}
        }else if(endB >= 0 && (endA < 0 || endB < endA)) {
            end = endB;
            cDelimLen = delimBLen;
            isDelimCharged = &isDelimChargedB;

			if (!isFirstDelimFound) {
				isFirstDelimFound = true;

				isDelimChargedB = true;
			}
        }else {
            end = (int)target.size();
            cDelimLen = 0;
            isDelimCharged = nullptr;
        }
        end -= 1;

        if(end >= cOffset) {
            std::string s = substringRange(target, cOffset, end);

            if (!s.empty()) {
                r->appendDirect(s);
            }

            cOffset = end + 1 + cDelimLen;

            if(isDelimCharged != nullptr) {
                *isDelimCharged = true;
            }
        }else {
            cOffset += cDelimLen;

            if(insertEmptyEntriesOnDelimRecurence && isDelimCharged != nullptr) {
                if (*isDelimCharged) {
                    r->appendDirect("");

                    isDelimChargedA = false;
                    isDelimChargedB = false;
                }
            }
        }
    }

    return r;
}

bool StringUtil::contains(std::string& s, std::string fragment) {
    bool contains = s.find(fragment) != std::string::npos;

    return contains;
}

int StringUtil::indexOf(std::string& s, std::string fragment) {
    std::size_t found = s.find(fragment);
    if (found != std::string::npos) {
        return (int)found;
    }

    return -1;
}

std::string StringUtil::substringRange(const std::string& target, int startIndex, int endIndex) {
    if(endIndex == -1) {
        if((int)target.size() == 0) {
            throw LogicException(LOC);
        }

        endIndex = (int)target.size() - 1;
    }

    if(startIndex > endIndex || endIndex >= (int)target.size()) {
        throw LogicException(LOC);
    }

    return substringSegment(target, startIndex, (endIndex - startIndex) + 1);
}

std::string StringUtil::substringSegment(const std::string& target, int startIndex, int len) {
    if(len == -1) {
        len = (int)target.size() - startIndex;
    }

    if(startIndex + len > (int)target.size()) {
        throw LogicException(LOC);
    }

    return target.substr((unsigned long)startIndex, (unsigned long)len);
}

void StringUtil::parsePropertyValueS(
    std::string& line, std::string propName,
    std::string delim,
    std::string* dest)
{
    std::shared_ptr<ArrayList<std::string>> propVal = split(line, delim, false);
    if(propVal->size() != 2) {
        throw LogicException(LOC);
    }

    std::string& prop = propVal->getReference(0);
    if(prop != propName) {
        throw LogicException(LOC);
    }

    std::string& valS = propVal->getReference(1);
    if(valS.empty()) {
        throw LogicException(LOC);
    }

    *dest = valS;
}

void StringUtil::parsePropertyValueI(
    std::string& line, std::string propName,
    std::string delim,
    int* dest)
{
    std::shared_ptr<ArrayList<std::string>> propVal = split(line, delim, false);
    if(propVal->size() != 2) {
        throw LogicException(LOC);
    }

    std::string& prop = propVal->getReference(0);
    if(prop != propName) {
        throw LogicException(LOC);
    }

    std::string& valS = propVal->getReference(1);
    if(valS.empty()) {
        throw LogicException(LOC);
    }

    int valI = parseInt(valS);

    *dest = valI;
}

void StringUtil::parsePropertyValueI(
    std::string* line, std::string propName,
    std::string delim,
    int* dest)
{
    if (line == nullptr) {
        throw LogicException(LOC);
    }

    parsePropertyValueI(
        *line, propName,
        delim,
        dest
    );
}

void StringUtil::parsePropertyValueII(
    std::string& line, std::string propName,
    std::string delim, std::string valsDelim,
    int* dest1, int* dest2)
{
    std::shared_ptr<ArrayList<std::string>> propVal = split(line, delim, false);
    if(propVal->size() != 2) {
        throw LogicException(LOC);
    }

    std::string& prop = propVal->getReference(0);
    if(prop != propName) {
        throw LogicException(LOC);
    }

    std::string& valsGroupS = propVal->getReference(1);

    std::shared_ptr<ArrayList<std::string>> valsList = split(valsGroupS, valsDelim, false);
    if(valsList->size() != 2) {
        throw LogicException(LOC);
    }

    std::string& val1S = valsList->getReference(0);
    if(val1S.empty()) {
        throw LogicException(LOC);
    }
    int val1I = parseInt(val1S);

    std::string& val2S = valsList->getReference(1);
    if(val2S.empty()) {
        throw LogicException(LOC);
    }
    int val2I = parseInt(val2S);

    *dest1 = val1I;
    *dest2 = val2I;
}

void StringUtil::parsePropertyValueIIII(
    std::string& line, std::string propName,
    std::string delim, std::string valsDelim,
    int* dest1, int* dest2, int* dest3, int* dest4)
{
    std::shared_ptr<ArrayList<std::string>> propVal = split(line, delim, false);
    if(propVal->size() != 2) {
        throw LogicException(LOC);
    }

    std::string& prop = propVal->getReference(0);
    if(prop != propName) {
        throw LogicException(LOC);
    }

    std::string& valsGroupS = propVal->getReference(1);

    std::shared_ptr<ArrayList<std::string>> valsList = split(valsGroupS, valsDelim, false);
    if(valsList->size() != 4) {
        throw LogicException(LOC);
    }

    std::string& val1S = valsList->getReference(0);
    if(val1S.empty()) {
        throw LogicException(LOC);
    }
    int val1I = parseInt(val1S);

    std::string& val2S = valsList->getReference(1);
    if(val2S.empty()) {
        throw LogicException(LOC);
    }
    int val2I = parseInt(val2S);

    std::string& val3S = valsList->getReference(2);
    if(val3S.empty()) {
        throw LogicException(LOC);
    }
    int val3I = parseInt(val3S);

    std::string& val4S = valsList->getReference(3);
    if(val4S.empty()) {
        throw LogicException(LOC);
    }
    int val4I = parseInt(val4S);

    *dest1 = val1I;
    *dest2 = val2I;
    *dest3 = val3I;
    *dest4 = val4I;
}

std::string StringUtil::floatToFixedPrecisionString(float val, int decimalsCount) {
    std::stringstream stream;
    stream << std::fixed << std::setprecision(decimalsCount) << val;
    std::string s = stream.str();

    return s;
}

int StringUtil::replace(std::string& str, std::string pattern, std::string newVal) {
	size_t start_pos = 0;
    int occurencesCount = 0;
	while ((start_pos = str.find(pattern, start_pos)) != std::string::npos) {
		str.replace(start_pos, pattern.length(), newVal);
		start_pos += newVal.length(); // Handles case where 'to' is a substring of 'from'

        occurencesCount++;
	}

	return occurencesCount;
}

void StringUtil::validateTuple1(std::string& line, std::string validationParamName) {
	if (line != validationParamName) {
		throw DataFormatException();
	}
}

int StringUtil::getTuple2Value_int(std::string& line, std::string delim, std::string validationParamName) {
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, delim, false);
	if (partsList->count() != 2) {
		throw DataFormatException();
	}

	int partIndex = 0;
	std::string& part = partsList->getReference(partIndex++);
	if (part != validationParamName) {
		throw DataFormatException();
	}

	part = partsList->getReference(partIndex++);
	int value = StringUtil::parseInt(part);

	return value;
}

float StringUtil::getTuple2Value_float(std::string& line, std::string delim, std::string validationParamName) {
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, delim, false);
	if (partsList->count() != 2) {
		throw DataFormatException();
	}

	int partIndex = 0;
	std::string& part = partsList->getReference(partIndex++);
	if (part != validationParamName) {
		throw DataFormatException();
	}

	part = partsList->getReference(partIndex++);
	float value = StringUtil::parseFloat(part);

	return value;
}

std::string StringUtil::getTuple2Value_string(std::string& line, std::string delim, std::string validationParamName) {
	std::shared_ptr<ArrayList<std::string>> partsList = StringUtil::split(line, delim, false);
	if (partsList->count() != 2) {
		throw DataFormatException();
	}

	int partIndex = 0;
	std::string& part = partsList->getReference(partIndex++);
	if (part != validationParamName) {
		throw DataFormatException();
	}

	part = partsList->getReference(partIndex++);

	return part;
}
