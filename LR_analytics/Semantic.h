#pragma once
#include <iostream>
#include <string>
#include "Lex.h"
#include"Production.h"
using namespace std;

#define FUNC  0 //函数	
#define VAR   1 //变量
#define ARRAY 2 //数组

#define TEMP  3	//临时变量
#define CONST 4	//常量
#define RET   5 //返回值

#define FUNVOID 6 //类型void
#define FUNINT 7  //类型int

typedef int type;
typedef int kind;

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

struct IdentNode
{
	int level = 0;
	IdentNode* parent = NULL;
	vector<IdentNode*> children;
	pair<string, int> content;
	int type = FUNINT;
	int kind =  VAR;
	int n = 0;
	int width = 0;
	vector<int> dimension;
	vector<string> params;
	string place;
	int quad;
	int true_list;
	int false_list;
	int x = -1; //供绘图使用
	int y = -1; //供绘图使用
	IdentNode() { clear(); }
	void clear()
	{
		level = -1;
		parent = NULL;
		children.clear();
		content = pair<string, int>("", -1);
	}
};

struct symbolTableItem
{
	int id;
	type t;
	kind k;
	int offset;
	vector<int> dimension;
	symbolTable* proctable = NULL;
};

struct symbolTable
{
	//双链表
	symbolTable* previous = NULL;
	symbolTable* next = NULL;
	//树形
	symbolTable* parent = NULL;
	int width = 0;
	vector<symbolTableItem> table;
	void clear()
	{
		previous = NULL;
		next = NULL;
		width = 0;
		table.clear();
	}
	void enter(int id, type t, kind k, int offset)
	{
		symbolTableItem e;
		e.id = id;
		e.t = t;
		e.k = k;
		e.offset = offset;
		for (auto i = 0; i < table.size(); i++)
		{
			if (table[i].id == id)
			{
				throw string("ERROR: 语义分析符号表错误:") + to_string(id) + string("重定义\n");
			}
		}
		table.push_back(e);
	}
	void enterdimension(int id, vector<int> dimension)
	{
		for (auto i = 0; i < table.size(); i++)
		{
			if (table[i].id == id && ((table[i].k == ARRAY) || (table[i].k == FUNC)))
			{
				table[i].dimension = dimension;
				break;
			}
		}
	}
	void enterproc(int id, symbolTable* newtable)
	{
		for (auto i = 0; i < table.size(); i++)
		{
			if (table[i].id == id && table[i].k == FUNC)
			{
				table[i].proctable = newtable;
			}
		}
	}
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
	vector<symbolTable> Symtable; //符号表
	SemanticSymtable(const int t_type, const string& t_name);
	int findSym(const string& id_name);	//查找符号,返回index
	int insertSym(const symbolTable& id); //插入符号,返回index
};

class SemanticAnalyzer
{
	/*语义分析类*/
public:
	vector<Quadruple> Quadruple_Code;//四元式表
	int main_index;//main函数对应的四元式标号
	int backpatching_level;//回填层次
	vector<int> backpatching_list;//回填列表
	int nextQ_index;//下一个四元式标号
	int cnt;//临时变量计数
	
