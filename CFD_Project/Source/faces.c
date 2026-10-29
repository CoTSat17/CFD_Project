#include <faces.h>

int compare_descending(const void* a, const void* b);
int sort_descending_faces_nodes(const void* a, const void* b);



int faces_equal(Face* face_1, Face* face_2) {
	for (int i = 0; i < face_1->element->element_type->node_per_face; i++) {
		if (face_1->nodes_id_sorted[i] != face_2->nodes_id_sorted[i]) return 0;
	}

	return 1;
}




Point face_center(Face* face) {
	Point center_point = { 0 , 0 , 0 };

	// For each node find its position
	for (int i = 0; i < face->element->element_type->node_per_face; i++) {

		/// Find the point inside the element data
		for (int j = 0; j < face->element->element_type->n_nodes; j++) {
			if (face->element->nodes[j]->id == face->nodes_id[i]) {
				Point position = face->element->nodes[j]->position;

				center_point.x += position.x;
				center_point.y += position.y;
				center_point.z += position.z;
			}
		}

	}

	// Calculate the average
	center_point.x = center_point.x / face->element->element_type->node_per_face;
	center_point.y = center_point.y / face->element->element_type->node_per_face;
	center_point.z = center_point.z / face->element->element_type->node_per_face;

	return center_point;
}



Faces* faces_create_from_element(Element* element) {
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





Faces* faces_sort(Faces* faces) {
	

	Faces* sorted_faces = malloc(sizeof(Faces));
	if (sorted_faces == NULL) return NULL;

	sorted_faces->n_faces = faces->n_faces;

	sorted_faces->face_array =
		malloc(faces->n_faces * sizeof(Face));

	if (sorted_faces->face_array == NULL) {
		free(sorted_faces);
		return NULL;
	}

	memcpy(	sorted_faces->face_array,faces->face_array,	faces->n_faces * sizeof(Face));

	// Sort the copy
	qsort(sorted_faces->face_array, sorted_faces->n_faces, sizeof(Face),sort_descending_faces_nodes);

	return sorted_faces;
}

int sort_descending_faces_nodes(const void* a, const void* b) {
	const Face* face_a = (const Face*)a;
	const Face* face_b = (const Face*)b;

	for (int i = 0; i < face_a->element->element_type->node_per_face; i++) {
		if (face_a->nodes_id_sorted[i] > face_b->nodes_id_sorted[i]) return -1;
		if (face_a->nodes_id_sorted[i] < face_b->nodes_id_sorted[i]) return  1;
	}

	return 0;

}



Face* faces_search_id(Faces* faces_struct, int face_id) {
	return &faces_struct->face_array[face_id - 1];


}