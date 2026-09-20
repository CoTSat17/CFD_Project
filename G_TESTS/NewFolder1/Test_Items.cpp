#include <pch.h>

#include "gtest/gtest.h"
#include <random>

extern "C" {
#include <point.h>
#include <node.h>
#include <element.h>
}


/// <summary>
/// Test the Point struct
/// </summary>
TEST(Point_Test, Point_Struct) {
	float x_position = 1;
	float y_position = 0;
	float z_position = -1;

	Point point = { x_position, y_position, z_position };

	EXPECT_FLOAT_EQ(point.x, x_position);
	EXPECT_FLOAT_EQ(point.y, y_position);
	EXPECT_FLOAT_EQ(point.z, z_position);
}

TEST(Point_Test, Point_Creation) {
	float x_position = 1;
	float y_position = 0;
	float z_position = -1;

	Point point = point_create(x_position, y_position, z_position);

	EXPECT_FLOAT_EQ(point.x, x_position);
	EXPECT_FLOAT_EQ(point.y, y_position);
	EXPECT_FLOAT_EQ(point.z, z_position);
}


TEST(Point_Test, Point_Dist) {
	Point point_1 = point_create(1, 0, 1);
	Point point_2 = point_create(1, 1, 1);

	float dist = point_dist(point_1, point_2);

	EXPECT_FLOAT_EQ(dist, 1.0);

}








TEST(Node_Test, Node_Struct) {
	Point point_1 = point_create(1, 0, 1);
	Node node = { point_1, 1 };

	EXPECT_FLOAT_EQ(node.position.x, 1);
	EXPECT_FLOAT_EQ(node.position.y, 0);
	EXPECT_FLOAT_EQ(node.position.z, 1);

	EXPECT_FLOAT_EQ(node.id, 1);
}

TEST(Node_Test, Node_Creation) {
	Node* node = node_create(1, 0, 1, 1);

	EXPECT_FLOAT_EQ(node->position.x, 1);
	EXPECT_FLOAT_EQ(node->position.y, 0);
	EXPECT_FLOAT_EQ(node->position.z, 1);

	EXPECT_FLOAT_EQ(node->id, 1);

	node_destroy(node);
}

TEST(Nodes_Test, Nodes_Search) {
	float positions[] = {	0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0};

	Nodes* nodes = nodes_create(positions, 12);

	Node* searched_node = nodes_search_id(nodes, 4);

	EXPECT_EQ(searched_node->id, 4);
	EXPECT_EQ(searched_node->position.x, 1.0);
	


	nodes_destroy(nodes);
}


