#pragma once
#include "element.h"



/// <summary>
/// <para>Struct for the interface between elements.</para>
/// <para>Its normal is the same as the normal of "Element_1".</para>
/// </summary>
typedef struct Interface {
	float flux;			///< Flux value between the elements
	Element* element_1; ///< Pointer to the "Element" struct that shares normal with the interface
	Element* element_2; ///< Pointer to the "Element" struct that has it's normal in the opposite direction 
} Interface;

/// <summary>
/// Creates a interface struct
/// <para>The flux default value is 0</para>
/// </summary>
/// <param name="element_1">Element" struct that shares normal with the interface</param>
/// <param name="element_2">"Element" struct that has it's normal in the opposite direction</param>
/// <returns></returns>
Interface* interface_create(Element* element_1, Element* element_2);


/// <summary>
/// Calculates the flux in the interface between the 2 elements
/// <para>--</para>
/// <para>The flux is calculates by "central diferencing"</para>
/// </summary>
/// <param name="interface">Interface where the flux will be calculated</param>
void interface_calculate_flux(Interface* interface);