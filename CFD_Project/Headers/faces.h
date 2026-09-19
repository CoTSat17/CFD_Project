#pragma once
#include <element.h>


/// <summary>
/// Face of a element of the mesh
/// </summary>
typedef struct Face {
	int* nodes_id;		///< Array of the id of the nodes that form the face
	Element* element;	///< Element that has that face
}Face;





/// <summary>
/// Group of the faces that form the mesh
/// </summary>
typedef struct Faces{
	Face* faces;
	int n_faces;
}Faces;



/// <summary>
/// Creates a list of the faces that form the mesh
/// <para> The "nodes" must be ordered anticlockwise </para>
/// </summary>
/// <param name="nodes_index">Array of the elements that form the mesh. 
///				The nodes must be ordered anticlockwise</param>
/// <param name="n_elements">Number of elements in the "elements" array</param>
Faces* create_faces(Elements* elements, int n_elements);
