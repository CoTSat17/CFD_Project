#include <materials.h>

Material* material_create( float material_data[]) {
	Material* material_struct = (Material*)malloc(sizeof(Material));
	if (material_struct == NULL) return NULL;

	material_struct->id = material_data[0];
	material_struct->density = material_data[1];
	material_struct->viscosity = material_data[2];
	material_struct->thermal_conductivity = material_data[3];
	material_struct->specific_heat = material_data[4];

	return material_struct;


}







void materials_fill(Materials* materials,float* material_data,int count) {
	int n_materials = count;
	materials->n_materials = n_materials;


	/// -----------------------------------------
	/// CREATE THE ARRAY OF POINTERS TO MATERIALS
	/// -----------------------------------------
	materials->material_array = (Material**)malloc(n_materials * sizeof(Material*));
	if (materials->material_array == NULL) return NULL;

	// iterate through each element
	for (int i = 0; i < n_materials; i++) {
		materials->material_array[i] = material_create(&material_data[i * 5]);
	}

	 
}
