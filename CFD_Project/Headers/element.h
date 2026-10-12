#pragma once
#include <node.h>

typedef struct Face Face;


typedef enum {
	ELEMENT_TRI,
	ELEMENT_QUAD,
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
	int node_per_face;
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
/// <param name="element_type">Type of element: ELEMENT_TRI,ELEMENT_QUAD & ELEMENT_HEXA </param>
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
/// Calculates distance between center of element and face
/// </summary>
/// <param name="element">element struct to be used</param>
/// <param name="face">face struct to be used</param>
/// <returns>Float: Distance</returns>
float element_dist_to_face(Element* element, Face* face);


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
/// <param name="element_type">Type of element: ELEMENT_TRI,ELEMENT_QUAD & ELEMENT_HEXA </param>
/// <param name="nodes_list">Nodes list with the created nodes</param>
/// <returns>Returns a "elements" struct with all the new elemnts and the quantity</returns>
Elements* elements_create(int* nodes_index, int size_array, Element_Types_Avialable element_type, Nodes* nodes_list);



/// <summary>
/// From a already existing elements struct, its filled with the corresponding data
///      
/// Takes the ID from the input file
/// </summary>
/// <param name="elements_struct">Struct to be filled</param>
/// <param name="elements_data">Array with the structure: [ id, node_id_1, node_id_2, node_id_3, ...]</param>
/// <param name="n_elements">Number of values that are in the "nodes_index" array</param>
/// <param name="element_type">Type of element: ELEMENT_TRI,ELEMENT_QUAD & ELEMENT_HEXA </param>
/// <param name="nodes_list">Nodes struct that contains nodes info</param>
void elements_fill(Elements* elements_struct,int* elements_data, int n_elements, Element_Types_Avialable element_type, Nodes* nodes_list);


/// <summary>
/// Updates the initial values of the elements
/// </summary>
/// <param name="elements_struct">Struct where elements are stored</param>
/// <param name="init_values">Array of initial values</param>
void elements_init_conditions(Elements* elements_struct, int* init_values);
