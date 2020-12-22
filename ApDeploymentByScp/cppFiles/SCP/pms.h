#ifndef _PMS_H_
#define _PMS_H_

#include <cmath>
#include "basis_pms.h"
#include "deci.h"
#include <sstream>

Satlike::Satlike()
{
	problem_weighted = 1;
	partial = 1; //1 if the instance has hard clauses, and 0 otherwise.

	max_clause_length = 0;
	min_clause_length = 100000000;

	//size of the instance
	num_vars = 0;	//var index from 1 to num_vars
	num_clauses = 0; //clause index from 0 to num_clauses-1
	num_hclauses = 0;
	num_sclauses = 0;

	print_time = 240;
	cutoff_time = 300;
}

void Satlike::settings()
{
	//steps
	tries = 0;
	max_tries = 100000000;
	max_flips = 200000000;
	max_non_improve_flip = 10000000;

	step = 1;

	large_clause_count_threshold = 0;
	soft_large_clause_count_threshold = 0;

	h_inc = 1;
	softclause_weight_threshold = 1000;

	rdprob = 0.01;
	hd_count_threshold = 42;
	rwprob = 0.091;
	smooth_probability = 0.000003;
}

void Satlike::allocate_memory()
{
	int malloc_var_length = num_vars + 10;
	int malloc_clause_length = num_clauses + 10;

	unit_clause = new lit[malloc_clause_length];

	var_lit = new lit *[malloc_var_length];
	var_lit_count = new int[malloc_var_length];
	clause_lit = new lit *[malloc_clause_length];
	clause_lit_count = new int[malloc_clause_length];
	clause_true_lit_thres = new int[malloc_clause_length];

	score = new long long[malloc_var_length];
	var_neighbor = new int *[malloc_var_length];
	var_neighbor_count = new int[malloc_var_length];
	time_stamp = new long long[malloc_var_length];
	neighbor_flag = new int[malloc_var_length];
	temp_neighbor = new int[malloc_var_length];

	org_clause_weight = new long long[malloc_clause_length];
	clause_weight = new long long[malloc_clause_length];
	unit_weight = new long long[malloc_clause_length];
	org_unit_weight = new long long[malloc_clause_length];
	sat_count = new int[malloc_clause_length];
	sat_var = new int[malloc_clause_length];
	clause_selected_count = new long long[malloc_clause_length];
	best_soft_clause = new int[malloc_clause_length];

	hardunsat_stack = new int[malloc_clause_length];
	index_in_hardunsat_stack = new int[malloc_clause_length];
	softunsat_stack = new int[malloc_clause_length];
	index_in_softunsat_stack = new int[malloc_clause_length];

	unsatvar_stack = new int[malloc_var_length];
	index_in_unsatvar_stack = new int[malloc_var_length];
	unsat_app_count = new int[malloc_var_length];

	goodvar_stack = new int[malloc_var_length];
	already_in_goodvar_stack = new int[malloc_var_length];

	cur_soln = new int[malloc_var_length];
	best_soln = new int[malloc_var_length];
	local_opt_soln = new int[malloc_var_length];

	large_weight_clauses = new int[malloc_clause_length];
	soft_large_weight_clauses = new int[malloc_clause_length];
	already_in_soft_large_weight_stack = new int[malloc_clause_length];

	best_array = new int[malloc_var_length];
	temp_lit = new int[malloc_var_length];
}

void Satlike::free_memory()
{
	int i;
	for (i = 0; i < num_clauses; i++)
		delete[] clause_lit[i];

	for (i = 1; i <= num_vars; ++i)
	{
		delete[] var_lit[i];
		delete[] var_neighbor[i];
	}

	delete[] var_lit;
	delete[] var_lit_count;
	delete[] clause_lit;
	delete[] clause_lit_count;
	delete[] clause_true_lit_thres;

	delete[] score;
	delete[] var_neighbor;
	delete[] var_neighbor_count;
	delete[] time_stamp;
	delete[] neighbor_flag;
	delete[] temp_neighbor;

	delete[] org_clause_weight;
	delete[] clause_weight;
	delete[] unit_weight;
	delete[] org_unit_weight;
	delete[] sat_count;
	delete[] sat_var;
	delete[] clause_selected_count;
	delete[] best_soft_clause;

	delete[] hardunsat_stack;
	delete[] index_in_hardunsat_stack;
	delete[] softunsat_stack;
	delete[] index_in_softunsat_stack;

	delete[] unsatvar_stack;
	delete[] index_in_unsatvar_stack;
	delete[] unsat_app_count;

	delete[] goodvar_stack;
	delete[] already_in_goodvar_stack;

	//delete [] fix;
	delete[] cur_soln;
	delete[] best_soln;
	delete[] local_opt_soln;

	delete[] large_weight_clauses;
	delete[] soft_large_weight_clauses;
	delete[] already_in_soft_large_weight_stack;

	delete[] best_array;
	delete[] temp_lit;
}

