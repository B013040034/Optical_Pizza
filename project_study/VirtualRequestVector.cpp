#include <vector>
#include <iostream>
#include <fstream>
#include "NetworkGraph.h"
#include "VirtualRequestVector.h"

// �qf���إߦh�ӵ��������ШD�զ���vector
VirtualRequestVector::VirtualRequestVector(std::ifstream& f) {
		v = std::vector<NetworkGraph>();

		while (!f.eof()) {

			std::string line;
			std::getline(f, line);

			// Ū�J�ɶ��ƭ�
			int time_properties[4];
			std::istringstream(line) >> time_properties[Time_Attribute::time_arrived] >> time_properties[Time_Attribute::time_started]
				>> time_properties[Time_Attribute::delta_time] >> time_properties[Time_Attribute::time_ended];

			/*std::cout << time_properties[Time_Attribute::time_arrived] << " " << time_properties[Time_Attribute::time_started]
				<< " " << time_properties[Time_Attribute::delta_time] << " " << time_properties[Time_Attribute::time_ended] << std::endl;*/ // debug

			// Ū�J�C�@��node��capacity
			std::vector<int> nodes_capacity = std::vector<int>();
			for (int temp = 0; std::getline(f, line);) {

				std::istringstream(line) >> temp;
				if (temp == -1) break;

				nodes_capacity.push_back(temp);
			}

			// Ū�J�C�@��link�M��FSC
			std::vector<std::vector<int>> links = std::vector<std::vector<int>>();
			for (int temp[3]; std::getline(f, line);) {

				std::istringstream(line) >> temp[0] >> temp[1] >> temp[2];
				if (temp[0] == -1) break;

				//std::istringstream(line) >> temp[1] >> temp[2]; // �o�˪��g�k�O�|�X�����A���ڤ����D������C

				//std::cout << temp[0] << " " << temp[1] << " " << temp[2] << std::endl; // DEBUG

				std::vector<int> temp_vector = std::vector<int>();
				temp_vector.push_back(temp[0]);
				temp_vector.push_back(temp[1]);
				temp_vector.push_back(temp[2]);

				links.push_back(temp_vector);
			}

			v.push_back(NetworkGraph(time_properties, nodes_capacity, links));

		} /* while */
	} /* VirtualRequestVector */

// ����[]�B��l �o�ˤ�����[
NetworkGraph VirtualRequestVector::operator[](int index) {
	return v[index];
}