	vector<SemanticSymtable> allTable;//程序所有符号表 
	vector<SemanticSymbol> symbolList;//语义分析过程的符号流
	
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
	//<Pro_m> ->[z]
	void SemanProd_Pro_m(const Production& pro);
	//<DeclareString> -> <DeclareString - option> | <DeclareString - option> <DeclareString>
	void SemanProd_DeclareString(const Production& pro);
	//<DeclareString-option> -> <ParameterDeclare> [ID] [SEMI] | <FunctionDeclare> <FunDec> <Block>
	void SemanProd_DecOption(const Production& pro);
	//<ParameterContent> -> [LeftSquareBracket] [NUM] [RightSquareBracket] <ArrayDeclare> | [z
	void SemanProd_ParaContent(const Production& pro);
	//<ArrayDeclare> -> [LeftSquareBracket] [NUM] [RightSquareBracket] <ArrayDeclare> | [z]
	void SemanProd_ArrayDeclare(const Production& pro);
	/*<FunctionDeclare> ->[VOID] | [INT]*/
	void SemanProd_FunctionDeclare(const Production& pro);
	//<FunDec> ->[ID] <CreateFunTable_m>[LPAREN] <VarList>[RPAREN
	void SemanProd_FunDec(const Production& pro);
	//<CreateFunTable_m> ->[z
	void SemanProd_CreateFunTable_m(const Production& pro);
	//<VarContent> -> <VarList> | [VOID] | [z]
	void SemanProd_VarContent(const Production& pro);
	//<VarList> -> <ParamDec> [COMMA] <VarList> | <ParamDec> 
	void SemanProd_VarList(const Production& pro);
	//<ParamDec> -> [INT] [ID
	void SemanProd_ParamDec(const Production& pro);
	//<Block> ->[LBBRACKET] <DefList> <StmtList>[RBBRACKET
	void SemanProd_Block(const Production& pro);
	//<DefList> -> <Def> [SEMI] <DefList> | [z]
	void SemanProd_DefList(const Production& pro);
	//<Def> ->[INT][ID] <ParameterContent>
	void SemanProd_Def(const Production& pro);
	//<StmtList> -> <Stmt> <StmtList> | <Stmt> 
	void SemanProd_StmtList(const Production& pro);
	//<Stmt> -> <AssignStmt>[SEMI] | <ReturnStmt>[SEMI] | <IfStmt> | <WhileStmt>
	void SemanProd_Stmt(const Production& pro);
	//<AssignStmt> ->[ID][ASSIGN] <Exp> | <Array>[ASSIGN] <Exp>
	void SemanProd_AssignStmt(const Production& pro);
	//<Exp> -> <ANDExp> | <ANDExp> [OR] <Exp>
	void SemanProd_Exp(const Production& pro);
	//<ANDExp> -> <NOTExp> | <NOTExp>[AND] <ANDExp>
	void SemanProd_ANDExp(const Production& pro);
	//<NOTExp> ->[NOT] <SubExp> | <SubExp>
	void SemanProd_NOTExp(const Production& pro);
	//<SubExp> -> <OPNUM> | <OPNUM> <Relop> <SubExp> 
	void SemanProd_SubExp(const Production& pro);
	//<Relop> ->[GT] | [LT] | [GE] | [LE] | [EQ] | [NQ]
	void SemanProd_Relop(const Production& pro);
	//<OPNUM> -> <Item> | <Item> <OPERAT1> <OPNUM>
	void SemanProd_OPNUM(const Production& pro);
	//<Item> -> <Factor> | <Factor> <OPERAT2> <Item>
	void SemanProd_Item(const Production& pro);
	//<OPERAT1> ->[PLUS] | [SUB] | [BinaryAnd] | [BinaryOr] | [BinaryXor]
	void SemanProd_OPERAT1(const Production& pro);
	//<OPERAT2> -> [MUL] | [DIV] 
	void SemanProd_OPERAT2(const Production& pro);
	//<Factor> -> [NUM] | [LPAREN] <Exp> [RPAREN] | [ID] | [ID] <CallStmt> | <Array> | [LPAREN] <AssignStmt> [RPAREN] 
	void SemanProd_Factor(const Production& pro);
	//<Array> -> [ID] [LeftSquareBracket] <Exp> [RightSquareBracket] | <Array> [LeftSquareBracket] <Exp> [RightSquareBracket] 
	void SemanProd_Array(const Production& pro);
	//<CallStmt> -> [LPAREN] <CallFun> [RPAREN]
	void SemanProd_CallStmt(const Production& pro);
	//<CallFun> ->[z] | <Args>
	void SemanProd_CallFun(const Production& pro);
	//<Args> -> <Exp>[COMMA] <Args> | <Exp>
	void SemanProd_Args(const Production& pro);
	//<ReturnStmt> -> [RETURN] <Exp> | [RETURN]
	void SemanProd_ReturnStmt(const Production& pro);
	//<WhileStmt> -> <WhileStmt_m1> [WHILE] [LPAREN] <Exp> [RPAREN] <WhileStmt_m2> <Block>
	void SemanProd_WhileStmt(const Production& pro);
	//<WhileStmt_m1> ->[z]
	void SemanProd_WhileStmt_m1(const Production& pro);
	//<WhileStmt_m2> ->[z]
	void SemanProd_WhileStmt_m2(const Production& pro);
	//<IfStmt> -> [IF] [LPAREN] <Exp> [RPAREN] <IfStmt_m2> <Block> <IfNext>
	void SemanProd_IfStmt(const Production& pro);
	//<IfStmt_m> -> [z]
	void SemanProd_IfStmt_m1(const Production& pro);
	//<IfNext> -> [z] | [ELSE] <IfStmt_next> <Block>
	void SemanProd_IfNext(const Production& pro);
	//<IfStmt_next>-> [z]
	void SemanProd_IfStmt_next(const Production& pro);
	
};

