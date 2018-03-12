#pragma once

#ifndef H_MOVEMENT
#define H_MOVEMENT

#include "Replication\GameObject.h"

class CMovement : public GameObject {
public:
	CMovement() :
		inputX(0.f)
		, inputY(0.f)
	{
	}
	float inputX;
	float inputY;
	static DataType* GetReflectionData() {
		return new DataType({
			MemberVariable("inputX", OffsetOf(CMovement, inputX), -1000.0f, 0.1f),
			MemberVariable("inputY", OffsetOf(CMovement, inputY), -1000.0f, 0.1f)
		}
		);
	}
	CLASS_IDENTIFICATION('CMOV', CMovement);
	void Reset() {
		inputX = 0.f;
		inputY = 0.f;
	}
};

#endif