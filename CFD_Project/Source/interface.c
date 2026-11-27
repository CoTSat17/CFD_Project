#include "interface.h"



int interface_create(Element* element_1, Element* element_2, Face* face, Interface* memory_position) {
	// -----------
	// CHECK INPUTS
	// -----------
	if (element_1 == NULL || element_2 == NULL || face == NULL || memory_position == NULL) return 1;

	// -----------
	// CREATE INTERFACE
	// -----------
	Interface* interface = memory_position;
	if (interface == NULL) return 1;

	interface->flux = 0;
	interface->element_1 = element_1;
	interface->element_2 = element_2;
	interface->face = face;

	return 0;
}



int interface_calculate_flux(Interface* interface) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (interface == NULL || interface->element_1 == NULL || interface->element_2 == NULL) return 1;

	// ------------
	// CALCULATE FLUX
	// ------------
	interface->flux =  (interface->element_1->test_value - interface->element_2->test_value) / element_dist(interface->element_1, interface->element_2);

	return 0;
}


int interface_update_element_value(Interface* interface, Materials* materials_struct, float dt) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (interface == NULL || interface->element_1 == NULL || interface->element_2 == NULL || 
		interface->face == NULL || materials_struct == NULL || dt <= 0) return 1;

	// ------------
	// GET VALUES
	// ------------
	Material* material = materials_struct->material_array[interface->element_1->material_id - 1];
	if (material == NULL) return 1;
	float density = material->density;
	float cp = material->specific_heat;
	float k = material->thermal_conductivity;
	if (density <= 0 || cp <= 0 || k <= 0) return 1;

	// ------------
	// UPDATE VALUE
	// ------------
	interface->element_1->test_value -= dt / (density * cp * interface->element_1->volume) * (k * interface->flux * interface->face->area);
	interface->element_2->test_value += dt / (density * cp * interface->element_2->volume) * (k * interface->flux * interface->face->area);

	return 0;
}





void interfaces_destroy(Interfaces** interfaces_struct) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (interfaces_struct == NULL || *interfaces_struct == NULL) return;

	// --------------
	// Destroy struct
	// --------------
	free(*interfaces_struct);
	*interfaces_struct = NULL;

}





int interfaces_calculate_flux(Interfaces* interfaces) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (interfaces == NULL) return 1;

	// ------------
	// CALCULATE FLUX
	// ------------
	for (int i = 0; i < interfaces->n_interfaces; i++) {
		if( interface_calculate_flux(&interfaces->interface_array[i]) == 1 ) return 1;
	}

	return 0;
}





int interfaces_update_element_value(Interfaces* interfaces, Materials* materials_struct, float dt) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (interfaces == NULL || interfaces->n_interfaces <= 0 || materials_struct == NULL || dt <= 0) return 1;

	// ------------
	// CALCULATE VALUES
	// ------------
	for (int i = 0; i < interfaces->n_interfaces; i++) {
		if( interface_update_element_value(&interfaces->interface_array[i], materials_struct, dt) == 1) return 1;
	}

	return 0;
}