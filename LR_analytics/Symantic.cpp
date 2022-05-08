#include "Semantic.h"

/* SemanticSymtable��*/
/*���캯��*/
SemanticSymtable::SemanticSymtable(const int t_type, const string& t_name) {
	this->type = t_type;
	this->name = t_name;
}

/*��ѯ����*/
int SemanticSymtable::findSym(const string& id_name) {
	for (int i = 0; i < Symtable.size(); ++i) {
		if (Symtable[i].name == id_name)
			return i;
	}
	return -1;//�Ҳ���!
}

/*���뺯��*/
int SemanticSymtable::insertSym(const Identifier& id) {
	if (findSym(id.name) != -1)
		return -1;//�޲���,�Ѵ���

	Symtable.push_back(id);
	return Symtable.size() - 1;//���
}


/*SemanticAnalyzer��*/
/*���캯��*/
SemanticAnalyzer::SemanticAnalyzer() {
	nextQ_index = 1;//��1��ʼ,0Ϊ(j,-,-,main_addr)
	main_index = -1;
	backpatching_level = 0;//�������
	cnt = 0;
	//ȫ�ַ��ű�
	allTable.push_back(SemanticSymtable(GLOBAL, "ȫ�ֱ�"));
	allTable.push_back(SemanticSymtable(TEMP, "��ʱ������"));
	cur_tableStack.push_back(0);
}

/*����������������з���������*/
void SemanticAnalyzer::insertSymbol(const SemanticSymbol& sym) {

	symbolList.push_back(sym);
}

/*��ӡ��Ԫʽ��*/
void SemanticAnalyzer::printQuadruple(const string outpath) {
	ofstream fout(outpath, ios::out);
	for (auto x : quadruple)
		fout << x.index << " ("
		<< x.op << ',' << x.arg1 << ',' << x.arg2<< ',' << x.res
		<< ")" << endl;
}