TEST(Nodes_Test, Nodes_Creation) {
	float positions[] = {	0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	Nodes* nodes = nodes_create(positions, 12);

	//TEST n_nodes
	EXPECT_EQ(nodes->n_nodes, 4);

	for (int i = 0; i < nodes->n_nodes; i++) {
		// TEST id of nodes is correct
		EXPECT_EQ(nodes->nodes[i]->id, i + 1);

		// TEST position of nodes
		EXPECT_EQ(nodes->nodes[i]->position.x, positions[i * 3]);
		EXPECT_EQ(nodes->nodes[i]->position.y, positions[i * 3 + 1]);
		EXPECT_EQ(nodes->nodes[i]->position.z, positions[i * 3 + 2]);
	}


	nodes_destroy(nodes);
}








TEST(Element_Test, Element_Struct) {
	int n_nodes = 3;
	float test_value = 4.5;
	int id = 1;

	Node** nodes= (Node**)malloc(sizeof(Node*) * n_nodes);

	
	Node* node_1 = node_create(1, 2, 3, 0);
	Node* node_2 = node_create(1, 2, 3, 1);
	Node* node_3 = node_create(1, 2, 3, 2);
	nodes[0] = node_1;
	nodes[1] = node_2;
	nodes[2] = node_3;

	Element_Type element_type = element_type_data[ELEMENT_TRI];

	Element element = { nodes, test_value, &element_type, id };

	for (int i = 0; i < n_nodes; i++) {
		EXPECT_EQ(element.nodes[i]->id, i);
	}
	EXPECT_EQ(element.element_type->n_nodes, n_nodes);
	EXPECT_EQ(element.element_type->n_faces, 3);
	EXPECT_EQ(element.test_value, test_value);
	EXPECT_EQ(element.id, id);


	node_destroy(node_1);
	node_destroy(node_2);
	node_destroy(node_3);
	free(nodes);
}



TEST(Element_Test, Element_Create) {
	int n_nodes = 3;
	int id = 1;

	Node** nodes = (Node**)malloc(sizeof(Node*) * n_nodes);

	Node* node_1 = node_create(1, 2, 3, 0);
	Node* node_2 = node_create(1, 2, 3, 1);
	Node* node_3 = node_create(1, 2, 3, 2);
	nodes[0] = node_1;
	nodes[1] = node_2;
	nodes[2] = node_3;

	Element* element = element_create(nodes, ELEMENT_TRI, id);


	for (int i = 0; i < n_nodes; i++) {
		EXPECT_EQ(element->nodes[i]->id, i);
	}

	EXPECT_EQ(element->element_type->n_nodes, n_nodes);
	EXPECT_EQ(element->element_type->n_faces, 3);
	EXPECT_EQ(element->test_value, 0);
	EXPECT_EQ(element->id, id);

	element_destroy(element);
	node_destroy(node_1);
	node_destroy(node_2);
	node_destroy(node_3);
	free(nodes);

}


TEST(Element_Test, Element_Dist) {
	int n_nodes = 3;
	int id = 1;

	Node** nodes_1 = (Node**)malloc(sizeof(Node*) * n_nodes);
	Node** nodes_2 = (Node**)malloc(sizeof(Node*) * n_nodes);

	Node* node_1 = node_create(0, 0, 0, 0);
	Node* node_2 = node_create(1, 0, 0, 1);
	Node* node_3 = node_create(0, 1, 0, 2);
	Node* node_4 = node_create(0, 0, 1, 2);
	nodes_1[0] = node_1;
	nodes_1[1] = node_2;
	nodes_1[2] = node_3;

	nodes_2[0] = node_1;
	nodes_2[1] = node_2;
	nodes_2[2] = node_4;

	Element* element_1 = element_create(nodes_1, ELEMENT_TRI, id);
	Element* element_2 = element_create(nodes_2, ELEMENT_TRI, id);

	float dist = element_dist(element_1, element_2);
	
	EXPECT_NEAR(dist, 0.47114, 0.001);

}




TEST(Element_Test, Element_Center) {
	int n_nodes = 3;
	int id = 1;

	Node** nodes = (Node**)malloc(sizeof(Node*) * n_nodes);

	Node* node_1 = node_create(0, 0, 0, 0);
	Node* node_2 = node_create(1, 0, 0, 1);
	Node* node_3 = node_create(0, 1, 0, 2);
	nodes[0] = node_1;
	nodes[1] = node_2;
	nodes[2] = node_3;

	Element* element = element_create(nodes, ELEMENT_TRI, id);

	Point center = element_center(element);

	EXPECT_NEAR(center.x, 0.3333, 0.001);
	EXPECT_NEAR(center.y, 0.3333, 0.001);
	EXPECT_FLOAT_EQ(center.z, 0.0);
	EXPECT_FLOAT_EQ(center.z, 0.0);


}






TEST(Elements_Test, Elements_Create) {
	float positions[] = {	0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	int element_index[] = { 1,		2,		3,
							2,		3,		4 };


	Nodes* nodes = nodes_create(positions, 12);

	Elements* elements = elements_create(element_index, 6, ELEMENT_TRI, nodes);


	// Test size
	EXPECT_EQ(elements->n_elements, 2);

	// Test elements
	EXPECT_EQ(elements->elements[0]->id, 1);
	EXPECT_EQ(elements->elements[0]->nodes[1]->id, 2);


}