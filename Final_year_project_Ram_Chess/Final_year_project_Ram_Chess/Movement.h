#pragma once
#include "Coord.h"

//Structure, which specifies a movement.
struct Movement
{
	Coord srcCoord, destCoord;
	Movement(Coord& srcCoord, Coord& destCoord);

	friend bool operator==(const Movement& lhs, const Movement& rhs)
	{
		return lhs.srcCoord == rhs.srcCoord && lhs.destCoord == rhs.destCoord;
	}
};

