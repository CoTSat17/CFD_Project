#include <boundary_conditions.h>


int boundary_create(Face* face, Boundary* memory_position) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (face == NULL || memory_position == NULL || face->element == NULL) return 1;


	// ------------
	// Create boundary
	// ------------
	Boundary* new_boundary = memory_position;

	new_boundary->face = face;
	new_boundary->element = face->element;
	new_boundary->BC_type = DIRICHLET;
	new_boundary->fixed_value = 0.0;
	new_boundary->flux = 0.0;


	return 0;


}


int boundary_update(Boundary* boundary, Boundary_Type bc_type, float value) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (boundary == NULL || bc_type >= N_BOUNDARIES) return 1;

	// ------------
	// UPDATE BOUNDARY
	// ------------
	boundary->BC_type = bc_type;
	boundary->fixed_value = value;

	return 0;

}





int boundary_calculate_flux(Boundary* boundary) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (boundary == NULL ) return 1;

	// The process depends on the type of boundary
	if (boundary->BC_type == NEUMAN) {
		boundary->flux = boundary->fixed_value;
	}
	else if (boundary->BC_type == DIRICHLET) {
		if ( boundary->element == NULL || boundary->face == NULL) return 1;
		boundary->flux = (boundary->fixed_value - boundary->element->test_value)
									/ element_dist_to_face(boundary->element, boundary->face);
	}


	return 0;

}





int boundary_update_element_value(Boundary* boundary, Materials* materials_struct, float dt) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (boundary == NULL || boundary->element == NULL || boundary->face == NULL || materials_struct == NULL || dt <= 0) return 1;


	// ------------
	// GET VALUES
	// ------------
	Material* material = materials_struct->material_array[boundary->element->material_id - 1];
	if (material == NULL) return 1;
	float density = material->density;
	float cp = material->specific_heat;
	float k = material->thermal_conductivity;
	if (density <= 0 || cp <= 0 || k <= 0) return 1;
	// ------------
	// UPDATE VALUE
	// ------------
	boundary->element->test_value += dt / (density * cp * boundary->element->volume) * (k * boundary->flux * boundary->face->area);


	return 0;
}





void boundaries_destroy(Boundaries** boundaries_struct) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (boundaries_struct == NULL || *boundaries_struct == NULL) return;

	// --------------
	// Destroy struct
	// --------------
	free(*boundaries_struct);
	*boundaries_struct = NULL;
}




int boundaries_calculate_flux(Boundaries* boundaries) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (boundaries == NULL) return 1;


	// ------------
	// CALCULATE FLUX
	// ------------
	for (int i = 0; i < boundaries->n_boundaries; i++) {
		if ( boundary_calculate_flux(&boundaries->boundary_array[i]) != 0) return 1;
	}

	return 0;
}




int boundaries_update_element_value(Boundaries* boundaries, Materials* materials_struct, float dt) {
	// ------------
	// CHECK INPUTS
	// ------------
	if (boundaries == NULL || boundaries->n_boundaries <= 0 || materials_struct == NULL || dt <= 0) return 1;

	// ------------
	// CALCULATE VALUES
	// ------------
	for (int i = 0; i < boundaries->n_boundaries; i++) {
		if ( boundary_update_element_value(&boundaries->boundary_array[i], materials_struct, dt) == 1) return 1;
	}

	return 0;
}
