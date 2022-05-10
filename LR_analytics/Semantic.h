#pragma once
#include <iostream>
#include <string>
#include "Lex.h"
#include"Production.h"
using namespace std;



struct SemanticSymbol
{
	/*�����������*/
	TokSym toke; //int line ,string content,string type {lineΪtoken�е�Line��contentΪstring�����token�ľ������ݣ�����type��identifier��int ���ߺ�content������ͬ}
	int table_index;//��������table��index
	int index;//������table�ڲ���index
};

struct Quadruple
{
	/*��Ԫʽ*/
	int index;//������
	string op;//�����
	string arg1;
	string arg2;
	string res;
};

struct Identifier
{
	/*��ʶ����Ϣ*/
#define FUNC  0 //����	
#define VAR   1 //����
#define TEMP  2	//��ʱ����
#define CONST 3	//����
#define RET   4 //����ֵ

	int type; //����5������
	string specialType; //����--�������� ����--���� ����--����
	string name; //��ʶ������,����ǳ�����Ϊȡֵ
	int paraNum; //����--��������
	int entry; //����--��ڵ�ַ
	int tableIndex; //�������ű�����

};


class SemanticSymtable
{
	/*���ű�*/
#define GLOBAL 0
#define FUNC   1
#define BLOCK  2

public:
	int type; //����3������
	string name;//���ű�����
	vector<Identifier> Symtable; //���ű�
	SemanticSymtable(const int t_type, const string& t_name);
	int findSym(const string& id_name);	//���ҷ���,����index
	int insertSym(const Identifier& id); //�������,����index
};

class SemanticAnalyzer
{
	/*���������*/
public:
	vector<Quadruple> quadruple;//��Ԫʽ��
	int main_index;//main������Ӧ����Ԫʽ���
	int backpatching_level;//������
	vector<int> backpatching_list;//�����б�
	int nextQ_index;//��һ����Ԫʽ���
	int cnt;//��ʱ��������
	vector<SemanticSymbol> symbolList;//����������̵ķ�����
	vector<SemanticSymtable> allTable;//�������з��ű�
	vector<int> cur_tableStack;//��ǰ�������Ӧ�ķ��ű�����ջ
public:
	//����
	SemanticAnalyzer();
	//�������
	void insertSymbol(const SemanticSymbol& sym);//�����еķ�����Ϣ����symbolList
	//��Ԫʽ�����
	void printQuadruple(const string outpath);
	//�����������
	void semanticANL(const Production& pro);//�������Ϊ����ʽ������
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

