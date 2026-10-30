#pragma once
#include <stdlib.h>



typedef struct Material {
	int id;
	float density;
	float viscosity;
	float thermal_conductivity;
	float specific_heat;
}Material;


/// <summary>
/// Creates a Material struct with the given data
/// </summary>
/// <para>---- Array_Structure: [id, density, viscosity, thermal_conductivity, specific_heat]</para>
/// </param>
Material* material_create( float material_data[]);



typedef struct Materials {
	Material** material_array;
	int n_materials;
}Materials;



void materials_fill(Materials* materials,float* material_data,int count);
