#include <faces.h>

int compare_descending(const void* a, const void* b);



//
//Faces* create_faces(Elements* elements) {
//	/// ----------------------------------------------------------------------------
//	/// Allocate memory for the "faces" struct and the array of "face" structs
//	/// ----------------------------------------------------------------------------
//	/// The elements must be the same type.
//	Faces* faces = (Faces*)malloc(sizeof(Faces) + elements->elements[0]->element_type->n_faces * elements->n_elements * sizeof(Face*));
//	Face* next_face_pointer = faces + sizeof(Faces*);
//
//
//	/// ----------------------------------------------------------------
//	/// CREATE A ARRAY OF "FACE" STRUCTS WITH THE FACES OF EACH ELEMENT
//	/// ----------------------------------------------------------------
//	/// The nodes that form each face are sorted from smallest to biggest
//	
//	/// Iterate through each element
//	for (int i = 0; i < elements->n_elements; i++) {
//	}
//}




Faces* create_element_faces(Element* element) {
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
		faces->face_array[i] = (Face){ .nodes_id = nodes, .element = element };

	}

	return faces;
}




Faces* create_faces(Elements* elements) {
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
	Faces* faces = (Faces*)malloc(sizeof(Faces) + n_faces_total * sizeof(Face) + n_nodes_total * sizeof(int));
	if (faces == NULL) return NULL;

	// Fill the Faces struct
	faces->face_array = (Face*)(faces + 1); // Beggining of the face_array 
	faces->n_faces = n_faces_total;
	faces->nodes_pool = (int*)(faces->face_array + n_faces_total); // Beggining of the face_array 

	faces->new_face = faces->face_array;	// Where a new Face struct must be added
	faces->new_nodes = faces->nodes_pool;	// Where a new Node_id must be added


	// Iterate through each element
	for (int i = 0; i < elements->n_elements; i++) {
		Element* element = elements->elements[i];

		// Iterate through each face of the element
		//the number of faces of the element is determined by it's type
		for (int j = 0; j < element->element_type->n_faces; j++) {
			// Save the "Face" struct in the designated spot in the memory 
			*faces->new_face = (Face){ .nodes_id = faces->new_nodes, .element = element };

			// Add to the "nodes_pool" the elements
			for (int k = 0; k < element->element_type->node_per_face; k++) {
				*faces->new_nodes = element->nodes[element->element_type->face_nodes[j][k]]->id;
				faces->new_nodes++;
			}

			// Sort the ID values of the nodes from smalles to biggest
			qsort(faces->new_face->nodes_id, element->element_type->node_per_face, sizeof(int), compare_descending);

			// Move the memory position where the next "Face" struct must be stored
			faces->new_face++;
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