#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<stack>
#include<set>
#include<queue>

#define MAX 128

using namespace std;

typedef set<int> IntSet;
typedef set<char> CharSet;
/*���ַ���s��nλ��������ַ�ch*/
void insert(string &s, int n, char ch)
{
	
	s += '#';
	
	for(int i = s.size() - 1; i > n; i--)
	{
		s[i] = s[i - 1];
	}
	
	s[n] = ch;
}

/*���ַ���s����Ԥ�����ڵ�һλ�ǲ���������*����)���ҵڶ�λ�ǲ�������(��֮��������ӷ���&��*/ 
void preprocess(string &s)
{
	
	int i = 0 , length = s.size();
	
	while(i < length)
	{
		if((s[i] >= 'a' && s[i] <= 'z')||s[i]=='#' || (s[i] == '*') || (s[i] == ')'))
		{
			if((s[i + 1] >= 'a' && s[i + 1] <= 'z')||s[i+1]=='#' || s[i + 1] == '(')
			{
			
				insert(s, i+1 , '&');
				length ++;
			}
		}

		i++;
	}
} 

/*��׺ת��׺ʱ�õ������ȼ��Ƚϣ���Ϊÿ����������һ��Ȩ�أ�ͨ��Ȩ�ش�С�Ƚ����ȼ�*/
int priority(char ch)
{

	if(ch == '*')
	{
		return 3;
	}
		
	if(ch == '&')
	{
		return 2;
	}
		
	if(ch == '|')
	{
		return 1;
	}
	
	if(ch == '(')
	{
		return 0;
	}
	return -1;
}

/*��׺���ʽת��׺���ʽ*/
string infixToSuffix(string s)
{
	
	preprocess(s);			/*���ַ�������Ԥ����*/
	
	string str;				/*Ҫ����ĺ�׺�ַ���*/
	stack<char> oper;		/*�����ջ*/
	
	for(int i = 0; i < s.size(); i++)
	{
		
		if(s[i] >= 'a' && s[i] <= 'z'||s[i]=='#' )	/*����ǲ�����ֱ�����*/
		{
			str += s[i];
		} 
		else							/*���������ʱ*/ 
		{
			
			if(s[i] == '(')			/*����������ѹ��ջ��*/
			{
				oper.push(s[i]);
			} 
			
			else if(s[i] == ')')	/*����������ʱ*/
			{
					
				char ch = oper.top();
				while(ch != '(')		/*��ջ��Ԫ�س�ջ��ֱ��ջ��Ϊ������*/
				{
					
					str += ch;
				
					oper.pop();
					ch = oper.top();
				}
				
				oper.pop();				/*��������ų�ջ*/ 
			}
			else					/*��������������ʱ*/ 
			{
				
				if(!oper.empty())			/*���ջ��Ϊ��*/ 
				{
					
					char ch = oper.top();
					while(priority(ch) >= priority(s[i]))	/*����ջ�����ȼ����ڵ��ڵ�ǰ������������*/ 
					{
						
						str +=	ch;
						oper.pop();
						
						if(oper.empty())	/*���ջΪ�������ѭ��*/ 
						{
							break;
						} 								
						else ch = oper.top();
					} 

					oper.push(s[i]);		/*�ٽ���ǰ�������ջ*/ 
				}
				
				else				/*���ջΪ�գ�ֱ�ӽ��������ջ*/
				{
					oper.push(s[i]);
				}
			}
		}
	}
	
	/*������ջ��Ϊ�գ����ջ��������ַ���*/
	while(!oper.empty())
	{
		
		char ch = oper.top();
		oper.pop();
		
		str += ch;
	}
	
	cout<<"*******************************************"<<endl<<endl;
	cout<<"��׺���ʽΪ��"<<s<<endl<<endl; 
	cout<<"��׺���ʽΪ��"<<str<<endl<<endl;

	return str;
} 
