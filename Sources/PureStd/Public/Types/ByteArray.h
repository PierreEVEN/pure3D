#pragma once
#include <cstdint>
#include <cstdio>

struct ByteArray {

	inline ByteArray() { SetData(nullptr, 0); }

	inline ByteArray(uint8_t* InData, const size_t InSize) {
		SetData(InData, InSize);
	}

	~ByteArray() {
		SetData(nullptr, 0);
	}

	inline uint8_t* GetData() const { return Data; }

	inline size_t GetLength() const { return Length; }

	inline void SetData(uint8_t* InData, size_t Size) {
		if (Data) delete Data;
		Data = InData;
		Length = Size;
	}



private:

	uint8_t* Data;
	size_t Length;

};