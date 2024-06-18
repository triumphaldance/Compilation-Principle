#include"rex_to_nfa.h"
/********************NFA转DFA********************/

struct Edge			/*定义DFA的转换弧*/
{
	
	char input;			/*弧上的值*/
	int Trans;			/*弧所指向的状态号*/
};

struct DfaState		/*定义DFA状态*/
{
	
	bool isEnd;			/*是否为终态，是为true，不是为false*/
	
	int index;			/*DFA状态的状态号*/
	IntSet closure;		/*NFA的ε-move()闭包*/
	
	int edgeNum;		/*DFA状态上的射出弧数*/
	Edge Edges[10];		/*DFA状态上的射出弧*/
};

DfaState DfaStates[MAX];		/*DFA状态数组*/
int dfaStateNum = 0;			/*DFA状态总数*/

struct DFA			/*定义DFA结构*/
{
	
	int startState;				/*DFA的初态*/
	
	set<int> endStates;			/*DFA的终态集*/
	set<char> terminator;		/*DFA的终结符集*/
	
	int trans[MAX][26];		/*DFA的转移矩阵*/
};
void initDFA(DfaState *DfaStates){
	int i, j;
	for (i = 0; i < MAX; i++)
	{

		DfaStates[i].index = i;
		DfaStates[i].isEnd = false;

		for (j = 0; j < 10; j++)
		{

			DfaStates[i].Edges[j].input = '#';
			DfaStates[i].Edges[j].Trans = -1;
		}
	}

}
/*求一个状态集的ε-cloure*/
IntSet epcloure(IntSet s)
{
	
	stack<int> epStack;		/*(此处栈和队列均可)*/

	IntSet::iterator it;
	for(it = s.begin(); it != s.end(); it++)
	{
		epStack.push(*it);			/*将该状态集中的每一个元素都压入栈中*/
	}
	
	while(!epStack.empty())			/*只要栈不为空*/
	{
		
		int temp = epStack.top();		/*从栈中弹出一个元素*/
		epStack.pop();
		
		IntSet::iterator iter;
		for(iter = NfaStates[temp].epTrans.begin(); iter != NfaStates[temp].epTrans.end(); iter++)
		{
			if(!s.count(*iter))				/*遍历它通过ε能转换到的状态集*/
			{								/*如果当前元素没有在集合中出现*/
				s.insert(*iter);			/*则把它加入集合中*/
				epStack.push(*iter);		/*同时压入栈中*/
			}
		}
	}
	
	return s;		/*最后的s即为ε-cloure*/
}

/*求一个状态集s的ε-cloure(move(ch))*/
IntSet moveEpCloure(IntSet s, char ch)
{
	
	IntSet temp;
	
	IntSet::iterator it;
	for(it = s.begin(); it != s.end(); it++)		/*遍历当前集合s中的每个元素*/
	{
		if(NfaStates[*it].input == ch)				/*如果对应转换弧上的值为ch*/
		{
			temp.insert(NfaStates[*it].chTrans);		/*则把该弧通过ch转换到的状态加入到集合temp中*/
		}
	}
	
	temp = epcloure(temp);			/*最后求temp的ε闭包*/
	return temp;
}

/*判断一个状态是否为终态*/
bool IsEnd(NFA n, IntSet s)
{
	
	IntSet::iterator it;
	for(it = s.begin(); it != s.end(); it++)	/*遍历该状态所包含的nfa状态集*/
	{
		if(*it == n.tail->index)				/*如果包含nfa的终态，则该状态为终态，返回true*/
		{
			return true;
		}
	}
	
	return false;		/*如果不包含，则不是终态，返回false*/
}