void Satlike::build_neighbor_relation()
{
	int i, j, count;
	int v, c, n;
	int temp_neighbor_count;

	for (v = 1; v <= num_vars; ++v)
	{
		neighbor_flag[v] = 1;
		temp_neighbor_count = 0;

		for (i = 0; i < var_lit_count[v]; ++i)
		{
			c = var_lit[v][i].clause_num;
			for (j = 0; j < clause_lit_count[c]; ++j)
			{
				n = clause_lit[c][j].var_num;
				if (neighbor_flag[n] != 1)
				{
					neighbor_flag[n] = 1;
					temp_neighbor[temp_neighbor_count++] = n;
				}
			}
		}

		neighbor_flag[v] = 0;

		var_neighbor[v] = new int[temp_neighbor_count];
		var_neighbor_count[v] = temp_neighbor_count;

		count = 0;
		for (i = 0; i < temp_neighbor_count; i++)
		{
			var_neighbor[v][count++] = temp_neighbor[i];
			neighbor_flag[temp_neighbor[i]] = 0;
		}
	}
}

void Satlike::build_instance(char *filename)
{
	istringstream iss;
	char line[1024];
	string line2;
	char tempstr1[10];
	char tempstr2[10];
	int cur_lit;
	int i, v, c;
	//int     temp_lit[MAX_VARS];

	ifstream infile(filename);
	if (!infile)
	{
		cout << "c the input filename " << filename << " is invalid, please input the correct filename." << endl;
		exit(-1);
	}

	/*** build problem data structures of the instance ***/

	getline(infile, line2);

	while (line2[0] != 'p')
	{
		getline(infile, line2);
	}
	for (i = 0; i < 1024; i++)
	{
		line[i] = line2[i];
	}
	int read_items;
	read_items = sscanf(line, "%s %s %d %d %lld", tempstr1, tempstr2, &num_vars, &num_clauses, &top_clause_weight);

	allocate_memory();

	for (c = 0; c < num_clauses; c++)
	{
		clause_lit_count[c] = 0;
		clause_true_lit_thres[c] = 1;
		clause_lit[c] = NULL;
	}

	for (v = 1; v <= num_vars; ++v)
	{
		var_lit_count[v] = 0;
		var_lit[v] = NULL;
		var_neighbor[v] = NULL;
	}

	num_hclauses = num_sclauses = 0;
	unit_clause_count = 0;
	//Now, read the clauses, one at a time.
	int lit_redundent, clause_redundent;

	c = 0;
	while (getline(infile, line2))
	{
		if (line2[0] == 'c')
			continue;
		else
		{
			iss.clear();
			iss.str(line2);
			iss.seekg(0, ios::beg);
		}
		clause_redundent = 0;
		clause_lit_count[c] = 0;

		iss >> org_clause_weight[c];
		iss >> clause_true_lit_thres[c];

		if (org_clause_weight[c] != top_clause_weight)
		{
			total_soft_weight += org_clause_weight[c];
			num_sclauses++;
		}
		else
			num_hclauses++;

		iss >> cur_lit;
		while (cur_lit != 0)
		{
			/*
			lit_redundent=0;
			for(int p=0; p<clause_lit_count[c]; p++)
			{
				if(cur_lit==temp_lit[p]){
					lit_redundent=1;
					break;
				}
				else if(cur_lit==-temp_lit[p]){
					clause_redundent=1;
					break;
				}
			}
			
			if(lit_redundent==0)
			{*/
			temp_lit[clause_lit_count[c]] = cur_lit;
			clause_lit_count[c]++;
			//}

			iss >> cur_lit;
		}

		//if(clause_redundent==0) //the clause is not tautology
		//{
		clause_lit[c] = new lit[clause_lit_count[c] + 1];

		for (i = 0; i < clause_lit_count[c]; ++i)
		{
			clause_lit[c][i].clause_num = c;
			clause_lit[c][i].var_num = abs(temp_lit[i]);

			if (temp_lit[i] > 0)
				clause_lit[c][i].sense = 1;
			else
				clause_lit[c][i].sense = 0;

			var_lit_count[clause_lit[c][i].var_num]++;
		}
		clause_lit[c][i].var_num = 0;
		clause_lit[c][i].clause_num = -1;

		if (clause_lit_count[c] == 1) //kkk
		{
			unit_clause[unit_clause_count++] = clause_lit[c][0];
		}

		if (clause_lit_count[c] > max_clause_length)
			max_clause_length = clause_lit_count[c];
		if (clause_lit_count[c] < min_clause_length)
			min_clause_length = clause_lit_count[c];

		c++;
		//}
		//else
		//{
		//	num_clauses--;
		//	clause_lit_count[c] = 0;
		//}
	}
	infile.close();

	//creat var literal arrays
	for (v = 1; v <= num_vars; ++v)
	{
		var_lit[v] = new lit[var_lit_count[v] + 1];
		var_lit_count[v] = 0; //reset to 0, for build up the array
	}
	//scan all clauses to build up var literal arrays
	for (c = 0; c < num_clauses; ++c)
	{
		for (i = 0; i < clause_lit_count[c]; ++i)
		{
			v = clause_lit[c][i].var_num;
			var_lit[v][var_lit_count[v]] = clause_lit[c][i];
			++var_lit_count[v];
		}
	}
	for (v = 1; v <= num_vars; ++v)
		var_lit[v][var_lit_count[v]].clause_num = -1;

	build_neighbor_relation();

	best_soln_feasible = 0;
	opt_unsat_weight = total_soft_weight + 1;
}

