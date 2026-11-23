#include <node.h>

Node* node_create(float x, float y, float z, int id) {
	Node* node = (Node*)malloc(sizeof(Node));
	if (node == NULL) return NULL;

	node->position = point_create(x, y, z);;
	node->id = id;


	return node;
}

void node_destroy(Node** node) {
	// Check for null pointers
	if (node == NULL || *node == NULL) return;


	// Free memory
	free(*node);
	*node = NULL;
}






Nodes* nodes_create(float* nodes_position, int size_array) {
	/// ------------
	/// CHECK INPUTS
	/// ------------
	if (nodes_position == NULL || size_array <= 0) return NULL;


	/// ------------
	/// RESERVE MEMORY
	/// ------------

	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	if (nodes == NULL) return NULL;

	// -----------
	// Set n_nodes
	// -----------
	nodes->n_nodes = size_array / 3;



	// -----------------------
	// Set the array of nodes
	// -----------------------

	nodes->nodes = (Node**)calloc(nodes->n_nodes, sizeof(Node*));
	if (nodes->nodes == NULL) {
		free(nodes);
		return NULL;
	}

	for (int i = 0; i < nodes->n_nodes; i++) {
		nodes->nodes[i] = node_create(nodes_position[i * 3], nodes_position[i * 3 + 1], nodes_position[i * 3 + 2], i + 1);
		// Chech for error
		if (nodes->nodes[i] == NULL) {
			nodes_destroy(&nodes);
			return NULL;
		}
	
	}

	return nodes;

}





int nodes_fill(Nodes* nodes_struct, float* nodes_data, int n_nodes) {
	// -----------
	// Check inputs
	// -----------
	if (nodes_struct == NULL || nodes_data == NULL || n_nodes <= 0)  {
		return 1;
	}



	// -----------
	// Set n_nodes
	// -----------
	nodes_struct->n_nodes = 0;



	// -----------------------
	// Set the array of nodes
	// -----------------------

	nodes_struct->nodes = (Node**)calloc(n_nodes, sizeof(Node*));
	if (nodes_struct->nodes == NULL) return 1;

	for (int i = 0; i < n_nodes; i++) {
		nodes_struct->nodes[i] = node_create(nodes_data[1 + i*4], nodes_data[2 + i*4], nodes_data[3+ i*4], nodes_data[i*4]);
		nodes_struct->n_nodes++;
		// Chech for error
		if (nodes_struct->nodes[i] == NULL) {
			nodes_clear(nodes_struct);
			return 1;		
		}
	}

	return 0;
}




void nodes_destroy(Nodes** nodes) {

	if (nodes == NULL || *nodes == NULL) return;

	// Empy the struct
	nodes_clear(*nodes);

	// Destroy struct
	free(*nodes);
	*nodes = NULL;

}



void nodes_clear(Nodes* nodes_struct) {
	if (nodes_struct == NULL) return;

	if (nodes_struct->nodes != NULL) {
		// Destroy nodes
		for (int i = 0; i < nodes_struct->n_nodes; i++) {
			node_destroy(&nodes_struct->nodes[i]);
		}
		free(nodes_struct->nodes);
	}

	nodes_struct->nodes = NULL;
	nodes_struct->n_nodes = 0;

}




Node* nodes_search_id(Nodes* nodes, int id) {
	// Check input
	if (nodes == NULL || nodes->nodes == NULL || id < 0) return NULL;

	for (int i = 0; i < nodes->n_nodes; i++) {
		if (nodes->nodes[i]->id == id) return nodes->nodes[i];
	}

	return NULL;
}