/*�������庯��*/
void SemanticAnalyzer::semanticANL(const Production& pro) {
	//<Program> -> <DeclareString>
	if (pro.Left.content == "<Program>")
		SemanProd_Program(pro);
	//<DeclareString-option> -> <ParameterDeclare> [ID] [SEMI] | <FunctionDeclare> <FunDec> <Block>
	else if (pro.Left.content == "<DeclareString-option>")
		SemanProd_DecOption(pro);
	/*<ParameterDeclare> ->[INT]*/
	else if (pro.Left.content == "<ParameterDeclare>")
		SemanProd_ParaDec(pro);
	/*<FunctionDeclare> ->[VOID] | [INT]*/
	else if (pro.Left.content == "<FunctionDeclare>")
		SemanProd_FunctionDeclare(pro);
	/*<FunDec> ->[ID] <CreateFunTable_m>[LPAREN] <VarList>[RPAREN]*/
	else if (pro.Left.content == "<FunDec>")
		SemanProd_FunDec(pro);
	/*<CreateFunTable_m> ->[z]*/
	else if (pro.Left.content == "<CreateFunTable_m>")
		SemanProd_CreateFunTable_m(pro);
	//<ParamDec> -> <ParameterDeclare>[ID]
	else if (pro.Left.content == "<ParamDec>")
		SemanProd_ParamDec(pro);
	/*<Block> ->[LBBRACKET] <DefList> <StmtList>[RBBRACKET]*/
	else if (pro.Left.content == "<Block>")
		SemanProd_Block(pro);
	/*<Def> -> <ParameterDeclare>[ID][SEMI]*/
	else if (pro.Left.content == "<Def>")
		SemanProd_Def(pro);
	/*<AssignStmt> ->[ID][ASSIGN] <Exp>*/
	else if (pro.Left.content == "<AssignStmt>")
		SemanProd_AssignStmt(pro);
	/*<Exp> -> <AddSubExp> | <Exp> <Relop> <AddSubExp>*/
	else if (pro.Left.content == "<Exp>")
		SemanProd_Exp(pro);
	/*<AddSubExp> -> <Item> | <Item>[PLUS] <Item> | <Item>[SUB] <Item>*/
	else if (pro.Left.content == "<AddSubExp>")
		SemanProd_AddSubExp(pro);
	/*<Item> -> <Factor> | <Factor>[MUL] <Factor> | <Factor>[DIV] <Factor>*/
	else if (pro.Left.content == "<Item>")
		SemanProd_Item(pro);
	/*<Factor> ->[NUM] | [LPAREN] <Exp>[RPAREN] | [ID] | <CallStmt>*/
	else if (pro.Left.content == "<Factor>")
		SemanProd_Factor(pro);
	/*<CallStmt> ->[ID][LPAREN] <CallFunCheck> <Args>[RPAREN]*/
	else if (pro.Left.content == "<CallStmt>")
		SemanProd_CallStmt(pro);
	/*<CallFunCheck> ->[z]*/
	else if (pro.Left.content == "<CallFunCheck>")
		SemanProd_CallFunCheck(pro);
	/*<Args> -> <Exp> [COMMA] <Args> | <Exp> | [z]*/
	else if (pro.Left.content == "<Args>")
		SemanProd_Args(pro);
	/*<ReturnStmt> ->[RETURN] <Exp> | [RETURN]*/
	else if (pro.Left.content == "<ReturnStmt>")
		SemanProd_ReturnStmt(pro);
	/*<Relop> ->[GT] | [LT] | [GE] | [LE] | [EQ] | [NQ]*/
	else if (pro.Left.content == "<Relop>")
		SemanProd_Relop(pro);
	/*<IfStmt> ->[IF] <IfStmt_m1>[LPAREN] <Exp>[RPAREN] <IfStmt_m2> <Block> <IfNext>*/
	else if (pro.Left.content == "<IfStmt>")
		SemanProd_IfStmt(pro);
	/*<IfStmt_m1> ->[z]*/
	else if (pro.Left.content == "<IfStmt_m1>")
		SemanProd_IfStmt_m1(pro);
	/*<IfStmt_m2> ->[z]*/
	else if (pro.Left.content == "<IfStmt_m2>")
		SemanProd_IfStmt_m2(pro);
	/*<IfNext> ->[z] | <IfStmt_next>[ELSE] <Block>*/
	else if (pro.Left.content == "<IfNext>")
		SemanProd_IfNext(pro);
	/*<IfStmt_next>->[z]*/
	else if (pro.Left.content == "<IfStmt_next>")
		SemanProd_IfStmt_next(pro);
	/*<WhileStmt> ->[WHILE] <WhileStmt_m1>[LPAREN] <Exp>[RPAREN] <WhileStmt_m2> <Block>*/
	else if (pro.Left.content == "<WhileStmt>")
		SemanProd_WhileStmt(pro);
	/*<WhileStmt_m1> ->[z]*/
	else if (pro.Left.content == "<WhileStmt_m1>")
		SemanProd_WhileStmt_m1(pro);
	/*<WhileStmt_m2> ->[z]*/
	else if (pro.Left.content == "<WhileStmt_m2>")
		SemanProd_WhileStmt_m2(pro);
	else {
		if (pro.Right[0] != symEps) {
			int count = pro.Right.size();
			while (count--)
				symbolList.pop_back();
		}
		symbolList.push_back({ {-1,"",pro.Left.content},-1, -1 });
	}

}

void SemanticAnalyzer::SemanProd_Program(const Production& pro)
{
	//���û�ж���main�������򱨴�
	if (main_index == -1) {
		cout << "[ERROR-10000] : δ����main����" << endl;		
	}
	int count = pro.Right.size();
	while (count--) {
		symbolList.pop_back();
	}
	//����ǰ�������Ԫʽ
	quadruple.insert(quadruple.begin(), { 0, "j","-" , "-", to_string(main_index) });

	symbolList.push_back({ {  -1,"",pro.Left.content}, -1, -1 });
}

