#include "interface.h"



void interface_create(Element* element_1, Element* element_2, Face* face, Interface* memory_position) {
	Interface* interface = memory_position;
	if (interface == NULL) return NULL;

	interface->flux = 0;
	interface->element_1 = element_1;
	interface->element_2 = element_2;
	interface->face = face;
	interface_calculate_area(interface);
	
}



void interface_calculate_flux(Interface* interface) {
	interface->flux = (interface->element_1->test_value - interface->element_2->test_value) / element_dist(interface->element_1, interface->element_2);
}


void interface_update_element_value(Interface* interface) {
	interface->element_1->test_value -= 0.25 / (1 * 1) * (1 * (interface->flux));
	interface->element_2->test_value += 0.25 / (1 * 1) * (1 * (interface->flux));
}



Node* interface_search_node_by_id(Interface* interface, int node_id) {
	Element* element = interface->element_1;
	for (int i = 0; i < element->element_type->n_nodes; i++) {

		if (element->nodes[i]->id == node_id) {
			return element->nodes[i];
		}

	}


}



void interface_calculate_area(Interface* interface_struct) {
	// CHECK THE DIMMENSION OF THE ELEMENTS
	int dimmension = interface_struct->element_1->element_type->dimmension;

	
	// 2D ELEMENT 
	if (dimmension == 2) {
		Point point_1 = interface_search_node_by_id(interface_struct, interface_struct->face->nodes_id[0])->position;
		Point point_2 = interface_search_node_by_id(interface_struct, interface_struct->face->nodes_id[1])->position;
		float dist = point_dist(point_1, point_2);
		interface_struct->area = dist;
	}
	// ----------
	// 3D ELEMENT 
	// ----------
	else if (dimmension == 3) {

		/// -----------------------------------------
		/// CREATE AN ARRAY WITH NODE POSITION DATA
		/// -----------------------------------------
		float* nodes_position = (float*)malloc(interface_struct->element_1->element_type->node_per_face * 3 * sizeof(float));
		float* new_value = nodes_position;

		for (int i = 0; i < interface_struct->element_1->element_type->node_per_face; i++) {
			Point point_1 = interface_search_node_by_id(interface_struct, interface_struct->face->nodes_id[i])->position;
			*new_value = point_1.x;
			new_value++;
			*new_value = point_1.y;
			new_value++;
			*new_value = point_1.z;
			new_value++;
		}
		// SAVE VALUES AS MATRIX
		Matrix* positions_matrix = matrix_create(nodes_position, 3, interface_struct->element_1->element_type->node_per_face);

		/// -----------------------------------------
		/// DETERMINE THE TYPE OF ELEMENT FOR VOLUME CALCULATION
		/// -----------------------------------------

		Gauss_Data_Types type;
		switch (interface_struct->element_1->element_type->type) {
		case ELEMENT_HEXA:
			type = HEX8;
			break;

		}

		interface_struct->area = calc_area(positions_matrix, type);

	}
}




Interfaces* interfaces_create(Faces* faces) {
	/// ------------------------------
	/// OBTAIN A ORDERED FACECES ARRAY
	/// ------------------------------
	Faces* sorted_faces = faces_sort(faces);




	// --------------------
	// RESERVE MEMORY SPACE
	// --------------------
	// Initial memory reservation that overstates the ammount of memory needed.
	Interfaces* interfaces = (Interfaces*)malloc(sizeof(Interfaces) + sorted_faces->n_faces * sizeof(Interface));
	if (interfaces == NULL) return NULL;


	interfaces->interface_array = (Interface*)(interfaces + 1);
	interfaces->n_interfaces = 0;

	Interface* new_interface = interfaces->interface_array;
	// ------------------------
	// DETERMINE THE INTERFACES
	// ------------------------
	for (int i = 0; i < sorted_faces->n_faces -1; i++) {
		if (faces_equal(&sorted_faces->face_array[i], &sorted_faces->face_array[i + 1])) {

			/// Search for the face in the non-ordered array to conserve correct node order
			interface_create(	sorted_faces->face_array[i].element,
								sorted_faces->face_array[i + 1].element,
								&sorted_faces->face_array[i],
								new_interface);


			interfaces->n_interfaces++;
			new_interface++;
			i++;

		}
	}

	/// FREE Sorted Array
	free(sorted_faces);
	sorted_faces = NULL;




	// -----------------
	// ADJUST THE MEMORY 
	// -----------------
	Interface* interface_adjusted = realloc(interfaces, sizeof(Interfaces) + interfaces->n_interfaces * sizeof(Interface));
	if (interface_adjusted == NULL) return NULL;

	return interface_adjusted;
}





void interfaces_calculate_flux(Interfaces* interfaces) {
	for (int i = 0; i < interfaces->n_interfaces; i++) {
		interface_calculate_flux(&interfaces->interface_array[i]);
	}
}





void interfaces_update_element_value(Interfaces* interfaces) {
	for (int i = 0; i < interfaces->n_interfaces; i++) {
		interface_update_element_value(&interfaces->interface_array[i]);
	}
}