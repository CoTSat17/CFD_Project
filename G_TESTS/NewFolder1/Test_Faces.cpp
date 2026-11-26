#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <faces.h>
#include <interface.h>
#include <boundary_conditions.h>
}


TEST(Face_Test, Faces_Create_From_Element) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0};

	int node_inex[] = { 1,	2,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Element* element = element_create(nodes->nodes, ELEMENT_TRI, 1);

	Faces* faces = faces_create_from_element(element);
	EXPECT_NE(faces, nullptr);


	// Check number of faces
	EXPECT_EQ(faces->n_faces, 3);

	// Check face 1
	EXPECT_EQ(faces->face_array[0].face_id, 1);
	EXPECT_EQ(faces->face_array[1].face_id, 2);
	EXPECT_EQ(faces->face_array[2].face_id, 3);

	// Check element pointer in each face
	EXPECT_EQ(faces->face_array[0].element, element);
	EXPECT_EQ(faces->face_array[1].element, element);
	EXPECT_EQ(faces->face_array[2].element, element);

	// Check node id 
	EXPECT_EQ(faces->face_array[0].nodes_id[0], 2);
	EXPECT_EQ(faces->face_array[0].nodes_id[1], 1);
	EXPECT_EQ(faces->face_array[1].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[1].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[2].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[2].nodes_id[1], 1);

}



TEST(Face_Test, Faces_Create_From_Element_ErrorHandeling) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0 };

	int node_inex[] = { 1,	2,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Element* element = element_create(nodes->nodes, ELEMENT_TRI, 1);

	Faces* faces = faces_create_from_element(NULL);
	EXPECT_EQ(faces, nullptr);


}












TEST(Face_Test, Faces_Create_From_Elements) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0};

	int node_inex[] = { 1,	2,	3,
						2,	3,	4};

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(elements);
	EXPECT_NE(faces, nullptr);

	EXPECT_EQ(faces->n_faces, 6);
	
	EXPECT_EQ(faces->face_array[0].element, elements->elements[0]);
	EXPECT_EQ(faces->face_array[0].nodes_id[0], 1);
	EXPECT_EQ(faces->face_array[0].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[1].element, elements->elements[0]);
	EXPECT_EQ(faces->face_array[1].nodes_id[0], 2);
	EXPECT_EQ(faces->face_array[1].nodes_id[1], 3);
	EXPECT_EQ(faces->face_array[2].element, elements->elements[0]);
	EXPECT_EQ(faces->face_array[2].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[2].nodes_id[1], 1);

	EXPECT_EQ(faces->face_array[3].element, elements->elements[1]);
	EXPECT_EQ(faces->face_array[3].nodes_id[0], 2);
	EXPECT_EQ(faces->face_array[3].nodes_id[1], 3);
	EXPECT_EQ(faces->face_array[4].element, elements->elements[1]);
	EXPECT_EQ(faces->face_array[4].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[4].nodes_id[1], 4);
	EXPECT_EQ(faces->face_array[5].element, elements->elements[1]);
	EXPECT_EQ(faces->face_array[5].nodes_id[0], 4);
	EXPECT_EQ(faces->face_array[5].nodes_id[1], 2);


	// Check face 1
	EXPECT_EQ(faces->face_array[0].face_id, 1);
	EXPECT_EQ(faces->face_array[1].face_id, 2);
	EXPECT_EQ(faces->face_array[2].face_id, 3);

}



TEST(Face_Test, Faces_Create_From_Elements_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_inex[] = { 1,	2,	3,
						2,	3,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(NULL);
	EXPECT_EQ(faces, nullptr);


}








TEST(Face_Test, Face_Equal) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_inex[] = { 1,	2,	3,
						2,	3,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Face** sorted_faces = faces_sort(faces);

	int faces_equal_1 = faces_equal(sorted_faces[2], sorted_faces[3]);
	EXPECT_EQ(faces_equal_1, 1);
	int faces_equal_2 = faces_equal(sorted_faces[0], sorted_faces[3]);
	EXPECT_EQ(faces_equal_2, 0);
}





TEST(Face_Test, Face_Center) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	2.0,	0.0,
								1.0,	2.0,	0.0 };

	int node_inex[] = { 1,	2,	4, 3};

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_QUAD, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Point face_center_position;
	int function_return = face_center(&faces->face_array[0], &face_center_position);
	EXPECT_NE(function_return, 1);

	EXPECT_EQ(face_center_position.x, 0.5);
	EXPECT_EQ(face_center_position.y, 0.0);
	EXPECT_EQ(face_center_position.z, 0.0);


	function_return = face_center(&faces->face_array[1], &face_center_position);
	EXPECT_NE(function_return, 1);

	EXPECT_EQ(face_center_position.x, 1.0);
	EXPECT_EQ(face_center_position.y, 1.0);
	EXPECT_EQ(face_center_position.z, 0.0);
}


