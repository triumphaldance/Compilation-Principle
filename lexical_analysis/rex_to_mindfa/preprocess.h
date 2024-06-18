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
/*在字符串s第n位后面插入字符ch*/
void insert(string &s, int n, char ch)
{
	
	s += '#';
	
	for(int i = s.size() - 1; i > n; i--)
	{
		s[i] = s[i - 1];
	}
	
	s[n] = ch;
}

/*对字符串s进行预处理，在第一位是操作数、‘*’或‘)’且第二位是操作数或‘(’之间加入连接符‘&’*/ 
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

/*中缀转后缀时用到的优先级比较，即为每个操作符赋一个权重，通过权重大小比较优先级*/
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

/*中缀表达式转后缀表达式*/
string infixToSuffix(string s)
{
	
	preprocess(s);			/*对字符串进行预处理*/
	
	string str;				/*要输出的后缀字符串*/
	stack<char> oper;		/*运算符栈*/
	
	for(int i = 0; i < s.size(); i++)
	{
		
		if(s[i] >= 'a' && s[i] <= 'z'||s[i]=='#' )	/*如果是操作数直接输出*/
		{
			str += s[i];
		} 
		else							/*遇到运算符时*/ 
		{
			
			if(s[i] == '(')			/*遇到左括号压入栈中*/
			{
				oper.push(s[i]);
			} 
			
			else if(s[i] == ')')	/*遇到右括号时*/
			{
					
				char ch = oper.top();
				while(ch != '(')		/*将栈中元素出栈，直到栈顶为左括号*/
				{
					
					str += ch;
				
					oper.pop();
					ch = oper.top();
				}
				
				oper.pop();				/*最后将左括号出栈*/ 
			}
			else					/*遇到其他操作符时*/ 
			{
				
				if(!oper.empty())			/*如果栈不为空*/ 
				{
					
					char ch = oper.top();
					while(priority(ch) >= priority(s[i]))	/*弹出栈中优先级大于等于当前运算符的运算符*/ 
					{
						
						str +=	ch;
						oper.pop();
						
						if(oper.empty())	/*如果栈为空则结束循环*/ 
						{
							break;
						} 								
						else ch = oper.top();
					} 

					oper.push(s[i]);		/*再将当前运算符入栈*/ 
				}
				
				else				/*如果栈为空，直接将运算符入栈*/
				{
					oper.push(s[i]);
				}
			}
		}
	}
	
	/*最后如果栈不为空，则出栈并输出到字符串*/
	while(!oper.empty())
	{
		
		char ch = oper.top();
		oper.pop();
		
		str += ch;
	}
	
	cout<<"*******************************************"<<endl<<endl;
	cout<<"中缀表达式为："<<s<<endl<<endl; 
	cout<<"后缀表达式为："<<str<<endl<<endl;

	return str;
} 
