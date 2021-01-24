#include "basis_pms.h"
#include "pms.h"

int main(int argc, char *argv[])
{
	//start_timing();
	start = chrono::steady_clock::now(); //for windows
	Satlike s;
	vector<int> init_solution;
	stringstream ss;
	ss<<argv[2];
	int cutofftime;
	ss>>cutofftime;
	s.build_instance(argv[1]);
	s.cutoff_time=cutofftime;
	s.local_search_with_decimation(init_solution, argv[1]);
	//s.constructSolnManually();
	//if(s.verify_sol()) cout<<"right"<<endl;
	//else cout<<"wrong"<<endl;

	//s.simple_print();
	s.print_best_solution();
	s.free_memory();

	return 0;
}
