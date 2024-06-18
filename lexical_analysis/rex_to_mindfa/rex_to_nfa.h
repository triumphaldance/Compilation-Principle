#include "preprocess.h"
#include <fstream>

/********************表达式转NFA********************/

struct NfaState /*定义NFA状态*/
{

	int index; /*NFA状态的状态号*/

	char input;	 /*NFA状态弧上的值*/
	int chTrans; /*NFA状态弧转移到的状态号*/

	IntSet epTrans; /*当前状态通过ε转移到的状态号集合*/
};

struct NFA
{

	NfaState *head; /*NFA的头指针*/
	NfaState *tail; /*NFA的尾指针*/
};

NfaState NfaStates[MAX]; /*NFA状态数组*/
int nfaStateNum = 0;	 /*NFA状态总数*/

/*从状态n1到状态n2添加一条弧，弧上的值为ch*/
void add(NfaState *n1, NfaState *n2, char ch)
{

	n1->input = ch;
	n1->chTrans = n2->index;
}

/*从状态n1到状态n2添加一条弧，弧上的值为ε*/
void add(NfaState *n1, NfaState *n2)
{

	n1->epTrans.insert(n2->index);
}

/*新建一个NFA（即从NFA状态数组中取出两个状态）*/
NFA creatNFA(int sum)
{

	NFA n;

	n.head = &NfaStates[sum];
	n.tail = &NfaStates[sum + 1];

	return n;
}

void initNFA(NfaState *NfaStates){
	int i, j;
	for (i = 0; i < MAX; i++)
	{

		NfaStates[i].index = i;
		NfaStates[i].input = '#';
		NfaStates[i].chTrans = -1;
	}
}

/*后缀表达式转nfa*/
NFA strToNfa(string s)
{

	stack<NFA> NfaStack; /*定义一个NFA栈*/
	initNFA(NfaStates);
	for (int i = 0; i < s.size(); i++) /*读取后缀表达式，每次读一个字符*/
	{

		if (s[i] >= 'a' && s[i] <= 'z' || s[i] == '#') /*遇到操作数*/
		{

			NFA n = creatNFA(nfaStateNum); /*新建一个NFA*/
			nfaStateNum += 2;			   /*NFA状态总数加2*/

			add(n.head, n.tail, s[i]); /*NFA的头指向尾，弧上的值为s[i]*/

			NfaStack.push(n); /*将该NFA入栈*/
		}

		else if (s[i] == '*') /*遇到闭包运算符*/
		{

			NFA n1 = creatNFA(nfaStateNum); /*新建一个NFA*/
			nfaStateNum += 2;				/*NFA状态总数加2*/

			NFA n2 = NfaStack.top(); /*从栈中弹出一个NFA*/
			NfaStack.pop();

			add(n2.tail, n1.head); /*n2的尾通过ε指向n1的头*/
			add(n2.tail, n1.tail); /*n2的尾通过ε指向n1的尾*/
			add(n1.head, n2.head); /*n1的头通过ε指向n2的头*/
			add(n1.head, n1.tail); /*n1的头通过ε指向n1的尾*/

			NfaStack.push(n1); /*最后将新生成的NFA入栈*/
		}

		else if (s[i] == '|') /*遇到或运算符*/
		{

			NFA n1, n2; /*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
			n2 = NfaStack.top();
			NfaStack.pop();

			n1 = NfaStack.top();
			NfaStack.pop();

			NFA n = creatNFA(nfaStateNum); /*新建一个NFA*/
			nfaStateNum += 2;			   /*NFA状态总数加2*/

			add(n.head, n1.head); /*n的头通过ε指向n1的头*/
			add(n.head, n2.head); /*n的头通过ε指向n2的头*/
			add(n1.tail, n.tail); /*n1的尾通过ε指向n的尾*/
			add(n2.tail, n.tail); /*n2的尾通过ε指向n的尾*/

			NfaStack.push(n); /*最后将新生成的NFA入栈*/
		}

		else if (s[i] == '&') /*遇到连接运算符*/
		{

			NFA n1, n2, n; /*定义一个新的NFA n*/

			n2 = NfaStack.top(); /*从栈中弹出两个NFA，栈顶为n2，次栈顶为n1*/
			NfaStack.pop();

			n1 = NfaStack.top();
			NfaStack.pop();

			add(n1.tail, n2.head); /*n1的尾通过ε指向n2的尾*/

			n.head = n1.head; /*n的头为n1的头*/
			n.tail = n2.tail; /*n的尾为n2的尾*/

			NfaStack.push(n); /*最后将新生成的NFA入栈*/
		}
	}

	return NfaStack.top(); /*最后的栈顶元素即为生成好的NFA*/
}

/*打印NFA函数*/
void printNFA(NFA nfa)
{

	cout << "***************     NFA     ***************" << endl
		 << endl;
	cout << "NFA总共有s" << nfaStateNum << "个状态，" << endl;
	cout << "初态为" << nfa.head->index << "，终态为" << nfa.tail->index << "。" << endl
		 << endl
		 << "转移函数为：" << endl;

	for (int i = 0; i < nfaStateNum; i++) /*遍历NFA状态数组*/
	{

		if (NfaStates[i].input != '#') /*如果弧上的值不是初始时的‘#’则输出*/
		{
			cout << NfaStates[i].index << "-->'" << NfaStates[i].input << "'-->" << NfaStates[i].chTrans << '\t';
		}

		IntSet::iterator it; /*输出该状态经过ε到达的状态*/
		for (it = NfaStates[i].epTrans.begin(); it != NfaStates[i].epTrans.end(); it++)
		{
			cout << NfaStates[i].index << "-->'" << "ε" << "'-->" << *it << '\t';
		}

		cout << endl;
	}
}
// nfa可视化
void printNFAGraph(NFA nfa, string filename)
{
	ofstream outFile(filename);

	if (!outFile)
	{
		cerr << "无法创建文件：" << filename << endl;
		return;
	}

	// 写入DOT文件头部信息
	outFile << "digraph NFA {" << endl;
	outFile << "\trankdir=LR;" << endl;
	outFile << "\tnode [shape=circle];" << endl;

	// 遍历NFA状态数组，输出状态之间的转移关系
	for (int i = 0; i < nfaStateNum; i++)
	{
		if (NfaStates[i].index == nfa.head->index)
		{
			outFile << "\t" << 's' << " -> " << NfaStates[i].index
					<< " [label=\"ε\"];" << endl;
		}
		if (NfaStates[i].input != '#')
		{
			outFile << "\t" << NfaStates[i].index << " -> " << NfaStates[i].chTrans
					<< " [label=\"" << NfaStates[i].input << "\"];" << endl;
		}

		// 输出经过ε转移的状态
		for (auto nextState : NfaStates[i].epTrans)
		{
			outFile << "\t" << NfaStates[i].index << " -> " << nextState
					<< " [label=\"ε\"];" << endl;
		}
		if (i == nfaStateNum - 1)
		{
			outFile << "\t" << NfaStates[i].index << " [shape=doublecircle];";
		}
	}

	// 写入DOT文件尾部信息
	outFile << "}" << endl;

	outFile.close();
	cout << "已成功将NFA转移函数输出到文件：" << filename << endl;
}