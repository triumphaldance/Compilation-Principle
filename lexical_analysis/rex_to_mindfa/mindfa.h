#include"nfa_to_dfa.h"

/******************DFA的最小化******************/
IntSet s[MAX];					/*划分出来的集合数组*/
DfaState minDfaStates[MAX];		/*minDfa状态数组*/


int minDfaStateNum = 0;			/*minDfa的状态总数，同时也是划分出的集合数*/

struct stateSet			/*划分状态集*/
{
	
	int index;			/*该状态集所能转换到的状态集标号*/  
	IntSet s;			/*该状态集中的dfa状态号*/
};

void initMinDFA(DfaState *minDfaStates){
	int i, j;
	for (i = 0; i < MAX; i++)
	{

		minDfaStates[i].index = i;
		minDfaStates[i].isEnd = false;

		for (int j = 0; j < 10; j++)
		{

			minDfaStates[i].Edges[j].input = '#';
			minDfaStates[i].Edges[j].Trans = -1;
		}
	}
}
/*当前划分总数为count，返回状态n所属的状态集标号i*/
 int findSetNum(int count, int n)
{
	
	for(int i = 0; i < count; i++)
	{
		if(s[i].count(n))
		{						
			return i;
		}
	}
}

/*最小化DFA*/
DFA minDFA(DFA d)
{
	initMinDFA(minDfaStates);
	int i, j;

	cout<<endl<<"*************     minDFA     **************"<<endl<<endl;
	
	DFA minDfa;
	minDfa.terminator = d.terminator;		/*把dfa的终结符集赋给minDfa*/
	
	memset(minDfa.trans, -1, sizeof(minDfa.trans));		/*初始化minDfa转移矩阵*/
	
	/*做第一次划分，即将终态与非终态分开*/
	bool endFlag = true;					/*判断dfa的所有状态是否全为终态的标志*/ 
	for(i = 0; i < dfaStateNum; i++)	/*遍历dfa状态数组*/
	{
		if(DfaStates[i].isEnd == false)			/*如果该dfa状态不是终态*/
		{

			endFlag = false;						/*标志应为false*/
			minDfaStateNum = 2;						/*第一次划分应该有两个集合*/
			
			s[1].insert(DfaStates[i].index);		/*把该状态的状态号加入s[1]集合中*/
		}
		else									/*如果该dfa状态是终态*/
		{
			s[0].insert(DfaStates[i].index);		/*把该状态的状态号加入s[0]集合中*/
		}
	}
	
	if(endFlag)					/*如果标志为真，则所有dfa状态都是终态*/
	{
		minDfaStateNum = 1;			/*第一次划分结束应只有一个集合*/
	}
	
	bool cutFlag = true;		/*上一次是否产生新的划分的标志*/
	while(cutFlag)				/*只要上一次产生新的划分就继续循环*/
	{
		
		int cutCount = 0;			/*需要产生新的划分的数量*/
		for(i = 0; i < minDfaStateNum; i++)			/*遍历每个划分集合*/
		{
			
			CharSet::iterator it;
			for(it = d.terminator.begin(); it != d.terminator.end(); it++)		/*遍历dfa的终结符集*/
			{
				
				int setNum = 0;				/*当前缓冲区中的状态集个数*/
				stateSet temp[20];			/*划分状态集“缓冲区”*/
				
				IntSet::iterator iter;
				for(iter = s[i].begin(); iter != s[i].end(); iter++)		/*遍历集合中的每个状态号*/
				{
					
					bool epFlag = true;			/*判断该集合中是否存在没有该终结符对应的转换弧的状态*/
					for(j = 0; j < DfaStates[*iter].edgeNum; j++)		/*遍历该状态的所有边*/
					{

						if(DfaStates[*iter].Edges[j].input == *it)		/*如果该边的输入为该终结符*/
						{

							epFlag = false;			/*则标志为false*/
							
							/*计算该状态转换到的状态集的标号*/
							int transNum = findSetNum(minDfaStateNum, DfaStates[*iter].Edges[j].Trans);
						
							int curSetNum = 0;			/*遍历缓冲区，寻找是否存在到达这个标号的状态集*/
							while((temp[curSetNum].index != transNum) && (curSetNum < setNum))
							{
								curSetNum++;
							}
							
							if(curSetNum == setNum)		/*缓冲区中不存在到达这个标号的状态集*/
							{
								
								/*在缓冲区中新建一个状态集*/
								temp[setNum].index = transNum;		/*该状态集所能转换到的状态集标号为transNum*/	
								temp[setNum].s.insert(*iter);		/*把当前状态添加到该状态集中*/
								
								setNum++;		/*缓冲区中的状态集个数加一*/
							}
							else			/*缓冲区中存在到达这个标号的状态集*/
							{
								temp[curSetNum].s.insert(*iter);	/*把当前状态加入到该状态集中*/
							}
						}
					}
					
					if(epFlag)		/*如果该状态不存在与该终结符对应的转换弧*/
					{
						
						/*寻找缓冲区中是否存在转换到标号为-1的状态集
						这里规定如果不存在转换弧，则它所到达的状态集标号为-1*/
						int curSetNum = 0;
						while((temp[curSetNum].index != -1) && (curSetNum < setNum))
						{
							curSetNum++;
						}
							
						if(curSetNum == setNum)			/*如果不存在这样的状态集*/
						{
							
							/*在缓冲区中新建一个状态集*/
							temp[setNum].index = -1;			/*该状态集转移到的状态集标号为-1*/
							temp[setNum].s.insert(*iter);		/*把当前状态加入到该状态集中*/
							
							setNum++;		/*缓冲区中的状态集个数加一*/
						}
						else			/*缓冲区中存在到达这个标号的状态集*/
						{
							temp[curSetNum].s.insert(*iter);	/*把当前状态加入到该状态集中*/
						}
					}	
				}
				
				if(setNum > 1)	/*如果缓冲区中的状态集个数大于1，表示同一个状态集中的元素能转换到不同的状态集，则需要划分*/
				{
					
					cutCount++;		/*划分次数加一*/
					
					/*为每组划分创建新的dfa状态*/
					for(j = 1; j < setNum; j++)		/*遍历缓冲区，这里从1开始是将第0组划分留在原集合中*/
					{
						
						IntSet::iterator t;
						for(t = temp[j].s.begin(); t != temp[j].s.end(); t++)
						{
							
							s[i].erase(*t);						/*在原来的状态集中删除该状态*/
							s[minDfaStateNum].insert(*t);		/*在新的状态集中加入该状态*/
						}
						
						minDfaStateNum++;		/*最小化DFA状态总数加一*/
					}
				}
			}	
		}
		
		if(cutCount == 0)		/*如果需要划分的次数为0，表示本次不需要进行划分*/
		{
			cutFlag = false;
		}
	}
	
	/*遍历每个划分好的状态集*/
	for(i = 0; i < minDfaStateNum; i++)
	{
		
		IntSet::iterator y;
		for(y = s[i].begin(); y != s[i].end(); y++)		/*遍历集合中的每个元素*/
		{
			
			if(*y == d.startState)			/*如果当前状态为dfa的初态，则该最小化DFA状态也为初态*/
			{
				minDfa.startState = i;
			}
			
			if(d.endStates.count(*y))		/*如果当前状态是终态，则该最小化DFA状态也为终态*/
			{
				
				minDfaStates[i].isEnd = true;
				minDfa.endStates.insert(i);		/*将该最小化DFA状态加入终态集中*/
			}
			
			for(j = 0; j < DfaStates[*y].edgeNum; j++)		/*遍历该DFA状态的每条弧，为最小化DFA创建弧*/
			{

				/*遍历划分好的状态集合，找出该弧转移到的状态现在属于哪个集合*/
				for(int t = 0; t < minDfaStateNum; t++)
				{
					if(s[t].count(DfaStates[*y].Edges[j].Trans))
					{
						
						bool haveEdge = false;		/*判断该弧是否已经创建的标志*/
						for(int l = 0; l < minDfaStates[i].edgeNum; l++)	/*遍历已创建的弧*/
						{					/*如果该弧已经存在*/
							if((minDfaStates[i].Edges[l].input == DfaStates[*y].Edges[j].input) && (minDfaStates[i].Edges[l].Trans == t))
							{
								haveEdge = true;		/*标志为真*/
							}
						}
						
						if(!haveEdge)		/*如果该弧不存在，则创建一条新的弧*/
						{
							
							minDfaStates[i].Edges[minDfaStates[i].edgeNum].input = DfaStates[*y].Edges[j].input;	/*弧的值与DFA的相同*/
							minDfaStates[i].Edges[minDfaStates[i].edgeNum].Trans = t;	/*该弧转移到的状态为这个状态集的标号*/
							
							minDfa.trans[i][DfaStates[*y].Edges[j].input - 'a'] = t;	/*更新转移矩阵*/
							
							minDfaStates[i].edgeNum++;		/*该状态的弧的数目加一*/
						}

						break;
					}
				}
			}
		}
	}
	
	return minDfa;
}