void Satlike::printInputInfo(){
	cout<<"var nums:"<<num_vars<<endl;
	cout<<"clause nums:"<<num_clauses<<endl;
	cout<<clause_lit_count[0]<<endl;
}

void Satlike::init(vector<int> &init_solution)
{
	int v, c;
	int j;

	local_soln_feasible = 0;
	local_opt_unsat_weight = top_clause_weight + 1; //???
	large_weight_clauses_count = 0;
	soft_large_weight_clauses_count = 0;

	//Initialize clause information
	for (c = 0; c < num_clauses; c++)
	{
		already_in_soft_large_weight_stack[c] = 0;
		clause_selected_count[c] = 0;

		if (org_clause_weight[c] == top_clause_weight)
		{
			clause_weight[c] = clause_true_lit_thres[c];
			org_unit_weight[c] = ceil((double)clause_weight[c] / (double)clause_true_lit_thres[c]);
			unit_weight[c] = org_unit_weight[c];
		}
		else
		{
			clause_weight[c] = org_clause_weight[c];
			org_unit_weight[c] = ceil((double)clause_weight[c] / (double)clause_true_lit_thres[c]);
			unit_weight[c] = org_unit_weight[c];

			if (clause_weight[c] > clause_true_lit_thres[c] && already_in_soft_large_weight_stack[c] == 0)
			{
				already_in_soft_large_weight_stack[c] = 1;
				soft_large_weight_clauses[soft_large_weight_clauses_count++] = c;
			}
		}
	}

	for (int i = 0; i < num_clauses; ++i)
	{
		if (org_unit_weight[i] != 1) //???
		{
			cout << "something si wrong " << endl;
		}
	}

	//init solution
	if (init_solution.size() == 0)
	{
		for (v = 1; v <= num_vars; v++)
		{
			cur_soln[v] = rand() % 2;
			time_stamp[v] = 0;
			unsat_app_count[v] = 0;
		}
	}
	else
	{
		for (v = 1; v <= num_vars; v++)
		{
			cur_soln[v] = init_solution[v];
			if (cur_soln[v] == 2)
				cur_soln[v] = rand() % 2;
			time_stamp[v] = 0;
			unsat_app_count[v] = 0;
		}
	}

	//init stacks
	hard_unsat_nb = 0;
	hardunsat_stack_fill_pointer = 0;
	softunsat_stack_fill_pointer = 0;
	unsatvar_stack_fill_pointer = 0;
	/* figure out sat_count, sat_var, soft_unsat_weight and init unsat_stack */
	soft_unsat_weight = 0;

	for (c = 0; c < num_clauses; ++c)
	{
		//cout<<"top weight:"<<top_clause_weight<<endl;
		sat_count[c] = 0;
		for (j = 0; j < clause_lit_count[c]; ++j)
		{

			if (cur_soln[clause_lit[c][j].var_num] == clause_lit[c][j].sense)
			{
				sat_count[c]++;
				sat_var[c] = clause_lit[c][j].var_num;
			}
		}
		//if(org_clause_weight[c] < top_clause_weight){
			//cout<<clause_lit[c][0].sense<<endl;
		//}
		if (sat_count[c] < clause_true_lit_thres[c])
		{
			if (org_clause_weight[c] != top_clause_weight){
				soft_unsat_weight += (clause_true_lit_thres[c] - sat_count[c]) * org_unit_weight[c];
				//cout<<"update cost:"<<soft_unsat_weight<<endl;
			}
				
			unsat(c);
		}
	}

	/*figure out score*/
	int sense;
	for (v = 1; v <= num_vars; v++)
	{
		score[v] = 0;
		for (int i = 0; i < var_lit_count[v]; ++i)
		{
			c = var_lit[v][i].clause_num;
			sense = var_lit[v][i].sense;
			if (sat_count[c] < clause_true_lit_thres[c])
			{
				if (sense != cur_soln[v])
				{
					score[v] += unit_weight[c];
				}
				else
					score[v] -= unit_weight[c];
			}
			else if (sat_count[c] == clause_true_lit_thres[c])
			{
				if (sense == cur_soln[v])
				{
					score[v] -= unit_weight[c];
				}
			}
		}
	}

	//init goodvars stack
	goodvar_stack_fill_pointer = 0;
	for (v = 1; v <= num_vars; v++)
	{
		if (score[v] > 0)
		{
			already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
			mypush(v, goodvar_stack);
		}
		else
			already_in_goodvar_stack[v] = -1;
	}
}