void SemanticAnalyzer::SemanProd_DecOption(const Production& pro)
{
	//����Ƕ������
	if (pro.Right.size() == 3) {
		// <ParameterDeclare> [ID] [SEMI]
		SemanticSymbol pardec = symbolList[0];//int
		SemanticSymbol ident = symbolList[1];//������

		//�����жϸñ����Ƿ��ڵ�ǰ���Ѿ�����
		bool existed = false;
		SemanticSymtable current_table = allTable[cur_tableStack.back()];
		if (current_table.findSym(ident.toke.content) != -1) {
			cout << "[ERROR-10001] : " << ident.toke.line << "�У�����" << ident.toke.content << "�ض���" << endl;			
		}

		//����һ��������table
		Identifier variable;
		variable.name = ident.toke.content;//������
		variable.type = VAR;
		variable.specialType = pardec.toke.content;//int

		//����table
		allTable[cur_tableStack.back()].insertSym(variable);

		//symbolList����
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();

		symbolList.push_back({{ident.toke.line,ident.toke.content,pro.Left.content }, cur_tableStack.back(), int(allTable[cur_tableStack.back()].Symtable.size() - 1)});
	}
	//����Ƕ��庯��
	else {
		SemanticSymbol ident = symbolList[symbolList.size() - 2];

		//��Ҫ�˳�������
		cur_tableStack.pop_back();
		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {ident.toke.line,ident.toke.content,pro.Left.content },ident.table_index,ident.index });
	}
}
/**********************************************************************************/
void SemanticAnalyzer::SemanProd_ParaDec(const Production& pro)
{
	//symbol_list�����һ����int
	SemanticSymbol specifier = symbolList.back();
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({{specifier.toke.line,specifier.toke.content ,pro.Left.content}, -1, -1});
}
void SemanticAnalyzer::SemanProd_FunctionDeclare(const Production& pro)
{
	//symbol_list�����һ����int
	SemanticSymbol specifier = symbolList.back();
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({ {specifier.toke.line,specifier.toke.content ,pro.Left.content}, -1, -1 });
}
/**********************************************************************************/

void SemanticAnalyzer::SemanProd_FunDec(const Production& pro)
{
	//symbol_list��CreateFunTable_m��¼��table��Ϣ
	SemanticSymbol specifier = symbolList[symbolList.size() - 4];
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({ {specifier.toke.line,specifier.toke.content ,pro.Left.content} ,specifier.table_index,specifier.index });
}

void SemanticAnalyzer::SemanProd_CreateFunTable_m(const Production& pro)
{
	//����������
//��ʱsymbolList���һ������Ϊ�������������ڶ���Ϊ��������ֵ
	SemanticSymbol ident = symbolList.back();
	SemanticSymbol specifier = symbolList[symbolList.size() - 2];

	//������ȫ�ֵ�table�жϺ������Ƿ��ض���
	if (allTable[0].findSym(ident.toke.content) != -1) {
		cout<< "[ERROR-10002] :" << ident.toke.line << "�У�����" << ident.toke.content << "�ض���" << endl;
	}

	//����������
	allTable.push_back(SemanticSymtable(FUNC, ident.toke.content));
	//��ȫ�ַ��ű�����ǰ�����ķ�������������������ڵ�ַ����л��
	allTable[0].insertSym({ FUNC,specifier.toke.content,ident.toke.content,0,0,int(allTable.size() - 1) });

	//������ѹ��ջ
	cur_tableStack.push_back(allTable.size() - 1);

	//����ֵ
	Identifier return_value;
	return_value.type = RET;
	return_value.name = allTable.back().name + "_return_value";
	return_value.specialType = specifier.toke.content;
	//���Ϊmain����������м�¼
	if (ident.toke.content == "main")
		main_index = nextQ_index;
	//������Ԫʽ
	quadruple.push_back({ nextQ_index++ , ident.toke.content,"-","-" ,"-" });
	//�������м��뷵�ر���
	allTable[cur_tableStack.back()].insertSym(return_value);
	//�մ�����Ҫpop
	//����pushback
	symbolList.push_back({ {ident.toke.line,ident.toke.content,pro.Left.content},0,int(allTable[0].Symtable.size() - 1) });	
}

