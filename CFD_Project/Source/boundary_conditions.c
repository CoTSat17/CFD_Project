#include <boundary_conditions.h>


void boundary_create(Face* face, Boundary* memory_position) {
	Boundary* new_boundary = memory_position;
	if (new_boundary == NULL) return NULL;

	new_boundary->face = face;
	new_boundary->element = face->element;
	new_boundary->BC_type = DIRICHLET;
	new_boundary->fixed_value = 0.0;
	new_boundary->flux = 0.0;
}








Boundaries* boundaries_create(Faces* faces) {
	// --------------------
	// RESERVE MEMORY SPACE
	// --------------------
	// Initial memory reservation that overstates the ammount of memory needed.
	Boundaries* boundaries = (Boundaries*)malloc(sizeof(Boundaries) + faces->n_faces * sizeof(Boundary));
	if (boundaries == NULL) return NULL;


	boundaries->boundary_array = (Boundary*)(boundaries + 1);
	boundaries->n_boundaries = 0;

	Boundary* new_boundary = boundaries->boundary_array;

	for (int i = 0; i < faces->n_faces-1; i++) {
		if ( ! faces_equal(&faces->face_array[i], &faces->face_array[i + 1])) {
			// If faces are not equal then it must be a boundary
			boundary_create(&faces->face_array[i], new_boundary); 

			boundaries->n_boundaries++;
			new_boundary++;

			if (i == faces->n_faces - 2) {
				boundary_create(&faces->face_array[i+1], new_boundary);

				boundaries->n_boundaries++;
				new_boundary++;
			}

		}

		else {
			// If the face is equal to the next then the next one can be skipped
			i++;
		}
	}




	// -----------------
	// ADJUST THE MEMORY 
	// -----------------
	Boundary* boundary_adjusted = realloc(boundaries, sizeof(Boundary) + boundaries->n_boundaries * sizeof(Boundary));
	if (boundary_adjusted == NULL) return NULL;

	return boundary_adjusted;
}