TEST(Face_Test, Face_Center_ErrorHandeling) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	2.0,	0.0,
								1.0,	2.0,	0.0 };

	int node_inex[] = { 1,	2,	4,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_QUAD, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Point face_center_position;
	int function_return = face_center(NULL, &face_center_position);
	EXPECT_EQ(function_return, 1);

	function_return = face_center(&faces->face_array[0], NULL);
	EXPECT_EQ(function_return, 1);

	faces->face_array[0].element->nodes[0]->id = 99;
	function_return = face_center(&faces->face_array[0], &face_center_position);
	EXPECT_EQ(function_return, 1);


	elements_destroy(&elements);

}



TEST(Face_Test, Face_Search_Node_by_Id) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	2.0,	0.0,
								1.0,	2.0,	0.0 };

	int node_inex[] = { 1,	2,	4, 3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_QUAD, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Node* node = face_search_node_by_id(&faces->face_array[0], 1);
	EXPECT_NE(node, nullptr);

	/// TEST

	EXPECT_EQ(node, nodes_search_id(nodes, 1));
}




TEST(Face_Test, Face_Search_Node_by_Id_ErrorHandeling) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	2.0,	0.0,
								1.0,	2.0,	0.0 };

	int node_inex[] = { 1,	2,	4, 3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_QUAD, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Node* node = face_search_node_by_id(NULL, 1);
	EXPECT_EQ(node, nullptr);

	node = face_search_node_by_id(&faces->face_array[0], 99);
	EXPECT_EQ(node, nullptr);

	node = face_search_node_by_id(&faces->face_array[0], -99);
	EXPECT_EQ(node, nullptr);
	/// TEST

}





TEST(Face_Test, Faces_Search_Id) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	2.0,	0.0,
								1.0,	2.0,	0.0 };

	int node_inex[] = { 1,	2,	4, 3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_QUAD, nodes);

	Faces* faces = faces_create_from_elements(elements);

	EXPECT_EQ(faces_search_id(faces, 1)->face_id, 1);
	EXPECT_EQ(faces_search_id(faces, 2)->face_id, 2);
}