void SemanticAnalyzer::SemanProd_ParamDec(const Production& pro)
{
	//symbolList���һ��Ϊ�������������ڶ���Ϊ����
	SemanticSymbol ident = symbolList.back();
	SemanticSymbol specifier = symbolList[symbolList.size() - 2];
	//��ǰ������
	SemanticSymtable& function_table = allTable[cur_tableStack.back()];


	//����Ѿ����й�����
	if (function_table.findSym(ident.toke.content) != -1) {
		cout << "[ERROR-10003] : " << ident.toke.line << "�У���������" << ident.toke.content << "�ض���" << endl;
	}
	//����������βα���
	int new_position = function_table.insertSym({ VAR ,specifier.toke.content ,ident.toke.content,-1,-1,-1 });
	//��ǰ������ȫ�ַ����е�����
	int table_position = allTable[0].findSym(function_table.name);
	//�βθ���++
	allTable[0].Symtable[table_position].paraNum++;

	//������Ԫʽ
	quadruple.push_back({ nextQ_index++, "defpar","-" , "-", ident.toke.content });

	//symbolList����
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({{ ident.toke.line,ident.toke.content,pro.Left.content }, cur_tableStack.back(), new_position });
}

void SemanticAnalyzer::SemanProd_Block(const Production& pro)
{
	//����symbolList
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({ { -1, to_string(nextQ_index), pro.Left.content }, -1, -1 });
}

void SemanticAnalyzer::SemanProd_Def(const Production& pro)
{
	//symbolList�ĵ����ڶ����������������Ǳ�����������
	SemanticSymbol ident = symbolList[symbolList.size() - 2];
	SemanticSymbol specifier = symbolList[symbolList.size() - 3];
	SemanticSymtable& current_table = allTable[cur_tableStack.back()];

	//�ض����򱨴�
	if (current_table.findSym(ident.toke.content) != -1)
	{
		cout << "[ERROR-10004] : " << ident.toke.line << "�У�����" << ident.toke.content << "�ض���" << endl;		
	}

	current_table.insertSym({ VAR,specifier.toke.content,ident.toke.content ,-1,-1,-1 });

	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({ {ident.toke.line ,ident.toke.content,pro.Left.content},cur_tableStack.back(),int(allTable[cur_tableStack.back()].Symtable.size() - 1) });
}

void SemanticAnalyzer::SemanProd_AssignStmt(const Production& pro)
{
	//symbolList�ĵ�����һ����������������Exp�ͱ�����
	SemanticSymbol ident = symbolList[symbolList.size() - 3];
	SemanticSymbol exp = symbolList.back();

	//���id�Ƿ���ڣ��������򱨴�
	bool existed = false;
	int table_index = -1, index = -1;
	//�ӵ�ǰ�㿪ʼ���ϱ���
	for (int scope_layer = cur_tableStack.size() - 1; scope_layer >= 0; scope_layer--) {
		auto current_table = allTable[cur_tableStack[scope_layer]];
		if ((index = current_table.findSym(ident.toke.content)) != -1) {
			existed = true;
			table_index = cur_tableStack[scope_layer];
			break;
		}
	}
	if (existed == false) {
		cout << "[ERROR-10005] : " << ident.toke.line << "�У�����" << ident.toke.line << "δ����" << endl;		
	}

	quadruple.push_back({ nextQ_index++, "=", exp.toke.content, "-", ident.toke.content });

	//����symbolList
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({ { ident.toke.line,ident.toke.content ,pro.Left.content },table_index, index});
}

void SemanticAnalyzer::SemanProd_Exp(const Production& pro)
{
	if (pro.Right.size() == 1) {
		SemanticSymbol exp = symbolList.back();
		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {exp.toke.line,exp.toke.content ,pro.Left.content },  exp.table_index, exp.index	});
	}
	else {
		SemanticSymbol sub_exp1 = symbolList[symbolList.size() - 3];
		SemanticSymbol op = symbolList[symbolList.size() - 2];
		SemanticSymbol sub_exp2 = symbolList[symbolList.size() - 1];
		int next_label_num = nextQ_index++;
		string new_tmp_var = "T" + to_string(cnt++);
		quadruple.push_back({ next_label_num, "j" + op.toke.content, sub_exp1.toke.content, sub_exp2.toke.content, to_string(next_label_num + 3) });
		quadruple.push_back({ nextQ_index++, "=", "0", "-", new_tmp_var });
		quadruple.push_back({ nextQ_index++, "j", "-", "-", to_string(next_label_num + 4) });
		quadruple.push_back({ nextQ_index++, "=", "1", "-", new_tmp_var });

		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {-1, new_tmp_var ,pro.Left.content},-1,-1 });
	}
}

