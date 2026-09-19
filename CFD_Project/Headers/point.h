#pragma once
#include <stdlib.h>
#include <math.h>


typedef struct Point {
	float x;
	float y;
	float z;
}Point;

/// <summary>
/// Create a point
/// </summary>
/// <param name="x">X coordinate position</param>
/// <param name="y">y coordinate position</param>
/// <param name="z">z coordinate position</param>
/// <returns>point</returns>
Point point_create(float x, float y, float z);

/// <summary>
/// Calculates the distance between 2 points
/// </summary>
/// <param name="point_1">First point</param>
/// <param name="point_2">Secont point</param>
/// <returns>Float: Distance value</returns>
float point_dist(Point point_1, Point point_2);