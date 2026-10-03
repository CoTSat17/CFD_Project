#pragma once
#include "element.h"
#include "faces.h"



/// <summary>
/// <para>Struct for the interface between elements.</para>
/// <para>Its normal is the same as the normal of "Element_1".</para>
/// </summary>
typedef struct Interface {
	float flux;			///< Flux value between the elements
	Element* element_1; ///< Pointer to the "Element" struct that shares normal with the interface
	Element* element_2; ///< Pointer to the "Element" struct that has it's normal in the opposite direction 
	Face* face;			///< Face that forms the interface
} Interface;

/// <summary>
/// Creates a interface struct at the desired position in memory
/// <para>The flux default value is 0</para>
/// </summary>
/// <param name="element_1">Element" struct that shares normal with the interface</param>
/// <param name="element_2">"Element" struct that has it's normal in the opposite direction</param>
/// <param name="face">One of the two Face struts that define the same face
/// <param name="memory_position"> Pointer where the new interface must be stored
/// <returns></returns>
void interface_create(Element* element_1, Element* element_2, Face* face, Interface* memory_position);


/// <summary>
/// Calculates the flux in the interface between the 2 elements
/// <para>--</para>
/// <para>The flux is calculates by "central diferencing"</para>
/// </summary>
/// <param name="interface">Interface where the flux will be calculated</param>
void interface_calculate_flux(Interface* interface);


/// <summary>
/// Calculates the new value of the elements based from the flux of the interface
/// <para>--</para>
/// The first element of the interface is stablished as the one matching the normal of the interface
/// </summary>
/// <param name="interface">Interface that will be calculated</param>
void interface_update_element_value(Interface* interface);



/// <summary>
/// Struct that stores the interfaces between elements
/// </summary>
typedef struct Interfaces {
	Interface* interface_array; // Array with all the element interfaces
	int n_interfaces;			// Number of all the interfaces stored
}Interfaces;	


/// <summary>
/// Creates the interfaces of the elements and returns a pointer
/// </summary>
/// <param name="faces">Faces that form the interface</param>
/// <returns>Pointer to the struct that stores the interfaces</returns>
Interfaces* interfaces_create(Faces* faces);


/// <summary>
/// Calculates the flux for all the interfaces.
/// <para>--</para>
/// <para>The flux is calculates by "central diferencing"</para>
/// </summary>
/// <param name="interfaces">Interfaces array to be calculated</param>
void interfaces_calculate_flux(Interfaces* interfaces);


/// <summary>
/// Calculates the new value of the elements based from the interfaces
/// <para>--</para>
/// The first element of the interface is stablished as the one matching the normal of the interface
/// </summary>
/// <param name="interfaces">Interfaces struct that will be calculated</param>
void interfaces_update_element_value(Interfaces* interfaces);
