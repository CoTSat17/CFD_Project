#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <interface.h>
}


TEST(Interface_Test, Interfaces_Create) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0};

	int node_index[] = { 1,	2,	4,	3,
						 2,	5,	6,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index)/ sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);


	Interfaces* interface = interfaces_create(faces);

	// CHECK NUMBER OF FACES
	EXPECT_EQ(interface->n_interfaces, 1);

	// CHECK FACES VALUES
	EXPECT_EQ(interface->interface_array[0].element_1, elements->elements[0]);
	EXPECT_EQ(interface->interface_array[0].element_2, elements->elements[1]);

	EXPECT_EQ(interface->interface_array[0].face->nodes_id[0], 4);
	EXPECT_EQ(interface->interface_array[0].face->nodes_id[1], 2);
}



TEST(Interface_Test, Interfaces_Calculate_Flux) {
	float nodes_position[] = {  0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
						 2,	5,	6,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);


	Interfaces* interfaces = interfaces_create(faces);

	interfaces_calculate_flux(interfaces);

	// Test for 0 value in both nodes
	EXPECT_EQ(interfaces->interface_array[0].flux, 0);


	elements->elements[0]->test_value = 1.0;
	interfaces_calculate_flux(interfaces);

	// Test for 0 value in both nodes
	EXPECT_EQ(interfaces->interface_array[0].flux, 1.0);

}





TEST(Interface_Test, Interface_Update_Element_Value) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
						 2,	5,	6,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_TETRA, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);

	Interfaces* interfaces = interfaces_create(faces);
	elements->elements[0]->test_value = 1.0;
	interfaces_calculate_flux(interfaces);


	interfaces_update_element_value(interfaces);



TEST(Interface_Test, Interface_Update_Element_Value) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
						 2,	5,	6,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);

	Interfaces* interfaces = interfaces_create(faces);
	elements->elements[0]->test_value = 1.0;
	interfaces_calculate_flux(interfaces);


	interfaces_update_element_value(interfaces);

	EXPECT_EQ(elements->elements[0]->test_value, 0.75);
	EXPECT_EQ(elements->elements[1]->test_value, 0.25);

}
