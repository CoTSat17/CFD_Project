
#include <pch.h>

#include "gtest/gtest.h"
extern "C" {
#include <node.h>
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
	float positions[] = { 0.0,	0.0,	0.0,
							1.0,	0.0,	0.0,
							0.0,	1.0,	0.0,
							1.0,	1.0,	0.0 };

	Nodes* nodes = nodes_create(positions, 12);

	Node* searched_node = nodes_search_id(nodes, 4);

	EXPECT_EQ(searched_node->id, 4);
	EXPECT_EQ(searched_node->position.x, 1.0);



	nodes_destroy(nodes);
}


TEST(Nodes_Test, Nodes_Creation) {
	float positions[] = { 0.0,	0.0,	0.0,
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


