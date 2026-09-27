#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <interface.h>
}


TEST(Interface_Test, Interface_Create) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0};

	int node_index[] = { 1,	2,	3,	4,
						 2,	5,	6,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index)/ sizeof(node_index[0]), ELEMENT_TETRA, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);


	Interfaces* interface = interfaces_create(faces);

	// CHECK NUMBER OF FACES
	EXPECT_EQ(interface->n_interfaces, 1);

	// CHECK FACES VALUES
	EXPECT_EQ(interface->interface_array[0].element_1, elements->elements[0]);
	EXPECT_EQ(interface->interface_array[0].element_2, elements->elements[1]);

	EXPECT_EQ(interface->interface_array[0].face->nodes_id[0], 3);
	EXPECT_EQ(interface->interface_array[0].face->nodes_id[1], 2);
}