void Satlike::init1(vector<int> &init_solution)
{
	int v, c;
	int j;

	local_soln_feasible = 0;
	local_opt_unsat_weight = top_clause_weight + 1; //???
	large_weight_clauses_count = 0;
	soft_large_weight_clauses_count = 0;

	//Initialize clause information
	for (c = 0; c < num_clauses; c++)
	{
		already_in_soft_large_weight_stack[c] = 0;
		clause_selected_count[c] = 0;

		if (org_clause_weight[c] == top_clause_weight)
		{
			clause_weight[c] = clause_true_lit_thres[c];
			org_unit_weight[c] = ceil((double)clause_weight[c] / (double)clause_true_lit_thres[c]);
			unit_weight[c] = org_unit_weight[c];
		}
		else
		{
			clause_weight[c] = org_clause_weight[c];
			org_unit_weight[c] = ceil((double)clause_weight[c] / (double)clause_true_lit_thres[c]);
			unit_weight[c] = org_unit_weight[c];

			if (clause_weight[c] > clause_true_lit_thres[c] && already_in_soft_large_weight_stack[c] == 0)
			{
				already_in_soft_large_weight_stack[c] = 1;
				soft_large_weight_clauses[soft_large_weight_clauses_count++] = c;
			}
		}
	}

	for (int i = 0; i < num_clauses; ++i)
	{
		if (org_unit_weight[i] != 1) //???
		{
			cout << "something si wrong " << endl;
		}
	}

	//init solution
	if (init_solution.size() == 0)
	{
		for (v = 1; v <= num_vars; v++)
		{
			cur_soln[v] = 0;
			time_stamp[v] = 0;
			unsat_app_count[v] = 0;
		}
	}
	else
	{
		for (v = 1; v <= num_vars; v++)
		{
			cur_soln[v] = init_solution[v];
			if (cur_soln[v] == 2)
				cur_soln[v] = 0;
			time_stamp[v] = 0;
			unsat_app_count[v] = 0;
		}
	}

	//init stacks
	hard_unsat_nb = 0;
	hardunsat_stack_fill_pointer = 0;
	softunsat_stack_fill_pointer = 0;
	unsatvar_stack_fill_pointer = 0;
	/* figure out sat_count, sat_var, soft_unsat_weight and init unsat_stack */
	soft_unsat_weight = 0;

	for (c = 0; c < num_clauses; ++c)
	{
		//cout<<"top weight:"<<top_clause_weight<<endl;
		sat_count[c] = 0;
		for (j = 0; j < clause_lit_count[c]; ++j)
		{

			if (cur_soln[clause_lit[c][j].var_num] == clause_lit[c][j].sense)
			{
				sat_count[c]++;
				sat_var[c] = clause_lit[c][j].var_num;
			}
		}
		//if(org_clause_weight[c] < top_clause_weight){
			//cout<<clause_lit[c][0].sense<<endl;
		//}
		if (sat_count[c] < clause_true_lit_thres[c])
		{
			if (org_clause_weight[c] != top_clause_weight){
				soft_unsat_weight += (clause_true_lit_thres[c] - sat_count[c]) * org_unit_weight[c];
				//cout<<"update cost:"<<soft_unsat_weight<<endl;
			}
				
			unsat(c);
		}
	}

	/*figure out score*/
	int sense;
	for (v = 1; v <= num_vars; v++)
	{
		score[v] = 0;
		for (int i = 0; i < var_lit_count[v]; ++i)
		{
			c = var_lit[v][i].clause_num;
			sense = var_lit[v][i].sense;
			if (sat_count[c] < clause_true_lit_thres[c])
			{
				if (sense != cur_soln[v])
				{
					score[v] += unit_weight[c];
				}
				else
					score[v] -= unit_weight[c];
			}
			else if (sat_count[c] == clause_true_lit_thres[c])
			{
				if (sense == cur_soln[v])
				{
					score[v] -= unit_weight[c];
				}
			}
		}
	}

	//init goodvars stack
	goodvar_stack_fill_pointer = 0;
	for (v = 1; v <= num_vars; v++)
	{
		if (score[v] > 0)
		{
			already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
			mypush(v, goodvar_stack);
		}
		else
			already_in_goodvar_stack[v] = -1;
	}
}

int Satlike::pick_var()
{
	int i, v;
	int best_var;

	if (goodvar_stack_fill_pointer > 0)
	{
		if ((rand() % MY_RAND_MAX_INT) * BASIC_SCALE < rdprob) //random
			return goodvar_stack[rand() % goodvar_stack_fill_pointer];

		if (goodvar_stack_fill_pointer < hd_count_threshold) //greedy
		{
			best_var = goodvar_stack[0];
			for (i = 1; i < goodvar_stack_fill_pointer; ++i)
			{
				v = goodvar_stack[i];
				if (score[v] > score[best_var])
					best_var = v;
				else if (score[v] == score[best_var])
				{
					if (time_stamp[v] < time_stamp[best_var])
						best_var = v;
				}
			}
			return best_var;
		}
		else //bms
		{

			int r = rand() % goodvar_stack_fill_pointer;
			best_var = goodvar_stack[r];

			for (i = 1; i < hd_count_threshold; ++i)
			{
				r = rand() % goodvar_stack_fill_pointer;
				v = goodvar_stack[r];
				if (score[v] > score[best_var])
					best_var = v;
				else if (score[v] == score[best_var])
				{
					if (time_stamp[v] < time_stamp[best_var])
						best_var = v;
				}
			}
			return best_var;
		}
	}
	update_clause_weights();
	int sel_c;
	lit *p;

	if (hardunsat_stack_fill_pointer > 0)
	{
		sel_c = hardunsat_stack[rand() % hardunsat_stack_fill_pointer];
	}
	else
	{
		sel_c = softunsat_stack[rand() % softunsat_stack_fill_pointer];
	}
	//randomly choose a variable in sel_c (rwprob%)
	if ((rand() % MY_RAND_MAX_INT) * BASIC_SCALE < rwprob)
		return clause_lit[sel_c][rand() % clause_lit_count[sel_c]].var_num;
	
	//choose a best variable in sel_c (1-rwprob %)
	best_var = clause_lit[sel_c][0].var_num;
	p = clause_lit[sel_c];
	for (p++; (v = p->var_num) != 0; p++)
	{
		if (score[v] > score[best_var])
			best_var = v;
		else if (score[v] == score[best_var])
		{
			if (time_stamp[v] < time_stamp[best_var])
				best_var = v;
		}
	}
	return best_var;
}

