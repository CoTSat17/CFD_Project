#include <pch.h>

#include "gtest/gtest.h"

extern "C" {
#include <point.h>
}


/// <summary>
/// Test the Point struct
/// </summary>
TEST(Point_Test, Point_Struct) {
	float x_position = 1;
	float y_position = 0;
	float z_position = -1;

	Point point = { x_position, y_position, z_position };

	EXPECT_FLOAT_EQ(point.x, x_position);
	EXPECT_FLOAT_EQ(point.y, y_position);
	EXPECT_FLOAT_EQ(point.z, z_position);
}

TEST(Point_Test, Point_Creation) {
	float x_position = 1;
	float y_position = 0;
	float z_position = -1;

	Point point = point_create(x_position, y_position, z_position);

	EXPECT_FLOAT_EQ(point.x, x_position);
	EXPECT_FLOAT_EQ(point.y, y_position);
	EXPECT_FLOAT_EQ(point.z, z_position);
}


TEST(Point_Test, Point_Dist) {
	Point point_1 = point_create(1, 0, 1);
	Point point_2 = point_create(1, 1, 1);

	float dist = point_dist(point_1, point_2);

	EXPECT_FLOAT_EQ(dist, 1.0);

}