void SemanticAnalyzer::SemanProd_AddSubExp(const Production& pro)
{
	if (pro.Right.size() == 1) {
		SemanticSymbol exp = symbolList.back();
		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {exp.toke.line,exp.toke.content , pro.Left.content },exp.table_index,exp.index });
	}
	else {
		SemanticSymbol sub_exp1 = symbolList[symbolList.size() - 3];
		SemanticSymbol op = symbolList[symbolList.size() - 2];
		SemanticSymbol sub_exp2 = symbolList[symbolList.size() - 1];
		string new_tmp_var = "T" + to_string(cnt++);
		quadruple.push_back({ nextQ_index++, op.toke.content, sub_exp1.toke.content, sub_exp2.toke.content, new_tmp_var });

		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {-1,new_tmp_var ,pro.Left.content }, -1, -1});
	}
}

void SemanticAnalyzer::SemanProd_Item(const Production& pro)
{
	if (pro.Right.size() == 1) {
		SemanticSymbol exp = symbolList.back();
		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({{exp.toke.line,exp.toke.content,pro.Left.content },exp.table_index, exp.index	});
	}
	else {
		SemanticSymbol sub_exp1 = symbolList[symbolList.size() - 3];
		SemanticSymbol op = symbolList[symbolList.size() - 2];
		SemanticSymbol sub_exp2 = symbolList[symbolList.size() - 1];
		std::string new_tmp_var = "T" + to_string(cnt++);
		quadruple.push_back({ nextQ_index++, op.toke.content, sub_exp1.toke.content, sub_exp2.toke.content, new_tmp_var });

		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {-1,new_tmp_var,pro.Left.content }, -1, -1});
	}
}

void SemanticAnalyzer::SemanProd_Factor(const Production& pro)
{
	if (pro.Right.size() == 1) {
		SemanticSymbol exp = symbolList.back();
		//�����ID������Ƿ���й�����
		if (pro.Right[0].content == "[ID]") {
			bool existed = false;
			for (int scope_layer = cur_tableStack.size() - 1; scope_layer >= 0; scope_layer--) {
				auto current_table = allTable[cur_tableStack[scope_layer]];
				if (current_table.findSym(exp.toke.content) != -1) {
					existed = true;
					break;
				}
			}
			if (existed == false) {
				cout << "[ERROR-10006] : "  << exp.toke.line << "�У�����" << exp.toke.content << "δ����" << endl;			
			}
		}

		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {exp.toke.line,exp.toke.content ,pro.Left.content },exp.table_index,exp.index });
	}
	else {
		SemanticSymbol exp = symbolList[symbolList.size() - 2];

		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ { exp.toke.line, exp.toke.content,pro.Left.content},exp.table_index, exp.index});
	}
}




void SemanticAnalyzer::SemanProd_CallStmt(const Production& pro)
{
	SemanticSymbol identifier = symbolList[symbolList.size() - 5];
	SemanticSymbol check = symbolList[symbolList.size() - 3];
	SemanticSymbol args = symbolList[symbolList.size() - 2];

	//��麯���Ƿ��壨��CallFunCheckʱ�Ѿ���飩

	//����������
	int para_num = allTable[check.table_index].Symtable[check.index].paraNum;
	if (para_num > stoi(args.toke.content)) {
		cout << "[ERROR-10007] : " << identifier.toke.line << "�У�����" << identifier.toke.content << "����ʱ��������" << endl;		
	}
	else if (para_num < stoi(args.toke.content)) {
		cout << "[ERROR-10008] : " << identifier.toke.line << "�У�����" << identifier.toke.content << "����ʱ��������" << endl;
	}

	// ���ɺ���������Ԫʽ 
	string new_tmp_var = "T" + to_string(cnt++);
	quadruple.push_back({ nextQ_index++, "call", identifier.toke.content, "-", new_tmp_var });

	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	// �µ�exp��valueΪ��ʱ������
	symbolList.push_back({ {-1, new_tmp_var , pro.Left.content},-1,-1 });
}

