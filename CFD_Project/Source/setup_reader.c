#include <file_reader.h>


void file_read(Nodes* nodes, Elements* elements) {
	///
	/// The imput file is divided in sections, 
	/// at the beggining of each section there is
	///		- a "KEY" describing the section 
	///		- a "COUNT" of the number of lines

	char file_path[] = "C:/Users/pedro/Desktop/New folder/Docs/Programación/CFD_Project/CFD_Project/setpup.txt";
	FILE* file = fopen(file_path, "r");
	if (!file) return -1;

	// Reserve 256 characters for each line
	char line[256];
	// Reserve 32 characters for the keys
	char key[32];
	// Number of values in each section
	int count;


	///
	/// ADVANCE THROUGH THE LINES
	///
	/// Each line is scanned, when it detects a header that contains what is described
	/// and the ammount it calls the correspoinding reader
	/// 
	/// It skips the lines that are empty
	while (fgets(line, sizeof line, file)) {

		if (line[0] == '\n') continue;

		// Search for the key indicating a new section
		sscanf_s(line, "%31s %d", &key, (unsigned)sizeof(key), &count);
		
		/// PROCESS NODES
		if (strcmp(key, "NODES") == 0) {
			printf("PROCESSING NODES \n");

			file_nodes_preprocesser(file, nodes, count);




		}

		/// PROCESS ELEMENTS
		if (strncmp(key, "ELEMENT",7) == 0) {
			printf("PROCESSING ELEMENTS \n");

			// Determine the element type
			Element_Types_Avialable element_type;
			if (strcmp(key, "ELEMENT_TRI") == 0) {
				element_type = ELEMENT_TRI;
			}
			else if (strcmp(key, "ELEMENT_QUAD") == 0) {
				element_type = ELEMENT_QUAD;
			}
			else if (strcmp(key, "ELEMENT_HEXA") == 0) {
				element_type = ELEMENT_HEXA;
			}

			// process the info
			file_elements_preprocesser(file, elements, count, element_type, nodes);


		}


		/// PROCESS INITIAL CONDITIONS
		if (strcmp(key, "INIT_VALUES") == 0) {
			printf("PROCESSING INIT_VALUES \n");
			file_elements_initial_values(file, elements, count);
		}
		


	}
}




void file_nodes_preprocesser(FILE* file, Nodes* nodes, int count) {

	/// -----------------------------
	/// RESERVE MEMORY FOR THE ARRAY
	/// -----------------------------
	/// For "count" nodes is: 4 float per node-> 1 id & 3 position.
	float* node_data = (float*)malloc(4* count * sizeof(float));


	/// -----------------
	/// PROCESS EACH LINE
	/// -----------------
	/// Each line is one node position with 4 values each
	/// the data is stored in a array thats used later to fill the array

	for (int i = 0; i < count*4; i++) {
		fscanf_s(file, "%f", &node_data[i]);
	}

	/// -----------------
	/// FILL NODES ARRAY
	/// -----------------
	/// With the array that stores the file data,
	/// fill the previously created "nodes" struct
	nodes_fill(nodes, node_data, count);

}



void file_elements_preprocesser(FILE* file, Elements* elements, int count, Element_Types_Avialable element_type, Nodes* nodes) {

	/// -----------------------------
	/// RESERVE MEMORY FOR THE ARRAY
	/// -----------------------------
	/// For "count" elements is: n_nodes_per_element + 1
	int n_ints = count * (element_type_data[element_type].n_nodes + 1);
	int* element_data = (int*)malloc(n_ints * sizeof(int));


	/// -----------------
	/// PROCESS EACH LINE
	/// -----------------
	/// The data is stored in a array thats used later to fill the array

	for (int i = 0; i < n_ints; i++) {
		fscanf_s(file, "%d", &element_data[i]);
	}

	/// -----------------
	/// FILL ELEMENTS ARRAY
	/// -----------------
	/// With the array that stores the file data,
	/// fill the previously created "elements" struct
	elements_fill(elements, element_data, count, element_type, nodes);

}


void file_elements_initial_values(FILE* file, Elements* elements, int count) {
	/// -----------------------------
	/// RESERVE MEMORY FOR THE ARRAY
	/// -----------------------------
	/// For "count" elements is: 2
	int n_floats = count * (2);
	float* init_value_data = (float*)malloc(n_floats * sizeof(float));


	/// -----------------
	/// PROCESS EACH LINE
	/// -----------------
	/// The data is stored in a array thats used later
	
	for (int i = 0; i < n_floats; i++) {
		fscanf_s(file, "%f", &init_value_data[i]);
	}


	/// ----------------------------------------
	/// APPLY THE INITIAL VALUE TO THE ELEMENTS
	/// ----------------------------------------
	/// With the array that stores the inital values,
	/// fill the previously created "elements" struct

	elements_init_conditions(elements, init_value_data);

}