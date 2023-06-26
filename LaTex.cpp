#include <iostream>
#include <string.h>
#include <map>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <stack>
#include <windows.h>
#define option_LEN 21
using namespace std;
typedef struct BiNode
{
	string ch;
	struct  BiNode* lchild, * rchild; //���Һ���ָ��
}BiNode, *BiTree;
map<string, int> operatorMp;	//�����׺��ǰ׺ 
/*�ҵ����ȼ���͵��Ǹ��������λ�� ----���λ��Ӧ������������ʾ�����߷�����Ӧ����ʼλ�ú��ַ���
	Ѱ�����һ���������ֻ�е�p=0��ʱ��ſ��ǣ���p��¼���ţ�
	��Ϊ������������һ������������ģ�����������һ������������������
	flag��¼���ֵ�����һ������� ������Ҫ������ȷ��
	*/
//�ٿ���һ�� * / �����ţ� ��׺ 
class Latex_index
{
	private:
	    BiNode *t;
	public:
		Latex_index(); 
		BiTree Formula_Analysis(BiTree& T,string s,int x,int y);
		void build(string s);
		string delete_l_r(string s); 
		string FindLowest(string s, int x, int y, int& c1, int& c2, int &c3, int& flag);
		int FindDivide(string s,int x,int y);
		int Is_cons(string s, int x, int y);
		int Equal_sign(string s);
		bool Is_legal(string str);
		void node_exchange(BiTree T);
		void node_associate(BiTree T); 
		void OrderTraverse(BiTree T);
}; 
Latex_index::Latex_index()
{
	this->t = NULL; 
}
string Latex_index::FindLowest(string s, int x, int y, int& c1, int& c2, int &c3, int& flag)	//�ȷֿ���׺ǰ׺ 
{
	int p = 0;//��¼��������û 
	int is_single = -1;//��¼��+- ^ �����Ƿ񷵻��ַ��������߷��ؿ��ַ�����ҪΪ��һ�����ڵ�ֽ�λ�ÿ��� 
	string opt;//��¼����� 
	for (int i = x; i < y; i++)	//--------������Ҫ����ַ�һ���ж� 
	{
		switch (s[i])
		{
		case'(':
		case'{':
			p++;
			break;
		case')':
		case'}':
			p--;
			break;
		case'+':
		case'-':
			if (!p)		//p>0���������棬p=0���������� 
			{
				c1 = i;	//��¼���ų��ֵ�λ�ã���Ϊ�ҵľ��������������������ȼ���͵��Ǹ�����
				is_single = 1;//�����¼���ֵ��ǵ����ַ��������
				flag = 2;
			}
			break;
		case'*':
		case'/':
			if(!p)
			{
				c2 = i; //------- ��������ŵ�λ�ø�ֵ
				is_single = 1; 
				flag = 2;
			}
			break;
		case'^':
			if (!p)		//������ ���ȼ���
			{
				c3 = i;		//------- ��������ŵ�λ�ø�ֵ  
				is_single = 1;
				flag = 2;
			}
			break;
		case'\\':	//����������ַ�����������������������ǳ������� 
			if (!p && c1 < 0) //���������+ - �Ͳ���Ҫ�ж��������ȼ��ߵķ���
			{
				is_single = 0;
				opt = ""; 	//��¼����ַ�������� 
				int index = i + 1;//���� /
				while (1)
				{//�ж��ַ���Ƭ���Ƿ�ͷ�ı�־��{ } ���� + -  ����   ������ñ�־ ֻ��ÿ�ַ������ж��Ƿ��������
					if (s[index] == '{' || s[index] == '}'|| s[index] == '(' || s[index] == ')' || s[index]=='+'||s[index]=='-')
					break;
					else
					{
						string ch(1, s[index]);
						opt += ch;
						index++;
					}
				}
				map<string, int>::iterator it; //ȡ��opt֮���жϣ���ȡ���ȼ� 
				for (it = operatorMp.begin(); it != operatorMp.end(); it++)
				{
					if (it->first == opt)		//�ҵ����������� 
					{
						switch (it->second) //���⣺ǰ�� ���г�����׺��ǰ׺������ụ��ı����
						{
							case 2://˫Ŀ
							{
								c2 = i;//------- ��������ŵ�λ�ø�ֵ 
								flag = 2;
								opt = "\\" + opt;
								break;
							}
							case 12://˫Ŀǰ׺
							{
								c2 = i;
								flag = 12;
								opt = "\\" + opt;
								break;
							}
							case 1: //��Ŀ 
							{
								if (flag == 0) 
								{
									c2 = i;//------- ��������ŵ�λ�ø�ֵ
									flag = 1;
									opt = "\\" + opt;
									break;
								}
							}
							case 0: //��������
								break;
						}
						break;
					}
				}
				i = index - 1;//��i��ֵ�������Ž�����λ��
			}
			break;
		}
	}
	if (is_single == 1 || is_single == -1)	//������һ����������ȼ�������+ - ^ * / 
		return "";
	else if (is_single == 0)	//�����Ǳ�+-�ߵ����ȼ� ���� 
		return opt;
}
//��һ�����⣺����ֵis_singleû�б仯��-1������ֵӦ���ǡ���;index �� i��λ��Ҫȷ����
//����ǰ׺�����
//�жϺϷ���
int Latex_index::FindDivide(string s,int x,int y)
{
	int p = 1;
	for (int i = x + 1; i < y; i++)	
	{
		if (s[i] == '{')
			p++;
		else if (s[i] == '}')
		{
			p--;
			if (p == 0)
			{
				return i;
			}
		}		
	}
}
int Latex_index::Is_cons(string s, int x, int y) 
{
	if (y - x > 9)
		return 0;
	string str;
	if (s[x] == '\\') //�ж���map���е�
	{
		str = s.substr(x + 1,y - x - 1); //��x+1λ ��ʼ y-x -1λ
		map<string, int>::iterator it;
		for (it = operatorMp.begin(); it != operatorMp.end(); it++)
		{
			if (it->first == str && it->second == 0)	//�����ǳ�������
			{
				return 1;
			}
		}	
		return 0;
	}
	else
		return 0;
}
string Latex_index::delete_l_r(string s)
{
	string str="";
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '\\')
		{
			if (s.length() < i + 5)//�������ʣ�Ĳ���5λ��˵���������   \left( \right) 
			{
				string ch(1, s[i]);
				str += ch;
			}
			else
			{
				string opt1 = s.substr(i + 1, 4);
				string opt2 = s.substr(i + 1, 5);
				int flag=0;
				if(opt1=="left")
				{
					i += 4; 
					flag=1;
				} 
				if(opt2 == "right")
				{
					i += 5;
					flag=1;
				}
				if(flag==0)
				{
					string ch(1, s[i]);
					str += ch;
				}
				
			}
		}
		else
		{
			string ch(1, s[i]);
			str += ch;
		}
	}
	return str;
}
void Latex_index::build(string str)
{
	if(Is_legal(str)) //Ӧ����str 
	{
		string s = delete_l_r(str);
		int len = s.length();
		int eq = Equal_sign(s);
		if (eq) //�м��� = �ֳ����ηֱ���,Ȼ��ϲ���һ�� 
		{
			string left = s.substr(1,eq - 1);
			string right = s.substr(eq + 1, len-eq - 2);
			BiTree T1=NULL,T2=NULL;
			Formula_Analysis(T1, left, 0, left.length());
			Formula_Analysis(T2, right, 0, right.length());
			node_exchange(T1);
			node_associate(T1);
			node_exchange(T2);
			node_associate(T2);
			this->t = new BiNode;
			this->t->ch = '=';
			this->t->lchild=T1;
			this->t->rchild=T2;
			OrderTraverse(this->t);
			cout << endl;
		}
		else
		{
			this->t = Formula_Analysis(this->t, s, 1, len - 1);
			node_exchange(this->t);
			node_associate(this->t);
			OrderTraverse(this->t);
			cout << endl;
		}
	}
	else
	cout<<"���벻�Ϸ���"<<endl;
}
//���⣺ֻ�����˿�ͷ��ǰ׺����� ������������ʽ�Ӷ������������µ� ˵���׾���û�жԱ��������ȼ��Ĵ�С
BiTree Latex_index::Formula_Analysis(BiTree& T, string s, int x, int y)//x��ʾ�ַ�������ʼ��ַ��y��ʾ������ַ��
{
	if (y - x == 1)		//��һ���ַ��������������
	{
		T = new BiNode;
		T->ch = s[x];
		T->lchild = NULL;
		T->rchild = NULL;
		return T;
	}
	else if (Is_cons(s, x, y) == 1) //�ǳ������� 
	{
		T = new BiNode;
		T->ch = s.substr(x, y - x);
		T->lchild = NULL;
		T->rchild = NULL;
		return T;
	}
	//�ж�ǰ׺ �������  ����ȫ�����жϣ�ֻ��������ʽ�� ������Ǿʹ��� 
	//�ҵ�ʽ�������ȼ���͵ķ���
	int c1 = -1, c2 = -1,c3 = -1, flag = 0;
	//c1��¼�����ҡ����ֵļӼ��ţ�p��¼����������
	string opt;
	opt = FindLowest(s, x, y, c1, c2, c3, flag);//��������ж�ȫ��
	if (c1 < 0)//�Ҳ����������������ȼ����š�+-��,��ô��͵����������Ŵ���ģ���������ȼ��ķ�����Ϊ�Ӹ�����
		c1 = c2;
	if (c1 < 0)//���c2û�䣬����ʽ�ӱ�һ�������Ÿ���������
		//����c3����û������ʽ����������ȼ�����^
	{
		if(c3 == -1)
		return Formula_Analysis(T, s, x + 1, y - 1);  //�Ѵ�����ȥ��
	}
	//���Ƕ�����׺�����  ----����flag�ж� 
	if (flag == 2)	//---	������׺����� 
	{
		if (opt == "" && c1 > 0)	// �� + - 
		{
			T = new BiNode;
			T->ch = s[c1];            //���ɸ����
			Formula_Analysis(T->lchild, s, x, c1);  //�ݹ鴴��������
			Formula_Analysis(T->rchild, s, c1 + 1, y); //�ݹ鴴��������
		}
		else if(opt == "" && c3 > 0)	// ��^
		{
			T = new BiNode;
			T->ch = s[c3];            //���ɸ����
			Formula_Analysis(T->lchild, s, x, c3);  //�ݹ鴴��������
			Formula_Analysis(T->rchild, s, c3 + 1, y); //�ݹ鴴��������
		}
		else				  // ��map��������
		{
			T = new BiNode;
			T->ch = opt;            //���ɸ����
			Formula_Analysis(T->lchild, s, x, c1);  //�ݹ鴴��������
			Formula_Analysis(T->rchild, s, c1 + opt.length(), y); //�ݹ鴴��������
		}
		return T;
	}
	else if (flag == 12)	//ǰ׺˫Ŀ�����
	{
		int divide = FindDivide(s, x + opt.length(), y);//�ֽ��{��λ��
		T = new BiNode;
		T->ch = opt;            //���ɸ����
		Formula_Analysis(T->lchild, s, x + opt.length(), divide + 1);  //�ݹ鴴��������
		Formula_Analysis(T->rchild, s, divide + 1, y);		//�ݹ鴴��������
		return T;
	}
	else if (flag == 1)  
	{
		T = new BiNode;
		T->ch = opt;            //���ɸ����
		Formula_Analysis(T->lchild, s, x + opt.length(), y);  //�ݹ鴴��������
		T->rchild = NULL;	//������Ϊ��
		return T;
	}
}

