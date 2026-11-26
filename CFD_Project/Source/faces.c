#include <faces.h>
#include <interface.h>
#include <boundary_conditions.h>

int compare_descending(const void* a, const void* b);
int sort_face_pointers(const void* a, const void* b);



int faces_equal(Face* face_1, Face* face_2) {
	// Check if the faces have different ammount of nodes
	//if they have different ammount they are different
	int face_1_nodes = face_1->element->element_type->node_per_face;
	int face_2_nodes = face_2->element->element_type->node_per_face;

	if (face_1_nodes != face_2_nodes) return 0;

	for (int i = 0; i < face_1_nodes; i++) {
		if (face_1->nodes_id_sorted[i] != face_2->nodes_id_sorted[i]) return 0;
	}

	return 1;
}




int face_center(Face* face, Point* center_point) {
	// ------------
	// CHECK INPUT
	// ------------
	if (face == NULL || center_point == NULL) return 1;


	// ------------
	// INITIALIZE
	// ------------
	*center_point = (Point){ 0 , 0 , 0 };
	int node_per_face = face->element->element_type->node_per_face;
	int node_found = 0; // 1 IF FOUND || 0 IF NOT FOUND

	// ------------
	// FIND EACH NODE
	// ------------
	for (int i = 0; i < node_per_face; i++) {
		// Search for the node
		Node* node = face_search_node_by_id(face, face->nodes_id[i]);
		// If not found reset point and return 1
		if (node == NULL) {
			*center_point = (Point){ 0 , 0 , 0 };
			return 1;
		}

		// If found add values
		center_point->x += node->position.x;
		center_point->y += node->position.y;
		center_point->z += node->position.z;

	}
	// Calculate the average
	center_point->x = center_point->x / node_per_face;
	center_point->y = center_point->y / node_per_face;
	center_point->z = center_point->z / node_per_face;

	return 0;
}




Node* face_search_node_by_id(Face* face, int node_id) {
	// ------------
	// CHECK INPUTS
	// ------------

	if (face == NULL || node_id < 0) return NULL;


	Element* element = face->element;
	for (int i = 0; i < element->element_type->n_nodes; i++) {

		if (element->nodes[i]->id == node_id) {
			return element->nodes[i];
		}

	}

	// If not found return null
	return NULL;


}


int face_calc_area(Face* face) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (face == NULL) return 1;

	// CHECK THE DIMMENSION OF THE ELEMENTS
	int dimmension = face->element->element_type->dimmension;


	// ------------
	// 2D ELEMENT 
	// ------------
	if (dimmension == 2) {
		// GET THE POINTS
		Node* point_1 = face_search_node_by_id(face, face->nodes_id[0]);
		Node* point_2 = face_search_node_by_id(face, face->nodes_id[1]);
		// Check for error
		if (point_1 == NULL || point_2 == NULL) {
			return 1;
		}

		// Calculate distances
		float dist = point_dist(point_1->position, point_2->position);
		face->area = dist;

		// Indicate success
		return 0;
	}


	// ----------
	// 3D ELEMENT 
	// ----------
	else if (dimmension == 3) {

		/// -----------------------------------------
		/// CREATE AN ARRAY WITH NODE POSITION DATA
		/// -----------------------------------------
		float* nodes_position = (float*)malloc(face->element->element_type->node_per_face * 3 * sizeof(float));
		if (nodes_position == NULL) return 1;
		float* new_value = nodes_position;

		// Get nodes
		for (int i = 0; i < face->element->element_type->node_per_face; i++) {
			Node* node_1 = face_search_node_by_id(face, face->nodes_id[i]);
			if (node_1 == NULL) {
				free(nodes_position);
				return 1;
			}

			Point point_1 = node_1->position;
			*new_value = point_1.x;
			new_value++;
			*new_value = point_1.y;
			new_value++;
			*new_value = point_1.z;
			new_value++;
		}


		// SAVE VALUES AS MATRIX
		Matrix* positions_matrix = matrix_create(nodes_position, 3, face->element->element_type->node_per_face);
		if (positions_matrix == NULL) {
			free(nodes_position);
			return 1;
		}

		/// -----------------------------------------
		/// DETERMINE THE TYPE OF ELEMENT FOR VOLUME CALCULATION
		/// -----------------------------------------

		Gauss_Data_Types type;
		switch (face->element->element_type->type) {
		case ELEMENT_HEXA:
			type = HEX8;
			break;

		}

		face->area = calc_area(positions_matrix, type);

		// Free memory and return 0 for all correct
		free(nodes_position);
		free(positions_matrix);
		return 0;
	}



	return 1;
}






