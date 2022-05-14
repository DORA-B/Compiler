//#include "Semantic.h"
//
//SemanticAnalyzer::SemanticAnalyzer()
//{
//	temp_counter = -1;
//	symbolTable* last_table = NULL; //指向最后的表项
//	symbolTable* global_table = NULL;//程序所有符号表 
//}
//
//SemanticAnalyzer::~SemanticAnalyzer()
//{
//	symbolTable* tp = last_table;
//	symbolTable* ntp = NULL;
//	while (tp != NULL)
//	{
//		ntp = tp->previous;
//		free(tp);
//		tp = ntp;
//	}
//}
//
//void SemanticAnalyzer::printQuadruple(const string outpath)
//{
//	cout << "********Intermediate Code********\n";
//	for (auto i = 0; i < this->Quadruple_Code.size(); i++)
//	{
//		cout << "(" << i << ")\t" << Quadruple_Code[i].op << '\t' << Quadruple_Code[i].arg1 << '\t' << Quadruple_Code[i].arg2 << '\t' << Quadruple_Code[i].res << endl;
//	}
//}
//
//string SemanticAnalyzer::newtemp()
//{
//	string temp_name = string("V") + to_string(offset_stack.back());
//	table_stack.back()->enter(temp_counter--, INT, VAR, offset_stack.back());
//	offset_stack.back() += 4;
//
//	EMIT("+", "$sp", to_string(4), "$sp");
//	return temp_name;
//}
//
//string SemanticAnalyzer::lookup(int id)
//{
//	symbolTable* tp = table_stack.back();
//	int offset;
//	while (tp)
//	{
//		for (auto i = 0; i < tp->table.size(); i++)
//		{
//			if (tp->table[i].id == id)
//			{
//				offset = tp->table[i].offset;
//				if (tp->table[i].k == VAR || tp->table[i].k == ARRAY)
//				{
//					if (tp->parent)
//						return string("V") + to_string(offset);
//					else
//						return string("G") + to_string(offset);
//				}
//			}
//		}
//		tp = tp->parent;
//	}
//	return "";
//}
//
//symbolTableItem* SemanticAnalyzer::find(int id)
//{
//	symbolTable* tp = table_stack.back();
//	int offset = -1;
//	while (tp)
//	{
//		for (auto i = 0; i < tp->table.size(); i++)
//		{
//			if (tp->table[i].id == id)
//			{
//				return &(tp->table[i]);
//			}
//		}
//		tp = tp->parent;
//	}
//	return NULL;
//}
//
//int SemanticAnalyzer::nextstat()
//{
//	return Quadruple_Code.size();
//}
//
//void SemanticAnalyzer::EMIT(string op, string arg1, string arg2, string result)
//{
//	/**********
//	op can be: nop j jal break ret jnz j< j<= j> j>= j== j!=
//	:= []= =[] + - & | ^
//	**********/
//	Quadruple quad = { op,arg1,arg2,result };
//	Quadruple_Code.push_back(quad);
//}
///*分析主体函数*/
//void SemanticAnalyzer::seman_analysis(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	//<Program> -> <Pro_m> <DeclareString>
//	if (pro.Left.content == "<Program>")
//		SemanProd_Program(pro, root, nameTable);
//	//<Pro_m> ->[z]
//	else if (pro.Left.content == "<Pro_m>")
//		SemanProd_Pro_m(pro, root, nameTable);
//	//<DeclareString> -> <DeclareString - option> 
//	//				   | <DeclareString - option> <DeclareString>
//	else if (pro.Left.content == "<DeclareString>")
//		SemanProd_DeclareString(pro, root, nameTable);
//	//<DeclareString-option> -> <ParameterDeclare> [ID] [SEMI] 
//	//						  | <FunctionDeclare> <FunDec> <Block>
//	else if (pro.Left.content == "<DeclareString-option>")
//		SemanProd_DecOption(pro, root, nameTable);
//	//<ParameterContent> -> [LeftSquareBracket] [NUM] [RightSquareBracket] <ArrayDeclare> | [z]
//	else if (pro.Left.content == "<ParameterContent>")
//		SemanProd_ParaContent(pro, root, nameTable);
//	//<ArrayDeclare> -> [LeftSquareBracket] [NUM] [RightSquareBracket] <ArrayDeclare> | [z]
//	else if (pro.Left.content == "<ArrayDeclare>")
//		SemanProd_ArrayDeclare(pro, root, nameTable);
//	/*<FunctionDeclare> ->[VOID] | [INT]*/
//	else if (pro.Left.content == "<FunctionDeclare>")
//		SemanProd_FunctionDeclare(pro, root, nameTable);
//	/*<FunDec> ->[ID] <CreateFunTable_m>[LPAREN] <VarList>[RPAREN]*/
//	else if (pro.Left.content == "<FunDec>")
//		SemanProd_FunDec(pro, root, nameTable);
//	/*<CreateFunTable_m> ->[z]*/
//	else if (pro.Left.content == "<CreateFunTable_m>")
//		SemanProd_CreateFunTable_m(pro, root, nameTable);
//	//<VarContent> -> <VarList> | [VOID] | [z]
//	else if (pro.Left.content == "<VarContent>")
//		SemanProd_VarContent(pro, root, nameTable);
//	//<VarList> -> <ParamDec> [COMMA] <VarList> | <ParamDec> 
//	else if (pro.Left.content == "<VarList>")
//		SemanProd_VarList(pro, root, nameTable);
//	//<ParamDec> -> [INT] [ID]
//	else if (pro.Left.content == "<ParamDec>")
//		SemanProd_ParamDec(pro, root, nameTable);
//	//<Block> ->[LBBRACKET] <DefList> <StmtList>[RBBRACKET]
//	else if (pro.Left.content == "<Block>")
//		SemanProd_Block(pro, root, nameTable);
//	//<DefList> -> <Def> [SEMI] <DefList> | [z]
//	else if (pro.Left.content == "<DefList>")
//		SemanProd_DefList(pro, root, nameTable);
//	//<Def> ->[INT][ID] <ParameterContent>
//	else if (pro.Left.content == "<Def>")
//		SemanProd_Def(pro, root, nameTable);
//	//<StmtList> -> <Stmt> <StmtList> | <Stmt> 
//	else if (pro.Left.content == "<StmtList>")
//		SemanProd_StmtList(pro, root, nameTable);
//	//<Stmt> -> <AssignStmt>[SEMI] | <ReturnStmt>[SEMI] | <IfStmt> | <WhileStmt>
//	else if (pro.Left.content == "<Stmt>")
//		SemanProd_Stmt(pro, root, nameTable);
//	//<AssignStmt> ->[ID][ASSIGN] <Exp> | <Array>[ASSIGN] <Exp>
//	else if (pro.Left.content == "<AssignStmt>")
//		SemanProd_AssignStmt(pro, root, nameTable);
//	//<Exp> -> <ANDExp> | <ANDExp> [OR] <Exp>
//	else if (pro.Left.content == "<Exp>")
//		SemanProd_Exp(pro, root, nameTable);
//	//<ANDExp> -> <NOTExp> | <NOTExp>[AND] <ANDExp>
//	else if (pro.Left.content == "<ANDExp>")
//		SemanProd_ANDExp(pro, root, nameTable);
//	//<NOTExp> ->[NOT] <SubExp> | <SubExp>
//	else if (pro.Left.content == "<NOTExp>")
//		SemanProd_NOTExp(pro, root, nameTable);
//	//<SubExp> -> <OPNUM> | <OPNUM> <Relop> <SubExp> 
//	else if (pro.Left.content == "<SubExp>")
//		SemanProd_SubExp(pro, root, nameTable);
//	//<Relop> ->[GT] | [LT] | [GE] | [LE] | [EQ] | [NQ]
//	else if (pro.Left.content == "<Relop>")
//		SemanProd_Relop(pro, root, nameTable);
//	//<OPNUM> -> <Item> | <Item> <OPERAT1> <OPNUM>
//	else if (pro.Left.content == "<OPNUM>")
//		SemanProd_OPNUM(pro, root, nameTable);
//	//<Item> -> <Factor> | <Factor> <OPERAT2> <Item>
//	else if (pro.Left.content == "<Item>")
//		SemanProd_Item(pro, root, nameTable);
//	//<OPERAT1> ->[PLUS] | [SUB] | [BinaryAnd] | [BinaryOr] | [BinaryXor]
//	else if (pro.Left.content == "<OPERAT1>")
//		SemanProd_OPERAT1(pro, root, nameTable);
//	//<OPERAT2> -> [MUL] | [DIV] 
//	else if (pro.Left.content == "<OPERAT2>")
//		SemanProd_OPERAT2(pro, root, nameTable);
//	//<Factor> -> [NUM] | [LPAREN] <Exp> [RPAREN] | [ID] | [ID] <CallStmt> | <Array> | [LPAREN] <AssignStmt> [RPAREN] 
//	else if (pro.Left.content == "<Factor>")
//		SemanProd_Factor(pro, root, nameTable);
//	//<Array> -> [ID] [LeftSquareBracket] <Exp> [RightSquareBracket] | <Array> [LeftSquareBracket] <Exp> [RightSquareBracket] 
//	else if (pro.Left.content == "<Array>")
//		SemanProd_Array(pro, root, nameTable);
//	//<CallStmt> -> [LPAREN] <CallFun> [RPAREN]
//	else if (pro.Left.content == "<CallStmt>")
//		SemanProd_CallStmt(pro, root, nameTable);
//	//<CallFun> ->[z] | <Args>
//	else if (pro.Left.content == "<CallFun>")
//		SemanProd_CallFun(pro, root, nameTable);
//	//<Args> -> <Exp>[COMMA] <Args> | <Exp>
//	else if (pro.Left.content == "<Args>")
//		SemanProd_Args(pro, root, nameTable);
//	//<ReturnStmt> -> [RETURN] <Exp> | [RETURN]
//	else if (pro.Left.content == "<ReturnStmt>")
//		SemanProd_ReturnStmt(pro, root, nameTable);
//	//<WhileStmt> -> <WhileStmt_m1> [WHILE] [LPAREN] <Exp> [RPAREN] <WhileStmt_m2> <Block>
//	else if (pro.Left.content == "<WhileStmt>")
//		SemanProd_WhileStmt(pro, root, nameTable);
//	//<WhileStmt_m1> ->[z]
//	else if (pro.Left.content == "<WhileStmt_m1>")
//		SemanProd_WhileStmt_m1(pro, root, nameTable);
//	//<WhileStmt_m2> ->[z]
//	else if (pro.Left.content == "<WhileStmt_m2>")
//		SemanProd_WhileStmt_m2(pro, root, nameTable);
//	//<IfStmt> -> [IF] [LPAREN] <Exp> [RPAREN] <IfStmt_m2> <Block> <IfNext>
//	else if (pro.Left.content == "<IfStmt>")
//		SemanProd_IfStmt(pro, root, nameTable);
//	//<IfStmt_m> -> [z]
//	else if (pro.Left.content == "<IfStmt_m>")
//		SemanProd_IfStmt_m1(pro, root, nameTable);
//	//<IfNext> -> [z] | [ELSE] <IfStmt_next> <Block>
//	else if (pro.Left.content == "<IfNext>")
//		SemanProd_IfNext(pro, root, nameTable);
//	//<IfStmt_next>-> [z]
//	else if (pro.Left.content == "<IfStmt_next>")
//		SemanProd_IfStmt_next(pro, root, nameTable);
//	else
//	{
//		cout << "There is no symbol like [" << pro.Left.content << "]" << endl;
//	}
//}
//
//void SemanticAnalyzer::SemanProd_Program(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	global_table = table_stack.back();
//	table_stack.pop_back();
//	global_table->width = offset_stack.back();
//	offset_stack.pop_back();
//}
//
//void SemanticAnalyzer::SemanProd_Pro_m(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	root->quad = nextstat();
//	symbolTable* t = new(nothrow)symbolTable;
//	if (!t) { exit(-3); }
//	if (last_table != NULL)
//	{
//		last_table->next = t;
//		t->previous = last_table;
//	}
//	if (!table_stack.empty())
//	{
//		t->parent = table_stack.back();
//	}
//	last_table = t;
//	table_stack.push_back(t);
//	offset_stack.push_back(0);
//}
//
//void SemanticAnalyzer::SemanProd_DeclareString(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	//Is here still anything to be done?
//}
//
//void SemanticAnalyzer::SemanProd_DecOption(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	Symbol first_symmpro = pro.Right[0];
//	//[INT] [ID] <ParameterDeclare> [SEMI]
//	if (first_symmpro.content == "[INT]")
//	{
//		root->type = INT;
//		root->kind = root->children[2]->kind;
//		root->n = root->children[2]->n;
//		root->width = 4 * root->n;
//		table_stack.back()->enter(root->children[1]->content.second, root->type, root->kind, offset_stack.back());
//		if (root->kind == ARRAY)
//		{
//			root->dimension = root->children[2]->dimension;
//			table_stack.back()->enterdimension(root->children[1]->content.second, root->dimension);
//		}
//		offset_stack.back() += root->width;
//	}
//	//<FunctionDeclare> <FunDec> <Block>
//	else if (first_symmpro.content == "<FunctionDeclare>")
//	{
//		root->type = root->children[0]->type;
//		root->kind = root->children[0]->kind;
//		root->children[1]->type = root->children[0]->type;
//
//
//		root->n = root->children[1]->n;
//		root->width = root->children[1]->width;
//
//
//
//		symbolTable* t = table_stack.back();
//		table_stack.pop_back();
//		t->width = t->table.empty() ? 0 : offset_stack.back() - t->table[0].offset;
//		offset_stack.pop_back();
//	}
//	else
//	{
//		exit(-3);
//	}
//}
//
//void SemanticAnalyzer::SemanProd_ParaContent(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	Symbol first_symmpro = pro.Right[0];
//	//<ParameterContent> ->[z]
//	if (first_symmpro.content == "[z]")
//	{
//		root->kind = VAR;
//		root->n = 1;
//	}
//	//<ParameterContent> -> <ArrayDeclare> | [z]
//	else if (first_symmpro.content == "<ArrayDeclare> ")
//	{
//		root->kind = ARRAY;
//		root->n = root->children[0]->n;
//		root->dimension = root->children[0]->dimension;
//		reverse(root->dimension.begin(), root->dimension.end());
//	}
//}
//
//void SemanticAnalyzer::SemanProd_ArrayDeclare(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	int short_long = int(pro.Right.size());
//	//[LeftSquareBracket] [NUM] [RightSquareBracket] 
//	if (short_long == 3)
//	{
//		root->kind = ARRAY;
//		root->n = root->children[1]->content.second;
//		root->dimension.push_back(root->children[1]->content.second);
//	}
//	// [LeftSquareBracket] [NUM] [RightSquareBracket] <ArrayDeclare>
//	else if (short_long == 4)
//	{
//		root->kind = ARRAY;
//		root->n = root->children[1]->content.second * root->children[3]->n;
//		root->dimension = root->children[3]->dimension;
//		root->dimension.push_back(root->children[1]->content.second);
//	}
//}
//
//void SemanticAnalyzer::SemanProd_FunctionDeclare(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	//Is here still anything to be done?
//	Symbol first_symmpro = pro.Right[0];
//	//<FunctionDeclare> ->[VOID] | [INT]
//	if (first_symmpro.content == "[VOID]")
//	{
//		root->type = VOID;
//		root->kind = FUNC;
//	}
//	else if (first_symmpro.content == "[INT]")
//	{
//		root->type = INT;
//		root->kind = FUNC;
//	}
//}
//
//void SemanticAnalyzer::SemanProd_FunDec(const Production& pro, IdentNode* root, map<int, string> nameTable)
//{
//	root->kind = FUNC;
//	root->n = 1;
//	root->width = -1 * root->n;
//
//	symbolTable* new_table = table_stack.back();
//	table_stack.pop_back();
//	int new_offset = offset_stack.back();
//	offset_stack.pop_back();
//
//	table_stack.back()->enter(root->children[0]->content.second, root->type, root->kind, root->children[1]->quad);
//	offset_stack.back() += 0;
//	table_stack.back()->enterproc(root->children[0]->content.second, new_table);
//	table_stack.back()->enterdimension(root->children[0]->content.second, root->children[3]->dimension);
//
//	table_stack.push_back(new_table);
//	offset_stack.push_back(new_offset);
//}
//
//