void Latex_index::node_exchange(BiTree T)//������ 
{
	BiTree temp;
	temp = T;
	if(!temp)
	{
		return;
	}
	if(temp->ch=="+" || temp->ch=="\\times")
	{
		if(temp->lchild->lchild || temp->rchild->rchild)
		{
			return;
		}
		if(temp->lchild->ch[0] > temp->rchild->ch[0])
		{
			string exg;
			exg = temp->lchild->ch;
			temp->lchild->ch = temp->rchild->ch;
			temp->rchild->ch = exg;
		}
	}
	node_exchange(temp->lchild);
	node_exchange(temp->rchild);
}

void Latex_index::node_associate(BiTree T)//����� 
{
	BiTree temp;
	temp = T;
	if(temp->ch=="+" || temp->ch=="-")
	{
		//cout<< 1;
		string cheng,chu;
		cheng = "\\times";
		chu = "\\div";
		if((temp->lchild->ch==cheng || temp->lchild->ch==chu) && temp->lchild->ch == temp->rchild->ch)
		{
			//cout<< 2;
			BiTree l,r;
			l = temp->lchild;
			r = temp->rchild;
			if(l->ch==cheng)
			{
				if(l->lchild->ch == r->lchild->ch || l->rchild->ch == r->lchild->ch || l->lchild->ch == r->rchild->ch || l->rchild->ch == r->rchild->ch)
				{
					string ass = temp->ch;
					temp->ch = l->ch;
					string a,b,c,d,mas,div1,div2;
					a = l->lchild->ch;
					b = l->rchild->ch;
					c = r->lchild->ch;
					d = r->rchild->ch;
					if(a==c)
					{
						mas = a;
						div1 = b;
						div2 = d;
					}
					if(a==d)
					{
						mas = a;
						div1 = b;
						div2 = c;
					}
					if(b==c)
					{
						mas = b;
						div1 = a;
						div2 = d;
					}
					if(b==d)
					{
						mas = b;
						div1 = a;
						div2 = c;
					}
					l->ch = mas;
					l->lchild = NULL;
					l->rchild = NULL;
					r->ch = ass;
					r->lchild->ch = div1;
					r->rchild->ch = div2;
				}
			}
			if(l->ch==chu)
			{
				if(l->lchild->ch == r->lchild->ch || l->rchild->ch == r->rchild->ch)
				{
					string ass,a,b,c,d,mas,div1,div2;
					ass = temp->ch;
					temp->ch = l->ch;
					a = l->lchild->ch;
					b = l->rchild->ch;
					c = r->lchild->ch;
					d = r->rchild->ch;
					if(a==c)
					{
						mas = a;
						div1 = b;
						div2 = d;
						l->ch = mas;
						l->lchild = NULL;
						l->rchild = NULL;
						r->ch = ass;
						r->lchild->ch = div1;
						r->rchild->ch = div2;
					}
					if(b==d)
					{
						mas = b;
						div1 = a;
						div2 = c;
						r->ch = mas;
						r->lchild = NULL;
						r->rchild = NULL;
						l->ch = ass;
						l->lchild->ch = div1;
						l->rchild->ch = div2;
					}
				}
			}
		}
	}
	return;
}

