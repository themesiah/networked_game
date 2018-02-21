#pragma once

#define STREAM_ENDIANNESS LITTLE
#ifndef PLATFORM_PS4 // POR PONER ALGUNA PLATAFORMA
#define PLATFORM_ENDIANNESS LITTLE
#endif

template <typename tFrom, typename tTo> class TypeAliaser
{
public:
	TypeAliaser(tFrom inFromValue);
	tTo& Get();
	union
	{
		tFrom mAsFromType;
		tTo mAsToType;
	};
};

template <typename T, size_t tSize> class ByteSwapper;

