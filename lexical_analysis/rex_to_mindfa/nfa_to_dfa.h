#include"rex_to_nfa.h"
/********************NFAתDFA********************/

struct Edge			/*����DFA��ת����*/
{
	
	char input;			/*���ϵ�ֵ*/
	int Trans;			/*����ָ���״̬��*/
};

struct DfaState		/*����DFA״̬*/
{
	
	bool isEnd;			/*�Ƿ�Ϊ��̬����Ϊtrue������Ϊfalse*/
	
	int index;			/*DFA״̬��״̬��*/
	IntSet closure;		/*NFA�Ħ�-move()�հ�*/
	
	int edgeNum;		/*DFA״̬�ϵ��������*/
	Edge Edges[10];		/*DFA״̬�ϵ������*/
};

DfaState DfaStates[MAX];		/*DFA״̬����*/
int dfaStateNum = 0;			/*DFA״̬����*/

struct DFA			/*����DFA�ṹ*/
{
	
	int startState;				/*DFA�ĳ�̬*/
	
	set<int> endStates;			/*DFA����̬��*/
	set<char> terminator;		/*DFA���ս����*/
	
	int trans[MAX][26];		/*DFA��ת�ƾ���*/
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
/*��һ��״̬���Ħ�-cloure*/
IntSet epcloure(IntSet s)
{
	
	stack<int> epStack;		/*(�˴�ջ�Ͷ��о���)*/

	IntSet::iterator it;
	for(it = s.begin(); it != s.end(); it++)
	{
		epStack.push(*it);			/*����״̬���е�ÿһ��Ԫ�ض�ѹ��ջ��*/
	}
	
	while(!epStack.empty())			/*ֻҪջ��Ϊ��*/
	{
		
		int temp = epStack.top();		/*��ջ�е���һ��Ԫ��*/
		epStack.pop();
		
		IntSet::iterator iter;
		for(iter = NfaStates[temp].epTrans.begin(); iter != NfaStates[temp].epTrans.end(); iter++)
		{
			if(!s.count(*iter))				/*������ͨ������ת������״̬��*/
			{								/*�����ǰԪ��û���ڼ����г���*/
				s.insert(*iter);			/*��������뼯����*/
				epStack.push(*iter);		/*ͬʱѹ��ջ��*/
			}
		}
	}
	
	return s;		/*����s��Ϊ��-cloure*/
}

/*��һ��״̬��s�Ħ�-cloure(move(ch))*/
IntSet moveEpCloure(IntSet s, char ch)
{
	
	IntSet temp;
	
	IntSet::iterator it;
	for(it = s.begin(); it != s.end(); it++)		/*������ǰ����s�е�ÿ��Ԫ��*/
	{
		if(NfaStates[*it].input == ch)				/*�����Ӧת�����ϵ�ֵΪch*/
		{
			temp.insert(NfaStates[*it].chTrans);		/*��Ѹû�ͨ��chת������״̬���뵽����temp��*/
		}
	}
	
	temp = epcloure(temp);			/*�����temp�Ħűհ�*/
	return temp;
}

/*�ж�һ��״̬�Ƿ�Ϊ��̬*/
bool IsEnd(NFA n, IntSet s)
{
	
	IntSet::iterator it;
	for(it = s.begin(); it != s.end(); it++)	/*������״̬��������nfa״̬��*/
	{
		if(*it == n.tail->index)				/*�������nfa����̬�����״̬Ϊ��̬������true*/
		{
			return true;
		}
	}
	
	return false;		/*�����������������̬������false*/
}

/*nfaתdfa������*/
DFA nfaToDfa(NFA n, string str)		/*����Ϊnfa�ͺ�׺���ʽ*/
{
	
	cout<<"***************     DFA     ***************"<<endl<<endl; 
	
	int i;
	DFA d;
	set<IntSet> states;		/*����һ���洢�������ϵļ��ϣ������ж����һ��״̬��s�Ħ�-cloure(move(ch))���Ƿ������״̬*/
	initDFA(DfaStates);
	memset(d.trans, -1, sizeof(d.trans));	/*��ʼ��dfa��ת�ƾ���*/ 
	
	for(i = 0; i < str.size(); i++)			/*������׺���ʽ*/
	{
		if(str[i] >= 'a' && str[i] <= 'z')		/*�����������������������뵽dfa���ս������*/
		{
			d.terminator.insert(str[i]);
		}
	}
	
	d.startState = 0;		/*dfa�ĳ�̬Ϊ0*/
	
	IntSet tempSet;
	tempSet.insert(n.head->index);	   	/*��nfa�ĳ�̬���뵽������*/
	
	DfaStates[0].closure = epcloure(tempSet);		/*��dfa�ĳ�̬*/
	DfaStates[0].isEnd = IsEnd(n, DfaStates[0].closure);		/*�жϳ�̬�Ƿ�Ϊ��̬*/
	
	dfaStateNum++;			/*dfa������һ*/
	
	queue<int> q;
	q.push(d.startState);		/*��dfa�ĳ�̬���������(�˴�ջ�Ͷ��о���)*/
	
	while(!q.empty())		/*ֻҪ���в�Ϊ�գ���һֱѭ��*/
	{
		
		int num = q.front();				/*��ȥ����Ԫ��*/
		q.pop();
		
		CharSet::iterator it;
		for(it = d.terminator.begin(); it != d.terminator.end(); it++)		/*�����ս����*/
		{
			
			IntSet temp = moveEpCloure(DfaStates[num].closure, *it);		/*����ÿ���ս���Ħ�-cloure(move(ch))*/
			//��ӡÿ�λ��� 
			IntSet::iterator t;
			cout<<endl;
			for(t = temp.begin(); t != temp.end(); t++)   
			{
				cout<<*t<<' ';
			}
			cout<<endl;
			//??�������������̬����ʼ״̬
			if(!states.count(temp) && !temp.empty())	/*����������״̬����Ϊ������֮ǰ�������״̬����ͬ�����½�һ��DFA״̬*/
			{
				
				states.insert(temp);				/*�����������״̬�����뵽״̬������*/

				DfaStates[dfaStateNum].closure = temp;
				
				DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;				/*��״̬�������뼴Ϊ��ǰ�ս��*/
				DfaStates[num].Edges[DfaStates[num].edgeNum].Trans = dfaStateNum;		/*��ת�Ƶ���״̬Ϊ���һ��DFA״̬*/
				DfaStates[num].edgeNum++;//??����	edgeNumû�г�ʼ��Ϊ0����ֱ��++��											/*��״̬������Ŀ��һ*/
				
				d.trans[num][*it - 'a'] = dfaStateNum;		/*����ת�ƾ���*/
				
				DfaStates[dfaStateNum].isEnd = IsEnd(n, DfaStates[dfaStateNum].closure);	/*�ж��Ƿ�Ϊ��̬*/
				
				q.push(dfaStateNum);		/*���µ�״̬�ż��������*/
				
				dfaStateNum++;		/*DFA״̬������һ*/
			}
			else			/*�������״̬����֮ǰ�����ĳ��״̬����ͬ*/
			{
				for(i = 0; i < dfaStateNum; i++)		/*����֮ǰ�������״̬����*/
				{
					if(temp == DfaStates[i].closure)		/*�ҵ���ü�����ͬ��DFA״̬*/
					{
						
						DfaStates[num].Edges[DfaStates[num].edgeNum].input = *it;		/*��״̬�������뼴Ϊ��ǰ�ս��*/
						DfaStates[num].Edges[DfaStates[num].edgeNum].Trans = i;			/*�û�ת�Ƶ���״̬��Ϊi*/
						DfaStates[num].edgeNum++;										/*��״̬������Ŀ��һ*/
						
						d.trans[num][*it - 'a'] = i;		/*����ת�ƾ���*/
						
						break;
					}
				}
			}
		}
	}
	
	/*����dfa����̬��*/
	for(i = 0; i < dfaStateNum; i++)	/*����dfa������״̬*/	
	{
		if(DfaStates[i].isEnd == true)		/*�����״̬����̬*/
		{
			d.endStates.insert(i);		/*�򽫸�״̬�ż��뵽dfa����̬����*/
		}
	}
	
	return d;
}

/*��ӡdfa����*/
void printDFA(DFA d)
{
	
	int i, j;
	cout<<"DFA�ܹ���"<<dfaStateNum<<"��״̬��"<<"��̬Ϊ"<<d.startState<<endl<<endl;
	
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
	
	cout<<endl<<"ת�ƾ���Ϊ��"<<endl<<"     ";
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

//dfa���ӻ�
void printDFAGraph(DFA dfa, string filename) {
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "�޷������ļ���" << filename << endl;
        return;
    }

    // д��DOT�ļ�ͷ����Ϣ
    outFile << "digraph DFA {" << endl;
    outFile << "\trankdir=LR;" << endl;
    outFile << "\tnode [shape=circle];" << endl;

    // ���DFA״̬֮���ת�ƹ�ϵ
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

    // д��DOT�ļ�β����Ϣ
    outFile << "}" << endl;

    outFile.close();
    cout << "�ѳɹ���DFAת�ƺ���������ļ���" << filename << endl;
}
