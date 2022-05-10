#pragma once
#include <iostream>
#include <string>
#include "Lex.h"
#include"Production.h"
using namespace std;



struct SemanticSymbol
{
	/*语义分析符号*/
	TokSym toke; //int line ,string content,string type {line为token中的Line，content为string，这个token的具体内容，最后的type是identifier或int 或者和content内容相同}
	int table_index;//符号所在table的index
	int index;//符号在table内部的index
};

struct Quadruple
{
	/*四元式*/
	int index;//索引号
	string op;//运算符
	string arg1;
	string arg2;
	string res;
};

struct Identifier
{
	/*标识符信息*/
#define FUNC  0 //函数	
#define VAR   1 //变量
#define TEMP  2	//临时变量
#define CONST 3	//常量
#define RET   4 //返回值

	int type; //上述5种类型
	string specialType; //函数--返回类型 常量--类型 变量--类型
	string name; //标识符名称,如果是常量则为取值
	int paraNum; //函数--参数个数
	int entry; //函数--入口地址
	int tableIndex; //函数符号表索引

};


class SemanticSymtable
{
	/*符号表*/
#define GLOBAL 0
#define FUNC   1
#define BLOCK  2

public:
	int type; //上述3种类型
	string name;//符号表名称
	vector<Identifier> Symtable; //符号表
	SemanticSymtable(const int t_type, const string& t_name);
	int findSym(const string& id_name);	//查找符号,返回index
	int insertSym(const Identifier& id); //插入符号,返回index
};

class SemanticAnalyzer
{
	/*语义分析类*/
public:
	vector<Quadruple> quadruple;//四元式表
	int main_index;//main函数对应的四元式标号
	int backpatching_level;//回填层次
	vector<int> backpatching_list;//回填列表
	int nextQ_index;//下一个四元式标号
	int cnt;//临时变量计数
	vector<SemanticSymbol> symbolList;//语义分析过程的符号流
	vector<SemanticSymtable> allTable;//程序所有符号表
	vector<int> cur_tableStack;//当前作用域对应的符号表索引栈
public:
	//构造
	SemanticAnalyzer();
	//插入符号
	void insertSymbol(const SemanticSymbol& sym);//将所有的符号信息放入symbolList
	//四元式表输出
	void printQuadruple(const string outpath);
	//语义分析主体
	void semanticANL(const Production& pro);//传入参数为产生式的两侧
private:
	void SemanProd_Program(const Production& pro);
	void SemanProd_DecOption(const Production& pro);
	void SemanProd_ParaContent(const Production& pro); //new add
	void SemanProd_ArrayDeclare(const Production& pro);//new add
	void SemanProd_ParaDec(const Production& pro); 
	void SemanProd_FunctionDeclare(const Production& pro);
	void SemanProd_FunDec(const Production& pro);
	void SemanProd_CreateFunTable_m(const Production& pro);
	void SemanProd_VarContent(const Production& pro); //new add
	void SemanProd_ParamDec(const Production& pro);
	void SemanProd_Block(const Production& pro);
	void SemanProd_Def(const Production& pro);
	void SemanProd_AssignStmt(const Production& pro);
	void SemanProd_Exp(const Production& pro);
	void SemanProd_AddSubExp(const Production& pro);
	void SemanProd_Item(const Production& pro);
	void SemanProd_Factor(const Production& pro);
	void SemanProd_Array(const Production& pro); //new add
	void SemanProd_CallStmt(const Production& pro);
	void SemanProd_CallFunCheck(const Production& pro);
	void SemanProd_Args(const Production& pro);
	void SemanProd_ReturnStmt(const Production& pro);
	void SemanProd_Relop(const Production& pro);
	void SemanProd_IfStmt(const Production& pro);
	void SemanProd_IfStmt_m1(const Production& pro);
	void SemanProd_IfStmt_m2(const Production& pro);
	void SemanProd_IfNext(const Production& pro);
	void SemanProd_IfStmt_next(const Production& pro);
	void SemanProd_WhileStmt(const Production& pro);
	void SemanProd_WhileStmt_m1(const Production& pro);
	void SemanProd_WhileStmt_m2(const Production& pro);
};