Faces* faces_create_from_element(Element* element) {
	// ------------
	// CHECK INPUT
	// ------------
	if (element == NULL) return NULL;


	/// -----------------
	/// ALLOCATES MEMORY 
	/// -----------------
	/// Allocates memory for the array that contains the faces, the array for each face and 
	/// the array on ids that form each face
	Faces* faces = (Faces*)malloc(sizeof(Faces) + 
			element->element_type->n_faces * (sizeof(Face) + element->element_type->node_per_face * sizeof(int)));
	if (faces == NULL) return NULL;
	// The memory is asigned as following:
	//		[Faces Struct][Face Struct][Face Struct]...[nodes_id][nodes_id][nodes_id]


	// Save the number of faces
	faces->n_faces = element->element_type->n_faces;

	// Asign the direction of the first [face Struct]
	faces->face_array = (Face*)(faces + 1);
	// Assing the direction of the first [nodes_id] array
	int* nodes_pool = (int*)(faces->face_array + faces->n_faces);

	// With the data from "Element data type" determine the different faces
	for (int i = 0; i < element->element_type->n_faces; i++) {

		// Create a array pointing to the "nodes_id" direction
		int* nodes = (int*)(nodes_pool + i* element->element_type->node_per_face);

		// Fill the "nodes_id" array with the nodes that form the face
		for (int j = 0; j < element->element_type->node_per_face; j++) {
			nodes[j] = element->nodes[ element->element_type->face_nodes[i][j] ]->id;
		}

		// Sort the ID values of the nodes from smalles to biggest
		qsort(nodes, element->element_type->node_per_face, sizeof(int), compare_descending);

		// Create the Face struct for this face
		faces->face_array[i] = (Face){ .nodes_id = nodes, .element = element, .face_id=i+1 };

	}

	return faces;
}




