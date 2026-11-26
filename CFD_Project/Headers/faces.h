#pragma once
#include <stdlib.h>
#include <element.h>
#include <string.h>

typedef struct Interfaces Interfaces;
typedef struct Boundaries Boundaries;

/// <summary>
/// Face of a element of the mesh
/// </summary>
typedef struct Face {
	int* nodes_id;		///< Pointer to array of ids that form the face
	int* nodes_id_sorted;
	Element* element;	///< Element that has that face
	int face_id;
	float area;			///< Area of the face
}Face;


/// <summary>
/// Checks if two diferent face structs define the same face
/// </summary>
/// <param name="face_1">First face to compare</param>
/// <param name="face_2">Second face to compare</param>
/// <returns>1 if equal // 0 if different</returns>
int faces_equal(Face* face_1, Face* face_2);


/// <summary>
/// Calculates the center point of the face
/// </summary>
/// <param name="face">Face struct where the center point will be calculated</param>
/// <param name="center_point">Pointer to "point" that stores the center</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int face_center(Face* face, Point* center_point);



/// <summary>
/// Searches for the Node struct with the corresponding id
/// </summary>
/// <param name="face">Face struct where the id is</param>
/// <param name="node_id">Id of the node</param>
/// <returns>Pointer to the node || NULL if faliure detected</returns>
Node* face_search_node_by_id(Face* face, int node_id);



/// <summary>
/// Calculates the area of the face
/// The value is stored in the struct
/// </summary>
/// <param name="face">Face struct where the area will be calculated</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int face_calc_area(Face* face);


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
/// <returns>Pointer to a "Faces" struct that contains an array with the faces
///  || NULL if faliure detected</returns>
Faces* faces_create_from_element(Element* element);



/// <summary>
/// Creates a list of the faces that form the mesh
/// <para> The "nodes" must be ordered anticlockwise </para>
/// </summary>
/// <param name="elemetns">Pointer to an array of pointers to the faces </param>
/// <returns>Pointer to a "Faces" struct that contains an array with the faces
///  || NULL if faliure detected</returns>
Faces* faces_create_from_elements(Elements* elements);





/// <summary>
/// Given a "Faces" struct it sorts the faces by descending order of its nodes
/// <para> Only the array of "Face" structs is ordered, the pool of node ids remains constant.
/// </summary>
/// <param name="faces">Faces struct to sort</param>
/// <returns>Pointer to a "Faces" struct that contains an array with the faces
///  || NULL if faliure detected</returns>
Face** faces_sort(Faces* faces);



Face* faces_search_id(Faces* faces_struct, int face_id);




/// <summary>
/// Classifys the faces into interfaces or boundary. 
/// <para> Fills the corresponding structs </para>
/// </summary>
/// <param name="faces">Array of faces to be clasified</param>
/// <param name="interfaces_struct">Struct where interfaces will be stored</param>
/// <param name="boundaries_struct">Struct where boundaries will be stored</param>
/// <returns> 0: OK  ||  1: ERROR </returns>
int faces_clasify(Faces* faces, Interfaces** interfaces_struct, Boundaries** boundaries_struct);
