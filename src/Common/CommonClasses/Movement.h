#pragma once

#ifndef H_MOVEMENT
#define H_MOVEMENT

#include "Replication\GameObject.h"

class CMovement : public GameObject {
public:
	enum Direction : uint8_t
	{
		LEFT = 0,
		TOP = 1,
		RIGHT = 2,
		BOTTOM = 3
	};
	CMovement() :
		GameObject()
		, inputX(0.f)
		, inputY(0.f)
	{
	}
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("inputX", OffsetOf(CMovement, inputX), -1000.0f, 0.1f),
			MemberVariable("inputY", OffsetOf(CMovement, inputY), -1000.0f, 0.1f)
		}
		);
	}
	CLASS_IDENTIFICATION('CMOV', CMovement);

	virtual void SerializeWrite(OutputMemoryBitStream& ms) override {
		if (inputX < 0.f)
		{
			ms.Serialize(LEFT, 2);
		}
		else if (inputX > 0.f)
		{
			ms.Serialize(RIGHT, 2);
		}
		else if (inputY < 0.f)
		{
			ms.Serialize(TOP, 2);
		}
		else if (inputY > 0.f)
		{
			ms.Serialize(BOTTOM, 2);
		}
	}

	void SetMovement(float x, float y) {
		inputX = x;
		inputY = y;
		if (x != 0.f || y != 0.f) {
			SetDirty();
		}
	}

	float* GetMovement()
	{
		float val[2];
		val[0] = inputX;
		val[1] = inputY;
		return val;
	}
private:
	float inputX;
	float inputY;
};

#endif