void Satlike::update_goodvarstack1(int flipvar)
{
	int v;

	//remove the vars no longer goodvar in goodvar stack
	for (int index = goodvar_stack_fill_pointer - 1; index >= 0; index--)
	{
		v = goodvar_stack[index];
		if (score[v] <= 0)
		{
			int top_v = mypop(goodvar_stack);
			goodvar_stack[index] = top_v;
			already_in_goodvar_stack[top_v] = index;
			already_in_goodvar_stack[v] = -1;
		}
	}

	//add goodvar
	for (int i = 0; i < var_neighbor_count[flipvar]; ++i)
	{
		v = var_neighbor[flipvar][i];
		if (score[v] > 0)
		{
			if (already_in_goodvar_stack[v] == -1)
			{
				already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
				mypush(v, goodvar_stack);
			}
		}
	}
}
void Satlike::update_goodvarstack2(int flipvar)
{
	if (score[flipvar] > 0 && already_in_goodvar_stack[flipvar] == -1)
	{
		already_in_goodvar_stack[flipvar] = goodvar_stack_fill_pointer;
		mypush(flipvar, goodvar_stack);
	}
	else if (score[flipvar] <= 0 && already_in_goodvar_stack[flipvar] != -1)
	{
		int index = already_in_goodvar_stack[flipvar];
		int last_v = mypop(goodvar_stack);
		goodvar_stack[index] = last_v;
		already_in_goodvar_stack[last_v] = index;
		already_in_goodvar_stack[flipvar] = -1;
	}
	int i, v;
	for (i = 0; i < var_neighbor_count[flipvar]; ++i)
	{
		v = var_neighbor[flipvar][i];
		if (score[v] > 0)
		{
			if (already_in_goodvar_stack[v] == -1)
			{
				already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
				mypush(v, goodvar_stack);
			}
		}
		else if (already_in_goodvar_stack[v] != -1)
		{
			int index = already_in_goodvar_stack[v];
			int last_v = mypop(goodvar_stack);
			goodvar_stack[index] = last_v;
			already_in_goodvar_stack[last_v] = index;
			already_in_goodvar_stack[v] = -1;
		}
	}
}

void Satlike::flip(int flipvar)
{
	int i, v, c;
	int index;
	lit *clause_c;

	int org_flipvar_score = score[flipvar];
	cur_soln[flipvar] = 1 - cur_soln[flipvar];

	for (i = 0; i < var_lit_count[flipvar]; ++i)
	{
		c = var_lit[flipvar][i].clause_num;
		clause_c = clause_lit[c];

		if (cur_soln[flipvar] == var_lit[flipvar][i].sense)
		{
			++sat_count[c];
			if (org_clause_weight[c] != top_clause_weight && sat_count[c] <= clause_true_lit_thres[c])
			{
				soft_unsat_weight -= org_unit_weight[c];
			}
			if (sat_count[c] == clause_true_lit_thres[c])
			{
				for (lit *p = clause_c; (v = p->var_num) != 0; p++)
				{
					if (p->sense != cur_soln[v])
					{
						score[v] -= unit_weight[c];
					}
				}
				sat(c);
			}
			else if (sat_count[c] == clause_true_lit_thres[c] + 1)
			{
				for (lit *p = clause_c; (v = p->var_num) != 0; p++)
				{
					if (p->sense == cur_soln[v])
					{
						score[v] += unit_weight[c];
					}
				}
			}
		}
		else // cur_soln[flipvar] != cur_lit.sense
		{
			--sat_count[c];
			if (org_clause_weight[c] != top_clause_weight && sat_count[c] < clause_true_lit_thres[c])
			{
				soft_unsat_weight += org_unit_weight[c];
			}
			if (sat_count[c] == clause_true_lit_thres[c])
			{
				for (lit *p = clause_c; (v = p->var_num) != 0; p++)
				{
					if (p->sense == cur_soln[v])
					{
						score[v] -= unit_weight[c];
					}
				}
			}
			else if (sat_count[c] == clause_true_lit_thres[c] - 1)
			{
				for (lit *p = clause_c; (v = p->var_num) != 0; p++)
				{
					if (p->sense != cur_soln[v])
					{
						score[v] += unit_weight[c];
					}
				}
				unsat(c);
			}
		} //end else
	}

	//update information of flipvar
	score[flipvar] = -org_flipvar_score;
	update_goodvarstack1(flipvar);
}

