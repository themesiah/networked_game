#include "Endianness.h"

template <typename tFrom, typename tTo> TypeAliaser<tFrom, tTo>::TypeAliaser(tFrom inFromValue) : mAsFromType(inFromValue) {}

template <typename tFrom, typename tTo> tTo& TypeAliaser<tFrom, tTo>::Get() {
	return mAsToType;
}

//specialize for 2...
template <typename T> class ByteSwapper<T, 2>
{
public:
	T Swap(T inData) const
	{
		uint16_t result = ByteSwap2(TypeAliaser<T, uint16_t>(inData).Get());
		return TypeAliaser<uint16_t, T>(result).Get();
	}
};

//specialize for 4...
template <typename T> class ByteSwapper<T, 4>
{
public:
	T Swap(T inData) const
	{
		uint32_t result = ByteSwap4(TypeAliaser<T, uint32_t>(inData).Get());
		return TypeAliaser<uint32_t, T>(result).Get();
	}
};

//specialize for 8...
template <typename T> class ByteSwapper<T, 8>
{
public:
	T Swap(T inData) const
	{
		uint64_t result = ByteSwap8(TypeAliaser<T, uint64_t>(inData).Get());
		return TypeAliaser<uint64_t, T>(result).Get();
	}
};

template <typename T> T ByteSwap(T inData)
{
	return ByteSwapper<T, sizeof(T) >().Swap(inData);
}