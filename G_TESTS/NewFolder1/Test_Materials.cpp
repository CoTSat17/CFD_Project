#include <pch.h>

#include "gtest/gtest.h"
extern "C" {
#include <materials.h>
}


TEST(Materials_Test, Material_Create) {
	
	float material_data[] = { 1.0, 1000.0, 2.0, 1.0, 3.0 };
	Material* material_struct = material_create(material_data);

	EXPECT_EQ(material_struct->id, 1.0);
	EXPECT_EQ(material_struct->density, 1000.0);
	EXPECT_EQ(material_struct->viscosity, 2.0);
	EXPECT_EQ(material_struct->thermal_conductivity, 1.0);
	EXPECT_EQ(material_struct->specific_heat, 3.0);

}


TEST(Materials_Test, Materials_Fill) {

	float materials_data[] = {	1.0,	1000.0,	2.0,	1.0,	3.0,
								2.0,	2000.0,	3.0,	2.0,	6.0};

	Materials* materials_struct = (Materials*)malloc(sizeof(Materials));
	materials_fill(materials_struct, materials_data, 2);


	EXPECT_EQ(materials_struct->material_array[0]->id, 1.0);
	EXPECT_EQ(materials_struct->material_array[0]->density, 1000.0);
	EXPECT_EQ(materials_struct->material_array[0]->viscosity, 2.0);
	EXPECT_EQ(materials_struct->material_array[0]->thermal_conductivity, 1.0);
	EXPECT_EQ(materials_struct->material_array[0]->specific_heat, 3.0);

	EXPECT_EQ(materials_struct->material_array[1]->id, 2.0);
	EXPECT_EQ(materials_struct->material_array[1]->density, 2000.0);
	EXPECT_EQ(materials_struct->material_array[1]->viscosity, 3.0);
	EXPECT_EQ(materials_struct->material_array[1]->thermal_conductivity, 2.0);
	EXPECT_EQ(materials_struct->material_array[1]->specific_heat, 6.0);
}