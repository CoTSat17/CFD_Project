#include <element.h>


Element* element_create(Node** nodes, Element_Types_Avialable element_type, int id) {
	// Allocate a single block for Element and its node pointers to reduce
	// number of allocations and improve cache locality.
	Element* element = (Element*)malloc(sizeof(Element) + element_type_data[element_type].n_nodes * sizeof(Node*));
	if (!element) return NULL;

	element->element_type = &element_type_data[element_type];
	// nodes array is placed immediately after the Element struct
	element->nodes = (Node**)(element + 1);
	for (int i = 0; i < element->element_type->n_nodes; i++) {
		element->nodes[i] = nodes[i];
	}

	element->id = id;
	element->test_value = 0;
	return element;
}



void element_destroy(Element* element) {
	// nodes were allocated in the same block as element, so only free once
	free(element);
}



float element_dist(Element* element_1, Element* element_2) {
	// Calculate the position of both elements center
	Point center_position_1 = element_center(element_1);
	Point center_position_2 = element_center(element_2);

	// Calculate distance
	float distance = point_dist(center_position_1, center_position_2);

	return distance;
}



Point element_center(Element* element_1) {
	Point center_position;
	center_position.x = 0.;
	center_position.y = 0.;
	center_position.z = 0.;


	for (int i = 0; i < element_1->element_type->n_nodes; i++) {
		center_position.x += element_1->nodes[i]->position.x;
		center_position.y += element_1->nodes[i]->position.y;
		center_position.z += element_1->nodes[i]->position.z;
	}

	if (element_1->element_type->n_nodes > 0) {
		float inv = 1.0f / (float)element_1->element_type->n_nodes;
		center_position.x *= inv;
		center_position.y *= inv;
		center_position.z *= inv;
	}

	return center_position;
}




Elements* elements_create(int* nodes_index, int size_array, Element_Types_Avialable element_type, Nodes* nodes_list) {
	Elements* elements = (Elements*)malloc(sizeof(Elements));
	if (elements == NULL) return NULL;

	int element_size = element_type_data[element_type].n_nodes;

	// --------------
	// Set N_elements
	// --------------
	elements->n_elements = size_array / element_size;



	// -----------------------------
	// Create the arrray of elements
	// -----------------------------

	// Allocate enought space
	elements->elements = (Element**)malloc(sizeof(Element*) * elements->n_elements);
	if (elements->elements == NULL) return NULL;

	// Iterate for each new element
	for (int i = 0; i < elements->n_elements; i++) {

		// Read the idexes and find the Node
		Node** element_nodes = (Node**)malloc(sizeof(Node*) * element_size);
		if (element_nodes == NULL) return NULL;

		for (int j = 0; j < element_size; j++) {
			element_nodes[j] = nodes_search_id(nodes_list, nodes_index[i * element_size + j]);
		}
		Element* element = element_create(element_nodes, element_type, i + 1);


		// Asign the new element to the elements array
		elements->elements[i] = element;
	}

	return elements;
}






Element_Type element_type_data[N_ELEMENTS] = {
	[ELEMENT_TRI] = {.n_nodes = 3,
						.n_faces = 3,
						.face_nodes =	 {	{0,1},	{1,2},	{2,0} },
						.node_per_face = 2
					},

	[ELEMENT_TETRA] = {.n_nodes = 4,
						.n_faces = 4,
						.face_nodes = {		{0,1},	{1,2},	{2,3},	{3,0} },
						.node_per_face = 2
					},

	[ELEMENT_HEXA] = {.n_nodes = 8,
						.n_faces = 6,
						.face_nodes = {		{0,1,2,3},	{4,5,6,7},	{0,1,5,4},	{1,2,6,5},	{2,3,7,6},	{3,0,4,7} },
						.node_per_face = 4
					},
};