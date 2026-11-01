#include "interface.h"



void interface_create(Element* element_1, Element* element_2, Face* face, Interface* memory_position) {
	Interface* interface = memory_position;
	if (interface == NULL) return NULL;

	interface->flux = 0;
	interface->element_1 = element_1;
	interface->element_2 = element_2;
	interface->face = face;
	face_calc_area(face);
	
}



void interface_calculate_flux(Interface* interface) {
	interface->flux =  (interface->element_1->test_value - interface->element_2->test_value) / element_dist(interface->element_1, interface->element_2);
}


void interface_update_element_value(Interface* interface, Materials* materials, float dt) {
	Material* material = materials->material_array[interface->element_1->material_id - 1];
	float density = material->density;
	float cp = material->specific_heat;
	float k = material->thermal_conductivity;

	interface->element_1->test_value -= dt / (density * cp * interface->element_1->volume) * (k * interface->flux * interface->face->area);
	interface->element_2->test_value += dt / (density * cp * interface->element_2->volume) * (k * interface->flux * interface->face->area);
}














void interfaces_calculate_flux(Interfaces* interfaces) {
	for (int i = 0; i < interfaces->n_interfaces; i++) {
		interface_calculate_flux(&interfaces->interface_array[i]);
	}
}





void interfaces_update_element_value(Interfaces* interfaces, Materials* materials_struct, float dt) {
	for (int i = 0; i < interfaces->n_interfaces; i++) {
		interface_update_element_value(&interfaces->interface_array[i], materials_struct, dt);
	}
}