void Latex_index::OrderTraverse(BiTree T)
{
	BiTree temp[100];   //����ָ�����͵�ָ������
    int in = 0;
    int out = 0;
    int num = 0;

    temp[in++] = T;  //�ȱ�����������ڵ� 

    while (in > out)
    {
        if (temp[out])
        {
            //cout << temp[out]->ch << " �� ";
            cout << "��" << ++num << "��ֵΪ��" << temp[out]->ch << endl;
            temp[in++] = temp[out]->lchild;
            temp[in++] = temp[out]->rchild;
        }
        out++;
    }
}
// �����ǣ�
// ${{a}\div{{c}+{b}}}$
// $\left(\frac{{a}+{b}}{{c}\times{d}}\right)$ 
// $\sin{{a}+{b}}$
// $\frac{a}{b}$
// $\sqrt{\frac{{a}+{b}}{{c}+{d}}}$
// $\frac{{a}-{1}}{{b}-{1}}$
// $\sqrt{a}+\sqrt{b}$     
// ${\alpha}+{\beta}$
// ����������{}������ ���߲����ж� ������� 
// ${a}^{2}+{b}^{2}$
// $({A}\land{B})\lor({A}\land{C})$
// $\frac{a}{b}=a/b$ 
// û���ǲ��ǵ����ַ��ĳ�������� 2ab  
// left right �������� �ڿ�ʼ��ʱ�� ȥ��ֻʣ����  
// û�����м��пո�  -----��һ��ȥ�ո� 
string option[option_LEN] ={"\\times", "\\div", "\\frac", "\\left(", "\\right)", "\\sqrt", 
							"\\sin", "\\cos", "\\tan", "\\pi", "\\neg", "\\land", "\\lor", 
							"\\oplus", "\\lambda", "\\lnot", "\\alpha", "\\beta", "\\gamma",
							"\\tau"};
