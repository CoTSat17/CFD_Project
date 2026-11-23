
#include <pch.h>

#include "gtest/gtest.h"
extern "C" {
#include <node.h>
}



TEST(Nodes_Test, Node_Create) {
	Node* node = node_create(1, 0, 1, 1);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_NE(node, nullptr);

	EXPECT_FLOAT_EQ(node->position.x, 1);
	EXPECT_FLOAT_EQ(node->position.y, 0);
	EXPECT_FLOAT_EQ(node->position.z, 1);

	EXPECT_FLOAT_EQ(node->id, 1);

	node_destroy(&node);
	EXPECT_EQ(node, nullptr);
}



TEST(Nodes_Test, Nodes_Search_Id) {
	float positions[] = {	0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	Nodes* nodes = nodes_create(positions, 12);

	Node* searched_node = nodes_search_id(nodes, 4);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(searched_node->id, 4);
	EXPECT_EQ(searched_node->position.x, 1.0);



	nodes_destroy(&nodes);
}
TEST(Nodes_Test, Nodes_Search_Id_IncorrectId) {
	float positions[] = { 0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	Nodes* nodes = nodes_create(positions, 12);

	Node* searched_node = nodes_search_id(nodes, -4);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(searched_node, nullptr);



	nodes_destroy(&nodes);
}


TEST(Nodes_Test, Nodes_Search_Id_NotPresentID) {
	float positions[] = {	0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	Nodes* nodes = nodes_create(positions, 12);

	Node* searched_node = nodes_search_id(nodes, 54);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(searched_node, nullptr);



	nodes_destroy(&nodes);
}

TEST(Nodes_Test, Nodes_Create) {
	float positions[] = {	0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	Nodes* nodes = nodes_create(positions, 12);
	EXPECT_NE(nodes, nullptr);
		
	/// -----
	/// TESTS 
	/// -----

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


	nodes_destroy(&nodes);
	EXPECT_EQ(nodes, nullptr);

}





TEST(Nodes_Test, Nodes_Fill) {
	float node_data[] = {	1.0,	0.0,	0.0,	0.0,
							2.0,	0.0,	1.0,	0.0,
							3.0,	1.0,	0.0,	0.0 };

	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	if (nodes == NULL) return;

	int function_return = nodes_fill(nodes, node_data, 3);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(function_return, 0);

	//TEST n_nodes
	EXPECT_EQ(nodes->n_nodes, 3);

	for (int i = 0; i < nodes->n_nodes; i++) {
		// TEST id of nodes is correct
		EXPECT_EQ(nodes->nodes[i]->id, i + 1.0);

		// TEST position of nodes
		EXPECT_EQ(nodes->nodes[i]->position.x, node_data[i * 4 + 1]);
		EXPECT_EQ(nodes->nodes[i]->position.y, node_data[i * 4 + 2]);
		EXPECT_EQ(nodes->nodes[i]->position.z, node_data[i * 4 + 3]);
	}


	nodes_destroy(&nodes);
	EXPECT_EQ(nodes, nullptr);
}


TEST(Nodes_Test, Nodes_Fill_NullInput) {
	float node_data[] = { 1.0,	0.0,	0.0,	0.0,
							2.0,	0.0,	1.0,	0.0,
							3.0,	1.0,	0.0,	0.0 };

	Nodes* nodes = NULL;

	int function_return = nodes_fill(nodes, node_data, 3);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(function_return, 1);


}

TEST(Nodes_Test, Nodes_Fill_IncorrectArraySize) {
	float node_data[] = {	1.0,	0.0,	0.0,	0.0,
							2.0,	0.0,	1.0,	0.0,
							3.0,	1.0,	0.0,	0.0 };

	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	if (nodes == NULL) return;

	int function_return = nodes_fill(nodes, node_data, -5);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(function_return, 1);
}



TEST(Nodes_Test, Nodes_Clear) {
	float node_data[] = {	1.0,	0.0,	0.0,	0.0,
							2.0,	0.0,	1.0,	0.0,
							3.0,	1.0,	0.0,	0.0 };

	Nodes* nodes = (Nodes*)calloc(1,sizeof(Nodes));
	if (nodes == NULL) return;

	nodes_fill(nodes, node_data, 3);
	nodes_clear(nodes);

	/// -----
	/// TESTS 
	/// -----

	EXPECT_EQ(nodes->n_nodes, 0);
	EXPECT_EQ(nodes->nodes, nullptr);
}