#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <boundary_conditions.h>
#include <solver_config.h>
}

TEST(Boundary_Test, Boundary_Create) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Element* element = element_create(&nodes->nodes[0], ELEMENT_QUAD, 1);
	EXPECT_NE(element, nullptr);

	Faces* faces = faces_create_from_element(element);
	EXPECT_NE(faces, nullptr);

	Boundary boundary;
	int boundary_response = boundary_create(&faces->face_array[0], &boundary);
	EXPECT_EQ(boundary_response, 0);

	EXPECT_EQ(boundary.BC_type, DIRICHLET);
	EXPECT_EQ(boundary.element->id, 1);
	EXPECT_EQ(boundary.fixed_value, 0.0);
	EXPECT_EQ(boundary.flux, 0.0);
	EXPECT_EQ(boundary.face->face_id, 1);
}

TEST(Boundary_Test, Boundary_Create_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Element* element = element_create(&nodes->nodes[0], ELEMENT_QUAD, 1);
	EXPECT_NE(element, nullptr);

	Faces* faces = faces_create_from_element(element);
	EXPECT_NE(faces, nullptr);

	Boundary boundary;
	int boundary_response = boundary_create(NULL, &boundary);
	EXPECT_EQ(boundary_response, 1);

	boundary_response = boundary_create(&faces->face_array[0], NULL);
	EXPECT_EQ(boundary_response, 1);

	faces->face_array[0].element = NULL;
	boundary_response = boundary_create(&faces->face_array[0], &boundary);
	EXPECT_EQ(boundary_response, 1);
}




TEST(Boundary_Test, Boundary_Update) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Element* element = element_create(&nodes->nodes[0], ELEMENT_QUAD, 1);
	EXPECT_NE(element, nullptr);

	Faces* faces = faces_create_from_element(element);
	EXPECT_NE(faces, nullptr);

	Boundary boundary;
	int boundary_response = boundary_create(&faces->face_array[0], &boundary);
	EXPECT_EQ(boundary_response, 0);

	EXPECT_EQ(boundary.BC_type, DIRICHLET);
	EXPECT_EQ(boundary.element->id, 1);
	EXPECT_EQ(boundary.fixed_value, 0.0);
	EXPECT_EQ(boundary.flux, 0.0);
	EXPECT_EQ(boundary.face->face_id, 1);

	int boundary_update_result = boundary_update(&boundary, NEUMAN, 100.0);
	EXPECT_EQ(boundary_update_result, 0);
	EXPECT_EQ(boundary.BC_type, NEUMAN);
	EXPECT_EQ(boundary.fixed_value, 100.0);
}


TEST(Boundary_Test, Boundary_Update_ErrorHandeling) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Element* element = element_create(&nodes->nodes[0], ELEMENT_QUAD, 1);
	EXPECT_NE(element, nullptr);

	Faces* faces = faces_create_from_element(element);
	EXPECT_NE(faces, nullptr);

	Boundary boundary;
	int boundary_response = boundary_create(&faces->face_array[0], &boundary);


	int boundary_update_result = boundary_update(NULL, NEUMAN, 100.0);
	EXPECT_EQ(boundary_update_result, 1);

	boundary_update_result = boundary_update(&boundary, N_BOUNDARIES, 100.0);
	EXPECT_EQ(boundary_update_result, 1);
}



TEST(Boundary_Test, Boundary_Calculate_Flux) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0};

	int node_index[] = { 1,	2,	4,	3};

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	int clasify_response = faces_clasify(faces, &interfaces, &boundaries);
	EXPECT_EQ(clasify_response, 0);


	// CHECK NUMBER OF BOUNDARIES
	EXPECT_EQ(boundaries->n_boundaries, 4);

	// STABLISH A DIRICHLET OF VALUE 100
	//resultant flux should be 200
	boundaries->boundary_array[0].fixed_value = 100;
	int boundary_flux_response = boundary_calculate_flux(&boundaries->boundary_array[0]);
	EXPECT_EQ(boundary_flux_response, 0);
	EXPECT_EQ(boundaries->boundary_array[0].flux, 200);

	// STABLISH A NEUMAN OF VALUE 100
	//resultant flux should be 100
	boundaries->boundary_array[1].BC_type = NEUMAN;
	boundaries->boundary_array[1].fixed_value = 100;
	boundary_flux_response = boundary_calculate_flux(&boundaries->boundary_array[1]);
	EXPECT_EQ(boundary_flux_response, 0);
	EXPECT_EQ(boundaries->boundary_array[1].flux, 100);

}


TEST(Boundary_Test, Boundary_Calculate_Flux_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	Faces* faces = faces_create_from_elements(elements);

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	int clasify_response = faces_clasify(faces, &interfaces, &boundaries);
	EXPECT_EQ(clasify_response, 0);



	int boundary_flux_response = boundary_calculate_flux(NULL);
	EXPECT_EQ(boundary_flux_response, 1);

	Element* del_element = boundaries->boundary_array[1].element;
	boundaries->boundary_array[1].element = NULL;
	boundary_flux_response = boundary_calculate_flux(&boundaries->boundary_array[1]);
	EXPECT_EQ(boundary_flux_response, 1);


	boundaries->boundary_array[1].element = del_element;
	boundaries->boundary_array[1].face = NULL;
	boundary_flux_response = boundary_calculate_flux(&boundaries->boundary_array[1]);
	EXPECT_EQ(boundary_flux_response, 1);
}




