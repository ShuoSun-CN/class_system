#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<fstream>
#include<cstring>
#include<iomanip>
#include<ctime>
#include<cstdlib>
#include"graph.h"
using namespace std;

struct CourseType
{

	char courseNo[5];
	char courseName[100];
	int period;
	int term;
	string preNeed[10];
	int preNeednum = 0;
	CourseType& operator =(const CourseType& e)
	{
		
		strcpy(courseNo, e.courseNo);
		strcpy(courseName, e.courseName);
		period = e.period;
		term = e.term;
		for (int i = 0;i < e.preNeednum;i++)
		{
			preNeed[i] = e.preNeed[i];
		}
		preNeednum = e.preNeednum;
		return *this;

	}
};
class ClassSchedule
{
private:
	int* indegree;  //储存每节课节点的入度
	int Classes_num;   //总共有多少课
	LinkQueue<CourseType> termq[9];//每学期
	string coursename[10][5];//储存每一节次的课程名称
	int weekday[5][10];  //每个学期工作日的占用情况
public:
	
	AdjMatrixDirGraph<CourseType> *coursegraph;//课程形成的图

	fstream infile;
	CourseType Classes[100];//储存读取的课程信息

	ClassSchedule();
	void ReadClasses();//读取课程
	void ShowClassInf();//展示原始课程数据
	void GetInDegree();//获取每个节点的入度
	bool TopSort();//拓扑排序
	void Write();//将每学期相应的课排到每天写入到文件中
	void Schedule(CourseType &cs);//给指定的课将它排到工作日
	bool Arrange3(int day, string name);//安排三学时的课到工作日
	bool Arrange2(int day, string name);//安排两学时的课到工作日
	int have3(int day);//返回某天是否含有三课时的空档，是上午还是下午
	int have2(int day);//返回某天时候含有两学时的空档，是上午还是下午
	string getchinese(int a);//输入阿拉伯数字给出相应的中文字符
 
};

int ClassSchedule::have3(int day)
{
	bool mor=true, aft = true;
	for (int i = 2;i < 5;i++)
	{
		if (weekday[day - 1][i] != 0)
		{
			mor = false;
			break;
		}
	}
	for (int i = 7;i < 10;i++)
	{
		if (weekday[day - 1][i] != 0)
		{
			aft = false;
			break;
		}
	}
	if (!mor && aft)
		return 1;
	if (mor && !aft)
		return 0;
	if (!mor && !aft)
		return -1;
	if (mor && aft)
	{
		srand((unsigned)time(NULL));
		return (rand() % 2);
	}
}
int ClassSchedule::have2(int day)
{
	bool mor=true, aft = true;
	for (int i = 0;i < 2;i++)
	{
		if (weekday[day - 1][i] != 0)
		{
			mor = false;
			break;
		}
	}
	for (int i = 5;i < 7;i++)
	{
		if (weekday[day - 1][i] != 0)
		{
			aft = false;
			break;
		}
	}
	if (!mor && aft)
		return 1;
	if (mor && !aft)
		return 0;
	if (!mor && !aft)
		return -1;
	if (mor && aft)
	{
		srand((unsigned)time(NULL));
		return (rand() % 2);
	}
}

bool ClassSchedule::Arrange2(int day, string name)
{
	int now = have2(day);
	if (now == -1)
		return false;
	if (now == 0)
	{
		coursename[0][day - 1] = coursename[1][day - 1] = name;
		weekday[day - 1][0] = weekday[day - 1][1] = 1;
		return true;
	}
	if (now == 1)
	{
		coursename[5][day - 1] = coursename[6][day - 1] = name;
		weekday[day - 1][5] = weekday[day - 1][6] = 1;
		return true;
	}
}
bool ClassSchedule::Arrange3(int day, string name)
{
	int now = have3(day);
	if (now == -1)
		return false;
	if (now == 0)
	{
		coursename[2][day - 1] = coursename[3][day - 1] = coursename[4][day - 1] = name;
		weekday[day - 1][2] = weekday[day - 1][3] = weekday[day - 1][4] = 1;
		return true;
	}
	if (now == 1)
	{
		coursename[7][day - 1] = coursename[8][day - 1] = coursename[9][day - 1] = name;
		weekday[day - 1][7] = weekday[day - 1][8] = weekday[day - 1][9] = 1;
		return true;
	}
}