void SemanticAnalyzer::SemanProd_CallFunCheck(const Production& pro)
{
	SemanticSymbol fun_id = symbolList[symbolList.size() - 2];

	int fun_id_pos = allTable[0].findSym(fun_id.toke.content);

	if (-1 == fun_id_pos) {
		cout << "[ERROR-10009] : " << fun_id.toke.line << "�У�����" << fun_id.toke.content << "����δ����" << endl;		
	}
	if (allTable[0].Symtable[fun_id_pos].type != FUNC) {
		cout << "[ERROR-10010] : " << fun_id.toke.line << "�У�����" << fun_id.toke.content << "����δ����" << endl;		
	}
	symbolList.push_back({ {fun_id.toke.line, fun_id.toke.content, pro.Left.content}, 0, fun_id_pos });
}

void SemanticAnalyzer::SemanProd_Args(const Production& pro)
{
	if (pro.Right.size() == 3) {
		SemanticSymbol exp = symbolList[symbolList.size() - 3];
		quadruple.push_back({ nextQ_index++, "param", exp.toke.content, "-", "-" });
		int aru_num = stoi(symbolList.back().toke.content) + 1;
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ { -1,to_string(aru_num),pro.Left.content }, -1, -1 });
	}
	else if (pro.Right[0].content == "<Exp>") {
		SemanticSymbol exp = symbolList.back();
		quadruple.push_back({ nextQ_index++, "param", exp.toke.content, "-", "-" });
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {-1, "1",pro.Left.content}, -1,-1 });
	}
	else if (pro.Right[0].content == "[z]") {
		symbolList.push_back({ {1, "0",pro.Left.content}, -1,-1 });
	}
}

void SemanticAnalyzer::SemanProd_ReturnStmt(const Production& pro)
{
	if (pro.Right.size() == 2) {
		//����ֵ
		SemanticSymbol return_exp = symbolList.back();
		//������
		SemanticSymtable function_table = allTable[cur_tableStack.back()];

		//�����Ԫʽ
		quadruple.push_back({ nextQ_index++,"=",return_exp.toke.content,"-",function_table.Symtable[0].name });

		//�����Ԫʽ
		quadruple.push_back({ nextQ_index++ ,"return",function_table.Symtable[0].name,"-",function_table.name });

		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ {-1, return_exp.toke.content,pro.Left.content }, -1, -1 });
	}
	else {
		//������
		SemanticSymtable function_table = allTable[cur_tableStack.back()];

		//��麯���ķ���ֵ�Ƿ�Ϊvoid
		if (allTable[0].Symtable[allTable[0].findSym(function_table.name)].specialType != "void") {
			cout << "[ERROR-10011] : " << symbolList.back().toke.line << "�У�����" << function_table.name << "�����з���ֵ" << endl;
		}

		//�����Ԫʽ
		quadruple.push_back({ nextQ_index++ ,"return","-","-",function_table.name });

		//����symbolList
		int count = pro.Right.size();
		while (count--)
			symbolList.pop_back();
		symbolList.push_back({ { -1, "",pro.Left.content }, -1, -1 });
	}
}

void SemanticAnalyzer::SemanProd_Relop(const Production& pro)
{
	SemanticSymbol op = symbolList.back();

	int count = pro.Right.size();
	while (count--) {
		symbolList.pop_back();
	}
	symbolList.push_back({ {-1,op.toke.content ,pro.Left.content}, -1,-1 });
}

