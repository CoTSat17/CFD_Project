#include <element.h>
#include <faces.h>

Element* element_create(Node** nodes, Element_Types_Avialable element_type, int id) {
	// --------------
	// Check inputs
	// --------------
	if (nodes == NULL || *nodes == NULL || N_ELEMENTS < element_type || element_type < 0 || id <0) return NULL;


	// --------------
	// MEMORY ALLOCATION
	// --------------
	// Allocate a single block for Element and its node pointers to reduce
	// number of allocations and improve cache locality.
	Element* element = (Element*)malloc(sizeof(Element) + element_type_data[element_type].n_nodes * sizeof(Node*));
	if (!element) return NULL;

	// Save element type
	element->element_type = &element_type_data[element_type];


	// --------------
	// Save nodes info
	// --------------
	// nodes array is placed immediately after the Element struct

	element->nodes = (Node**)(element + 1);
	for (int i = 0; i < element->element_type->n_nodes; i++) {
		// Check nodes
		if (nodes[i] == NULL) {
			element_destroy(&element);
			return NULL;
		}
		// Save node
		element->nodes[i] = nodes[i];

	}

	// --------------
	// Save values
	// --------------
	element->id = id;
	element->test_value = 0;
	element->material_id = 1;
	if (element_volume(element) == 1) {
		element_destroy(&element);
		return NULL;
	}
	


	return element;
}



void element_destroy(Element** element) {
	if (element == NULL || *element == NULL) return;

	// nodes were allocated in the same block as element, so only free once
	free(*element);
	*element = NULL;
}



float element_dist(Element* element_1, Element* element_2) {

	// Calculate the position of both elements center
	Point center_position_1; element_center(element_1, &center_position_1);
	Point center_position_2; element_center(element_2, &center_position_2);

	// Calculate distance
	float distance = point_dist(center_position_1, center_position_2);

	return distance;
}


float element_dist_to_face(Element* element, Face* face) {
	Point element_center_point; element_center(element, &element_center_point);
	Point face_center_point =    face_center(face);

	// Calculate distance
	float distance = point_dist(element_center_point, face_center_point);

	return distance;
}




int element_center(Element* element_1, Point* center_point){
	/// ------------
	/// CHECK INPUTS
	/// ------------
	if (element_1 == NULL || center_point == NULL) return 1;

	/// ------------
	/// INITIALIZE CENTER POINT
	/// ------------
	center_point->x = 0.;
	center_point->y = 0.;
	center_point->z = 0.;

	/// ------------
	/// CALCULATE WITH AVERAGE
	/// ------------
	for (int i = 0; i < element_1->element_type->n_nodes; i++) {
		if (element_1->nodes[i] == NULL) return 1;
		
		center_point->x += element_1->nodes[i]->position.x;
		center_point->y += element_1->nodes[i]->position.y;
		center_point->z += element_1->nodes[i]->position.z;
	}

	if (element_1->element_type->n_nodes > 0) {
		float inv = 1.0f / (float)element_1->element_type->n_nodes;
		center_point->x *= inv;
		center_point->y *= inv;
		center_point->z *= inv;
	}

	return 0;
}



int element_volume(Element* element_struct) {
	/// ------------
	/// CHECK INPUTS
	/// ------------
	if (element_struct == NULL) {
		return 1;
	}



	/// -----------------------------------------
	/// CREATE AN ARRAY WITH NODE POSITION DATA
	/// -----------------------------------------
	
	int n_nodes = element_struct->element_type->n_nodes;
	
	int element_dimmension = element_struct->element_type->dimmension;
	float* nodes_position = (float*)malloc(n_nodes * 3 * sizeof(float));
	if (nodes_position == NULL) return 1;

	float* new_value = nodes_position;

	for (int node = 0; node < n_nodes; node++) {
		*new_value = element_struct->nodes[node]->position.x;
		new_value++;
		*new_value = element_struct->nodes[node]->position.y;
		new_value++;
		*new_value = element_struct->nodes[node]->position.z;
		new_value++;
	}

	// SAVE VALUES AS MATRIX
	Matrix* positions_matrix = matrix_create(nodes_position, 3, n_nodes);


	/// -----------------------------------------
	/// DETERMINE THE TYPE OF ELEMENT FOR VOLUME CALCULATION
	/// -----------------------------------------


	Gauss_Data_Types type;
	switch (element_struct->element_type->type) {
		case ELEMENT_TRI: 
			type = TRI3;
			break;
		case ELEMENT_QUAD: 
			type = QUAD4;
			break;
		case ELEMENT_HEXA:
			type = HEX8;
			break;
		default:
			return 1;

	}

	/// -----------------------------------------
	/// CALCULATE AND FREE
	/// -----------------------------------------
	element_struct->volume = calc_volume(positions_matrix, type);

	free(positions_matrix);
	positions_matrix = NULL;
	free(nodes_position);
	nodes_position = NULL;
	new_value = NULL;



	return 0;
}




