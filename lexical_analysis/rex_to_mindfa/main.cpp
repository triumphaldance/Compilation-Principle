#include "mindfa.h"

// �� DOT �ļ�ת��Ϊ PNG ͼƬ
void dotToPng(const std::string &dotFileName, const std::string &pngFileName)
{
	// ʹ��ϵͳ������� Graphviz �� dot ��� DOT �ļ�ת��Ϊ PNG ͼƬ
	std::string command = "D:\\Applications\\Graphviz\\bin\\dot -Tpng \"" + dotFileName + "\" -o \"" + pngFileName + "\"";
	int result = std::system(command.c_str());

	if (result != 0)
	{
		std::cerr << "ת��ʧ�ܣ�" << std::endl;
	}
	else
	{
		std::cout << "ת���ɹ������ɵ�ͼƬΪ��" << pngFileName << std::endl;
	}
}

int main()
{

	/*��������1
	string str = "(a|b)*abb";*/

	/*��������2*/
	// string str = "(a*|b*|#)*(#|b*)";

	/*��������3*/

	string str = "a*(a*|b*)*|a*";
	// string str;
	// cout << "������������ʽ: ";
	// cin >> str;
	// cout << "�������������ʽ��: " << str << endl;
	// while (true)
	// {

	str = infixToSuffix(str); /*����׺���ʽת��Ϊ��׺���ʽ*/

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
	// 	cout << "������������ʽ: ";
	// 	cin >> str;
	// 	cout << "�������������ʽ��: " << str << endl;
	// }

	return 0;
}
