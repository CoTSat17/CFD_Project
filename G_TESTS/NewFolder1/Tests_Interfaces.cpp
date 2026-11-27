#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <interface.h>
#include <solver_config.h>
}

TEST(Interface_Test, Interface_Create) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0};

	int node_index[] = {	1,	2,	4,	3,
							2,	5,	6,	4};

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8,ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Interface interface;
	int interface_response = interface_create(elements->elements[0], elements->elements[1], &faces->face_array[2], &interface);
	EXPECT_EQ(interface_response, 0);


	// TESTS
	EXPECT_EQ(interface.flux, 0.0);
	EXPECT_EQ(interface.element_1->id, 1);
	EXPECT_EQ(interface.element_2->id, 2);
	EXPECT_EQ(interface.face->face_id, 3);
	 
}


TEST(Interface_Test, Interface_Create_ErrorHandeling) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);



	// TESTS
	Interface interface;
	int interface_response = interface_create(NULL, elements->elements[1], &faces->face_array[2], &interface);
	EXPECT_EQ(interface_response, 1);

	interface_response = interface_create(elements->elements[0], NULL, &faces->face_array[2], &interface);
	EXPECT_EQ(interface_response, 1);

	interface_response = interface_create(elements->elements[0], elements->elements[1], NULL, &interface);
	EXPECT_EQ(interface_response, 1);

	interface_response = interface_create(elements->elements[0], elements->elements[1], &faces->face_array[2], NULL);
	EXPECT_EQ(interface_response, 1);


}



TEST(Interface_Test, Interface_Calculate_Flux) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };



	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);


	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Interface interface;
	int interface_response = interface_create(elements->elements[0], elements->elements[1], &faces->face_array[2], &interface);
	EXPECT_EQ(interface_response, 0);


	// TESTS
	EXPECT_EQ(interface.flux, 0.0);

	elements->elements[0]->test_value = 10.0;
	int interface_calculate_flux_response = interface_calculate_flux(&interface);
	EXPECT_EQ(interface_calculate_flux_response, 0);
	EXPECT_NEAR(interface.flux, 10.0, 0.001);

	elements->elements[1]->test_value = 20.0;
	interface_calculate_flux_response = interface_calculate_flux(&interface);
	EXPECT_EQ(interface_calculate_flux_response, 0);
	EXPECT_NEAR(interface.flux, -10.0, 0.001);

}


TEST(Interface_Test, Interface_Calculate_Flux_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };



	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);


	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Interface interface;
	int interface_response = interface_create(elements->elements[0], elements->elements[1], &faces->face_array[2], &interface);
	EXPECT_EQ(interface_response, 0);


	// TESTS
	EXPECT_EQ(interface.flux, 0.0);

	int interface_calculate_flux_response = interface_calculate_flux(NULL);
	EXPECT_EQ(interface_calculate_flux_response, 1);

	interface.element_1 = NULL;
	interface_calculate_flux_response = interface_calculate_flux(&interface);
	EXPECT_EQ(interface_calculate_flux_response, 1);

}


TEST(Interface_Test, Interface_Update_Element_Value) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = {	1,	2,	4,	3,
							2,	5,	6,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	EXPECT_EQ(interfaces->interface_array[0].flux, 0.0);

	elements->elements[0]->test_value = 10.0;
	int interface_calculate_flux_response = interface_calculate_flux(&interfaces->interface_array[0]);
	EXPECT_EQ(interface_calculate_flux_response, 0);
	EXPECT_NEAR(interfaces->interface_array[0].flux, -10.0, 0.001);


	int interfaces_update_element_value_response = interfaces_update_element_value(interfaces, &materials_struct, solver_struct.dt);
	EXPECT_EQ(interfaces_update_element_value_response, 0);
	EXPECT_NEAR(elements->elements[0]->test_value, 9.995, 0.001);
	EXPECT_NEAR(elements->elements[1]->test_value, 0.005, 0.001);


}


