#pragma once
#include <stdlib.h>
#include <element.h>


/// <summary>
/// Face of a element of the mesh
/// </summary>
typedef struct Face {
	int* nodes_id;		///< Pointer to array of ids that form the face
	Element* element;	///< Element that has that face
}Face;


/// <summary>
/// Checks if two diferent face structs define the same face
/// </summary>
/// <param name="face_1">First face to compare</param>
/// <param name="face_2">Second face to compare</param>
/// <returns>1 if equal // 0 if different</returns>
int faces_equal(Face* face_1, Face* face_2);



/// <summary>
/// Group of the faces that form the mesh
/// </summary>
typedef struct Faces{
	Face* face_array;
	int n_faces;	 // Number of faces stored
	int* nodes_pool; // Pointer to where all the faces store the nodes_id
	Face* new_face;  // Pointer to where a new "Face" struct must be added
	int* new_nodes;  // Pointer to where a new node_ids must be added
}Faces;

/// <summary>
/// Creates a list with tha faces of the given element
/// <para> _ </para>
/// <para> All memory for the "faces", "face" and "node_ids" is allocated in a single block </para>
/// </summary>
/// <param name="element">Element where the faces must be extracted</param>
/// <returns>Pointer to a "Faces" struct that contains an array with the faces</returns>
Faces* faces_create_from_element(Element* element);



/// <summary>
/// Creates a list of the faces that form the mesh
/// <para> The "nodes" must be ordered anticlockwise </para>
/// </summary>
/// <param name="elemetns">Pointer to an array of pointers to the faces </param>
Faces* faces_create_from_elements(Elements* elements);





/// <summary>
/// Given a "Faces" struct it sorts the faces by descending order of its nodes
/// <para> Only the array of "Face" structs is ordered, the pool of node ids remains constant.
/// </summary>
/// <param name="faces">Faces struct to sort</param>
void faces_sort(Faces* faces);