void Satlike::local_search(vector<int> &init_solution)
{
	settings();
	max_flips = 200000000;
	init(init_solution);
	cout << "time is " << get_runtime() << endl;
	cout << "hard unsat nb is " << hard_unsat_nb << endl;
	cout << "soft unsat nb is " << soft_unsat_weight << endl;
	cout << "goodvar nb is " << goodvar_stack_fill_pointer << endl;
}

void Satlike::print_best_solution()
{
	if(best_soln_feasible==1)
	{
	if (verify_sol())
		cout << opt_unsat_weight << '\t' << opt_time << '\t' << tries << '\t' << step << endl;
	else
		cout << "verify solion wrong " << endl;
	}
	else cout<<"fail to find feasible solution!"<<endl;
	
	ofstream ofile("E:/Study/FinalProject/ApDeployment/ApDeploymentByScp/data/solution.res");
	//ofile << num_vars << " ";
	int cnt = 0;
	for (int i = 1; i <= num_vars; i++)
	{
		//ofile << best_soln[i] << " ";
		if(best_soln[i] == 1){
			cnt++;
			ofile << i-1 << " ";
		}
	}
	//ofile << endl;
	//ofile<<cnt<<endl;
}

void Satlike::local_search_with_decimation(vector<int> &init_solution, char *inputfile)
{
	settings();

	Decimation deci(var_lit, var_lit_count, clause_lit, org_clause_weight, top_clause_weight);
	deci.make_space(num_clauses, num_vars);

	for (int i = 1; i <= num_vars; ++i)
	{
		local_opt_soln[i] = rand() % 2; //???
	}

	for (tries = 1; tries < max_tries; ++tries)
	{
		deci.init(local_opt_soln, best_soln, unit_clause, unit_clause_count, clause_lit_count, clause_true_lit_thres);

		deci.unit_prosess();
		cout<<"unit process"<<endl;

		init_solution.resize(num_vars + 1);
		
		for (int i = 1; i <= num_vars; ++i)
		{
			init_solution[i] = deci.fix[i];
		}

		init(init_solution);
		cout<<"cost after init:"<<soft_unsat_weight<<endl;
		
		for (step = 1; step < max_flips; ++step)
		{
			if (hard_unsat_nb == 0 && (soft_unsat_weight < local_opt_unsat_weight || local_soln_feasible == 0))
			{
				//if (soft_unsat_weight < top_clause_weight) //???
				//{
					local_soln_feasible = 1;
					local_opt_unsat_weight = soft_unsat_weight;
					max_flips = step + max_non_improve_flip;
					for (int v = 1; v <= num_vars; ++v)
						local_opt_soln[v] = cur_soln[v];
				//}
			}
			if (hard_unsat_nb == 0 && (soft_unsat_weight < opt_unsat_weight || best_soln_feasible == 0))
			{
				cout<<"update best solution"<<endl;
				//if (soft_unsat_weight < top_clause_weight) 
				//{
					best_soln_feasible = 1;
					opt_unsat_weight = soft_unsat_weight;
					opt_time = get_runtime();
					for (int v = 1; v <= num_vars; ++v)
						best_soln[v] = cur_soln[v];

					if (opt_unsat_weight == 0)
					{
						return;
					}
				//}
			}
			if (step % 1000 == 0)
			{
				double elapse_time = get_runtime();

				if (elapse_time >= cutoff_time)
				{
					deci.free_memory();
					return;
				}
			}
			int flipvar = pick_var();
			//cout<<"flip:"<<flipvar<<" from "<<cur_soln[flipvar]<<endl;
			flip(flipvar);
			time_stamp[flipvar] = step;
		}
	}
}

void Satlike::check_softunsat_weight()
{
	int c, j, flag;
	long long verify_unsat_weight = 0;

	for (c = 0; c < num_clauses; ++c)
	{
		flag = 0;
		int tem_clause_true_lit_count = 0;
		for (j = 0; j < clause_lit_count[c]; ++j)
		{
			if (cur_soln[clause_lit[c][j].var_num] == clause_lit[c][j].sense)
			{
				tem_clause_true_lit_count++;
			}
		}
		if (tem_clause_true_lit_count >= clause_true_lit_thres[c])
			flag = 1;

		if (flag == 0)
		{
			if (org_clause_weight[c] == top_clause_weight) //verify hard clauses
			{
				continue;
			}
			else
			{
				verify_unsat_weight += org_unit_weight[c] * (clause_true_lit_thres[c] - tem_clause_true_lit_count);
			}
		}
	}

	if (verify_unsat_weight != soft_unsat_weight)
	{
		cout << step << endl;
		cout << "verify unsat weight is" << verify_unsat_weight << " and soft unsat weight is " << soft_unsat_weight << endl;
	}
	//return 0;
}