void printMinDFA(DFA d)
{
	
	int i, j;
	cout<<"minDFA总共有"<<minDfaStateNum<<"个状态，"<<"初态为"<<d.startState<<endl<<endl;
	
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
	for(i = 0; i < minDfaStateNum; i++)
	{
		for(j = 0; j < minDfaStates[i].edgeNum; j++)
		{
			
			if(minDfaStates[minDfaStates[i].Edges[j].Trans].isEnd == true)
			{
				cout<<minDfaStates[i].index<<"-->'"<<minDfaStates[i].Edges[j].input;
				cout<<"'--><"<<minDfaStates[i].Edges[j].Trans<<">\t";
			}
			else
			{
				cout<<minDfaStates[i].index<<"-->'"<<minDfaStates[i].Edges[j].input;
				cout<<"'-->"<<minDfaStates[i].Edges[j].Trans<<'\t';
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
	
	for(i = 0; i < minDfaStateNum; i++)
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
	cout<<endl<<"*******************************************"<<endl;
}
// //将minDFA的转移函数输出为Graphviz DOT语言的文件
void printMinDFAGraph(DFA dfa, string filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "无法创建文件：" << filename << endl;
        return;
    }

    // 写入DOT文件头部信息
    outFile << "digraph MinDFA {" << endl;
    outFile << "\trankdir=LR;" << endl;
    outFile << "\tnode [shape=circle];" << endl;

    // 输出minDFA状态之间的转移关系
    for (int i = 0; i < minDfaStateNum; i++) {
		if(minDfaStates[i].index==dfa.startState){
			outFile<<'\t'<<'s'<<" -> "<<DfaStates[i].index<<endl;
		}
        for (int j = 0; j < minDfaStates[i].edgeNum; j++) {
            if (minDfaStates[minDfaStates[i].Edges[j].Trans].isEnd == true) {
                outFile << "\t" << minDfaStates[i].index << " -> <" << minDfaStates[i].Edges[j].Trans
                        << "> [label=\"" << minDfaStates[i].Edges[j].input << "\"];" << endl;
				outFile << "\t" << minDfaStates[i].Edges[j].Trans << " [shape=doublecircle];" << endl;
            } else {
                outFile << "\t" << minDfaStates[i].index << " -> " << minDfaStates[i].Edges[j].Trans
                        << " [label=\"" << minDfaStates[i].Edges[j].input << "\"];" << endl;
            }
        }
    }

    // 写入DOT文件尾部信息
    outFile << "}" << endl;

    outFile.close();
    cout << "已成功将minDFA转移函数输出到文件：" << filename << endl;
	cout<<endl<<"*******************************************"<<endl;
}

