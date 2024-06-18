#include"nfa_to_dfa.h"

/******************DFA����С��******************/
IntSet s[MAX];					/*���ֳ����ļ�������*/
DfaState minDfaStates[MAX];		/*minDfa״̬����*/


int minDfaStateNum = 0;			/*minDfa��״̬������ͬʱҲ�ǻ��ֳ��ļ�����*/

struct stateSet			/*����״̬��*/
{
	
	int index;			/*��״̬������ת������״̬�����*/  
	IntSet s;			/*��״̬���е�dfa״̬��*/
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
/*��ǰ��������Ϊcount������״̬n������״̬�����i*/
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

/*��С��DFA*/
DFA minDFA(DFA d)
{
	initMinDFA(minDfaStates);
	int i, j;

	cout<<endl<<"*************     minDFA     **************"<<endl<<endl;
	
	DFA minDfa;
	minDfa.terminator = d.terminator;		/*��dfa���ս��������minDfa*/
	
	memset(minDfa.trans, -1, sizeof(minDfa.trans));		/*��ʼ��minDfaת�ƾ���*/
	
	/*����һ�λ��֣�������̬�����̬�ֿ�*/
	bool endFlag = true;					/*�ж�dfa������״̬�Ƿ�ȫΪ��̬�ı�־*/ 
	for(i = 0; i < dfaStateNum; i++)	/*����dfa״̬����*/
	{
		if(DfaStates[i].isEnd == false)			/*�����dfa״̬������̬*/
		{

			endFlag = false;						/*��־ӦΪfalse*/
			minDfaStateNum = 2;						/*��һ�λ���Ӧ������������*/
			
			s[1].insert(DfaStates[i].index);		/*�Ѹ�״̬��״̬�ż���s[1]������*/
		}
		else									/*�����dfa״̬����̬*/
		{
			s[0].insert(DfaStates[i].index);		/*�Ѹ�״̬��״̬�ż���s[0]������*/
		}
	}
	
	if(endFlag)					/*�����־Ϊ�棬������dfa״̬������̬*/
	{
		minDfaStateNum = 1;			/*��һ�λ��ֽ���Ӧֻ��һ������*/
	}
	
	bool cutFlag = true;		/*��һ���Ƿ�����µĻ��ֵı�־*/
	while(cutFlag)				/*ֻҪ��һ�β����µĻ��־ͼ���ѭ��*/
	{
		
		int cutCount = 0;			/*��Ҫ�����µĻ��ֵ�����*/
		for(i = 0; i < minDfaStateNum; i++)			/*����ÿ�����ּ���*/
		{
			
			CharSet::iterator it;
			for(it = d.terminator.begin(); it != d.terminator.end(); it++)		/*����dfa���ս����*/
			{
				
				int setNum = 0;				/*��ǰ�������е�״̬������*/
				stateSet temp[20];			/*����״̬������������*/
				
				IntSet::iterator iter;
				for(iter = s[i].begin(); iter != s[i].end(); iter++)		/*���������е�ÿ��״̬��*/
				{
					
					bool epFlag = true;			/*�жϸü������Ƿ����û�и��ս����Ӧ��ת������״̬*/
					for(j = 0; j < DfaStates[*iter].edgeNum; j++)		/*������״̬�����б�*/
					{

						if(DfaStates[*iter].Edges[j].input == *it)		/*����ñߵ�����Ϊ���ս��*/
						{

							epFlag = false;			/*���־Ϊfalse*/
							
							/*�����״̬ת������״̬���ı��*/
							int transNum = findSetNum(minDfaStateNum, DfaStates[*iter].Edges[j].Trans);
						
							int curSetNum = 0;			/*������������Ѱ���Ƿ���ڵ��������ŵ�״̬��*/
							while((temp[curSetNum].index != transNum) && (curSetNum < setNum))
							{
								curSetNum++;
							}
							
							if(curSetNum == setNum)		/*�������в����ڵ��������ŵ�״̬��*/
							{
								
								/*�ڻ��������½�һ��״̬��*/
								temp[setNum].index = transNum;		/*��״̬������ת������״̬�����ΪtransNum*/	
								temp[setNum].s.insert(*iter);		/*�ѵ�ǰ״̬��ӵ���״̬����*/
								
								setNum++;		/*�������е�״̬��������һ*/
							}
							else			/*�������д��ڵ��������ŵ�״̬��*/
							{
								temp[curSetNum].s.insert(*iter);	/*�ѵ�ǰ״̬���뵽��״̬����*/
							}
						}
					}
					
					if(epFlag)		/*�����״̬����������ս����Ӧ��ת����*/
					{
						
						/*Ѱ�һ��������Ƿ����ת�������Ϊ-1��״̬��
						����涨���������ת�����������������״̬�����Ϊ-1*/
						int curSetNum = 0;
						while((temp[curSetNum].index != -1) && (curSetNum < setNum))
						{
							curSetNum++;
						}
							
						if(curSetNum == setNum)			/*���������������״̬��*/
						{
							
							/*�ڻ��������½�һ��״̬��*/
							temp[setNum].index = -1;			/*��״̬��ת�Ƶ���״̬�����Ϊ-1*/
							temp[setNum].s.insert(*iter);		/*�ѵ�ǰ״̬���뵽��״̬����*/
							
							setNum++;		/*�������е�״̬��������һ*/
						}
						else			/*�������д��ڵ��������ŵ�״̬��*/
						{
							temp[curSetNum].s.insert(*iter);	/*�ѵ�ǰ״̬���뵽��״̬����*/
						}
					}	
				}
				
				if(setNum > 1)	/*����������е�״̬����������1����ʾͬһ��״̬���е�Ԫ����ת������ͬ��״̬��������Ҫ����*/
				{
					
					cutCount++;		/*���ִ�����һ*/
					
					/*Ϊÿ�黮�ִ����µ�dfa״̬*/
					for(j = 1; j < setNum; j++)		/*�����������������1��ʼ�ǽ���0�黮������ԭ������*/
					{
						
						IntSet::iterator t;
						for(t = temp[j].s.begin(); t != temp[j].s.end(); t++)
						{
							
							s[i].erase(*t);						/*��ԭ����״̬����ɾ����״̬*/
							s[minDfaStateNum].insert(*t);		/*���µ�״̬���м����״̬*/
						}
						
						minDfaStateNum++;		/*��С��DFA״̬������һ*/
					}
				}
			}	
		}
		
		if(cutCount == 0)		/*�����Ҫ���ֵĴ���Ϊ0����ʾ���β���Ҫ���л���*/
		{
			cutFlag = false;
		}
	}
	
	/*����ÿ�����ֺõ�״̬��*/
	for(i = 0; i < minDfaStateNum; i++)
	{
		
		IntSet::iterator y;
		for(y = s[i].begin(); y != s[i].end(); y++)		/*���������е�ÿ��Ԫ��*/
		{
			
			if(*y == d.startState)			/*�����ǰ״̬Ϊdfa�ĳ�̬�������С��DFA״̬ҲΪ��̬*/
			{
				minDfa.startState = i;
			}
			
			if(d.endStates.count(*y))		/*�����ǰ״̬����̬�������С��DFA״̬ҲΪ��̬*/
			{
				
				minDfaStates[i].isEnd = true;
				minDfa.endStates.insert(i);		/*������С��DFA״̬������̬����*/
			}
			
			for(j = 0; j < DfaStates[*y].edgeNum; j++)		/*������DFA״̬��ÿ������Ϊ��С��DFA������*/
			{

				/*�������ֺõ�״̬���ϣ��ҳ��û�ת�Ƶ���״̬���������ĸ�����*/
				for(int t = 0; t < minDfaStateNum; t++)
				{
					if(s[t].count(DfaStates[*y].Edges[j].Trans))
					{
						
						bool haveEdge = false;		/*�жϸû��Ƿ��Ѿ������ı�־*/
						for(int l = 0; l < minDfaStates[i].edgeNum; l++)	/*�����Ѵ����Ļ�*/
						{					/*����û��Ѿ�����*/
							if((minDfaStates[i].Edges[l].input == DfaStates[*y].Edges[j].input) && (minDfaStates[i].Edges[l].Trans == t))
							{
								haveEdge = true;		/*��־Ϊ��*/
							}
						}
						
						if(!haveEdge)		/*����û������ڣ��򴴽�һ���µĻ�*/
						{
							
							minDfaStates[i].Edges[minDfaStates[i].edgeNum].input = DfaStates[*y].Edges[j].input;	/*����ֵ��DFA����ͬ*/
							minDfaStates[i].Edges[minDfaStates[i].edgeNum].Trans = t;	/*�û�ת�Ƶ���״̬Ϊ���״̬���ı��*/
							
							minDfa.trans[i][DfaStates[*y].Edges[j].input - 'a'] = t;	/*����ת�ƾ���*/
							
							minDfaStates[i].edgeNum++;		/*��״̬�Ļ�����Ŀ��һ*/
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
	cout<<"minDFA�ܹ���"<<minDfaStateNum<<"��״̬��"<<"��̬Ϊ"<<d.startState<<endl<<endl;
	
	cout<<"������ĸ��Ϊ�� ";
	set<char>::iterator it;
	for(it = d.terminator.begin(); it != d.terminator.end(); it++)
	{
		cout<<*it<<' ';
	}
	cout<<'}'<<endl<<endl;
	
	cout<<"��̬��Ϊ�� "; 
	IntSet::iterator iter;
	for(iter = d.endStates.begin(); iter != d.endStates.end(); iter++)
	{
		cout<<*iter<<' ';
	}
	cout<<'}'<<endl<<endl;
	
	cout<<"ת�ƺ���Ϊ��"<<endl;
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
	
	cout<<endl<<"ת�ƾ���Ϊ��"<<endl<<"     ";
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
// //��minDFA��ת�ƺ������ΪGraphviz DOT���Ե��ļ�
void printMinDFAGraph(DFA dfa, string filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "�޷������ļ���" << filename << endl;
        return;
    }

    // д��DOT�ļ�ͷ����Ϣ
    outFile << "digraph MinDFA {" << endl;
    outFile << "\trankdir=LR;" << endl;
    outFile << "\tnode [shape=circle];" << endl;

    // ���minDFA״̬֮���ת�ƹ�ϵ
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

    // д��DOT�ļ�β����Ϣ
    outFile << "}" << endl;

    outFile.close();
    cout << "�ѳɹ���minDFAת�ƺ���������ļ���" << filename << endl;
	cout<<endl<<"*******************************************"<<endl;
}

