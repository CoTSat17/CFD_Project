#include <node.h>

Node* node_create(float x, float y, float z, int id) {
	Point position = point_create(x, y, z);

	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) return NULL;
	node->position = position;
	node->id = id;


	return node;
}

void node_destroy(Node* node) {
	free(node);
	node = NULL;
}






Nodes* nodes_create(float* nodes_position, int size_array) {
	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	if (nodes == NULL) return NULL;

	// -----------
	// Set n_nodes
	// -----------
	nodes->n_nodes = size_array / 3;



	// -----------------------
	// Set the array of nodes
	// -----------------------

	nodes->nodes = (Node**)malloc(sizeof(Node*) * nodes->n_nodes);
	if (nodes->nodes == NULL) return NULL;

	for (int i = 0; i < nodes->n_nodes; i++) {
		nodes->nodes[i] = node_create(nodes_position[i * 3], nodes_position[i * 3 + 1], nodes_position[i * 3 + 2], i + 1);
	}

	return nodes;

}




void nodes_destroy(Nodes* nodes) {
	for (int i = 0; i < nodes->n_nodes; i++) {
		node_destroy(nodes->nodes[i]);
	}

	free(nodes->nodes);
	free(nodes);
	nodes->nodes = NULL;
	nodes = NULL;

}



Node* nodes_search_id(Nodes* nodes, int id) {
	for (int i = 0; i < nodes->n_nodes; i++) {
		if (nodes->nodes[i]->id == id) return nodes->nodes[i];
	}

	return NULL;
}