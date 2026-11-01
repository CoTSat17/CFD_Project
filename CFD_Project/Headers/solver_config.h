#pragma once


typedef struct Solver_Config {
	float t_start;
	float t_end;
	float dt;
}Solver_Config;

void fill_solver_config(Solver_Config* solver_struct, float* solver_data);
