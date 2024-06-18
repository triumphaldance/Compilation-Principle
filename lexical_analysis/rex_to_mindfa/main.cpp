#include "mindfa.h"

// 将 DOT 文件转换为 PNG 图片
void dotToPng(const std::string &dotFileName, const std::string &pngFileName)
{
	// 使用系统命令调用 Graphviz 的 dot 命令将 DOT 文件转换为 PNG 图片
	std::string command = "D:\\Applications\\Graphviz\\bin\\dot -Tpng \"" + dotFileName + "\" -o \"" + pngFileName + "\"";
	int result = std::system(command.c_str());

	if (result != 0)
	{
		std::cerr << "转换失败！" << std::endl;
	}
	else
	{
		std::cout << "转换成功！生成的图片为：" << pngFileName << std::endl;
	}
}

int main()
{

	/*测试样例1
	string str = "(a|b)*abb";*/

	/*测试样例2*/
	// string str = "(a*|b*|#)*(#|b*)";

	/*测试样例3*/

	string str = "a*(a*|b*)*|a*";
	// string str;
	// cout << "请输入正则表达式: ";
	// cin >> str;
	// cout << "你输入的正则表达式是: " << str << endl;
	// while (true)
	// {

	str = infixToSuffix(str); /*将中缀表达式转换为后缀表达式*/

	NFA n = strToNfa(str);
	printNFA(n);
	string filename = "dot/nfa.dot";
	printNFAGraph(n, filename);
	DFA d = nfaToDfa(n, str);
	printDFA(d);
	string filename1 = "dot/dfa.dot";
	printDFAGraph(d, filename1);
	DFA minDfa = minDFA(d);
	printMinDFA(minDfa);
	string filename2 = "dot/mindfa.dot";
	printMinDFAGraph(minDfa, filename2);
	dotToPng("dot/nfa.dot", "png/nfa.png");
	dotToPng("dot/dfa.dot", "png/dfa.png");
	dotToPng("dot/minDFA.dot", "png/mindfa.png");
	// 	cin.clear();
	// 	cout << "*******************************************"<<endl;
	// 	cout << "请输入正则表达式: ";
	// 	cin >> str;
	// 	cout << "你输入的正则表达式是: " << str << endl;
	// }

	return 0;
}