TEST(Boundary_Test, Boundary_Update_Element_Value) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								1.0,	1.0,	0.0,
								0.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	3,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };



	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_element(elements->elements[0]);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);

	Boundary boundary;
	int boundary_response = boundary_create(&faces->face_array[0], &boundary);
	EXPECT_EQ(boundary_response, 0);

	int boundary_update_response = boundary_update(&boundary, DIRICHLET, 10.0);
	EXPECT_EQ(boundary_update_response, 0);

	boundary_calculate_flux(&boundary);


	// DIRICHLET
	int boundary_update_element_response = boundary_update_element_value(&boundary, &materials_struct, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 0);
	EXPECT_NEAR(elements->elements[0]->test_value, 0.01, 0.001);

	// NEUMAN
	boundary_update(&boundary, NEUMAN, 10.0);
	boundary_calculate_flux(&boundary);
	boundary_update_element_response = boundary_update_element_value(&boundary, &materials_struct, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 0);
	EXPECT_NEAR(elements->elements[0]->test_value, 0.015, 0.001);


}


TEST(Boundary_Test, Boundary_Update_Element_Value_ErrorHandeling) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								1.0,	1.0,	0.0,
								0.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	3,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };



	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_element(elements->elements[0]);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);

	Boundary boundary;
	int boundary_response = boundary_create(&faces->face_array[0], &boundary);
	EXPECT_EQ(boundary_response, 0);

	int boundary_update_response = boundary_update(&boundary, DIRICHLET, 10.0);
	EXPECT_EQ(boundary_update_response, 0);

	boundary_calculate_flux(&boundary);

	/// TEST
	int boundary_update_element_response = boundary_update_element_value(NULL, &materials_struct, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 1);

	boundary_update_element_response = boundary_update_element_value(&boundary, NULL, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 1);

	boundary_update_element_response = boundary_update_element_value(&boundary, &materials_struct, 0.0);
	EXPECT_EQ(boundary_update_element_response, 1);

	Element* element_del = boundary.element;
	boundary.element = NULL;
	boundary_update_element_response = boundary_update_element_value(&boundary, &materials_struct, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 1);

	boundary.element = element_del;
	Face* face_del = boundary.face;
	boundary.face = NULL;
	boundary_update_element_response = boundary_update_element_value(&boundary, &materials_struct, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 1);

	boundary.face = face_del;
	materials_struct.material_array[0]->density = 0.0;
	boundary_update_element_response = boundary_update_element_value(&boundary, &materials_struct, solver_struct.dt);
	EXPECT_EQ(boundary_update_element_response, 1);
}






TEST(Boundary_Test, Boundaries_Update_Element_Value) {
	float nodes_data[] = {	1.0,	0.0,	0.0,	0.0,
							2.0,	1.0,	0.0,	0.0,
							3.0,	0.0,	1.0,	0.0,
							4.0,	1.0,	1.0,	0.0 };

	int elements_data[] = { 1,	1,	2,	4,	3 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };

	float solver_data[] = { 0.0, 1.0, 0.001 };

	Nodes nodes;
	nodes_fill(&nodes, nodes_data, 4);

	Elements elements;
	elements_fill(&elements, elements_data, 1, ELEMENT_QUAD, &nodes);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);



	Faces* faces = faces_create_from_elements(&elements);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	int faces_clasify_result = faces_clasify(faces, &interfaces, &boundaries);
	EXPECT_EQ(faces_clasify_result, 0);


	// CHECK NUMBER OF BOUNDARIES
	EXPECT_EQ(boundaries->n_boundaries, 4);

	// STABLISH A DIRICHLET OF VALUE 100
	//resultant flux should be 200
	boundaries->boundary_array[0].fixed_value = 100;
	// first update to the element should be 0.0001
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, &materials_struct, solver_struct.dt);
	EXPECT_NEAR(elements.elements[0]->test_value, 0.0001, 1e-7);
	// second update to the element should be 0.00019999959
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, &materials_struct, solver_struct.dt);
	EXPECT_NEAR(elements.elements[0]->test_value, 0.00019999959, 1e-7);

	// STABLISH A NEUMAN OF VALUE 100
	//resultant flux should be 100
	boundaries->boundary_array[0].BC_type = NEUMAN;
	boundaries->boundary_array[0].fixed_value = 100;
	// first update to the element should be 0.00024999959
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, &materials_struct, solver_struct.dt);
	EXPECT_NEAR(elements.elements[0]->test_value, 0.00024999959, 1e-4);
	// second update to the element should be 0.00024999959
	boundaries_calculate_flux(boundaries);
	boundaries_update_element_value(boundaries, &materials_struct, solver_struct.dt);
	EXPECT_NEAR(elements.elements[0]->test_value, 0.00029999959, 1e-4);
}


