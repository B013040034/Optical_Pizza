#include <vector>
#include <fstream>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include "names.h"
#include "NetworkGraph.h"

// 實體網路的constructor
NetworkGraph::NetworkGraph(std::ifstream& f) {

	std::cout << "###Constructing physical network...###" << std::endl;
	is_virtual = false;

	std::string line;
	int c;
	// 讀入所有的點和其容量
	std::getline(f, line);
	std::istringstream(line) >> c;
	// 新增所有實體節點
	while (c != -1) {

		Vertex u;
		u = add_vertex(g);
		vertex_capacity_map[u] = c;

		std::cout << "Vertex " << vertex_index_map[u] << " with capacity " << vertex_capacity_map[u] << " inserted." << std::endl;
		
		std::getline(f, line);
	
		std::istringstream(line) >> c;
	}
	// 新增所有實體鏈路
	int index = 0;
	while (!f.eof()) {

		int n1, n2, d;
		std::getline(f, line);
		std::istringstream(line) >> n1 >> n2 >> d;

		Vertex u, v;
		Edge e;
		bool inserted;
		u = vertex_index_map[n1];
		v = vertex_index_map[n2];
		tie(e, inserted) = add_edge(u, v, g);
		
		if (inserted) {
			edge_distance_map[e] = d;
			edge_index_map[e] = index++;
			std::cout << "Edge with vertex " << vertex_index_map[u]
				<< " and vertex " << vertex_index_map[v] << " inserted. And its distance is " << edge_distance_map[e] << "." << std::endl;
		}
		else {
			std::cout << "Failed to insert edge with vertex " << vertex_index_map[u]
				<< " and vertex " << vertex_index_map[v] << "." << std::endl;
		}
	}
	//std::cout << "Number of edges = " << num_edges(g) << std::endl; // DEBUG 顯示edge的數量

	// 將每一個實體節點的capacity timeline全部設定為該節點的預設capacity。這就是在還沒有任何請求進來之前節點的處理能力。
	physical_capacity_timeline = (unsigned int**)malloc(num_vertices(g) * sizeof(unsigned int*));
	unsigned int *physical_capacity_timeline_storage = (unsigned int*)malloc(num_vertices(g) * TIMELINE_MAX * sizeof(unsigned int));
	for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
		physical_capacity_timeline[vertex_index_map[*v_p.first]] = &physical_capacity_timeline_storage[vertex_index_map[*v_p.first] * TIMELINE_MAX];
		for (int j = 0; j < TIMELINE_MAX; j++) {
			physical_capacity_timeline[vertex_index_map[*v_p.first]][j] = vertex_capacity_map[*v_p.first];
		}
	}

	// 將每一個實體邊的每一個時間點的FS全部設定為'1'
	FS_timeline = (char***)malloc(num_vertices(g) * sizeof(char**));
	char **FS_timeline_timeStorage = (char**)malloc(num_vertices(g) * TIMELINE_MAX * sizeof(char*));
	char *FS_timeline_FSStorage = (char*)malloc(num_vertices(g) * TIMELINE_MAX * B * sizeof(char));
	for (std::pair<edge_iterator, edge_iterator> e_p = edges(g); e_p.first != e_p.second; e_p.first++) {
		FS_timeline[edge_index_map[*e_p.first]] = &FS_timeline_timeStorage[edge_index_map[*e_p.first] * TIMELINE_MAX];
		for (int j = 0; j < TIMELINE_MAX; j++) {
			FS_timeline_timeStorage[edge_index_map[*e_p.first] * TIMELINE_MAX + j] = 
				&FS_timeline_FSStorage[edge_index_map[*e_p.first] * TIMELINE_MAX * B + j * TIMELINE_MAX];
			for (int k = 0; k < B; k++)
				if (edge_index_map[*e_p.first] == 2)
					if (k > (B / 2))
						FS_timeline[edge_index_map[*e_p.first]][j][k] = (char)((int)k + (int)'0');
					else if(j < (TIMELINE_MAX / 2))
						FS_timeline[edge_index_map[*e_p.first]][j][k] = '2';
					else FS_timeline[edge_index_map[*e_p.first]][j][k] = (char)((int)j + (int)'0');
				else
					FS_timeline[edge_index_map[*e_p.first]][j][k] = '1';
		}
	}

	// 計算每一個實體節點的初始LRC
	for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
		vertex_LRC_map[*v_p.first] = physical_vertex_LRC(*v_p.first);
	}

	std::cout << "###Finished constructing physical network.###" << std::endl << std::endl;
};	/* NetworkGraph() */

