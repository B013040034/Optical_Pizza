#pragma once
#include <vector>
#include "NetworkGraph.h"
class VirtualRequestVector
{
public:

	std::vector<NetworkGraph> v;

	VirtualRequestVector(std::ifstream& f);

	NetworkGraph operator[](int index);
};

