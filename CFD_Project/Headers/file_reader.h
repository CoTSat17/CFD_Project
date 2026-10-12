#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <node.h>
#include <element.h>

/// <summary>
/// Reads the imput file and stores the information on the given pointers
/// </summary>
/// <param name="nodes">Pointer where the nodes will be stored</param>
void file_read(Nodes* nodes, Elements* elements);


/// <summary>
/// From the input file it processes the section of NODES
/// 
/// <para> Calls "nodes_fill" to save the info in the struct</para>
/// </summary>
/// <param name="file">File that contains the information</param>
/// <param name="nodes">Struct where the info is saved</param>
/// <param name="count">Number of Nodes</param>
void file_nodes_preprocesser(FILE* file, Nodes* nodes, int count);


/// <summary>
/// From the input file it processes the section of ELEMENTS
/// 
/// <para> Calls "elements_fill" to save the info in the struct</para> 
/// </summary>
/// <param name="file">File that contains the information</param>
/// <param name="elements">Struct where the info is saved</param>
/// <param name="count">Number of Nodes</param>
/// <param name="element_type">element: ELEMENT_TRI,ELEMENT_QUAD & ELEMENT_HEXA</param>
/// <param name="nodes">Nodes struct that contains nodes info</param>
void file_elements_preprocesser(FILE* file, Elements* elements, int count, Element_Types_Avialable element_type, Nodes* nodes);
