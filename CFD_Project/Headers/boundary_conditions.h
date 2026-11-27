#pragma once
#include "faces.h"
#include "materials.h"

typedef enum 
{
	DIRICHLET,
	NEUMAN,
	N_BOUNDARIES
}Boundary_Type;



typedef struct Boundary {
	Face* face;  // Face where the BC is situated
	Boundary_Type BC_type; // Type of boundary condition applied
	float flux; 
	float fixed_value; 
	Element* element;
}Boundary;


/// <summary>
/// Creates a boundary struct at the desired position in memory
/// <para>By default a Dirichlet with value 0.0 is implemented.</para>
/// </summary>
/// <param name="face">Face where the boundary condition is located
/// <param name="memory_position"> Pointer where the new boundary must be stored
/// <returns> 0: OK  ||  1: ERROR </returns>
int boundary_create(Face* face, Boundary* memory_position);





/// <summary>
/// Updates the boundary condition to the dessired type and value
/// </summary>
/// <param name="boundary">Boundary to be changed</param>
/// <param name="bc_type">Type of boundary dessired:	DIRICHLET; NEUMAN</param>
/// <param name="value">Value of the boundary condition</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int boundary_update(Boundary* boundary, Boundary_Type bc_type, float value);


/// <summary>
/// Calculates the flux due to boundary conditions
/// <para>--</para>
/// </summary>
/// <param name="boundary">boundary where the flux will be calculated</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int boundary_calculate_flux(Boundary* boundary);

/// <summary>
/// Calculates the new element value due to the effects of the BC
/// </summary>
/// <param name="boundary">Boundary to be implemented</param>
/// <param name="materials_struct">Pointer to "materias_struct" where the fluid materials are stored</param>
/// <param name="dt">Time delta in each calculation</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int boundary_update_element_value(Boundary* boundary, Materials* materials_struct, float dt);


typedef struct Boundaries {
	Boundary* boundary_array;
	int n_boundaries;
}Boundaries;


/// <summary>
/// Destroy Boundaries struct and the "boundary struct" that form it
/// </summary>
/// <param name="boundaries_struct">Boundaries struct to be deleted</param>
void boundaries_destroy(Boundaries** boundaries_struct);



/// <summary>
/// Calculates the flux due to boundary conditions
/// <para>--</para>
/// </summary>
/// <param name="boundaries"> Group of boundaries where the flux will be calculated</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int boundaries_calculate_flux(Boundaries* boundaries);



/// <summary>
/// Calculates the new element value due to the effects of the BC
/// </summary>
/// <param name="boundary">Group of boundary to be implemented</param>
/// <param name="materials_struct">Pointer to "materias_struct" where the fluid materials are stored</param>
/// <param name="dt">Time delta in each calculation</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int boundaries_update_element_value(Boundaries* boundaries, Materials* materials_struct, float dt);
