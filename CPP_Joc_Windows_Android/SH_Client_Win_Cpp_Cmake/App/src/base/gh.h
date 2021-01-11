#pragma once

// This contains global_header code.

#define pub public:
#define prot protected:
#define priv private:

#define dCtor(CLASS)\
	CLASS(const CLASS&) = delete;\
	CLASS(CLASS&&) = default;\
	CLASS& operator=(const CLASS&) = delete;\
	CLASS& operator=(CLASS&&) = default;

#define dOpsDec(CLASS)\
    bool operator==(const CLASS& other) const noexcept;\
    bool operator!=(const CLASS& other) const noexcept;\
    bool operator>(const CLASS& other) const noexcept;\
    bool operator<(const CLASS& other) const noexcept;\
    bool operator>=(const CLASS& other) const noexcept;\
    bool operator<=(const CLASS& other) const noexcept;

#define dOpsDef(CLASS, MEMBER_VAR)\
bool CLASS::operator==(const CLASS& other) const noexcept {return MEMBER_VAR == other.MEMBER_VAR;}\
bool CLASS::operator!=(const CLASS& other) const noexcept {return MEMBER_VAR == other.MEMBER_VAR;}\
bool CLASS::operator>(const CLASS& other) const noexcept {return MEMBER_VAR > other.MEMBER_VAR;}\
bool CLASS::operator<(const CLASS& other) const noexcept {return MEMBER_VAR < other.MEMBER_VAR;}\
bool CLASS::operator>=(const CLASS& other) const noexcept {return MEMBER_VAR >= other.MEMBER_VAR;}\
bool CLASS::operator<=(const CLASS& other) const noexcept {return MEMBER_VAR <= other.MEMBER_VAR;}

#define dOpsDefB(WRAPPER_CLASS, CLASS, MEMBER_VAR)\
bool WRAPPER_CLASS::CLASS::operator==(const CLASS& other) const noexcept {return MEMBER_VAR == other.MEMBER_VAR;}\
bool WRAPPER_CLASS::CLASS::operator!=(const CLASS& other) const noexcept {return MEMBER_VAR == other.MEMBER_VAR;}\
bool WRAPPER_CLASS::CLASS::operator>(const CLASS& other) const noexcept {return MEMBER_VAR > other.MEMBER_VAR;}\
bool WRAPPER_CLASS::CLASS::operator<(const CLASS& other) const noexcept {return MEMBER_VAR < other.MEMBER_VAR;}\
bool WRAPPER_CLASS::CLASS::operator>=(const CLASS& other) const noexcept {return MEMBER_VAR >= other.MEMBER_VAR;}\
bool WRAPPER_CLASS::CLASS::operator<=(const CLASS& other) const noexcept {return MEMBER_VAR <= other.MEMBER_VAR;}
