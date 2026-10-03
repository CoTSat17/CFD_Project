#include "interface.h"



void interface_create(Element* element_1, Element* element_2, Face* face, Interface* memory_position) {
	Interface* interface = memory_position;
	if (interface == NULL) return NULL;

	interface->flux = 0;
	interface->element_1 = element_1;
	interface->element_2 = element_2;
	interface->face = face;

	
}



void interface_calculate_flux(Interface* interface) {
	interface->flux = (interface->element_1->test_value - interface->element_2->test_value) / element_dist(interface->element_1, interface->element_2);
}


void interface_update_element_value(Interface* interface) {
	interface->element_1->test_value -= 0.25 / (1 * 1) * (1 * (interface->flux));
	interface->element_2->test_value += 0.25 / (1 * 1) * (1 * (interface->flux));
}


Interfaces* interfaces_create(Faces* faces) {
	// --------------------
	// RESERVE MEMORY SPACE
	// --------------------
	// Initial memory reservation that overstates the ammount of memory needed.
	Interfaces* interfaces = (Interfaces*)malloc(sizeof(Interfaces) + faces->n_faces * sizeof(Interface));
	if (interfaces == NULL) return NULL;


	interfaces->interface_array = (Interface*)(interfaces + 1);
	interfaces->n_interfaces = 0;

	Interface* new_interface = interfaces->interface_array;
	// ------------------------
	// DETERMINE THE INTERFACES
	// ------------------------
	for (int i = 0; i < faces->n_faces -1; i++) {
		if (faces_equal(&faces->face_array[i], &faces->face_array[i + 1])) {

			interface_create(	faces->face_array[i].element,
								faces->face_array[i + 1].element,
								&faces->face_array[i],
								new_interface);

			interfaces->n_interfaces++;
			new_interface++;
			i++;

		}
	}

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