TEST(Interface_Test, Interface_Update_Element_Value_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	int interface_calculate_flux_response = interface_calculate_flux(&interfaces->interface_array[0]);
	EXPECT_EQ(interface_calculate_flux_response, 0);

	
	
	// TESTS

	int interfaces_update_element_value_response = interfaces_update_element_value(NULL, &materials_struct, solver_struct.dt);
	EXPECT_EQ(interfaces_update_element_value_response, 1);

	interfaces_update_element_value_response = interfaces_update_element_value(interfaces, NULL, solver_struct.dt);
	EXPECT_EQ(interfaces_update_element_value_response, 1);

	interfaces_update_element_value_response = interfaces_update_element_value(interfaces, &materials_struct, 0.0);
	EXPECT_EQ(interfaces_update_element_value_response, 1);

	Element* element_del = interfaces->interface_array[0].element_1;
	interfaces->interface_array[0].element_1 = NULL;
	interfaces_update_element_value_response = interfaces_update_element_value(interfaces, &materials_struct, solver_struct.dt);
	EXPECT_EQ(interfaces_update_element_value_response,1);

	interfaces->interface_array[0].element_1 = element_del;
	Face* face_del = interfaces->interface_array[0].face;
	interfaces->interface_array[0].face = NULL;
	interfaces_update_element_value_response = interfaces_update_element_value(interfaces, &materials_struct, solver_struct.dt);
	EXPECT_EQ(interfaces_update_element_value_response, 1);

	interfaces->interface_array[0].face = face_del;
	materials_struct.material_array[0]->density = 0.0;
	interfaces_update_element_value_response = interfaces_update_element_value(interfaces, &materials_struct, solver_struct.dt);
	EXPECT_EQ(interfaces_update_element_value_response, 1);
}








TEST(Interface_Test, Interfaces_Calculate_Flux) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	elements->elements[0]->test_value = 10.0;
	int interfaces_calculate_flux_response = interfaces_calculate_flux(interfaces);
	EXPECT_EQ(interfaces_calculate_flux_response, 0);
	EXPECT_NEAR(interfaces->interface_array[0].flux, -10.0, 0.001);

	elements->elements[1]->test_value = 20.0;
	interfaces_calculate_flux_response = interfaces_calculate_flux(interfaces);
	EXPECT_EQ(interfaces_calculate_flux_response, 0);
	EXPECT_NEAR(interfaces->interface_array[0].flux, 10.0, 0.001);

}


TEST(Interface_Test, Interfaces_Calculate_Flux_ErrorHandeling) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);


	// TESTS
	int interfaces_calculate_flux_response = interfaces_calculate_flux(NULL);
	EXPECT_EQ(interfaces_calculate_flux_response, 1);

	interfaces->interface_array[0].element_1 = NULL;
	interfaces_calculate_flux_response = interfaces_calculate_flux(interfaces);
	EXPECT_EQ(interfaces_calculate_flux_response, 1);
}



TEST(Interface_Test, Interfaces_Update_Element_Value) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	elements->elements[0]->test_value = 10.0;
	int interfaces_calculate_flux_response = interfaces_calculate_flux(interfaces);
	EXPECT_EQ(interfaces_calculate_flux_response, 0);
	EXPECT_NEAR(interfaces->interface_array[0].flux, -10.0, 0.001);

	int intefaces_update_element_value = interfaces_update_element_value(interfaces, &materials_struct, solver_struct.dt);
	EXPECT_EQ(intefaces_update_element_value, 0);
	EXPECT_NEAR(elements->elements[0]->test_value, 9.995, 0.001);
	EXPECT_NEAR(elements->elements[1]->test_value, 0.005, 0.001);



}




TEST(Interface_Test, Interfaces_Update_Element_Value_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0 };

	int node_index[] = { 1,	2,	4,	3,
							2,	5,	6,	4 };

	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 2.0 };
	float solver_data[] = { 0.0, 1.0, 1.0 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	EXPECT_NE(nodes, nullptr);
	Elements* elements = elements_create(node_index, 8, ELEMENT_QUAD, nodes);
	EXPECT_NE(elements, nullptr);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	Materials materials_struct;
	materials_fill(&materials_struct, material_data, 1);

	Solver_Config solver_struct;
	fill_solver_config(&solver_struct, solver_data);


	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	elements->elements[0]->test_value = 10.0;
	int interfaces_calculate_flux_response = interfaces_calculate_flux(interfaces);
	EXPECT_EQ(interfaces_calculate_flux_response, 0);
	EXPECT_NEAR(interfaces->interface_array[0].flux, -10.0, 0.001);


	// TESTS
	int intefaces_update_element_value = interfaces_update_element_value(NULL, &materials_struct, solver_struct.dt);
	EXPECT_EQ(intefaces_update_element_value, 1);

	intefaces_update_element_value = interfaces_update_element_value(interfaces, NULL, solver_struct.dt);
	EXPECT_EQ(intefaces_update_element_value, 1);

	intefaces_update_element_value = interfaces_update_element_value(interfaces, &materials_struct, 0.0);
	EXPECT_EQ(intefaces_update_element_value, 1);

	intefaces_update_element_value = interfaces_update_element_value(interfaces, &materials_struct, -5.0);
	EXPECT_EQ(intefaces_update_element_value, 1);

	interfaces->interface_array[0].element_1 = NULL;
	intefaces_update_element_value = interfaces_update_element_value(interfaces, &materials_struct, solver_struct.dt);
	EXPECT_EQ(intefaces_update_element_value, 1);

}