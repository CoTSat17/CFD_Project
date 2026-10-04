#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <boundary_conditions.h>
}


TEST(Boundary_Test, Boundaries_Create) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
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

	Boundaries* boundaries = boundaries_create(faces);


	// CHECK NUMBER OF FACES
	EXPECT_EQ(boundaries->n_boundaries, 6);

	// CHECK FACES VALUES

	EXPECT_EQ(boundaries->boundary_array[0].BC_type, DIRICHLET);
	EXPECT_EQ(boundaries->boundary_array[0].fixed_value, 0);

}

