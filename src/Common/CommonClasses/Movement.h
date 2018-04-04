#pragma once

#ifndef H_MOVEMENT
#define H_MOVEMENT

#include "Replication\GameObject.h"

class CMovement : public GameObject {
public:
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