Elements* elements_create(int* nodes_index, int size_array, Element_Types_Avialable element_type, Nodes* nodes_list) {
	/// ------------
	/// CHECK INPUTS
	/// ------------
	if (nodes_index == NULL || size_array <= 0 || nodes_list == NULL) return NULL;



	/// ------------
	/// RESERVE MEMORY
	/// ------------
	Elements* elements = (Elements*)calloc(1,sizeof(Elements));
	if (elements == NULL) return NULL;

	int element_size = element_type_data[element_type].n_nodes;



	// --------------
	// Initialize elements
	// --------------
	if (size_array % element_size != 0) {
		elements_destroy(&elements);
		return NULL;
	}

	elements->n_elements = 0;
	elements->elements = NULL;

	int total_elements = size_array / element_size;

	// -----------------------------
	// Create the arrray of elements
	// -----------------------------

	// Allocate enought space
	elements->elements = (Element**)malloc(sizeof(Element*) * total_elements);
	if (elements->elements == NULL) {
		elements_destroy(&elements);
		return NULL;
	}

	// Iterate for each new element
	for (int i = 0; i < total_elements; i++) {

		// Read the idexes and find the Node
		Node** element_nodes = (Node**)malloc(sizeof(Node*) * element_size);
		if (element_nodes == NULL) {
			elements_destroy(&elements);
			return NULL;
		}

		// Search for nodes
		for (int j = 0; j < element_size; j++) {
			element_nodes[j] = nodes_search_id(nodes_list, nodes_index[i * element_size + j]);
		}

		// Create elements
		Element* element = element_create(element_nodes, element_type, i + 1);
		free(element_nodes);
		if (element == NULL) {
			elements_destroy(&elements);
			return NULL;
		}

		// Asign the new element to the elements array
		elements->elements[i] = element;
		elements->n_elements++;
	}

	return elements;
}







int elements_fill(Elements* elements_struct, int* elements_data, int n_elements, Element_Types_Avialable element_type, Nodes* nodes_list) {
	/// ------------
	/// CHECK INPUTS
	/// ------------
	if (elements_struct == NULL || elements_data == NULL || nodes_list == NULL || n_elements <= 0) return 1;




	// --------------
	// Set N_elements
	// --------------
	int element_size = element_type_data[element_type].n_nodes;
	elements_struct->n_elements = 0;



	// -----------------------------
	// Create the arrray of pointes to elements
	// -----------------------------

	// Allocate enought space
	elements_struct->elements = (Element**)malloc(sizeof(Element*) * n_elements);
	if (elements_struct->elements == NULL) return NULL;

	// Iterate for each new element
	for (int i = 0; i < n_elements; i++) {

		// Read the idexes and find the Node
		Node** element_nodes = (Node**)malloc(sizeof(Node*) * element_size);
		if (element_nodes == NULL) return NULL;

		for (int j = 0; j < element_size; j++) {
			element_nodes[j] = nodes_search_id(nodes_list, elements_data[i * (element_size +1)+ j + 1]);
		}

		// Asign the new element to the elements array
		elements_struct->elements[i] = element_create(element_nodes, element_type, elements_data[(element_size + 1) * i]);
		elements_struct->n_elements++;
		// Chech for error
		if (elements_struct->elements[i] == NULL) {
			elements_clear(elements_struct);
			return 1;
		}
	}

}


















Element_Type element_type_data[N_ELEMENTS] = {
	[ELEMENT_TRI] = {.n_nodes = 3,
						.n_faces = 3,
						.face_nodes =	 {	{0,1},	{1,2},	{2,0} },
						.node_per_face = 2,
						.dimmension = 2,
						.type = ELEMENT_TRI
					},

	[ELEMENT_QUAD] = {.n_nodes = 4,
						.n_faces = 4,
						.face_nodes = {		{0,1},	{1,2},	{2,3},	{3,0} },
						.node_per_face = 2,
						.dimmension = 2,
						.type = ELEMENT_QUAD
					},

	[ELEMENT_HEXA] = {.n_nodes = 8,
						.n_faces = 6,
						.face_nodes = {		{0,1,2,3},	{4,5,6,7},	{0,1,5,4},	{1,2,6,5},	{2,3,7,6},	{3,0,4,7} },
						.node_per_face = 4,
						.dimmension = 3,
						.type = ELEMENT_HEXA
					},
};








int elements_init_conditions(Elements* elements_struct, float* init_values) {
	/// ------------
	/// CHECK INPUTS
	/// ------------
	if (elements_struct == NULL || init_values == NULL) return 1;

	int element_id;
	int element_value;

	for (int i = 0; i < elements_struct->n_elements; i++) {
		element_id = init_values[i*2];
		element_value = init_values[i*2 + 1];
		Element* element = elements_search_id(elements_struct, element_id);
		if (element == NULL) return 1;

		element->test_value = element_value;
	}
	
	return 0;

}


void elements_destroy(Elements** elements_struct) {
	if (elements_struct == NULL || *elements_struct == NULL) return;

	// Empy the struct
	elements_clear(*elements_struct);

	// Destroy struct
	free(*elements_struct);
	*elements_struct = NULL;
}


void elements_clear(Elements* elements_struct) {
	if (elements_struct == NULL) return;

	if (elements_struct->elements != NULL) {
		// Destroy nodes
		for (int i = 0; i < elements_struct->n_elements; i++) {
			element_destroy(&elements_struct->elements[i]);
		}
		free(elements_struct->elements);
	}

	elements_struct->elements = NULL;
	elements_struct->n_elements = 0;

}




Element* elements_search_id(Elements* elements_struct, int id) {
	for (int i = 0; i < elements_struct->n_elements; i++) {
		if (elements_struct->elements[i]->id == id) return elements_struct->elements[i];
	}

	return NULL;
}





void elements_calc_volume(Elements* elements_struct) {
	for (int i = 0; i < elements_struct->n_elements; i++) {
		element_volume(elements_struct->elements[i]);
	}
}