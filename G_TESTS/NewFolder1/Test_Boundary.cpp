#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <boundary_conditions.h>
#include <solver_config.h>
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

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);


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






TEST(Boundary_Test, Boundary_Update_Element_Value) {
	float nodes_data[] = {	1.0,	0.0,	0.0,	0.0,
							2.0,	1.0,	0.0,	0.0,
							3.0,	0.0,	1.0,	0.0,
							4.0,	1.0,	1.0,	0.0 };

	int elements_data[] = { 1,	1,	2,	4,	3 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };

	float solver_data[] = { 0.0, 1.0, 0.001 };

	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	nodes_fill(nodes, nodes_data, 4);

	Elements* elements = (Elements*)malloc(sizeof(Elements));
	elements_fill(elements, elements_data, 1, ELEMENT_QUAD, nodes);
	elements_calc_volume(elements);

	Materials* materials_struct = (Materials*)malloc(sizeof(Materials));
	materials_fill(materials_struct, material_data, 1);

	Solver_Config* solver_struct = (Solver_Config*)malloc(sizeof(Solver_Config));
	fill_solver_config(solver_struct, solver_data);



	Faces* faces = faces_create_from_elements(elements);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);


	// CHECK NUMBER OF BOUNDARIES
	EXPECT_EQ(boundaries->n_boundaries, 4);

	// STABLISH A DIRICHLET OF VALUE 100
	//resultant flux should be 200
	boundaries->boundary_array[0].fixed_value = 100;
	// first update to the element should be 0.0001
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, materials_struct, solver_struct->dt);
	EXPECT_NEAR(elements->elements[0]->test_value, 0.0001, 1e-7);
	// second update to the element should be 0.00019999959
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, materials_struct, solver_struct->dt);
	EXPECT_NEAR(elements->elements[0]->test_value, 0.00019999959, 1e-7);

	// STABLISH A NEUMAN OF VALUE 100
	//resultant flux should be 100
	boundaries->boundary_array[0].BC_type = NEUMAN;
	boundaries->boundary_array[0].fixed_value = 100;
	// first update to the element should be 0.00024999959
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, materials_struct, solver_struct->dt);
	EXPECT_NEAR(elements->elements[0]->test_value, 0.00024999959, 1e-4);
	// second update to the element should be 0.00024999959
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, materials_struct, solver_struct->dt);
	EXPECT_NEAR(elements->elements[0]->test_value, 0.00029999959, 1e-4);
}


