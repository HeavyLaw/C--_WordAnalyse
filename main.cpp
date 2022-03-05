#include <iostream>
#include <fstream>
#include <string>
#define keynum 8

using namespace std;

int in(char ch,string s);
int ischar(char ch);
int iskeyword(string s);
int isdigit(char ch);

static char *keyword[keynum]={"if","else","for","while","do","int","read","write"};
static string doubleword="+-*/%=!><";
static string singleword="(){};,";
int main()
{


    ifstream fin("test.txt",ios::in);
    ofstream fout;
    if(!fin.is_open())
    {
        cout<<"test��ʧ��!"<<endl;
        return 0;
    }
    fout.open("test1out.txt",ios::out);
	if(!fout.is_open())
    {
        cout<<"test1out��ʧ��!"<<endl;
        return 0;
    }
    /*while(!fin.eof())
    {
        cout<<fin.get()<<" "<<fin.tellg()<<endl;
    }*/
	int row=1;//��¼��ǰ�ļ���ָ���ڵڼ���
    while(!fin.eof())
    {
        string wordclass="";
        string wordvalue="";
        char ch=fin.get();
        if(isdigit(ch)||ch=='.')//Ϊ����,�����򸡵���
        {
			int num_point=0;//��¼���ִ���.�ĸ�������ֹ�Ƿ�����
			bool illegal=false;//�����ֿ�ͷ�ķǷ���ʶ����־
			if(ch=='.') num_point++;
            wordvalue+=ch;
            while(1)
            {
                ch=fin.get();
                if(isdigit(ch))//���ֺ������ֻ�.��
                    wordvalue+=ch;
                else if(ch=='.')
				{
					num_point++;
					wordvalue+=ch;
				}
				else if(ischar(ch)||ch=='_')//���ֿ�ͷ�������ĸ���»��ߣ��Ƿ���ʶ��
				{
					wordvalue+=ch;
					while( (in(ch,singleword)<0) && (in(ch,doubleword)<0) && !(ch==' ') && !(ch=='\n'))//�������Ƿ��ı�ʶ��ʶ�����
					{
						ch=fin.get();
						wordvalue+=ch;
					}
					int p=fin.tellg();
                    fin.seekg(p-1);
					illegal=true;
					break;
				}
				else//�����ֺ�С������ɵ��ַ���ʶ�����
				{
					int p=fin.tellg();
                    fin.seekg(p-1);
					break;
				}
            }

			if(illegal)//Ϊ�Ƿ���ʶ��
			{
				cout<<"�Ƿ���ʶ�����ڵ�"<<row<<"�С���ʾ��"<<wordvalue<<endl;
				fout<<"ERROR"<<"\t"<<wordvalue<<"\n";
				continue;
			}

			if(num_point==0)
				wordclass="NUM";
			else if(num_point==1)
				wordclass="Float";
			else
			{
				cout<<"�Ƿ����룡�ڵ�"<<row<<"�С���ʾ��"<<wordvalue<<endl;
				fout<<"ERROR"<<"\t"<<wordvalue<<"\n";
				continue;
			}
			if(wordvalue[0]=='.')
				wordvalue.insert(0,"0");
			if(wordvalue[wordvalue.length()-1]=='.')
				wordvalue.append("0");
            fout<<wordclass<<"\t"<<wordvalue<<"\n";
			cout<<wordclass<<"\t"<<wordvalue<<"\n";
        }
        else if(in(ch,singleword)>=0)//���ֽ��,ʶ��( ) { } ; ,
        {
            wordclass=ch;
            wordvalue=ch;
            fout<<wordclass<<"\t"<<wordvalue<<"\n";
			cout<<wordclass<<"\t"<<wordvalue<<"\n";
        }
        else if(in(ch,doubleword)>=0)  //ʶ��˫�ֽ��
        {
            char ch2=fin.get();
            string s="  ";
            s[0]=ch;
            s[1]=ch2;
            if((s[1]=='=')||(s=="++")||(s=="--")||(s=="//"))    //ʶ��>= <= == != += -= *= /= %= ++ --
            {
                wordvalue=s;
                wordclass=s;
            }else if(s=="/*"||s=="*/")
            {
                wordvalue=s;
                wordclass="exp";
            }else//�ǵ����ַ���Ҫ����һ���ַ�
            {
                wordvalue=ch;
                wordclass=ch;
				int p=fin.tellg();
                fin.seekg(p-1);
            }
            fout<<wordclass<<"\t"<<wordvalue<<"\n";
			cout<<wordclass<<"\t"<<wordvalue<<"\n";
        }
		else if(ischar(ch)||ch=='_')//ʶ���ʶ��
		{
			wordvalue=ch;
			ch=fin.get();
			while(ischar(ch)||isdigit(ch)||ch=='_')
			{
				wordvalue+=ch;
				ch=fin.get();
			}
			int p=fin.tellg();
            fin.seekg(p-1);//�����һ��������һ��
			if (iskeyword(wordvalue))
			    wordclass=wordvalue;
			else 
				wordclass="ID";
			fout<<wordclass<<"\t"<<wordvalue<<"\n";
			cout<<wordclass<<"\t"<<wordvalue<<"\n";
		}
		else if(ch==' '||ch=='\n')
		{
			if(ch=='\n') row++;
		}
		else if(ch!=-1)//�ļ��������-1�����⽫����ж�Ϊ�Ƿ��ַ�
		{
			cout<<"�Ƿ����룡�ڵ�"<<row<<"�С�"<<endl;
			fout<<"ERROR"<<"\t"<<wordvalue<<"\n";
		}
    }
	fin.close();
	fout.close();
}
int in(char ch,string s)
{
    if(s.find(ch)==string::npos)
        return -1;
    else
        return s.find(ch);
}
int ischar(char ch)
{
	if ((int(ch)>=int('a')&&int(ch)<=int('z')) || (int(ch)>=int('A')&&int(ch)<=int('Z')))
		return 1;
	else return 0;
}
int isdigit(char ch)
{
	if(int(ch)>=int('0')&&int(ch)<=int('9'))
		return 1;
	else return 0;
}
int iskeyword(string s)
{
	for (int i=0;i<keynum;i++)
	{
		if(s==keyword[i])
			return 1;
	}
	return 0;
}
