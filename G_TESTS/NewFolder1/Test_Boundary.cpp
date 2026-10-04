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
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);

	Boundaries* boundaries = boundaries_create(faces);


	// CHECK NUMBER OF FACES
	EXPECT_EQ(boundaries->n_boundaries, 6);

	// CHECK FACES VALUES

	EXPECT_EQ(boundaries->boundary_array[0].BC_type, DIRICHLET);
	EXPECT_EQ(boundaries->boundary_array[0].fixed_value, 0);

}



TEST(Boundary_Test, Boundary_Calculate_Flux) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0};

	int node_index[] = { 1,	2,	4,	3};

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);

	Boundaries* boundaries = boundaries_create(faces);


	// CHECK NUMBER OF BOUNDARIES
	EXPECT_EQ(boundaries->n_boundaries, 4);

	// STABLISH A DIRICHLET OF VALUE 100
	//resultant flux should be 200
	boundaries->boundary_array[0].fixed_value = 100;
	boundary_calculate_flux(&boundaries->boundary_array[0]);
	EXPECT_EQ(boundaries->boundary_array[0].flux, 200);

	// STABLISH A NEUMAN OF VALUE 100
	//resultant flux should be 100
	boundaries->boundary_array[1].BC_type = NEUMAN;
	boundaries->boundary_array[1].fixed_value = 100;
	boundary_calculate_flux(&boundaries->boundary_array[1]);
	EXPECT_EQ(boundaries->boundary_array[1].flux, 100);
}
