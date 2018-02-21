#pragma once

#ifndef H_REFLECTION
#define H_REFLECTION

#include <inttypes.h>
#include <string>
#include <vector>
#include <initializer_list>

enum EPrimitiveType
{
	EPT_Int,
	EPT_String,
	EPT_Float,
	EPT_Bool,
	EPT_MapIntInt
};
class MemberVariable
{
public:
	MemberVariable(const char* inName,
		EPrimitiveType inPrimitiveType, uint32_t inOffset, bool entropyCompression = false, const void* defaultEntropyValue = nullptr) :
		mName(inName),
		mPrimitiveType(inPrimitiveType),
		mOffset(inOffset)
	{
		mEntropyCompression = entropyCompression;
		mDefaultEntropyValue = defaultEntropyValue;
		mFloatCompression = false;
	}

	MemberVariable(const char* inName, uint32_t inOffset, float fMin, float fPrecision, bool entropyCompression = false, const void* defaultEntropyValue = nullptr) :
		mName(inName),
		mPrimitiveType(EPrimitiveType::EPT_Float),
		mOffset(inOffset),
		mFMin(fMin),
		mFPrecision(fPrecision)
	{
		mEntropyCompression = entropyCompression;
		mDefaultEntropyValue = defaultEntropyValue;
		mFloatCompression = true;
	}
	
	EPrimitiveType GetPrimitiveType() const { return mPrimitiveType; }
	uint32_t GetOffset() const { return mOffset; }
	bool mEntropyCompression;
	const void* mDefaultEntropyValue;
	bool mFloatCompression;
	float mFMin;
	float mFPrecision;
private:
	std::string mName;
	EPrimitiveType mPrimitiveType;
	uint32_t mOffset;
	
};
class DataType
{
public:
	DataType(std::initializer_list<MemberVariable> inMVs) :
		mMemberVariables(inMVs)
	{
	}
	const std::vector<MemberVariable>& GetMemberVariables() const
	{
		return mMemberVariables;
	}
private:
	std::vector<MemberVariable> mMemberVariables;
};

#endif