void Satlike::constructSolnManually(){
	istringstream iss;
	string line;
	ifstream infile("testres");
	if (!infile)
	{
		cout << "c the input filename is invalid, please input the correct filename." << endl;
		exit(-1);
	}
	getline(infile, line);
	iss.str(line);
	iss.seekg(0, ios::beg);
	vector<int> nums;
	int num;
	iss>>num;
	while(num!=-1){
		nums.push_back(num);
		iss>>num;
	}
	cout<<nums.size()<<endl;
	for(int num1:nums) cout<<num1<<' ';
	for(int num1:nums) best_soln[num1+1] = 1;	
}

bool Satlike::verify_sol()
{
	int c, j, flag;
	long long verify_unsat_weight = 0;

	for (c = 0; c < num_clauses; ++c)
	{
		flag = 0;
		int tem_clause_true_lit_count = 0;
		for (j = 0; j < clause_lit_count[c]; ++j)
		{
			if (best_soln[clause_lit[c][j].var_num] == clause_lit[c][j].sense)
			{
				tem_clause_true_lit_count++;
			}
		}
		if (tem_clause_true_lit_count >= clause_true_lit_thres[c])
			flag = 1;

		if (flag == 0)
		{
			if (org_clause_weight[c] == top_clause_weight) //verify hard clauses
			{
				//output the clause unsatisfied by the solution
				cout << "c Error: hard clause " << c << " is not satisfied" << endl;

				cout << "c ";
				for (j = 0; j < clause_lit_count[c]; ++j)
				{
					if (clause_lit[c][j].sense == 0)
						cout << "-";
					cout << clause_lit[c][j].var_num << " ";
				}
				cout << endl;
				cout << "c ";
				for (j = 0; j < clause_lit_count[c]; ++j)
					cout << best_soln[clause_lit[c][j].var_num] << " ";
				cout << endl;
				return 0;
			}
			else
			{
				verify_unsat_weight += org_unit_weight[c] * (clause_true_lit_thres[c] - tem_clause_true_lit_count);
				/*
				cout << "c wanning: soft clause " << c << " is not satisfied" << endl;

				cout << "c org clause weight " << org_clause_weight[c] << " " << endl;

				for (j = 0; j < clause_lit_count[c]; ++j)
				{
					if (clause_lit[c][j].sense == 0)
						cout << "-";
					cout << clause_lit[c][j].var_num << " ";
				}
				cout << endl;
				//cout << "c ";
				for (j = 0; j < clause_lit_count[c]; ++j)
					cout << best_soln[clause_lit[c][j].var_num] << " ";
				cout << endl;*/
				//return 0;
			}
		}
	}

	if (verify_unsat_weight == opt_unsat_weight)
		return 1;
	else
	{
		cout << "c Error: find opt=" << opt_unsat_weight << ", but verified opt=" << verify_unsat_weight << endl;
	}
	return 0;
}

void Satlike::simple_print()
{
	if (best_soln_feasible == 1)
	{
		if (verify_sol() == 1)
			cout << opt_unsat_weight << '\t' << opt_time << endl;
		else
			cout << "solution is wrong " << endl;
	}
	else
		cout << -1 << '\t' << -1 << endl;
}

void Satlike::increase_weights()
{
	int i, c, v;
	for (i = 0; i < hardunsat_stack_fill_pointer; ++i)
	{
		c = hardunsat_stack[i];
		clause_weight[c] += (h_inc * clause_true_lit_thres[c]);

		unit_weight[c] += h_inc;

		if (clause_weight[c] == (h_inc + 1))
			large_weight_clauses[large_weight_clauses_count++] = c;

		for (lit *p = clause_lit[c]; (v = p->var_num) != 0; p++)
		{
			if (p->sense != cur_soln[v])
			{
				score[v] += h_inc;
				if (score[v] > 0 && already_in_goodvar_stack[v] == -1)
				{
					already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
					mypush(v, goodvar_stack);
				}
			}
			else
			{
				score[v] -= h_inc;
				if (already_in_goodvar_stack[v] != -1 && score[v] < 0)
				{
					int top_v = mypop(goodvar_stack);
					goodvar_stack[already_in_goodvar_stack[v]] = top_v;
					already_in_goodvar_stack[top_v] = already_in_goodvar_stack[v];
					already_in_goodvar_stack[v] = -1;
				}
			}
		}
	}
	for (i = 0; i < softunsat_stack_fill_pointer; ++i)
	{
		c = softunsat_stack[i];

		if (clause_weight[c] > softclause_weight_threshold * clause_true_lit_thres[c])
		{
			continue;
		}
		else
		{
			clause_weight[c] += clause_true_lit_thres[c];
			unit_weight[c] += 1;
		}

		if (clause_weight[c] > 1 && already_in_soft_large_weight_stack[c] == 0)
		{
			already_in_soft_large_weight_stack[c] = 1;
			soft_large_weight_clauses[soft_large_weight_clauses_count++] = c;
		}

		for (lit *p = clause_lit[c]; (v = p->var_num) != 0; p++)
		{
			if (p->sense != cur_soln[v])
			{
				score[v] += 1;
				if (score[v] > 0 && already_in_goodvar_stack[v] == -1)
				{
					already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
					mypush(v, goodvar_stack);
				}
			}
			else
			{
				score[v] -= 1;
				if (already_in_goodvar_stack[v] != -1 && score[v] < 0)
				{
					int top_v = mypop(goodvar_stack);
					goodvar_stack[already_in_goodvar_stack[v]] = top_v;
					already_in_goodvar_stack[top_v] = already_in_goodvar_stack[v];
					already_in_goodvar_stack[v] = -1;
				}
			}
		}
	}
}