/*nfa转dfa主函数*/
DFA nfaToDfa(NFA n, string str)		/*参数为nfa和后缀表达式*/
{
	
	cout<<"***************     DFA     ***************"<<endl<<endl; 
	
	int i;
	DFA d;
	set<IntSet> states;		/*定义一个存储整数集合的集合，用于判断求出一个状态集s的ε-cloure(move(ch))后是否出现新状态*/
	initDFA(DfaStates);
	memset(d.trans, -1, sizeof(d.trans));	/*初始化dfa的转移矩阵*/ 
	
	for(i = 0; i < str.size(); i++)			/*遍历后缀表达式*/
	{
		if(str[i] >= 'a' && str[i] <= 'z')		/*如果遇到操作数，则把它加入到dfa的终结符集中*/
		{
			d.terminator.insert(str[i]);
		}
	}
	
	d.startState = 0;		/*dfa的初态为0*/
	
	IntSet tempSet;
	tempSet.insert(n.head->index);	   	/*将nfa的初态加入到集合中*/
	
	DfaStates[0].closure = epcloure(tempSet);		/*求dfa的初态*/
	DfaStates[0].isEnd = IsEnd(n, DfaStates[0].closure);		/*判断初态是否为终态*/
	
	dfaStateNum++;			/*dfa数量加一*/
	
	queue<int> q;
	q.push(d.startState);		/*把dfa的初态存入队列中(此处栈和队列均可)*/
	
	while(!q.empty())		/*只要队列不为空，就一直循环*/
	{
		
		int num = q.front();				/*出去队首元素*/
		q.pop();
		
		CharSet::iterator it;
		for(it = d.terminator.begin(); it != d.terminator.end(); it++)		/*遍历终结符集*/
		{
			
			IntSet temp = moveEpCloure(DfaStates[num].closure, *it);		/*计算每个终结符的ε-cloure(move(ch))*/
			//打印每次划分 
			IntSet::iterator t;
			cout<<endl;
			for(t = temp.begin(); t != temp.end(); t++)   
			{
				cout<<*t<<' ';
			}
			cout<<endl;
			//??但是这里如果终态即初始状态
			if(!states.count(temp) && !temp.empty())	/*如果求出来的状态集不为空且与之前求出来的状态集不同，则新建一个DFA状态*/
			{
				
				states.insert(temp);				/*将新求出来的状态集加入到状态集合中*/

				DfaStates[dfaStateNum].closure = temp;
				
				DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;				/*该状态弧的输入即为当前终结符*/
				DfaStates[num].Edges[DfaStates[num].edgeNum].Trans = dfaStateNum;		/*弧转移到的状态为最后一个DFA状态*/
				DfaStates[num].edgeNum++;//??这里	edgeNum没有初始化为0可以直接++吗											/*该状态弧的数目加一*/
				
				d.trans[num][*it - 'a'] = dfaStateNum;		/*更新转移矩阵*/
				
				DfaStates[dfaStateNum].isEnd = IsEnd(n, DfaStates[dfaStateNum].closure);	/*判断是否为终态*/
				
				q.push(dfaStateNum);		/*将新的状态号加入队列中*/
				
				dfaStateNum++;		/*DFA状态总数加一*/
			}
			else			/*求出来的状态集在之前求出的某个状态集相同*/
			{
				for(i = 0; i < dfaStateNum; i++)		/*遍历之前求出来的状态集合*/
				{
					if(temp == DfaStates[i].closure)		/*找到与该集合相同的DFA状态*/
					{
						
						DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;		/*该状态弧的输入即为当前终结符*/
						DfaStates[num].Edges[DfaStates[num].edgeNum].Trans = i;			/*该弧转移到的状态即为i*/
						DfaStates[num].edgeNum++;										/*该状态弧的数目加一*/
						
						d.trans[num][*it - 'a'] = i;		/*更新转移矩阵*/
						
						break;
					}
				}
			}
		}
	}
	
	/*计算dfa的终态集*/
	for(i = 0; i < dfaStateNum; i++)	/*遍历dfa的所有状态*/	
	{
		if(DfaStates[i].isEnd == true)		/*如果该状态是终态*/
		{
			d.endStates.insert(i);		/*则将该状态号加入到dfa的终态集中*/
		}
	}
	
	return d;
}

