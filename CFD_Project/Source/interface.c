#include "interface.h""



Interface* interface_create(Element* element_1, Element* element_2) {
	Interface* interface = (Interface*)malloc(sizeof(Interface));
	if (interface == NULL) return NULL;

	interface->flux = 0;
	interface->element_1 = element_1;
	interface->element_2 = element_2;

	return interface;
}



void interface_calculate_flux(Interface* interface) {
	interface->flux = (interface->element_1->test_value - interface->element_2->test_value) / element_dist(interface->element_1, interface->element_2);
}