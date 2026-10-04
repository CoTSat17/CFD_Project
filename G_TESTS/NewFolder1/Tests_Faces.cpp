#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <faces.h>
}


TEST(Face_Test, Faces_Create_From_Element) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0};

	int node_inex[] = { 1,	2,	3 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Element* element = element_create(nodes->nodes, ELEMENT_TRI, 1);

	Faces* faces = faces_create_from_element(element);


	// Check number of faces
	EXPECT_EQ(faces->n_faces, 3);

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

	EXPECT_EQ(faces->n_faces, 6);
	
	EXPECT_EQ(faces->face_array[0].element, elements->elements[0]);
	EXPECT_EQ(faces->face_array[0].nodes_id[0], 2);
	EXPECT_EQ(faces->face_array[0].nodes_id[1], 1);
	EXPECT_EQ(faces->face_array[1].element, elements->elements[0]);
	EXPECT_EQ(faces->face_array[1].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[1].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[2].element, elements->elements[0]);
	EXPECT_EQ(faces->face_array[2].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[2].nodes_id[1], 1);

	EXPECT_EQ(faces->face_array[3].element, elements->elements[1]);
	EXPECT_EQ(faces->face_array[3].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[3].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[4].element, elements->elements[1]);
	EXPECT_EQ(faces->face_array[4].nodes_id[0], 4);
	EXPECT_EQ(faces->face_array[4].nodes_id[1], 3);
	EXPECT_EQ(faces->face_array[5].element, elements->elements[1]);
	EXPECT_EQ(faces->face_array[5].nodes_id[0], 4);
	EXPECT_EQ(faces->face_array[5].nodes_id[1], 2);
}




TEST(Face_Test, Faces_Sort) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_inex[] = { 1,	2,	3,
						2,	3,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(elements);

	faces_sort(faces);

	EXPECT_EQ(faces->face_array[0].nodes_id[0], 4);
	EXPECT_EQ(faces->face_array[0].nodes_id[1], 3);
	EXPECT_EQ(faces->face_array[1].nodes_id[0], 4);
	EXPECT_EQ(faces->face_array[1].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[2].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[2].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[3].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[3].nodes_id[1], 2);
	EXPECT_EQ(faces->face_array[4].nodes_id[0], 3);
	EXPECT_EQ(faces->face_array[4].nodes_id[1], 1);
	EXPECT_EQ(faces->face_array[5].nodes_id[0], 2);
	EXPECT_EQ(faces->face_array[5].nodes_id[1], 1);
}




TEST(Face_Test, Face_Equal) {
	float nodes_position[] = { 0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0 };

	int node_inex[] = { 1,	2,	3,
						2,	3,	4 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), ELEMENT_TRI, nodes);

	Faces* faces = faces_create_from_elements(elements);

	faces_sort(faces);

	int faces_equal_1 = faces_equal(&faces->face_array[2], &faces->face_array[3]);
	EXPECT_EQ(faces_equal_1, 1);
	int faces_equal_2 = faces_equal(&faces->face_array[0], &faces->face_array[3]);
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

	Point face_center_position = face_center(&faces->face_array[0]);

	EXPECT_EQ(face_center_position.x, 0.5);
	EXPECT_EQ(face_center_position.y, 0.0);
	EXPECT_EQ(face_center_position.z, 0.0);

	face_center_position = face_center(&faces->face_array[1]);

	EXPECT_EQ(face_center_position.x, 1.0);
	EXPECT_EQ(face_center_position.y, 1.0);
	EXPECT_EQ(face_center_position.z, 0.0);
}