/*打印dfa函数*/
void printDFA(DFA d)
{
	
	int i, j;
	cout<<"DFA总共有"<<dfaStateNum<<"个状态，"<<"初态为"<<d.startState<<endl<<endl;
	
	cout<<"有穷字母表为｛ ";
	set<char>::iterator it;
	for(it = d.terminator.begin(); it != d.terminator.end(); it++)
	{
		cout<<*it<<' ';
	}
	cout<<'}'<<endl<<endl;
	
	cout<<"终态集为｛ "; 
	IntSet::iterator iter;
	for(iter = d.endStates.begin(); iter != d.endStates.end(); iter++)
	{
		cout<<*iter<<' ';
	}
	cout<<'}'<<endl<<endl;
	
	cout<<"转移函数为："<<endl;
	for(i = 0; i < dfaStateNum; i++)
	{
		for(j = 0; j < DfaStates[i].edgeNum; j++)
		{
			
			if(DfaStates[DfaStates[i].Edges[j].Trans].isEnd == true)
			{
				cout<<DfaStates[i].index<<"-->'"<<DfaStates[i].Edges[j].input;
				cout<<"'--><"<<DfaStates[i].Edges[j].Trans<<">\t";
			}
			else
			{
				cout<<DfaStates[i].index<<"-->'"<<DfaStates[i].Edges[j].input;
				cout<<"'-->"<<DfaStates[i].Edges[j].Trans<<'\t';
			}
		}
		cout<<endl;
	}
	
	cout<<endl<<"转移矩阵为："<<endl<<"     ";
	CharSet::iterator t;
	for(t = d.terminator.begin(); t != d.terminator.end(); t++)
	{
		cout<<*t<<"   ";
	}
	cout<<endl;
	
	for(i = 0; i < dfaStateNum; i++)
	{
		
		if(d.endStates.count(i))
		{
			cout<<'<'<<i<<">  ";
		}
		else
		{
			cout<<' '<<i<<"   ";
		}
		
		for(j = 0; j < 26; j++)
		{
			if(d.terminator.count(j + 'a'))
			{
				if(d.trans[i][j] != -1)
				{
					cout<<d.trans[i][j]<<"   ";
				}
				else
				{
					cout<<"    "; 
				}
			}
		}
		
		cout<<endl;
	}
}

//dfa可视化
void printDFAGraph(DFA dfa, string filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "无法创建文件：" << filename << endl;
        return;
    }

    // 写入DOT文件头部信息
    outFile << "digraph DFA {" << endl;
    outFile << "\trankdir=LR;" << endl;
    outFile << "\tnode [shape=circle];" << endl;

    // 输出DFA状态之间的转移关系
    for (int i = 0; i < dfaStateNum; i++) {
		if(DfaStates[i].index==dfa.startState){
			outFile<<'\t'<<'s'<<" -> "<<DfaStates[i].index<<endl;
		}
        for (int j = 0; j < DfaStates[i].edgeNum; j++) {
            if (DfaStates[DfaStates[i].Edges[j].Trans].isEnd == true) {
                outFile << "\t" << DfaStates[i].index << " -> <" << DfaStates[i].Edges[j].Trans
                        << "> [label=\"" << DfaStates[i].Edges[j].input << "\"];" << endl;
				outFile << "\t" << DfaStates[i].Edges[j].Trans << " [shape=doublecircle];" << endl;
            } else {
                outFile << "\t" << DfaStates[i].index << " -> " << DfaStates[i].Edges[j].Trans
                        << " [label=\"" << DfaStates[i].Edges[j].input << "\"];" << endl;
            }
        }
    }

    // 写入DOT文件尾部信息
    outFile << "}" << endl;

    outFile.close();
    cout << "已成功将DFA转移函数输出到文件：" << filename << endl;
}