// 虛擬網路的constructor
NetworkGraph::NetworkGraph(int time_properties[], std::vector<int> nodes_capacity, std::vector<std::vector<int>> links) {

	std::cout << "###Constructing virtual network request...###" << std::endl;

	is_virtual = true;

	// 新增時間資訊
	for (int i = 0; i < 4; i++)
		this->time_properties[i] = time_properties[i];

	bool inserted;

	Vertex u, v;
	// 新增所有node和其capacity
	for (int c : nodes_capacity) {
		u = add_vertex(g);
		vertex_capacity_map[u] = c;
		//vertex_index_map[u] = i; // 這一行顯示無法修改vertex_index_map的值，很奇怪，
		// 應該是因為vertex_index_map在add_vertex的時候就按照順序新增了

		std::cout << "Inserted node " << vertex_index_map[u] << " with capacity consume " << vertex_capacity_map[u] << std::endl;
	}

	Edge e;

	// 將edges新增到g中
	for (std::vector<int> l : links) {

		u = vertex_index_map[l[0]];
		v = vertex_index_map[l[1]];
		tie(e, inserted) = add_edge(u, v, g);

		if (inserted) {
			edge_FSC_map[e] = l[2];
			std::cout << "Insert link with node " << vertex_index_map[u]
				<< " and node " << vertex_index_map[v] << " and its capacity is " << edge_FSC_map[e] << std::endl;
		}
		else {
			std::cout << "Failed to insert link with node " << vertex_index_map[u]
				<< " and node " << vertex_index_map[v] << std::endl;
		}
	} /* for */
	
	// Traverse每一個虛擬點，並且計算其LRC
	for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
		int lrc = 0;
		
		// 在這裡使用out_edge_iterator時遭遇了一個嚴重的問題
		// e_p不能前面不能加上*號，但是v_p就要。這到底是什麼原理？！
		for (std::pair<out_edge_iterator, out_edge_iterator> e_p = out_edges((*v_p.first), g); e_p.first != e_p.second; e_p.first++)
			lrc += edge_FSC_map[*e_p.first];
		
		lrc *= vertex_capacity_map[*v_p.first];
		vertex_LRC_map[*v_p.first] = lrc;
		
		std::cout << "Node " << vertex_index_map[*v_p.first]
			<< " has LRC " << vertex_LRC_map[*v_p.first] << std::endl;
	}

	std::cout << "###Finished constructing virtual network request.###" << std::endl << std::endl;
}	/* NetworkGraph() */

// 印出該實體或虛擬網路的資料，debug用。
void NetworkGraph::print() {

	if(is_virtual){}
	else {
		// DEBUG: 顯示每一個實體節點的capacity timeline
		for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
			std::cout << "Capacity timeline of vertex " << vertex_index_map(*v_p.first) << ":" << std::endl;
			for (int i = 0; i < TIMELINE_MAX; i++)
				std::cout << physical_capacity_timeline[vertex_index_map(*v_p.first)][i];
			std::cout << std::endl;
		}
		std::cout << std::endl;

		// DEBUG: 顯示每一個實體邊的每一個時間點的FS，注意在B跟TIMELINE_MAX為正常的情況下，這會輸出非常非常多東西
		for (std::pair<edge_iterator, edge_iterator> e_p = edges(g); e_p.first != e_p.second; e_p.first++) {
			std::cout << "FS timeline of edge " << edge_index_map(*e_p.first) << " at all time:" << std::endl;
			for (int j = 0; j < TIMELINE_MAX; j++) {
				std::cout << "Time " << j << ": ";
				for (int i = 0; i < B; i++)
					std::cout << FS_timeline[edge_index_map(*e_p.first)][j][i];
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

}

// 將虛擬網路in_g嵌入此實體網路
bool NetworkGraph::embed_request(NetworkGraph in_g) {

	if (is_virtual) {
		std::cout << "Virtual network can not be embed." << std::endl;
		return false;
	}

	if (!in_g.is_virtual) {
		std::cout << "Embedding network has to be virtual." << std::endl;
		return false;
	}




}

// 計算單一實體節點的LRC，因為現在不確定其公式，就直接使用capacity當作LRC(暫時的)。
int NetworkGraph::physical_vertex_LRC(Vertex u) {
	
	if (is_virtual) {
		std::cout << "Warning: Tried to compute virtual vertex LRC with physical function." << std::endl;
		return -1;
	}

	return vertex_capacity_map[u];

}