void ClassSchedule::ReadClasses()
{
	int tem_class = 0;
	while (!infile.eof())
	{
	infile >> Classes[tem_class].courseNo >> Classes[tem_class].courseName >> Classes[tem_class].period >> Classes[tem_class].term;//将常规的文件读取，四个数据
	char temp = infile.get();//判断是否有前提条件
	while (temp != '\n'&&(!infile.eof()))//如果读取四个数据后,不换行说明可能存在先修课程
	{
		if (temp == ' ')//如果是空格就继续读取
		{
			temp = infile.get();
			continue;
		}
		infile.putback(temp);//如果不是空格那么久将该字符退回到输入流中，将数据读取到先修课程之一

		infile >> Classes[tem_class].preNeed[Classes[tem_class].preNeednum];//读取先修课程并且将当前课程的先修课程要求数+1
		Classes[tem_class].preNeednum++;
		temp = infile.get();
	}
	tem_class++;//一个课程数据读取完进行下一个数据的读取

	}
	Classes_num = tem_class;
}
ClassSchedule::ClassSchedule()
{

	char infilename[256];
	cout << "请输入课程表单文件名:";
	cin >> infilename;
	infile.open(infilename, ios::in);
	ReadClasses();//读取课程表的信息
	coursegraph = new  AdjMatrixDirGraph<CourseType>(Classes,Classes_num);//同时将有向图初始化
	indegree = new int[Classes_num];
	coursegraph->GenerateEdge();

}
void ClassSchedule::ShowClassInf()
{
	for (int i = 0;i < Classes_num;i++)
	{
		cout <<left<<setw(10)<< Classes[i].courseNo << left << setw(20) << Classes[i].courseName << left << setw(8) << Classes[i].period << left << setw(8) << Classes[i].term << left << setw(8) << endl;
	}
}//展示读取的课程文件的课程
void ClassSchedule::GetInDegree()
{
	for (int i = 0;i < coursegraph->GetVexNum();i++)
	{
		indegree[i] = 0;
	}
	for (int i = 0;i < coursegraph->GetVexNum();i++)
	{
		for (int j = coursegraph->FirstAdjVex(i);j != -1;j = coursegraph->NextAdjVex(i, j))
		{
			indegree[j]++;
		}
	}
}
bool ClassSchedule::TopSort()
{
	int count = 0;
	int termNo0 = 0;//确定有固定的学期课的数目
	int coursenumperterm[9] = {0};
	for (int v = 0;v < Classes_num;v++)
	{
		int tempterm = Classes[v].term;
		if (tempterm != 0)
		{
			coursenumperterm[tempterm]++;//并把该学期的上课长度确定
			termNo0++;
			termq[tempterm].InQueue(Classes[v]);//将有固定的学期的课放入相应学期的队列中
			count++;
		}
	}
	GetInDegree();//统计节点入度数

	LinkQueue<int> Indegree0;
	LinkQueue<int> lastsort;
	for (int v = 0;v < coursegraph->GetVexNum();v++)
	{
		int tempterm = Classes[v].term;
		if (tempterm == 0)
		{
			if (indegree[v] == 0)
			{
				Indegree0.InQueue(v);

			}
		}
		
	}

	
	while (!Indegree0.Empty())
	{
		int v1;
		Indegree0.OutQueue(v1);
		lastsort.InQueue(v1);
		count++;
		for (int v2 = coursegraph->FirstAdjVex(v1);v2 != -1;v2 = coursegraph->NextAdjVex(v1, v2))
		{
			if (--indegree[v2] == 0)
			{
				Indegree0.InQueue(v2);
			}
		}
	}
	if (count < Classes_num)
	{
		return false;//有闭环退出
	}
	//现在根据课程数均匀的将电脑排序的课程排给每个学期
	int average = Classes_num / 8;
	int rest = Classes_num % 8;
	for (int t = 1;t < 9;t++)
	{
		int bias = 0;
		if (t <= rest)
		{
			bias = 1;
		}//将剩余课程数均匀的分给每个学期
		for (int i = termq[t].Length();i <average+bias;i++)
		{
			int temp;
			lastsort.OutQueue(temp);
			termq[t].InQueue(Classes[temp]);
		}
	}
	return true;//排序完成

}
void ClassSchedule::Schedule(CourseType&e)
{
	if (e.period == 6)
	{
		int day;
		for (day = 1;day < 6;day++)
		{
			if (have3(day) !=-1)
			{
				Arrange3(day, e.courseName);
				break;
			}
		}
		for (day = day + 2;day < 6;day++)
		{
			if (have3(day) != -1)
			{
				Arrange3(day, e.courseName);
				break;
			}
		}
		if (day > 5)
		{
			cout << "安排冲突！" << endl;
			exit(0);
		}
		
	}
	if (e.period == 5)
	{
		int day;
		for (day = 1;day < 6;day++)
		{
			if (have3(day) != -1)
			{
				Arrange3(day, e.courseName);
				break;
			}
		}
		for (day = day + 2;day < 6;day++)
		{
			if (have2(day) != -1)
			{
				Arrange2(day, e.courseName);
				break;
			}
		}
		if (day > 5)
		{
			cout << "安排冲突！" << endl;
			exit(0);
		}
	}
	if (e.period == 4)
	{
		int day;
		for (day = 1;day < 6;day++)
		{
			if (have2(day) != -1)
			{
				Arrange2(day, e.courseName);
				break;
			}
		}
		for (day = day + 2;day < 6;day++)
		{
			if (have2(day) != -1)
			{
				Arrange2(day, e.courseName);
				break;
			}
		}
		if (day > 5)
		{
			cout << "安排冲突！" << endl;
			exit(0);
		}
	}
	if (e.period == 3)
	{
		int day;
		for (day = 1;day < 6;day++)
		{
			if (have3(day) != -1)
			{
				Arrange3(day, e.courseName);
				break;
			}
		}
		if (day > 5)
		{
			cout << "安排冲突！" << endl;
			exit(0);
		}
	}
}
void ClassSchedule::Write()
{


	char schedulename[] = "schedule_term0.doc";
	
	for (int i = 1;i < 9;i++)
	{
		
		schedulename[strlen(schedulename) - 5] += 1;
		fstream schedule;
		schedule.open(schedulename, ios::out);
		schedule << "第" << getchinese(i) << "学期课程表" << endl;
		schedule << endl;
		schedule << left << setw(10) << "节次" << left << setw(20) << "星期一" << left << setw(20) << "星期二" << left << setw(20) << "星期三"
			<< left << setw(20) << "星期四" << left << setw(20) << "星期五" << endl;
		schedule << endl;

		for (int temp = 0;temp < 5;temp++)
		{
			for (int j = 0;j < 10;j++)
			{
				weekday[temp][j] = 0;
			}
		}
		for (int temp = 0;temp < 10;temp++)
		{
			for (int j = 0;j < 5;j++)
			{
				coursename[temp][j] = " ";
			}
		}

		CourseType Class;
		while (!termq[i].Empty())
		{
			termq[i].OutQueue(Class);
			Schedule(Class);
		}
		for (int jie = 0;jie < 10;jie++)
		{
			string jieci = "第" + getchinese(jie+1) + "节";
			schedule  << left<<setw(10)<<jieci<< left << setw(20) << coursename[jie][0] << left << setw(20)
				<< coursename[jie][1] << left << setw(20)
				<< coursename[jie][2] << left << setw(20)
				<< coursename[jie][3] << left << setw(20)
				<< coursename[jie][4] << endl;
			if (jie == 1)
			{
				schedule << endl;
				schedule << "课间休息" << endl;
				schedule << endl;
			}
			if (jie == 4)
			{
				schedule << endl;
				schedule << "午间休息" << endl;
				schedule << endl;
			}
			if (jie == 6)
			{
				schedule << endl;
				schedule << "课间休息" << endl;
				schedule << endl;
			}
			if (jie == 9)
			{
				schedule << endl;
				schedule << "晚自习" << endl;
				schedule << endl;
			}
		}
		schedule.close();
		
	}
}
string ClassSchedule::getchinese(int a)
{
	switch (a)
	{
	case 1:
		return "一";
	case 2:
		return "二";
	case 3:
		return "三";
	case 4:
		return "四";
	case 5:
		return "五";
	case 6:
		return "六";
	case 7:
		return "七";
	case 8:
		return "八";
	case 9:
		return "九";
	case 10:
		return "十";

		
	}
}