Faces* faces_create_from_elements(Elements* elements) {
	// ------------
	// CHECK INPUT
	// ------------
	if (elements == NULL) return NULL;



	// ------------------------ 
	// DETERMINE SIZE OF MEMORY
	// ------------------------ 
	// All the memory necesary for all the information of the "Faces" struct is stored in a single allocation
	//this necesitates to determine the number of faces and nodes that must be stored.
	int n_faces_total = 0;
	int n_nodes_total = 0;
	
	for (int i = 0; i < elements->n_elements; i++) {
		n_faces_total += elements->elements[i]->element_type->n_faces;
		n_nodes_total += elements->elements[i]->element_type->n_faces * elements->elements[i]->element_type->node_per_face;
	}

	// ---------------
	// ALLOCATE MEMORY
	// ---------------
	Faces* faces = (Faces*)malloc(sizeof(Faces) + n_faces_total * sizeof(Face) + n_nodes_total * 2 * sizeof(int));
	if (faces == NULL) return NULL;

	// Fill the Faces struct
	faces->face_array = (Face*)(faces + 1); // Beggining of the face_array 
	faces->n_faces = n_faces_total;
	faces->nodes_pool = (int*)(faces->face_array + n_faces_total); // Beggining of the face_array 

	Face* new_face = faces->face_array;	// Where a new Face struct must be added
	int* new_nodes = faces->nodes_pool;	// Where a new Node_id must be added
	int* new_sorted_nodes = (int*)(new_nodes + n_nodes_total);


	// Iterate through each element
	for (int i = 0; i < elements->n_elements; i++) {
		Element* element = elements->elements[i];

		// Iterate through each face of the element
		//the number of faces of the element is determined by it's type
		for (int j = 0; j < element->element_type->n_faces; j++) {
			// Save the "Face" struct in the designated spot in the memory 
			*new_face = (Face){ .nodes_id = new_nodes, .nodes_id_sorted = new_sorted_nodes, .element = element, .face_id= i*element->element_type->n_faces + j + 1 };

			// Add to the "nodes_pool" the elements
			for (int k = 0; k < element->element_type->node_per_face; k++) {
				*new_nodes = element->nodes[element->element_type->face_nodes[j][k]]->id;
				*new_sorted_nodes = element->nodes[element->element_type->face_nodes[j][k]]->id;
				new_nodes++;
				new_sorted_nodes++;
			}

			// Sort the ID values of the nodes from smalles to biggest
			qsort(new_face->nodes_id_sorted, element->element_type->node_per_face, sizeof(int), compare_descending);

			// Move the memory position where the next "Face" struct must be stored
			new_face++;
		}
	}

	return faces;
}








/// <summary>
/// Used in qsort for a int array.
/// </summary>
int compare_descending(const void* a, const void* b)
{
	return *(int*)b - *(int*)a;
}





Face** faces_sort(Faces* faces) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (faces == NULL || faces->n_faces <= 0) return NULL;

	// ---------------
	// ALLOCATE MEMORY
	// ---------------
	Face** sorted_faces = malloc(faces->n_faces * sizeof(Face*));
	if (sorted_faces == NULL) return NULL;

	// ---------------
	// COPY FACES DIRECTIONS
	// ---------------
	for (int i = 0; i < faces->n_faces; i++) {
		sorted_faces[i] = &faces->face_array[i];
		// Check for error in the faces
		if (sorted_faces[i] == NULL || sorted_faces[i]->element == NULL) {
			free(sorted_faces);
			return NULL;
		}
	}

	// ---------------
	// SORT FACES
	// ---------------
	qsort(sorted_faces,
		faces->n_faces,
		sizeof(Face*),
		sort_face_pointers);


	return sorted_faces;



}

int sort_face_pointers(const void* a, const void* b) {

    const Face* face_a = *(Face* const*)a;
    const Face* face_b = *(Face* const*)b;

    int n_a = face_a->element->element_type->node_per_face;
    int n_b = face_b->element->element_type->node_per_face;

    // Compare number of nodes
    if (n_a < n_b) return -1;
    if (n_a > n_b) return 1;

    // Compare sorted node IDs
    for (int i = 0; i < n_a; i++) {

        if (face_a->nodes_id_sorted[i] < face_b->nodes_id_sorted[i]) return -1;
        if (face_a->nodes_id_sorted[i] > face_b->nodes_id_sorted[i]) return 1;
    }

    return 0;
}



Face* faces_search_id(Faces* faces_struct, int face_id) {
	return &faces_struct->face_array[face_id - 1];


}