void Satlike::smooth_weights()
{
	int i, clause, v;

	for (i = 0; i < large_weight_clauses_count; i++)
	{
		clause = large_weight_clauses[i];
		if (sat_count[clause] >= clause_true_lit_thres[clause])
		{
			clause_weight[clause] -= h_inc * clause_true_lit_thres[clause]; //kkk
			unit_weight[clause] -= h_inc;

			if (clause_weight[clause] == 1)
			{
				large_weight_clauses[i] = large_weight_clauses[--large_weight_clauses_count];
				i--;
			}
			//update score
			if (sat_count[clause] == clause_true_lit_thres[clause])
			{
				for (int j = 0; j < clause_lit_count[clause]; ++j)
				{
					v = clause_lit[clause][j].var_num;
					if (cur_soln[v] == clause_lit[clause][j].sense)
					{
						score[v] += h_inc;
						if (score[v] > 0 && already_in_goodvar_stack[v] == -1)
						{
							already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
							mypush(v, goodvar_stack);
						}
					}
				}
			}
		}
	}

	for (i = 0; i < soft_large_weight_clauses_count; i++)
	{
		clause = soft_large_weight_clauses[i];
		if (sat_count[clause] > clause_true_lit_thres[clause]) //kkk should be >= ?
		{
			clause_weight[clause] -= clause_true_lit_thres[clause];
			unit_weight[clause]--;
			if (clause_weight[clause] == org_clause_weight[clause] && already_in_soft_large_weight_stack[clause] == 1)
			{
				already_in_soft_large_weight_stack[clause] = 0;
				soft_large_weight_clauses[i] = soft_large_weight_clauses[--soft_large_weight_clauses_count];
				i--;
			}
			if (sat_count[clause] == clause_true_lit_thres[clause])
			{
				for (int j = 0; j < clause_lit_count[clause]; ++j)
				{
					v = clause_lit[clause][j].var_num;
					if (cur_soln[v] == clause_lit[clause][j].sense)
					{
						score[v]++;
						if (score[v] > 0 && already_in_goodvar_stack[v] == -1)
						{
							already_in_goodvar_stack[v] = goodvar_stack_fill_pointer;
							mypush(v, goodvar_stack);
						}
					}
				}
			}
		}
	}
}

void Satlike::update_clause_weights()
{

	if (((rand() % MY_RAND_MAX_INT) * BASIC_SCALE) < smooth_probability)
	{
		smooth_weights();
	}
	else
	{
		increase_weights();
	}
}

inline void Satlike::unsat(int clause)
{
	if (org_clause_weight[clause] == top_clause_weight)
	{
		index_in_hardunsat_stack[clause] = hardunsat_stack_fill_pointer;
		mypush(clause, hardunsat_stack);
		hard_unsat_nb++;
	}
	else
	{
		index_in_softunsat_stack[clause] = softunsat_stack_fill_pointer;
		mypush(clause, softunsat_stack);
		//soft_unsat_weight += org_clause_weight[clause];
	}
}

inline void Satlike::sat(int clause)
{
	int index, last_unsat_clause;

	if (org_clause_weight[clause] == top_clause_weight)
	{

		last_unsat_clause = mypop(hardunsat_stack);
		index = index_in_hardunsat_stack[clause];
		hardunsat_stack[index] = last_unsat_clause;
		index_in_hardunsat_stack[last_unsat_clause] = index;

		hard_unsat_nb--;
	}
	else
	{
		last_unsat_clause = mypop(softunsat_stack);
		index = index_in_softunsat_stack[clause];
		softunsat_stack[index] = last_unsat_clause;
		index_in_softunsat_stack[last_unsat_clause] = index;

		//soft_unsat_weight -= org_clause_weight[clause];
	}
}

void Satlike::check_new_score()
{
	long long tem_score = 0;
	int sense, c, v, i;

	for (v = 1; v <= num_vars; v++)
	{
		tem_score = 0;
		for (i = 0; i < var_lit_count[v]; ++i)
		{
			c = var_lit[v][i].clause_num;
			sense = var_lit[v][i].sense;
			if (sat_count[c] < clause_true_lit_thres[c])
			{
				if (sense != cur_soln[v])
				{
					tem_score += unit_weight[c];
				}
				else
					tem_score -= unit_weight[c];
			}
			else if (sat_count[c] == clause_true_lit_thres[c])
			{
				if (sense == cur_soln[v])
				{
					tem_score -= unit_weight[c];
				}
			}
		}
		if (tem_score != score[v])
		{
			cout << "score is worng in variable " << v << endl;
			cout << "tem_score is " << tem_score << endl;
			cout << "score function is " << score[v] << endl;
			//cout << "flip num is " << step << endl;

			break;
		}
	}
}

#endif
