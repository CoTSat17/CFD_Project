#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <node.h>
#include <element.h>
#include <materials.h>
#include <solver_config.h>

/// <summary>
/// Reads the imput file and stores the information on the given pointers
/// </summary>
/// <param name="nodes">Pointer where the nodes will be stored</param>
/// <param name="elements">Pointer where the elements will be stored</param>
/// <param name="materials">Pointer where the materials will be stored</param>
/// <param name="solver_config">Pointer where the solver config will be stored</param>
void file_read(Nodes* nodes, Elements* elements, Materials* materials, Solver_Config* solver_config);


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
/// <param name="count">Number of Elements</param>
/// <param name="element_type">element: ELEMENT_TRI,ELEMENT_QUAD & ELEMENT_HEXA</param>
/// <param name="nodes">Nodes struct that contains nodes info</param>
void file_elements_preprocesser(FILE* file, Elements* elements, int count, Element_Types_Avialable element_type, Nodes* nodes);




/// <summary>
/// From the input file it processes the section of INIT_VALUES.
/// 
/// <para> Calls "elements_init_conditions" to save the info in the elements</para> 
/// </summary>
/// <param name="file">File that contains the information</param>
/// <param name="elements">Struct where the info is saved</param>
/// <param name="count">Number of Elements</param>
void file_elements_initial_values(FILE* file, Elements* elements, int count);


/// <summary>
/// From the input file it processes the section of MATERIALS
/// 
/// <para> Calls "materials_fill" to save the info of the materials</para> 
/// </summary>
/// <param name="file">File that contains the information</param>
/// <param name="materials">Struct where the info is saved</param>
/// <param name="count">Number of materials</param>
void file_materials_proprocesser(FILE* file, Materials* materials, int count);


/// <summary>
/// From the input file it processes the section of SOLVER
/// 
/// <para> Calls "fill_solver_config" to save the info</para> 
/// </summary>
/// <param name="file">File that contains the information</param>
/// <param name="solver_struct">Struct where the info is saved</param>
void file_solver_proprocesser(FILE* file, Solver_Config* solver_struct);

