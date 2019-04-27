#include "names.h"
#include "VirtualRequestVector.h"
#include <fstream>
#include <iostream>
bool vone() {

	std::ifstream p_f("input_files/physical_network.txt");

	if (!p_f)
	{
		std::cout << "�䤣��physical_network file" << std::endl;
		return EXIT_FAILURE;
	}

	NetworkGraph p = NetworkGraph(p_f);

	std::ifstream v_f("input_files/virtual_requests.txt");

	if (!v_f)
	{
		std::cout << "�䤣��virtual_requests file" << std::endl;
		return EXIT_FAILURE;
	}

	VirtualRequestVector v = VirtualRequestVector(v_f);

	p.print();
	
	// TODO::��ܴO�J���� �� �P�w�ݨD���u���v.docx

	p.embed_request(v[0]); // p���մO�Jv[0]


	return true;
}
