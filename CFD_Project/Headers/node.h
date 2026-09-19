#pragma once
#include "point.h"





/// <summary>
/// Struct that defines a node
/// </summary>
/// <param name="position">Point Struct that indicates the location of the node</param>
/// <param name="id">ID value of the node</param>
typedef struct Node {
	Point position;
	int id;
}Node;

/// <summary>
/// Creates the Node in the dessired coordinates
/// </summary>
/// <param name="x">X coordinate position</param>
/// <param name="y">y coordinate position</param>
/// <param name="z">z coordinate position</param>
/// <param name="id"> id of the node</param>
/// <returns>Node pointer</returns>
Node* node_create(float x, float y, float z, int id);


/// <summary>
/// Free the memory of the node, also frees the memory of the corresponding point
/// </summary>
/// <param name="node">Node that will be destroid</param>
void node_destroy(Node* node);



/// <summary>
/// Contains all the created nodes
/// </summary>
/// <param name="nodes"> Array of the nodes created</param>
/// <param name="n_nodes"> Total amount of nodes created</param>
typedef struct Nodes {
	Node** nodes;
	int n_nodes;
}Nodes;

/// <summary>
/// Creates the nodes defined by an array of the coordinates.
///      
/// Automatically gives a ID for each node
/// </summary>
/// <param name="nodes_position">Array of nodes that contains the coordinates of the nodes</param>
/// <param name="size_array">Number of values that are in the "nodes_position" array</param>
/// <returns>Returns a nodes struct with all the new node and the quantity</returns>
Nodes* nodes_create(float* nodes_position, int size_array);

/// <summary>
/// Destroys and frees the memory of all the "nodes" struct and all the "node" in it
/// </summary>
/// <param name="nodes">Nodes struct to be deleted</param>
void nodes_destroy(Nodes* nodes);

/// <summary>
/// Searches the nodes for the corresponding ID node
/// </summary>
/// <param name="nodes">Nodes list to be searched</param>
/// <param name="id">The ID of the dessired node</param>
/// <returns>Pointer to the Node with the desired ID     /// NULL if ID is not found</returns>
Node* nodes_search_id(Nodes* nodes, int id);