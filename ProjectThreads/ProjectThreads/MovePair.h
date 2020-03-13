#pragma once
#include "Pair.h"
class MovePair
{

public:
	Pair from;
	Pair to;

	MovePair(Pair p1, Pair p2) {
		from = p1;
		to = p2;
	}
};

