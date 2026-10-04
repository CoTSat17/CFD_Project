#pragma once
#include "faces.h"

typedef enum 
{
	DIRICHLET,
	NEUMAN,
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
/// <para>By default a Dirichlet with value 0 is implemented.</para>
/// </summary>
/// <param name="face">Face where the boundary condition is located
/// <param name="memory_position"> Pointer where the new boundary must be stored
void boundary_create(Face* face, Boundary* memory_position);


/// <summary>
/// Updates the boundary condition to the dessired type and value
/// </summary>
/// <param name="boundary">Boundary to be changed</param>
/// <param name="bc_type">Type of boundary dessired:	DIRICHLET; NEUMAN</param>
/// <param name="value">Value of the boundary condition</param>
void boundary_update(Boundary* boundary, Boundary_Type bc_type, float value);


/// <summary>
/// Calculates the flux due to boundary conditions
/// <para>--</para>
/// </summary>
/// <param name="boundary">boundary where the flux will be calculated</param>
void boundary_calculate_flux(Boundary* boundary);



typedef struct Boundaries {
	Boundary* boundary_array;
	int n_boundaries;
}Boundaries;

/// <summary>
/// Creates the boundaries of the mesh
/// </summary>
/// <param name="faces">Faces that form the mehs</param>
/// <returns>Pointer to the struct that stores the Boundaries</returns>
Boundaries* boundaries_create(Faces* faces);



/// <summary>
/// Calculates the flux due to boundary conditions
/// <para>--</para>
/// </summary>
/// <param name="boundaries"> Group of boundaries where the flux will be calculated</param>
void boundaries_calculate_flux(Boundaries* boundaries);