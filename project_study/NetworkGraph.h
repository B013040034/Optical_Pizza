#pragma once
#include "names.h"
class NetworkGraph
{
public:
	bool is_virtual;
	int time_properties[4];

	// 這裡f必須用指標的方式傳入，我不知道為什麼。
	NetworkGraph(std::ifstream& f);
	NetworkGraph(int time_properties[], std::vector<int> nodes_capacity, std::vector<std::vector<int>> links);

	bool embed_request(NetworkGraph in_g);
	void print();

private:

	Graph g;
	VertexIndexMap vertex_index_map = get(vertex_index, g);
	VertexCapacityMap vertex_capacity_map = get(vertex_name, g);
	VertexLRCMap vertex_LRC_map = get(vertex_index2, g);
	EdgeIndexMap edge_index_map = get(edge_index, g);
	EdgeDistanceMap edge_distance_map = get(edge_weight, g);
	EdgeFSCMap edge_FSC_map = get(edge_capacity, g);
	
	int edge_count;
	int vertex_count;

	unsigned int **physical_capacity_timeline;
	char ***FS_timeline;

	int physical_vertex_LRC(Vertex);

};