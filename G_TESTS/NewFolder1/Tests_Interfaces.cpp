#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <interface.h>
#include <solver_config.h>
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

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	interfaces_calculate_flux(interfaces);

	// Test for 0 value in both nodes
	EXPECT_EQ(interfaces->interface_array[0].flux, 0);


	elements->elements[0]->test_value = 1.0;
	interfaces_calculate_flux(interfaces);

	// Test for 0 value in both nodes
	EXPECT_EQ(interfaces->interface_array[0].flux, 1.0);

}





TEST(Interface_Test, Interface_Update_Element_Value) {
	float nodes_data[] = { 1.0,	0.0,	0.0,	0.0,
							2.0,	1.0,	0.0,	0.0,
							3.0,	0.0,	1.0,	0.0,
							4.0,	1.0,	2.0,	0.0 };

	int elements_data[] = { 1,	1,	2,	3,
							2,	2,	3,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 3.0 };

	float solver_data[] = { 0.0, 1.0, 0.001 };

	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	nodes_fill(nodes, nodes_data, 4);

	Elements* elements = (Elements*)malloc(sizeof(Elements));
	elements_fill(elements, elements_data, 2, ELEMENT_TRI, nodes);
	elements_calc_volume(elements);

	Materials* materials_struct = (Materials*)malloc(sizeof(Materials));
	materials_fill(materials_struct, material_data, 1);

	Solver_Config* solver_struct = (Solver_Config*)malloc(sizeof(Solver_Config));
	fill_solver_config(solver_struct, solver_data);


	elements->elements[0]->test_value = 100.0;

	Faces* faces = faces_create_from_elements(elements);

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);


	interfaces_calculate_flux(interfaces);


	interfaces_update_element_value(interfaces, materials_struct, solver_struct->dt);


}



