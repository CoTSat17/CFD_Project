#pragma once
#include <node.h>



typedef enum {
	ELEMENT_TRI,
	ELEMENT_TETRA,
	ELEMENT_HEXA,
	N_ELEMENTS

}Element_Types_Avialable;


#define MAX_NODES_PER_ELEM 8
#define MAX_FACES_PER_ELEM 6
#define MAX_NODES_PER_FACE 4

/// <summary>
/// Struct with info of each of the element types.
/// <para> Contains info like number of nodes or which elements form each face.</para>
/// </summary>
typedef struct Element_Type {
	int n_nodes;
	int n_faces;
	int face_nodes[MAX_FACES_PER_ELEM][MAX_NODES_PER_ELEM];
}Element_Type;


extern Element_Type element_type_data[N_ELEMENTS];





typedef struct Element {
	Node** nodes;
	float  test_value;
	Element_Type*    element_type;
	int    id;
}Element;

/// <summary>
/// Create a new element
/// </summary>
/// <param name="nodes">Array of pointer to the nodes that form the element</param>
/// <param name="element_type">Type of element: ELEMENT_TRI,ELEMENT_TETRA & ELEMENT_HEXA </param>
/// <param name="id">Element id</param>
/// <returns>Element pointer: Pointer to the new element</returns>
Element* element_create(Node** nodes, Element_Types_Avialable element_type, int id);

/// <summary>
/// Frees the memory allocated to the element
/// </summary>
/// <param name="element">Element to desrtoy</param>
void element_destroy(Element* element);



/// <summary>
/// Calculates the distance between the center of 2 nodes
/// </summary>
/// <param name="element_1">First Element</param>
/// <param name="element_2">Second Element</param>
/// <returns>Float: Distance between the centers</returns>
float element_dist(Element* element_1, Element* element_2);


/// <summary>
/// Calculates the position of the center of an element 
/// #TODO: For now it does a vertex average so it induces error at deformed elements
/// </summary>
/// <param name="element_1">Element to where the position is calculated</param>
/// <returns>Point: Position of the center  </returns>
Point element_center(Element* element_1);







typedef struct Elements {
	Element** elements;
	int n_elements;
} Elements;

/// <summary>
/// Creates the elements defined by an array of indexes of nodes.
///      
/// Automatically gives a ID for each element
/// </summary>
/// <param name="nodes_index">Array of nodes ids that define which nodes form each element</param>
/// <param name="size_array">Number of values that are in the "nodes_index" array</param>
/// <param name="element_type">Type of element: ELEMENT_TRI,ELEMENT_TETRA & ELEMENT_HEXA </param>
/// <param name="nodes_list">Nodes list with the created nodes</param>
/// <returns>Returns a "elements" struct with all the new elemnts and the quantity</returns>
Elements* elements_create(int* nodes_index, int size_array, Element_Types_Avialable element_type, Nodes* nodes_list);




