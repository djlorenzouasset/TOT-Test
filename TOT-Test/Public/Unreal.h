#pragma once
#include "framework.h"

template<typename T>
class TArray
{
private:
	T* Data;
	int32_t NumElements;
	int32_t MaxElements;

public:
	TArray()
	{
		Data = nullptr;
		NumElements = MaxElements = 0;
	}

	T* GetData() { return Data; }
	int32_t Num() { return NumElements; }

public:
	T& operator[](int32_t idx) { return Data[idx]; }
};

class FRequestPayload
{
public:
	void** VTable;
};

class FRequestPayloadInMemory : public FRequestPayload
{
public:
	TArray<uint8_t> Buffer;
};

class FCurlHttpRequest
{
public:
	char pad[0xB8];
	FRequestPayload* RequestPayload;
};