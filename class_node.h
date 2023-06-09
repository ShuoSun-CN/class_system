#include<iostream>
#include<cstring>
struct CourseType
{
	char courseNo[5];
	char courseName[100];
	int period;
	int term;
	string preNeed[10];
	int preNeednum=0;
};