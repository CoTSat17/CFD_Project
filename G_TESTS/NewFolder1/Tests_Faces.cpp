#include "pch.h"

#include "gtest/gtest.h"

extern "C" {
#include <faces.h>
}


TEST(Face_Test, Interface_Create) {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								1.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0};

	int node_inex[] = { 1,	2,	3,	4,
						2,	5,	4,	6 };

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_inex, sizeof(node_inex) / sizeof(node_inex[0]), 4, nodes);




}


