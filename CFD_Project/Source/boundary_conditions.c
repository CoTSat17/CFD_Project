#include <boundary_conditions.h>


void boundary_create(Face* face, Boundary* memory_position) {
	Boundary* new_boundary = memory_position;
	if (new_boundary == NULL) return NULL;

	new_boundary->face = face;
	new_boundary->element = face->element;
	new_boundary->BC_type = DIRICHLET;
	new_boundary->fixed_value = 0.0;
	new_boundary->flux = 0.0;
	face_calc_area(face);
}


void boundary_calculate_flux(Boundary* boundary) {
	// The process depends on the type of boundary
	if (boundary->BC_type == NEUMAN) {
		boundary->flux = boundary->fixed_value;
	}
	else if (boundary->BC_type == DIRICHLET) {
		boundary->flux = (boundary->fixed_value - boundary->element->test_value)
									/ element_dist_to_face(boundary->element, boundary->face);
	}
}





void boundary_update_element_value(Boundary* boundary, Materials* materials_struct, float dt) {

	Material* material = materials_struct->material_array[boundary->element->material_id - 1];
	float density = material->density;
	float cp = material->specific_heat;
	float k = material->thermal_conductivity;

	boundary->element->test_value += dt / (density * cp * boundary->element->volume) * (k * boundary->flux * boundary->face->area);

}










void boundaries_calculate_flux(Boundaries* boundaries) {
	for (int i = 0; i < boundaries->n_boundaries; i++) {
		boundary_calculate_flux(&boundaries->boundary_array[i]);		
	}
}




void boundaries_update_element_value(Boundaries* boundaries, Materials* materials_struct, float dt) {
	for (int i = 0; i < boundaries->n_boundaries; i++) {
		boundary_update_element_value(&boundaries->boundary_array[i], materials_struct, dt);
	}
}
