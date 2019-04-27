#include "names.h"
#include "VirtualRequestVector.h"
#include <fstream>
#include <iostream>
bool vone() {

	std::ifstream p_f("input_files/physical_network.txt");

	if (!p_f)
	{
		std::cout << "找不到physical_network file" << std::endl;
		return EXIT_FAILURE;
	}

	NetworkGraph p = NetworkGraph(p_f);

	std::ifstream v_f("input_files/virtual_requests.txt");

	if (!v_f)
	{
		std::cout << "找不到virtual_requests file" << std::endl;
		return EXIT_FAILURE;
	}

	VirtualRequestVector v = VirtualRequestVector(v_f);

	p.print();
	
	// TODO::選擇嵌入順序 見 判定需求的優先權.docx

	p.embed_request(v[0]); // p嘗試嵌入v[0]


	return true;
}
