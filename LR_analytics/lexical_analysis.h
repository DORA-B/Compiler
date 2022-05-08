#pragma once
#ifndef _LEXICAL_ANALYSIS_H
#define _LEXICAL_ANALYSIS_H
#include"Symbol.h"
#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<map>
#include<vector>
#include<queue>
using namespace std;
enum class LexComponent :int {
	//default 0
	Undefined,
	//comment marks 1~3
	MultipleLineCommentLeft,
	MultipleLineCommentRight,
	SingleLineComment,
	//brackets 4~9
	LeftBrace,
	RightBrace,
	LeftSquareBracket,
	RightSquareBracket,
	LeftBracket,
	RightBracket,
	//binary operators 10~25
	Plus,
	Minus,
	Multiply,
	Divide,
	Modulus,
	Equal,
	NotEqual,
	Greater,
	Less,
	GreaterEqual,
	LessEqual,
	LogicalAnd,
	LogicalOr,
	BinaryAnd,
	BinaryOr,
	BinaryXor,
	//unary operators 26~29
	BinaryNot,
	LogicalNot,
	Increment,
	Decrement,
	//assignment 30~38
	Assignment,
	AddAssignment,
	SubtractAssignment,
	MultiplyAssignment,
	DivideAssignment,
	ModulusAssignment,
	BinaryAndAssignment,
	BinaryOrAssignment,
	BinaryXorAssignment,
	//special  39~46
	Dot,
	Comma,
	Semicolon,
	Colon,
	NumberSign,
	QuestionMark,
	LeftShift,
	RightShift,
	//keywords 47~79
	Auto,
	Break,
	Bool,
	Case,
	Char,
	Const,
	Continue,
	Default,
	Do,
	Double,
	Else,
	Enum,
	Extern,
	Float,
	For,
	Goto,
	If,
	Int,
	Long,
	Register,
	Return,
	Short,
	Signed,
	Sizeof,
	Static,
	Struct,
	Switch,
	Typedef,
	Union,
	Unsigned,
	Void,
	Volatile,
	While,
	//ID 79
	ID,
	//constants 81~85
	True,
	False,
	ConstString,//˫���ſ�ͷ��β���ַ�������
	ConstChar,//�����ſ�ͷ��β�ĵ����ַ����������ٵ���������˫����
	Digit
};
bool is_undefined(LexComponent i);
bool is_comment_mark(LexComponent i);
bool is_bracket(LexComponent i);
bool is_binary_operator(LexComponent i);
bool is_unary_operator(LexComponent i);
bool is_assignment(LexComponent i);
bool is_id(LexComponent i);
bool is_constant(LexComponent i);
extern const char* ComponentString[];
extern const char* ComponentName[];
extern map<string, LexComponent> LexMap;
extern const int symbolNum;
void init_map();

typedef struct {
	int line;
	string content;
	LexComponent type;
} Token;

class LexAnalyzer {
public:
	LexAnalyzer() { init(); }
	int resultNum;//�������vector��Ԫ������
	//Token get_rslt_elem(int index) { return index < LexResult.size() ? LexResult[index] : Token{ -1,"",LexComponent::Undefined }; }//��ȡ����е�index������
	void init() { resultNum = 0; curLine = 1; LexResult.clear(); }//����
	bool lex_analyze(string inFile);
	void output_result(string outFile);
	void getResult();//LexResult->Result
	int curLine;//��ǰɨ�������
	queue<Symbol>Result;//������﷨����
	vector<Token>LexResult;//�洢�ʷ��������
};

#endif