int faces_clasify(Faces* faces, Interfaces** interfaces_struct, Boundaries** boundaries_struct) {
	/// ------------
	// CHECK INPUTS
	/// ------------
	if (faces == NULL || interfaces_struct == NULL || boundaries_struct == NULL) return 1;
	*interfaces_struct = NULL;
	*boundaries_struct = NULL;

	/// ------------------------------
	/// OBTAIN A ORDERED FACECES ARRAY
	/// ------------------------------
	Face** sorted_faces = faces_sort(faces);
	if (sorted_faces == NULL) return 1;



	// --------------------
	// RESERVE MEMORY SPACE
	// --------------------
	// Initial memory reservation that overstates the ammount of memory needed.
	// Reserve for both "interfaces" & "boundaries"

	*interfaces_struct = malloc(sizeof(Interfaces) + faces->n_faces * sizeof(Interface));
	if (*interfaces_struct == NULL) {
		free(sorted_faces);
		return 1;
	}

	*boundaries_struct = malloc(sizeof(Boundaries) + faces->n_faces * sizeof(Boundary));
	if (*boundaries_struct == NULL) {
		free(sorted_faces);
		free(*interfaces_struct);
		*interfaces_struct = NULL;
		return 1;
	}



	// --------------------------------------------
	// INITIATE NECESSARY VARIABLES IN STRUCTS
	// --------------------------------------------

	// Variables necesary for Interface_struct creation
	(*interfaces_struct)->interface_array = (Interface*)((*interfaces_struct) + 1);
	(*interfaces_struct)->n_interfaces = 0;

	Interface* new_interface = (*interfaces_struct)->interface_array;

	// Variables necesary for Boundary_struct creation
	(*boundaries_struct)->boundary_array = (Boundary*)((*boundaries_struct) + 1);
	(*boundaries_struct)->n_boundaries = 0;

	Boundary* new_boundary = (*boundaries_struct)->boundary_array;

	// ------------------------
	// DETERMINE THE INTERFACES
	// ------------------------
	Face* face_1 = NULL;
	Face* face_2 = NULL;
	for (int i = 0; i < faces->n_faces; i++) {
		face_1 = sorted_faces[i];
		int last = (i + 1) == faces->n_faces;

		if (!last  && faces_equal(face_1, sorted_faces[i + 1])) {

			face_2 = sorted_faces[i + 1];

			/// Search for the face in the non-ordered array to conserve correct node order
			int interface_reation = interface_create(face_1->element,
				face_2->element,
				face_1,
				new_interface);
			if (interface_reation == 1) {
				free(sorted_faces);
				free(*interfaces_struct);
				*interfaces_struct = NULL;

				boundaries_destroy(&boundaries_struct);

				return 1;
			}

			(*interfaces_struct)->n_interfaces++;
			new_interface++;
			i++; // If the face is equal to the next then the next one can be skipped

		}

		else {
			// If faces are not equal then it must be a boundary
			int boundary_reation = boundary_create(face_1, new_boundary);
			if (boundary_reation == 1) {
				free(sorted_faces);
				free(*interfaces_struct);
				*interfaces_struct = NULL;

				boundaries_destroy(&boundaries_struct);
				return 1;
			}

			(*boundaries_struct)->n_boundaries++;
			new_boundary++;

		}
	}

	/// FREE Sorted Array
	free(sorted_faces);
	sorted_faces = NULL;




	// -----------------
	// ADJUST THE MEMORY 
	// -----------------
	Interfaces* new_interfaces_struct = realloc(*interfaces_struct, sizeof(Interfaces) + (*interfaces_struct)->n_interfaces * sizeof(Interface));
	if (new_interfaces_struct == NULL) {
		free(*interfaces_struct);
		*interfaces_struct = NULL;

		boundaries_destroy(&boundaries_struct);
		return 1;
	}
	*interfaces_struct = new_interfaces_struct;
	(*interfaces_struct)->interface_array = (Interface*)((*interfaces_struct) + 1);

	Boundaries* new_boundaries_struct = realloc((*boundaries_struct), sizeof(Boundaries) + (*boundaries_struct)->n_boundaries * sizeof(Boundary));
	if (new_boundaries_struct == NULL) {
		free(*interfaces_struct);
		*interfaces_struct = NULL;

		boundaries_destroy(&boundaries_struct);
		return 1;
	}
	*boundaries_struct = new_boundaries_struct;
	(*boundaries_struct)->boundary_array = (Boundary*)((*boundaries_struct) + 1);


	return 0;
}
