#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<string>
#include<ClassSchedule.h>
using namespace std;
int main()
{


	ClassSchedule aa;
	aa.TopSort();
	aa.Write();
	cout << "�ſ���ɣ�����" << endl;
	cout << "�����ɸ���ѧ����Ӧ�Ŀα��ļ�������(�ļ���Ϊschedule_term)" << endl;
	cout << "�����word�����СС�ڵ���6�ż���" << endl;
	cout << endl;
	while (true)
	{
		int select;
		cout << "1.�鿴ԭʼ�α�����" << endl;
		cout << "2.�鿴ָ��ѧ�ڿα�" << endl;
		cout << "3.�˳�" << endl;
		cout << "������ѡ��:";
		cin >> select;
		if (select == 1)
		{
	         aa.ShowClassInf();
		}

		else if (select == 2)
		{
			fstream infile;
			char filename[] = "schedule_term0.doc";
			cout << "������Ҫ�鿴��ѧ�ڣ�1-8��:" << endl;
			int term0;
			cin >> term0;
			filename[13] +=term0 ;
			infile.open(filename);
			if(infile.is_open())
			{ 			string temp;
			while (getline(infile, temp))
			{
				cout << temp << endl;;
			}
			cout << temp;
			}
			else
			{
				cout << "�ļ��򿪴���" << endl;
			}
			infile.close();

		}
		else
		{
			break;
		}
		
	}


	return 0;
}