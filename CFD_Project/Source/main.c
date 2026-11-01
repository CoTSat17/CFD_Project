#include "main.h"

int main() {

	Nodes* nodes = (Nodes*)malloc(sizeof(Nodes));
	Elements* elements = (Elements*)malloc(sizeof(Elements));
	Materials* materials_struct = (Materials*)malloc(sizeof(Materials));
	Solver_Config* solver_struct = (Solver_Config*)malloc(sizeof(Solver_Config));

	file_read(nodes, elements, materials_struct, solver_struct);

	Faces* faces = faces_create_from_elements(elements);

	Interfaces* interfaces = NULL;
	Boundaries* boundaries = NULL;
	faces_clasify(faces, &interfaces, &boundaries);

	

	for (int i = 0; i < 100000000; i++) {
		printf("\n\n -- TIME STEP -- %d\n", i + 1);
		for (int j = 0; j < elements->n_elements; j++) {
			printf("%f\t", elements->elements[j]->test_value);
		}
		interfaces_calculate_flux(interfaces);
		interfaces_update_element_value(interfaces, materials_struct, solver_struct->dt);
	}

	return 0;

}