TEST(Face_Test, Face_Calc_Area) {
	// TEST 3D

	float nodes_data_3d[] = {
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

	int elements_data_3d[] = {
		1, 1, 2, 3, 4, 5, 6, 7, 8, // Element 1
		2, 2, 9,10, 3, 6,11, 12, 7  // Element 2
	};



	Nodes* nodes_3d = (Nodes*)malloc(sizeof(Nodes));
	nodes_fill(nodes_3d, nodes_data_3d, 12);

	Elements* elements_3d = (Elements*)malloc(sizeof(Elements));
	elements_fill(elements_3d, elements_data_3d, 2, ELEMENT_HEXA, nodes_3d);

	Faces* faces_3d = faces_create_from_elements(elements_3d);

	int function_return = face_calc_area(&faces_3d->face_array[0]);

	EXPECT_EQ(function_return, 0);
	EXPECT_EQ(faces_3d->face_array[0].area, 1.0);





	float node_data_2d[] = { 1.0,	0.0,	0.0,	0.0,
						2.0,	0.0,	1.0,	0.0,
						3.0,	1.0,	0.0,	0.0 };

	Nodes* nodes_2d = (Nodes*)malloc(sizeof(Nodes));
	if (nodes_2d == NULL) return;

	nodes_fill(nodes_2d, node_data_2d, 3);

	Element* element_2d = element_create(nodes_2d->nodes, ELEMENT_TRI, 1);
	EXPECT_NE(element_2d, nullptr);

	Faces* faces_2d = faces_create_from_element(element_2d);
	EXPECT_NE(faces_2d, nullptr);

	function_return = face_calc_area(&faces_2d->face_array[0]);

	EXPECT_EQ(function_return, 0);
	EXPECT_EQ(faces_2d->face_array[0].area, 1.0);
}


TEST(Face_Test, Face_Calc_Area_ErrorHandeling) {

	// TEST 3D

	float nodes_data_3d[] = {
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

	int elements_data_3d[] = {
		1, 1, 2, 3, 4, 5, 6, 7, 8, // Element 1
		2, 2, 9,10, 3, 6,11, 12, 7  // Element 2
	};



	Nodes* nodes_3d = (Nodes*)malloc(sizeof(Nodes));
	nodes_fill(nodes_3d, nodes_data_3d, 12);

	Elements* elements_3d = (Elements*)malloc(sizeof(Elements));
	elements_fill(elements_3d, elements_data_3d, 2, ELEMENT_HEXA, nodes_3d);

	Faces* faces_3d = faces_create_from_elements(elements_3d);



	int function_return = face_calc_area(NULL);
	EXPECT_EQ(function_return, 1);

	faces_3d->face_array[0].nodes_id[0] = 99;
	function_return = face_calc_area(&faces_3d->face_array[0]);
	EXPECT_EQ(function_return, 1);



	// TEST 2D

	float node_data_2d[] = { 1.0,	0.0,	0.0,	0.0,
						2.0,	0.0,	1.0,	0.0,
						3.0,	1.0,	0.0,	0.0 };

	Nodes* nodes_2d = (Nodes*)malloc(sizeof(Nodes));
	if (nodes_2d == NULL) return;

	nodes_fill(nodes_2d, node_data_2d, 3);

	Element* element_2d = element_create(nodes_2d->nodes, ELEMENT_TRI, 1);
	EXPECT_NE(element_2d, nullptr);

	Faces* faces_2d = faces_create_from_element(element_2d);
	EXPECT_NE(faces_2d, nullptr);



	function_return = face_calc_area(NULL);
	EXPECT_EQ(function_return, 1);

	faces_2d->face_array[0].nodes_id[0] = 99;
	function_return = face_calc_area(&faces_2d->face_array[0]);
	EXPECT_EQ(function_return, 1);
}




TEST(Face_Test, Faces_Clasify) {
	float nodes_data[] = {	1.0,	0.0,	0.0,	0.0,
							2.0,	1.0,	0.0,	0.0,
							3.0,	0.0,	1.0,	0.0,
							4.0,	1.0,	1.0,	0.0 };

	int elements_data[] = { 1,	1,	2,	3,
							2,	2,	3,	4 };


	Nodes* nodes = (Nodes*)calloc(1, sizeof(Nodes));
	nodes_fill(nodes, nodes_data, 4);

	Elements* elements = (Elements*)calloc(1, sizeof(Elements));
	elements_fill(elements, elements_data, 2, ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	int function_response = faces_clasify(faces, &interfaces, &boundaries);
	EXPECT_EQ(function_response, 0);


	// TEST INTERFACES
	EXPECT_EQ(interfaces->n_interfaces, 1);

	EXPECT_EQ(interfaces->interface_array[0].element_1->id, 2);
	EXPECT_EQ(interfaces->interface_array[0].element_2->id, 1);

	EXPECT_EQ(interfaces->interface_array[0].face->nodes_id_sorted[0],3 );
	EXPECT_EQ(interfaces->interface_array[0].face->nodes_id_sorted[1],2 );


	// TEST INTERFACES
	EXPECT_EQ(boundaries->n_boundaries, 4);



}



TEST(Face_Test, Faces_Clasify_ErrorHandeling) {
	float nodes_data[] = { 1.0,	0.0,	0.0,	0.0,
							2.0,	1.0,	0.0,	0.0,
							3.0,	0.0,	1.0,	0.0,
							4.0,	1.0,	1.0,	0.0 };

	int elements_data[] = { 1,	1,	2,	3,
							2,	2,	3,	4 };


	Nodes* nodes = (Nodes*)calloc(1, sizeof(Nodes));
	nodes_fill(nodes, nodes_data, 4);

	Elements* elements = (Elements*)calloc(1, sizeof(Elements));
	elements_fill(elements, elements_data, 2, ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(elements);

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;

	int function_response = faces_clasify(NULL, &interfaces, &boundaries);
	EXPECT_EQ(function_response, 1);
	EXPECT_EQ(interfaces, nullptr);
	EXPECT_EQ(boundaries, nullptr);

	function_response = faces_clasify(faces, NULL, &boundaries);
	EXPECT_EQ(function_response, 1);
	EXPECT_EQ(interfaces, nullptr);
	EXPECT_EQ(boundaries, nullptr);

	function_response = faces_clasify(faces, &interfaces, NULL);
	EXPECT_EQ(function_response, 1);
	EXPECT_EQ(interfaces, nullptr);
	EXPECT_EQ(boundaries, nullptr);

	Element* del_element = faces->face_array[0].element;
	faces->face_array[0].element = NULL;
	function_response = faces_clasify(faces, &interfaces, &boundaries);
	EXPECT_EQ(function_response, 1);
	EXPECT_EQ(interfaces, nullptr);
	EXPECT_EQ(boundaries, nullptr);


}