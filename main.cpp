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
	cout << "排课完成！！！" << endl;
	cout << "已生成各个学期相应的课表文件！！！(文件名为schedule_term)" << endl;
	cout << "请调节word字体大小小于等于6号即可" << endl;
	cout << endl;
	while (true)
	{
		int select;
		cout << "1.查看原始课表数据" << endl;
		cout << "2.查看指定学期课表" << endl;
		cout << "3.退出" << endl;
		cout << "请输入选择:";
		cin >> select;
		if (select == 1)
		{
	         aa.ShowClassInf();
		}

		else if (select == 2)
		{
			fstream infile;
			char filename[] = "schedule_term0.doc";
			cout << "请输入要查看的学期（1-8）:" << endl;
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
				cout << "文件打开错误" << endl;
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