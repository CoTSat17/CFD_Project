#include <solver_config.h>



void fill_solver_config(Solver_Config* solver_struct, float* solver_data) {

	solver_struct->t_start = solver_data[0];
	solver_struct->t_end = solver_data[1];
	solver_struct->dt = solver_data[2];


}


