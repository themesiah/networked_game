#pragma once

#ifndef H_SERIALIZE_UTILS
#define H_SERIALIZE_UTILS

#include "OutputMemoryBitStream.h"
#include "InputMemoryBitStream.h"
#include "CustomReflection.h"
#include "ReflectionData.h"
#include <map>

#define OffsetOf(c, mv) ((size_t) & (static_cast<c*>(nullptr)->mv))

#define SERIALIZE_TYPE_READ(TYPE) \
if (entropyCompression) {\
	TYPE def;\
	memcpy(&def, mv.mDefaultEntropyValue, sizeof(TYPE));\
	bool isDefault;\
	ms.Serialize(isDefault);\
	if (isDefault) {\
		memcpy(mvData, mv.mDefaultEntropyValue, sizeof(TYPE));\
	} else {\
		ms.Serialize(*(TYPE*)mvData);\
	}\
} else {\
	ms.Serialize(*(TYPE*)mvData);\
}\

#define SERIALIZE_TYPE_WRITE(TYPE) \
if (entropyCompression) {\
	TYPE def;\
	memcpy(&def, mv.mDefaultEntropyValue, sizeof(TYPE));\
	TYPE data;\
	memcpy(&data, mvData, sizeof(TYPE));\
	bool isDefault = data == def;\
	ms.Serialize(isDefault);\
	if (!isDefault) {\
		ms.Serialize(*(TYPE*)mvData);\
	}\
} else {\
	ms.Serialize(*(TYPE*)mvData);\
}\

#define SERIALIZE_FLOAT_WRITE \
if (entropyCompression) {\
	float def;\
	memcpy(&def, mv.mDefaultEntropyValue, sizeof(float));\
	float data;\
	memcpy(&data, mvData, sizeof(float));\
	bool isDefault = data == def;\
	ms.Serialize(isDefault);\
	if (!isDefault) {\
		if (mv.mFloatCompression) {\
			ms.Serialize(*(float*)mvData, mv.mFMin, mv.mFPrecision); \
		} else {\
			ms.Serialize(*(float*)mvData); \
		}\
	}\
} else {\
	if (mv.mFloatCompression) {\
		ms.Serialize(*(float*)mvData, mv.mFMin, mv.mFPrecision); \
	} else {\
		ms.Serialize(*(float*)mvData); \
	}\
}\

#define SERIALIZE_FLOAT_READ \
if (entropyCompression) {\
	float def;\
	memcpy(&def, mv.mDefaultEntropyValue, sizeof(float));\
	bool isDefault;\
	ms.Serialize(isDefault);\
	if (isDefault) {\
		memcpy(mvData, mv.mDefaultEntropyValue, sizeof(float));\
	} else {\
		if (mv.mFloatCompression) {\
			ms.Serialize(*(float*)mvData, mv.mFMin, mv.mFPrecision); \
		} else {\
			if (mv.mFloatCompression) {\
				ms.Serialize(*(float*)mvData, mv.mFMin, mv.mFPrecision); \
			} else {\
				ms.Serialize(*(float*)mvData); \
			}\
		}\
	}\
} else {\
	if (mv.mFloatCompression) {\
		ms.Serialize(*(float*)mvData, mv.mFMin, mv.mFPrecision); \
		} else {\
		ms.Serialize(*(float*)mvData); \
		}\
}\

class SerializeUtils
{
public:
	static void SerializeRead(InputMemoryBitStream& ms, uint8_t* begin, uint32_t classIdentifier) {
		DataType * dataType = GET_REFLECTION_DATA(classIdentifier);
		for (MemberVariable mv : dataType->GetMemberVariables())
		{
			void* mvData = begin + mv.GetOffset();
			bool entropyCompression = mv.mEntropyCompression;
			switch (mv.GetPrimitiveType())
			{
			case EPT_Int:
				SERIALIZE_TYPE_READ(int);
				break;
			case EPT_Short:
				SERIALIZE_TYPE_READ(short);
				break;
			case EPT_String:
				ms.Serialize(*(std::string*) mvData);
				break;
			case EPT_Float:
				SERIALIZE_FLOAT_READ;
				break;
			case EPT_Bool:
				SERIALIZE_TYPE_READ(bool);
				break;
			case EPT_MapIntInt:
				ms.Serialize(*(std::map<int, int>*)mvData);
				break;
			case EPT_IntArray:
				ms.Serialize(*(std::vector<int>*)mvData);
				break;
			}
		}
	}

	static void SerializeWrite(OutputMemoryBitStream& ms, uint8_t* begin, uint32_t classIdentifier) {
		DataType * dataType = GET_REFLECTION_DATA(classIdentifier);
		for (MemberVariable mv : dataType->GetMemberVariables())
		{
			void* mvData = begin + mv.GetOffset();
			bool entropyCompression = mv.mEntropyCompression;
			switch (mv.GetPrimitiveType())
			{
			case EPT_Int:
				SERIALIZE_TYPE_WRITE(int);
				break;
			case EPT_Short:
				SERIALIZE_TYPE_WRITE(short);
				break;
			case EPT_String:
				ms.Serialize(*(std::string*) mvData);
				break;
			case EPT_Float:
				SERIALIZE_FLOAT_WRITE;
				break;
			case EPT_Bool:
				SERIALIZE_TYPE_WRITE(bool);
				break;
			case EPT_MapIntInt:
				ms.Serialize(*(std::map<int, int>*)mvData);
				break;
			case EPT_IntArray:
				ms.Serialize(*(std::vector<int>*)mvData);
				break;
			}
		}
	}

};

#endif



