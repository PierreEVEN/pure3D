#pragma once

#include <vector>
#include <inttypes.h>
#include <cstdlib>
#include <string>
#include <iosfwd>
#include <iostream>
#include <memory>

#ifndef STRING_MINIMAL_LENGTH
#define STRING_MINIMAL_LENGTH 16
#endif

class String;
class IStringable
{
public:
	virtual const String ToString() const = 0;
};

class String;

/* Value To String */
const String& ToString(const String& value);
String ToString(int64_t value);
String ToString(uint64_t value);
String ToString(int32_t value);
String ToString(uint32_t value);
String ToString(double value);
String ToString(float value);

int32_t Atoi(const String& value);
double Atof(const String& value);

class String
{
public:
	inline static const char ENDL = '\n';

	/* Constructors */
	String() = default;
	~String();

	inline String(const char* value) {
		if (value)
		{
			length = 0;
			while (value[length] != '\0')
				length++;

			SetLength(length);

			for (int i = 0; i < length; ++i)
				data[i] = value[i];
		}
	}

	inline String(const char& value) { SetLength(1); data[length - 1] = value; }
	inline String(String&& other) { CopyTo(other, this); }
	inline String(const String& other) { CopyTo(other, this); }
	inline String(const IStringable& other) { CopyTo(other.ToString(), this); }
	inline String(int32_t other) : String(ToString(other)) {}
	inline String(uint32_t other) : String(ToString(other)) {}
	inline String(int64_t other) : String(ToString(other)) {}
	inline String(uint64_t other) : String(ToString(other)) {}
	inline String(float other) : String(ToString(other)) {}
	inline String(double other) : String(ToString(other)) {}

	/** Iterators */
	typedef char* iterator;
	typedef const char* const_iterator;
	iterator begin() { return &data[0]; }
	const_iterator begin() const { return &data[0]; }
	const_iterator cbegin() const { return &data[0]; }
	iterator end() { return &data[length]; }
	const_iterator end() const { return &data[length]; }
	const_iterator cend() const { return &data[length]; }

	/* Getters */
	inline const size_t& Length() const { return length; }
	inline const char* GetData() const {
		data[length] = '\0';
		return data;
	}
	
	/* Operator bool */
	inline operator bool() const {
		return *this != "";
	}

	/* Operator == */
	bool operator==(const String& _Val) const;

	/* Operator != */
	bool operator!=(const String& _Val) const;

	/* Operator < */
	bool operator<(const String& _Val) const;

	/* Operator /= */
	inline String& operator/=(const String& _Val) { return Append(String('/') + _Val); }

	/* Operator / */
	inline String operator/(const String& _Val) const { return Concatenate(String(GetData()), String(String('/') + _Val)); }

	/* Operator * */
	inline const char* operator*() { return GetData(); }

	/* Operator [] */
	inline const char& operator[](size_t pos) const { return data[pos]; }

	/* Operator -= */
	inline String& operator-=(const size_t _Val) { SetLength(length - _Val); return *this; }

	/* Operator << */
	inline String& operator<<(const String& _Val) { return Append(_Val); }
	inline String& operator<<(const char* _Val) { return Append(_Val); }
	inline String& operator<<(const char _Val) { return Append(_Val); }
	inline String& operator<<(void* _Val) { return Append("ptr"); }
	inline String& operator<<(const int64_t _Val) { return Append(ToString(_Val)); }
	inline String& operator<<(const uint64_t _Val) { return Append(ToString(_Val)); }
	inline String& operator<<(const int32_t _Val) { return Append(ToString(_Val)); }
	inline String& operator<<(const uint32_t _Val) { return Append(ToString(_Val)); }
	inline String& operator<<(const float _Val) { return Append(ToString(_Val)); }
	inline String& operator<<(const double _Val) { return Append(ToString(_Val)); }
	inline String& operator<<(const IStringable& _Val) { return Append(_Val.ToString()); }

	/* Operator += */
	inline String& operator+=(const String& _Val) { return Append(_Val); }
	inline String& operator+=(const char* _Val) { return Append(_Val); }
	inline String& operator+=(const char _Val) { return Append(_Val); }
	inline String& operator+=(void* _Val) { return Append("ptr"); }
	inline String& operator+=(const int64_t _Val) { return Append(ToString(_Val)); }
	inline String& operator+=(const uint64_t _Val) { return Append(ToString(_Val)); }
	inline String& operator+=(const int32_t _Val) { return Append(ToString(_Val)); }
	inline String& operator+=(const uint32_t _Val) { return Append(ToString(_Val)); }
	inline String& operator+=(const float _Val) { return Append(ToString(_Val)); }
	inline String& operator+=(const double _Val) { return Append(ToString(_Val)); }
	inline String& operator+=(const IStringable& _Val) { return Append(_Val.ToString()); }

	/* Operator + */
	inline String operator+(const String& _Val) const { return Concatenate(String(GetData()), String(_Val)); }
	inline String operator+(const char* _Val) const { return Concatenate(String(GetData()), String(_Val)); }
	inline String operator+(const char _Val) const { return Concatenate(String(GetData()), String(_Val)); }
	inline String operator+(const int64_t _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const uint64_t _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const int32_t _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const uint32_t _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const double _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const float _Val) const { return Concatenate(String(GetData()), ToString(_Val)); }
	inline String operator+(const IStringable& _Val) const { return Concatenate(String(GetData()), _Val.ToString()); }

