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
	int* indegree;  //����ÿ�ڿνڵ�����
	int Classes_num;   //�ܹ��ж��ٿ�
	LinkQueue<CourseType> termq[9];//ÿѧ��
	string coursename[10][5];//����ÿһ�ڴεĿγ�����
	int weekday[5][10];  //ÿ��ѧ�ڹ����յ�ռ�����
public:
	
	AdjMatrixDirGraph<CourseType> *coursegraph;//�γ��γɵ�ͼ

	fstream infile;
	CourseType Classes[100];//�����ȡ�Ŀγ���Ϣ

	ClassSchedule();
	void ReadClasses();//��ȡ�γ�
	void ShowClassInf();//չʾԭʼ�γ�����
	void GetInDegree();//��ȡÿ���ڵ�����
	bool TopSort();//��������
	void Write();//��ÿѧ����Ӧ�Ŀ��ŵ�ÿ��д�뵽�ļ���
	void Schedule(CourseType &cs);//��ָ���Ŀν����ŵ�������
	bool Arrange3(int day, string name);//������ѧʱ�Ŀε�������
	bool Arrange2(int day, string name);//������ѧʱ�Ŀε�������
	int have3(int day);//����ĳ���Ƿ�������ʱ�Ŀյ��������绹������
	int have2(int day);//����ĳ��ʱ������ѧʱ�Ŀյ��������绹������
	string getchinese(int a);//���밢�������ָ�����Ӧ�������ַ�
 
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
	infile >> Classes[tem_class].courseNo >> Classes[tem_class].courseName >> Classes[tem_class].period >> Classes[tem_class].term;//��������ļ���ȡ���ĸ�����
	char temp = infile.get();//�ж��Ƿ���ǰ������
	while (temp != '\n'&&(!infile.eof()))//�����ȡ�ĸ����ݺ�,������˵�����ܴ������޿γ�
	{
		if (temp == ' ')//����ǿո�ͼ�����ȡ
		{
			temp = infile.get();
			continue;
		}
		infile.putback(temp);//������ǿո���ô�ý����ַ��˻ص��������У������ݶ�ȡ�����޿γ�֮һ

		infile >> Classes[tem_class].preNeed[Classes[tem_class].preNeednum];//��ȡ���޿γ̲��ҽ���ǰ�γ̵����޿γ�Ҫ����+1
		Classes[tem_class].preNeednum++;
		temp = infile.get();
	}
	tem_class++;//һ���γ����ݶ�ȡ�������һ�����ݵĶ�ȡ

	}
	Classes_num = tem_class;
}
ClassSchedule::ClassSchedule()
{

	char infilename[256];
	cout << "������γ̱��ļ���:";
	cin >> infilename;
	infile.open(infilename, ios::in);
	ReadClasses();//��ȡ�γ̱����Ϣ
	coursegraph = new  AdjMatrixDirGraph<CourseType>(Classes,Classes_num);//ͬʱ������ͼ��ʼ��
	indegree = new int[Classes_num];
	coursegraph->GenerateEdge();

}
void ClassSchedule::ShowClassInf()
{
	for (int i = 0;i < Classes_num;i++)
	{
		cout <<left<<setw(10)<< Classes[i].courseNo << left << setw(20) << Classes[i].courseName << left << setw(8) << Classes[i].period << left << setw(8) << Classes[i].term << left << setw(8) << endl;
	}
}//չʾ��ȡ�Ŀγ��ļ��Ŀγ�
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
	int termNo0 = 0;//ȷ���й̶���ѧ�ڿε���Ŀ
	int coursenumperterm[9] = {0};
	for (int v = 0;v < Classes_num;v++)
	{
		int tempterm = Classes[v].term;
		if (tempterm != 0)
		{
			coursenumperterm[tempterm]++;//���Ѹ�ѧ�ڵ��Ͽγ���ȷ��
			termNo0++;
			termq[tempterm].InQueue(Classes[v]);//���й̶���ѧ�ڵĿη�����Ӧѧ�ڵĶ�����
			count++;
		}
	}
	GetInDegree();//ͳ�ƽڵ������

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
		return false;//�бջ��˳�
	}
	//���ڸ��ݿγ������ȵĽ���������Ŀγ��Ÿ�ÿ��ѧ��
	int average = Classes_num / 8;
	int rest = Classes_num % 8;
	for (int t = 1;t < 9;t++)
	{
		int bias = 0;
		if (t <= rest)
		{
			bias = 1;
		}//��ʣ��γ������ȵķָ�ÿ��ѧ��
		for (int i = termq[t].Length();i <average+bias;i++)
		{
			int temp;
			lastsort.OutQueue(temp);
			termq[t].InQueue(Classes[temp]);
		}
	}
	return true;//�������

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
			cout << "���ų�ͻ��" << endl;
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
			cout << "���ų�ͻ��" << endl;
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
			cout << "���ų�ͻ��" << endl;
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
			cout << "���ų�ͻ��" << endl;
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
		schedule << "��" << getchinese(i) << "ѧ�ڿγ̱�" << endl;
		schedule << endl;
		schedule << left << setw(10) << "�ڴ�" << left << setw(20) << "����һ" << left << setw(20) << "���ڶ�" << left << setw(20) << "������"
			<< left << setw(20) << "������" << left << setw(20) << "������" << endl;
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
			string jieci = "��" + getchinese(jie+1) + "��";
			schedule  << left<<setw(10)<<jieci<< left << setw(20) << coursename[jie][0] << left << setw(20)
				<< coursename[jie][1] << left << setw(20)
				<< coursename[jie][2] << left << setw(20)
				<< coursename[jie][3] << left << setw(20)
				<< coursename[jie][4] << endl;
			if (jie == 1)
			{
				schedule << endl;
				schedule << "�μ���Ϣ" << endl;
				schedule << endl;
			}
			if (jie == 4)
			{
				schedule << endl;
				schedule << "�����Ϣ" << endl;
				schedule << endl;
			}
			if (jie == 6)
			{
				schedule << endl;
				schedule << "�μ���Ϣ" << endl;
				schedule << endl;
			}
			if (jie == 9)
			{
				schedule << endl;
				schedule << "����ϰ" << endl;
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
		return "һ";
	case 2:
		return "��";
	case 3:
		return "��";
	case 4:
		return "��";
	case 5:
		return "��";
	case 6:
		return "��";
	case 7:
		return "��";
	case 8:
		return "��";
	case 9:
		return "��";
	case 10:
		return "ʮ";

		
	}
}
