#include "main.h"

int main() {
	float nodes_position[] = {	0.0,	0.0,	0.0,
								0.0,	1.0,	0.0,
								1.0,	0.0,	0.0,
								1.0,	1.0,	0.0,
								2.0,	0.0,	0.0,
								2.0,	1.0,	0.0,
								3.0,	1.0,	0.0,
								3.0,	1.0,	0.0,
								4.0,	1.0,	0.0,
								4.0,	1.0,	0.0
								};

	int node_index[] = { 1,	3,	4,	2,
						 3,	5,	6,	4,
						 5,	7,	8,	6,
						 7,	9,	10,	8
						};

	Nodes* nodes = nodes_create(nodes_position, sizeof(nodes_position) / sizeof(nodes_position[0]));
	Elements* elements = elements_create(node_index, sizeof(node_index) / sizeof(node_index[0]), ELEMENT_TETRA, nodes);
	Faces* faces = faces_create_from_elements(elements);
	faces_sort(faces);

	Interfaces* interfaces = interfaces_create(faces);
	elements->elements[0]->test_value = 1.0;
	

	for (int i = 0; i < 10; i++) {
		printf("\n\n -- TIME STEP -- %d\n", i + 1);
		for (int j = 0; j < elements->n_elements; j++) {
			printf("%f\t", elements->elements[j]->test_value);
		}
		interfaces_calculate_flux(interfaces);
		interfaces_update_element_value(interfaces);
	}

	return 0;

}