bool Latex_index::Is_legal(string str)  		 
{
	int len = str.length();
    int num = 0;//�жϻ����Ÿ����Ƿ�ƥ�� 
	int num_sqrt = 0;//�ж������Ÿ����Ƿ�ƥ��
	int num_left = 0;//�ж�С���Ÿ����Ƿ�ƥ�� 
    int flag = 0;//�ж�LaTex������Ƿ���ȷ 
    int flag_sum = 1;
    string op;
    for(int i = 0; i < len; i++){
	    if(str[i] == '{'){
	    	//cout<<str[i]<<endl;
	       	num++;
		}
	    if(str[i] == '}'){
	    	//cout<<str[i]<<endl;
	        num--;
		}
		if(str[i] == '['){
			num_sqrt++;
			//cout<<str[i]<<endl;
		}
		if(str[i] == ']'){
			num_sqrt--;
			//cout<<str[i]<<endl; 
		}
		if(str[i] == '('){
			num_left++;
			//cout<<str[i]<<endl;
		}
		if(str[i] == ')'){
			num_left--;
			//cout<<str[i]<<endl; 
		}
	    if(str[i] == (char)92){
	    	str[i] = '\\';
	    	op += str[i];
	    	i++;
       		while(str[i] != '{' && str[i] != '=' && str[i] != '/' 
			   && str[i] != '*' && str[i] != '+' && str[i] != '-'
			   && str[i] != '^' && str[i] != '[' && str[i] != '}'
			   && str[i] != '$'){
       			if(str[i] == (char)92){
       				str[i] = '\\';
       				break;
				   }
				else{
					op += str[i];
       				//cout<<"op:"<<op<<endl;
					i++;
				}	
			}
			i--;
			for(int j = 0; j < option_LEN; j++){
				if(op == option[j]){
					flag = 1;
					break;
				}
				else{
					flag = 0;
				}
			}
			//cout<<"--"<<flag<<endl;
			op.clear();
			if(flag == 0){
				flag_sum = 0;
			}
		}	
 	}
 	if(str[0] != '$' || str[len - 1] != '$'){//��ͷ��β������$
	 	cout<<"$����!"<<endl;
		return false;
	} 
    else if(num != 0){//�����Ų�ƥ�� 
		cout<<"�����Ÿ�����ƥ��!"<<endl;
		return false;
	}
	else if(num_sqrt != 0){//�����Ų�ƥ�� 
		cout<<"�����Ÿ�����ƥ��!"<<endl;
		return false;
	}
	else if(num_left != 0){//Բ���Ų�ƥ��
		cout<<"Բ���Ÿ�����ƥ��!"<<endl;
		return false;
	}
	else if(flag_sum != 1){//latex�������ƥ��
		cout<<"LaTex���ű�ʾ����!"<<endl;
		return false;
	} 
	else{return true;}
}
int Latex_index::Equal_sign(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == '=')
		{
			return i;
		}
	}
	return 0;
}
int main()
{
	//��׺����� -----˫Ŀ 
	operatorMp.insert(pair<string, int>{"times", 2});
	operatorMp.insert(pair<string, int>{"div", 2});
	operatorMp.insert(pair<string, int>{"land", 2});	//��
	operatorMp.insert(pair<string, int>{"lor", 2});		//��	
	operatorMp.insert(pair<string, int>{"oplus", 2});	//���
	operatorMp.insert(pair<string, int>{"to", 2});		//
	//ǰ׺�����------˫Ŀ 
	operatorMp.insert(pair<string, int>{"frac", 12});
	//ǰ׺�����------��Ŀ 
	operatorMp.insert(pair<string, int>{"sin", 1});
	operatorMp.insert(pair<string, int>{"cos", 1});
	operatorMp.insert(pair<string, int>{"tan", 1});
	operatorMp.insert(pair<string, int>{"sec", 1});
	operatorMp.insert(pair<string, int>{"neg", 1});
	operatorMp.insert(pair<string, int>{"sqrt", 1});
	//��������&ϣ����ĸ
	operatorMp.insert(pair<string, int>{"pi", 0});
	operatorMp.insert(pair<string, int>{"alpha", 0});
	operatorMp.insert(pair<string, int>{"beta", 0});
	operatorMp.insert(pair<string, int>{"gamma", 0});
	operatorMp.insert(pair<string, int>{"delta", 0});
	operatorMp.insert(pair<string, int>{"lambda", 0});
	operatorMp.insert(pair<string, int>{"tau", 0});
	operatorMp.insert(pair<string, int>{"epsilon", 0});
	operatorMp.insert(pair<string, int>{"sigma", 0});
	while(1)
	{
		string s;
		cout<<"������LaTex��ʽ:";
		cin >> s;	//����Ĺ�ʽ��$ $��ʽ
		Latex_index lt;
		lt.build(s);
		fflush(stdin);
		system("pause");
		system("cls");
	}
	
	return 0;
}