	/* Operator = */
	inline String operator=(const String& other) { return CopyTo(other, this); }
	inline String operator=(const char other) { return CopyTo(other, this); }
	inline String operator=(const char* other) { return CopyTo(other, this); }
	inline String operator=(const int64_t other) { return CopyTo(ToString(other), this); }
	inline String operator=(const int32_t other) { return CopyTo(ToString(other), this); }
	inline String operator=(const double other) { return CopyTo(ToString(other), this); }
	inline String operator=(const float other) { return CopyTo(ToString(other), this); }
	inline String operator=(const IStringable& other) { return CopyTo(other.ToString(), this); }


	friend std::ostream& operator<<(std::ostream& output, const String& D) {
		output.write(D.GetData(), D.Length() + 1);;
		return output;
	}

	friend std::istream& operator>>(std::istream& input, String& D) {
		D << (input.rdbuf()->sgetc());
		return input;
	}

	/* Tests */
	inline static bool IsAlpha(const char& chr) { return (chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z'); }
	inline static bool IsNumeric(const char& chr) { return (chr >= '0' && chr <= '9'); }
	inline static bool IsAlphanumeric(const char& chr) { return IsAlpha(chr) || IsNumeric(chr); }

	inline const size_t CountChar(const char inChar) const { int Count = 0; for (const auto& chr : *this) Count += chr == inChar; return Count; }

	/* String operations */
	inline static const String Concatenate(const String& left, const String& right) {
		String res;
		res.SetLength(left.length + right.length);
		for (size_t i = 0; i < left.length; ++i)
			res.data[i] = left[i];
		for (size_t i = left.length; i < res.length; ++i)
			res.data[i] = right[i - left.length];
		return res;
	}

	inline String& Append(const String& other) {
		size_t thisLength = length;
		SetLength(thisLength + other.length);
		for (size_t i = thisLength; i < length; ++i)
			data[i] = other.data[i - thisLength];
		return *this;
	}

	inline String SubString(const size_t From) {
		return SubString(From, Length() - 1);
	}

	inline String SubString(size_t From, const size_t To) {
		String Result = "";
		for (size_t i = From; i <= To && i < Length(); ++i) {
			Result << data[i];
		}
		return Result;
	}

	inline bool IsStartingWith(const String& start) const {
		if (start.length > length) return false;
		for (size_t i = 0; i < start.length; ++i)
			if ((*this)[i] != start[i])
				return false;
		return true;
	}

	template<typename T>
	inline static const String ConcatenateArray(T& iterable, String separator = ", ")
	{
		String result = "";
		size_t elemCount = iterable.cend() - iterable.cbegin();
		for (auto& elem : iterable)
		{
			result += ToString(elem);
			elemCount--;
			if (elemCount > 0) result += separator;
		}
		return result;
	}

	static bool SplitString(const String& lineData, std::vector<char> separators, String& left, String& right, bool bFromStart = true);

	inline bool Contains(const String& other) const {
		if (other.Length() == 0) return true;
		if (other.Length() > Length()) return false;
		size_t progress = 0;
		for (int i = 0; i < Length(); ++i) {
			if (data[i] != other[progress]) {
				progress = 0;
			}
			if (data[i] == other[progress]) {
				if (progress == other.Length() - 1) return true;
				progress++;
			}
		}
		return false;
	}


	inline static bool const IsStartingWith(const String& base, const String& start) {
		if (start.length > base.length) return false;
		for (int i = 0; i < start.length; ++i)
			if (base[i] != start[i])
				return false;
		return true;
	}

	inline static bool const IsEndingWith(const String& base, const String& end) {
		if (end.length > base.length) return false;
		for (int i = 1; i <= end.length; ++i)
			if (base[base.length - i] != end[end.length - i])
				return false;
		return true;
	}

	static String RemoveBorderSpaces(const String& line);
	static String Replace(const String& InString, const char Pattern, const String& NewValue);


	static const String GetFileName(const String& path);
	static const String GetFileShortName(const String& path);
	static const String GetFilePath(const String& path);
	static const String GetFileExtension(const String& path);

	static const std::vector<String> ParseStringCharArray(const char* charString, size_t length);

	inline static void ResetCharArray(char* str, const size_t& strLength) { for (int i = 0; i < strLength; ++i) str[i] = '\0'; }

	template<typename... Arguments>
	inline static String Format(const String& FormatStr, Arguments... args) {
		int Size = snprintf(nullptr, 0, FormatStr.GetData(), args...) + 1;
		if (Size <= 0) return FormatStr;
		std::unique_ptr<char[]> Buffer(new char[Size]);
		snprintf(Buffer.get(), Size, FormatStr.GetData(), args ...);
		return String(Buffer.get());
	}

private:

	inline static const String& CopyTo(const String& from, String* to) {
		to->SetLength(from.length);
		for (int i = 0; i < to->length; ++i)
		{
			to->data[i] = from.data[i];
		}
		return *to;
	}


	inline void SetLength(const size_t& newSize) {
		if (allocLength < newSize + 1 || newSize + 1 + STRING_MINIMAL_LENGTH < allocLength)
		{
			allocLength = newSize + 1 + STRING_MINIMAL_LENGTH;
			data = (char*)realloc(data, allocLength);
		}
		length = newSize;
	}

	char* data = new char[STRING_MINIMAL_LENGTH];

	size_t length = 0;
	size_t allocLength = STRING_MINIMAL_LENGTH;
};

namespace std {
	template<> struct hash<String> {

		std::size_t operator()(const String& t) const {
			hash<std::string> chrHash;
			std::size_t result = chrHash(t.GetData());
			return result;
		}
	};
}

inline String operator+(const char& chr, const String& str) { return String(chr) + str; }
inline String operator+(const char* chr, const String& str) { return String(chr) + str; }
inline String operator/(const char* chr, const String& str) { return String(chr) / str; }