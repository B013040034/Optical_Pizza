#include <vector>
#include <fstream>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include "names.h"
#include "NetworkGraph.h"

// ���������constructor
NetworkGraph::NetworkGraph(std::ifstream& f) {

	std::cout << "###Constructing physical network...###" << std::endl;
	is_virtual = false;

	std::string line;
	int c;
	// Ū�J�Ҧ����I�M��e�q
	std::getline(f, line);
	std::istringstream(line) >> c;
	// �s�W�Ҧ�����`�I
	while (c != -1) {

		Vertex u;
		u = add_vertex(g);
		vertex_capacity_map[u] = c;

		std::cout << "Vertex " << vertex_index_map[u] << " with capacity " << vertex_capacity_map[u] << " inserted." << std::endl;
		
		std::getline(f, line);
	
		std::istringstream(line) >> c;
	}
	// �s�W�Ҧ��������
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
	//std::cout << "Number of edges = " << num_edges(g) << std::endl; // DEBUG ���edge���ƶq

	// �N�C�@�ӹ���`�I��capacity timeline�����]�w���Ӹ`�I���w�]capacity�C�o�N�O�b�٨S������ШD�i�Ӥ��e�`�I���B�z��O�C
	physical_capacity_timeline = (unsigned int**)malloc(num_vertices(g) * sizeof(unsigned int*));
	unsigned int *physical_capacity_timeline_storage = (unsigned int*)malloc(num_vertices(g) * TIMELINE_MAX * sizeof(unsigned int));
	for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
		physical_capacity_timeline[vertex_index_map[*v_p.first]] = &physical_capacity_timeline_storage[vertex_index_map[*v_p.first] * TIMELINE_MAX];
		for (int j = 0; j < TIMELINE_MAX; j++) {
			physical_capacity_timeline[vertex_index_map[*v_p.first]][j] = vertex_capacity_map[*v_p.first];
		}
	}

	// �N�C�@�ӹ����䪺�C�@�Ӯɶ��I��FS�����]�w��'1'
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

	// �p��C�@�ӹ���`�I����lLRC
	for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
		vertex_LRC_map[*v_p.first] = physical_vertex_LRC(*v_p.first);
	}

	std::cout << "###Finished constructing physical network.###" << std::endl << std::endl;
};	/* NetworkGraph() */

// ����������constructor
NetworkGraph::NetworkGraph(int time_properties[], std::vector<int> nodes_capacity, std::vector<std::vector<int>> links) {

	std::cout << "###Constructing virtual network request...###" << std::endl;

	is_virtual = true;

	// �s�W�ɶ���T
	for (int i = 0; i < 4; i++)
		this->time_properties[i] = time_properties[i];

	bool inserted;

	Vertex u, v;
	// �s�W�Ҧ�node�M��capacity
	for (int c : nodes_capacity) {
		u = add_vertex(g);
		vertex_capacity_map[u] = c;
		//vertex_index_map[u] = i; // �o�@����ܵL�k�ק�vertex_index_map���ȡA�ܩ_�ǡA
		// ���ӬO�]��vertex_index_map�badd_vertex���ɭԴN���Ӷ��Ƿs�W�F

		std::cout << "Inserted node " << vertex_index_map[u] << " with capacity consume " << vertex_capacity_map[u] << std::endl;
	}

	Edge e;

	// �Nedges�s�W��g��
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
	
	// Traverse�C�@�ӵ����I�A�åB�p���LRC
	for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
		int lrc = 0;
		
		// �b�o�̨ϥ�out_edge_iterator�ɾD�J�F�@���Y�������D
		// e_p����e������[�W*���A���Ov_p�N�n�C�o�쩳�O�����z�H�I
		for (std::pair<out_edge_iterator, out_edge_iterator> e_p = out_edges((*v_p.first), g); e_p.first != e_p.second; e_p.first++)
			lrc += edge_FSC_map[*e_p.first];
		
		lrc *= vertex_capacity_map[*v_p.first];
		vertex_LRC_map[*v_p.first] = lrc;
		
		std::cout << "Node " << vertex_index_map[*v_p.first]
			<< " has LRC " << vertex_LRC_map[*v_p.first] << std::endl;
	}

	std::cout << "###Finished constructing virtual network request.###" << std::endl << std::endl;
}	/* NetworkGraph() */

// �L�X�ӹ���ε�����������ơAdebug�ΡC
void NetworkGraph::print() {

	if(is_virtual){}
	else {
		// DEBUG: ��ܨC�@�ӹ���`�I��capacity timeline
		for (std::pair<vertex_iterator, vertex_iterator> v_p = vertices(g); *v_p.first != *v_p.second; *v_p.first++) {
			std::cout << "Capacity timeline of vertex " << vertex_index_map(*v_p.first) << ":" << std::endl;
			for (int i = 0; i < TIMELINE_MAX; i++)
				std::cout << physical_capacity_timeline[vertex_index_map(*v_p.first)][i];
			std::cout << std::endl;
		}
		std::cout << std::endl;

		// DEBUG: ��ܨC�@�ӹ����䪺�C�@�Ӯɶ��I��FS�A�`�N�bB��TIMELINE_MAX�����`�����p�U�A�o�|��X�D�`�D�`�h�F��
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

// �N��������in_g�O�J���������
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

// �p���@����`�I��LRC�A�]���{�b���T�w�䤽���A�N�����ϥ�capacity��@LRC(�Ȯɪ�)�C
int NetworkGraph::physical_vertex_LRC(Vertex u) {
	
	if (is_virtual) {
		std::cout << "Warning: Tried to compute virtual vertex LRC with physical function." << std::endl;
		return -1;
	}

	return vertex_capacity_map[u];

}