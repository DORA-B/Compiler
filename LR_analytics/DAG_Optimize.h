#pragma once
#ifndef DAG_OPTIMIZE
#define DAG_OPTIMIZE
#include"LR1.h"

struct blockItem
{
	int begin;
	int end;
	vector<string> wait_variable;   //待用
	vector<string> useless_variable;//无用
	vector<string> active_variable; //活跃
};

struct DAGitem
{
	bool useful = false;
	bool isleaf;
	string value;
	string op;
	vector<string> label;  //标识符
	int parent = -1;
	int left_child = -1;
	int right_child = -1;
	int tri_child = -1;
	bool isremain = false;
	Quadruple code;
	bool operator== (DAGitem item)
	{
		bool con1 = (this->isleaf == item.isleaf)&&(this->value == item.value)
			&& (this->op == item.op) && this->label.size() == item.label.size()
			&& (this->parent == item.parent) && (this->left_child == item.left_child)&& (this->right_child == item.right_child);
		bool con2 = true;
		for (int i = 0; i < this->label.size() && i < item.label.size(); i++)
		{
			if (this->label[i] != item.label[i])
			{
				con2 = false;
				break;
			}
		}
		return con1 && con2;
	}
};
//if (elem.find_first_not_of("0123456789") == string::npos)

class optimizerAnalysis
{
private:
	map<int, string> name_table;
	symbolTable* global_table;
	map<int, string> label_map;
	int temp_counter = 0;
	vector<vector<DAGitem>> DAG_group;
	vector<Quadruple> unoptimized_code;
	vector<blockItem> unoptimized_block;
	bool preOptimize();
	void partition();
	vector<DAGitem> geneDAG(int block_no);
	void _utilizeChildren(vector<DAGitem>& DAG, int now);
	string newtemp();
	void optimize();

public:
	vector<Quadruple> OptimiseQuadruple;
	vector<blockItem> block_group;
	optimizerAnalysis(map<int, string> nt, symbolTable* gt, vector<Quadruple> ic);
	void showIntermediateCode();
	void showBlockGroup();
	void showDAG();
	double analysis();
};
#endif