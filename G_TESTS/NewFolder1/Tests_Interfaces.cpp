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


	Interfaces* interface = interfaces_create(faces);

	// CHECK NUMBER OF FACES
	EXPECT_EQ(interface->n_interfaces, 1);

	// CHECK FACES VALUES
	EXPECT_EQ(interface->interface_array[0].element_1, elements->elements[0]);
	EXPECT_EQ(interface->interface_array[0].element_2, elements->elements[1]);

	EXPECT_EQ(interface->interface_array[0].face->nodes_id[0], 2);
	EXPECT_EQ(interface->interface_array[0].face->nodes_id[1], 4);
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
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_HEXA, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);

	Interfaces* interfaces = interfaces_create(faces);
	elements->elements[0]->test_value = 1.0;
	interfaces_calculate_flux(interfaces);


	interfaces_update_element_value(interfaces);
}





TEST(Interface_Test, Interfaces_Calc_Area) {
	float nodes_data[] = {
		1, 0, 0, 0,  // Nodo 1
		2, 1, 0, 0,  // Nodo 2
		3, 1, 1, 0,  // Nodo 3
		4, 0, 1, 0,  // Nodo 4
		5, 0, 0, 1,  // Nodo 5
		6, 1, 0, 1,  // Nodo 6
		7, 1, 1, 1,  // Nodo 7
		8, 0, 1, 1,  // Nodo 8
		9, 2, 0, 0,  // Nodo 9
		10, 2, 1, 0,  // Nodo 10
		11, 2, 0, 1,  // Nodo 11
		12, 2, 1, 1   // Nodo 12
	};

	int elements_data[] = {
		1, 1, 2, 3, 4, 5, 6, 7, 8, // Element 1
		2, 2, 9,10, 3, 6,11, 12, 7  // Element 2
	};



	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	nodes_fill(nodes, nodes_data, 12);

	Elements* elements = (Elements*)malloc(sizeof(Elements));
	elements_fill(elements, elements_data, 2, ELEMENT_HEXA, nodes);

	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);


	Interfaces* interface = interfaces_create(faces);


	EXPECT_EQ(interface->interface_array[0].area, 1.0);

}