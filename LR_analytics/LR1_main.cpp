#include "LR1.h"

int main() {
	LR1 lr1;
	init_map();
	/*词法分析*/
	lr1.lex.lex_analyze("input.txt");

	lr1.lex.output_result("Lexoutput.txt");
	/*语法分析*/
	string dat_path = "grammar.txt";
	lr1.read_generators(dat_path);
	lr1.get_firstset_of_vn();
	//for (int i = 0; i < lr1.generator.size(); ++i)
	//	lr1.get_firstset_of_string(lr1.generator[i].Right);

	lr1.init_items();
	//int i = 0;
	//for (auto a = lr1.eventclo.begin(); a != lr1.eventclo.end(); a++)
	//{
	//	cout << "closure" << i++ << endl;
	//	for (auto b = a->Es.begin(); b != a->Es.end(); b++)
	//	{
	//		cout << *b << endl;
	//	}
	//	cout << endl;
	//}


	lr1.getTable();

	lr1.printTable("resultTable.txt");
	lr1.grammartree("tree.dot",lr1.lex.Result);

	//lr1.SemanticAnalysis.printQuadruple("quadrupleresult.txt");

	return 0;
}