void SemanticAnalyzer::SemanProd_IfStmt(const Production& pro)
{
	SemanticSymbol ifstmt_m2 = symbolList[symbolList.size() - 3];
	SemanticSymbol ifnext = symbolList[symbolList.size() - 1];

	if (ifnext.toke.content.empty()) {
		//ֻ��ifû��else
		//�����
		quadruple[backpatching_list.back()].res = ifstmt_m2.toke.content;
		backpatching_list.pop_back();

		//�ٳ���
		quadruple[backpatching_list.back()].res = to_string(nextQ_index);
		backpatching_list.pop_back();
	}
	else {
		//if�����
		quadruple[backpatching_list.back()].res = to_string(nextQ_index);
		backpatching_list.pop_back();
		//if�����
		quadruple[backpatching_list.back()].res = ifstmt_m2.toke.content;
		backpatching_list.pop_back();
		//if�ٳ���
		quadruple[backpatching_list.back()].res = ifnext.toke.content;
		backpatching_list.pop_back();
	}
	backpatching_level--;

	//popback
	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();
	symbolList.push_back({ {-1,"",pro.Left.content},-1,-1 });
}

void SemanticAnalyzer::SemanProd_IfStmt_m1(const Production& pro)
{
	backpatching_level++;
	symbolList.push_back({ {-1,to_string(nextQ_index),pro.Left.content},-1,-1 });
}

void SemanticAnalyzer::SemanProd_IfStmt_m2(const Production& pro)
{
	SemanticSymbol if_exp = symbolList[symbolList.size() - 2];

	//��������Ԫʽ : �ٳ���
	quadruple.push_back({ nextQ_index++,"j=",if_exp.toke.content,"0","" });
	backpatching_list.push_back(quadruple.size() - 1);

	//��������Ԫʽ : �����
	quadruple.push_back({ nextQ_index++,"j=","-","-","" });
	backpatching_list.push_back(quadruple.size() - 1);

	symbolList.push_back({ {-1,to_string(nextQ_index),pro.Left.content},-1,-1 });
}

void SemanticAnalyzer::SemanProd_IfNext(const Production& pro)
{
	SemanticSymbol if_stmt_next = symbolList[symbolList.size() - 3];

	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();

	symbolList.push_back({ { -1,if_stmt_next.toke.content,pro.Left.content }, -1, -1 });
}

void SemanticAnalyzer::SemanProd_IfStmt_next(const Production& pro)
{
	//if ���������(else ֮ǰ)(������)
	quadruple.push_back({ nextQ_index++,"j","-","-","" });
	backpatching_list.push_back(quadruple.size() - 1);
	symbolList.push_back({ {-1,to_string(nextQ_index),pro.Left.content},-1,-1 });
}

void SemanticAnalyzer::SemanProd_WhileStmt(const Production& pro)
{
	SemanticSymbol whilestmt_m1 = symbolList[symbolList.size() - 6];
	SemanticSymbol whilestmt_m2 = symbolList[symbolList.size() - 2];

	// ��������ת�� while �������ж���䴦
	quadruple.push_back({ nextQ_index++,"j","-","-" ,whilestmt_m1.toke.content });

	//���������
	quadruple[backpatching_list.back()].res = whilestmt_m2.toke.content;
	backpatching_list.pop_back();

	//����ٳ���
	quadruple[backpatching_list.back()].res = to_string(nextQ_index);
	backpatching_list.pop_back();

	backpatching_level--;

	int count = pro.Right.size();
	while (count--)
		symbolList.pop_back();

	symbolList.push_back({ {-1, "",pro.Left.content},-1,-1 });
}

void SemanticAnalyzer::SemanProd_WhileStmt_m1(const Production& pro)
{
	backpatching_level++;
	symbolList.push_back({ {-1,to_string(nextQ_index), pro.Left.content }, -1, -1 });
}

void SemanticAnalyzer::SemanProd_WhileStmt_m2(const Production& pro)
{
	SemanticSymbol while_exp = symbolList[symbolList.size() - 2];

	//�ٳ���
	quadruple.push_back({ nextQ_index++,"j=",while_exp.toke.content,"0","" });
	backpatching_list.push_back(quadruple.size() - 1);
	//�����
	quadruple.push_back({ nextQ_index++ ,"j","-","-" ,"" });
	backpatching_list.push_back(quadruple.size() - 1);

	symbolList.push_back({ { -1,to_string(nextQ_index),pro.Left.content }, -1, -1 });
}
