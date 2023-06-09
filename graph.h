
#ifndef __ADJ_MATRIX_DIR_GRAPH_H__
#define __ADJ_MATRIX_DIR_GRAPH_H__

#include <iostream>								// 编译预处理命令
#include<cstring>
#include <cstdlib>								// 含C函数exit()的声明（stdlib.h与cstdlib是C的头文件）
using namespace std;							// 使用命名空间std 
#include "lk_queue.h"							// 链队列

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 10000						// 缺省大小
#endif

// 有向图的邻接矩阵类模板
template <class ElemType>
class AdjMatrixDirGraph
{
protected:
	// 数据成员:
	int vexNum, edgeNum;						// 顶点个数和边数
	int** matrix;								// 邻接矩阵
	ElemType* elems;							// 顶点元素
	mutable bool* tag;							// 顶点标志

// 辅助函数模板:
	void DFS(int v, void (*visit)(const ElemType& e)) const;	// 从顶点v出发进行深度优先搜索图
	void BFS(int v, void (*visit)(const ElemType&)) const;		// 从第顶点v出发进行广度优先搜索图
	int GetIndex(string v);
public:
	// 抽象数据类型方法声明及重载编译系统默认方法声明:
	AdjMatrixDirGraph(ElemType es[], int vertexNum = DEFAULT_SIZE);
	// 构造数据元素为es[]，顶点个数为vertexNum，边数为0的有向图
	virtual ~AdjMatrixDirGraph();				// 析构函数模板
	void DFSTraverse(void (*visit)(const ElemType& e)) const;	// 对图进行深度优先遍历
	void BFSTraverse(void (*visit)(const ElemType&)) const;	// 对图进行广度优先遍历
	bool GetElem(int v, ElemType& e) const;		// 用e返回顶点v的元素值	
	bool SetElem(int v, const ElemType& e);		// 设置顶点v的元素值为e
	int GetVexNum() const;						// 返回顶点个数			 
	int GetEdgeNum() const;						// 返回边数个数			 
	int FirstAdjVex(int v) const;				// 返回顶点v的第一个邻接点			 
	int NextAdjVex(int v1, int v2) const;		// 返回顶点v1的相对于v2的下一个邻接点			 
	void InsertEdge(int v1, int v2);			// 插入顶点为v1和v2的边			 
	void DeleteEdge(int v1, int v2);			// 删除顶点为v1和v2的边			 
	bool GetTag(int v) const;					// 返回顶点v的标志		 
	void SetTag(int v, bool val) const;			// 设置顶点v的标志为val		 
	AdjMatrixDirGraph(const AdjMatrixDirGraph<ElemType>& source);	// 复制构造函数模板
	void GenerateEdge();
	AdjMatrixDirGraph<ElemType>& operator =(const AdjMatrixDirGraph<ElemType>& source);


	// 重载赋值运算符
};

template <class ElemType>
void Display(const AdjMatrixDirGraph<ElemType>& g);						// 显示邻接矩阵有向图


template<class ElemType>
void AdjMatrixDirGraph<ElemType> ::GenerateEdge()
{
	for (int i = 0;i < vexNum;i++)
	{
		for (int j = 0;j < elems[i].preNeednum;j++)
		{
			int index = GetIndex(elems[i].preNeed[j]);
			InsertEdge(index, i);
		}
	}
}


// 有向图的邻接矩阵类模板的实现部分
template <class ElemType>
void AdjMatrixDirGraph<ElemType>::DFS(int v, void (*visit)(const ElemType& e)) const
// 初始条件：存在图
// 操作结果：从顶点v出发进行深度优先搜索图
{
	SetTag(v, true);							// 设置访问标志
	ElemType e;									// 临时变量
	GetElem(v, e);								// 取顶点v的数据元素
	(*visit)(e);								// 访问顶点v的数据元素
	for (int w = FirstAdjVex(v); w != -1; w = NextAdjVex(v, w))
	{	// 对v的尚未访问过的邻接顶点w递归调用DFS
		if (!GetTag(w))	DFS(w, visit);
	}
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::DFSTraverse(void (*visit)(const ElemType& e)) const
// 初始条件：存在图
// 操作结果：对图进行深度优先遍历
{
	int v;
	for (v = 0; v < GetVexNum(); v++)
	{	// 对每个顶点设置访问标志
		SetTag(v, false);
	}

	for (v = 0; v < GetVexNum(); v++)
	{	// 对尚未访问的顶点按DFS进行深度优先搜索
		if (!GetTag(v))	DFS(v, visit);
	}
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::BFS(int v, void (*visit)(const ElemType&)) const
// 初始条件：存在图
// 操作结果：从第顶点v出发进行广度优先搜索图
{
	SetTag(v, true);						// 设置访问标志
	ElemType e;								// 临时变量
	GetElem(v, e);							// 取顶点v的数据元素
	(*visit)(e);							// 访问顶点v的数据元素
	LinkQueue<int> q;						// 定义队列
	q.InQueue(v);							// v入队
	while (!q.Empty())
	{	// 队列q非空, 进行循环
		int u, w;							// 临时顶点
		q.OutQueue(u);						// 出队
		for (w = FirstAdjVex(u); w >= 0; w = NextAdjVex(u, w))
		{	// 对u尚未访问过的邻接顶点w进行访问
			if (!GetTag(w))
			{	// 对w进行访问
				SetTag(w, true);			// 设置访问标志
				GetElem(w, e);				// 取顶点w的数据元素
				(*visit)(e);				// 访问顶点w的数据元素
				q.InQueue(w);				// w入队
			}
		}
	}
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::BFSTraverse(void (*visit)(const ElemType&)) const
// 初始条件：存在图
// 操作结果：对图进行广度优先遍历
{
	int v;
	for (v = 0; v < GetVexNum(); v++)
	{	// 对每个顶点设置访问标志
		SetTag(v, false);
	}

	for (v = 0; v < GetVexNum(); v++)
	{	// 对尚未访问的顶点按BFS进行深度优先搜索
		if (!GetTag(v)) BFS(v, visit);
	}
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>::AdjMatrixDirGraph(ElemType es[], int vertexNum)
// 操作结果：构造数据元素为es[]，顶点个数为vertexNum，边数为0的有向图
{
	if (vertexNum < 0)
	{	// 出现异常
		cout << "顶点个数不能为负!" << endl;	// 提示信息
		exit(1);								// 退出程序
	}

	vexNum = vertexNum;							// 顶点数为vertexNum
	edgeNum = 0;								// 边数为0

	elems = new ElemType[vexNum];				// 生成顶点元素标数组

	int u, v;									// 临时变量 
	for (v = 0; v < vexNum; v++)
	{	// 初始化数据元素
		elems[v] = es[v];
	}

	tag = new bool[vexNum];						// 生成标志数组
	for (v = 0; v < vexNum; v++)
	{	// 初始化标志数组
		tag[v] = false;
	}

	matrix = (int**)new int* [vexNum];			// 生成邻接矩阵
	for (v = 0; v < vexNum; v++)
	{	// 生成邻接矩阵的行
		matrix[v] = new int[vexNum];
	}

	for (u = 0; u < vexNum; u++)
	{
		for (v = 0; v < vexNum; v++)
		{	// 为邻接矩阵元素赋值
			matrix[u][v] = 0;
		}
	}
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>::~AdjMatrixDirGraph()
// 操作结果：释放对象所占用空间
{
	delete[]elems;								// 释放元素
	delete[]tag;								// 释放标志

	for (int iPos = 0; iPos < vexNum; iPos++)
	{	// 释放邻接矩阵的行
		delete[]matrix[iPos];
	}
	delete[]matrix;							// 释放邻接矩阵
}

template <class ElemType>
bool AdjMatrixDirGraph<ElemType>::GetElem(int v, ElemType& e) const
// 操作结果：用e返回顶点v的元素值, v的取值范围为0 ≤ v ＜ vexNum, v合法时返回
//	true, 否则返回false
{
	if (v < 0 || v >= vexNum)
	{	// v范围错
		return false;	// 元素不存在
	}
	else
	{	// v合法
		e = elems[v];	// 将顶点v的元素值赋给e
		return true;	// 元素存在
	}
}

template <class ElemType>
bool AdjMatrixDirGraph<ElemType>::SetElem(int v, const ElemType& e)
// 操作结果：设置顶点v的元素值为e，v的取值范围为0 ≤ v ＜ vexNum, v合法时返回
//	true, 否则返回false
{
	if (v < 0 || v >= vexNum)
	{	// v范围错
		return false;		// 位置错
	}
	else
	{	// v合法
		elems[v] = e;		// 顶点元素
		return true;		// 成功
	}
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::GetVexNum() const
// 操作结果：返回顶点个数			 
{
	return vexNum;
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::GetEdgeNum() const
// 操作结果：返回边数个数
{
	return edgeNum;
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::FirstAdjVex(int v) const
// 操作结果：返回顶点v的第1个邻接点			 
{
	if (v < 0 || v >= vexNum)
	{	// 出现异常
		cout << "v不合法!" << endl;					// 提示信息
		exit(2);									// 退出程序
	}

	for (int cur = 0; cur < vexNum; cur++)
	{	// 查找邻接点
		if (matrix[v][cur] != 0) return cur;
	}

	return -1;										// 返回-1表示无邻接点
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::NextAdjVex(int v1, int v2) const
// 操作结果：返回顶点v1的相对于v2的下1个邻接点			 
{
	if (v1 < 0 || v1 >= vexNum)
	{	// 出现异常
		cout << "v1不合法!" << endl;				// 提示信息
		exit(3);									// 退出程序
	}
	if (v2 < 0 || v2 >= vexNum)
	{	// 出现异常
		cout << "v2不合法!" << endl;				// 提示信息
		exit(4);									// 退出程序
	}
	if (v1 == v2)
	{	// 出现异常
		cout << "v1不能等于v2!" << endl;			// 提示信息
		exit(5);									// 退出程序
	}

	for (int cur = v2 + 1; cur < vexNum; cur++)
	{	// 查找邻接点
		if (matrix[v1][cur] != 0) return cur;
	}

	return -1;										// 返回-1表示无邻接点
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::InsertEdge(int v1, int v2)
// 操作结果：插入顶点为v1和v2，权为w的边			 
{
	if (v1 < 0 || v1 >= vexNum)
	{	// 出现异常
		cout << "v1不合法!" << endl;				// 提示信息
		exit(6);									// 退出程序
	}
	if (v2 < 0 || v2 >= vexNum)
	{	// 出现异常
		cout << "v2不合法!" << endl;				// 提示信息
		exit(7);									// 退出程序
	}
	if (v1 == v2)
	{	// 出现异常
		cout << "v1不能等于v2!" << endl;			// 提示信息
		exit(8);									// 退出程序
	}

	if (matrix[v1][v2] == 0)
	{	// 原有向图无边<v1, v2>，插入后边数自增1
		edgeNum++;
	}
	matrix[v1][v2] = 1;								// 修改<v1, v2>对应的邻接矩阵元素值
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::DeleteEdge(int v1, int v2)
// 操作结果：删除顶点为v1和v2的边			 
{
	if (v1 < 0 || v1 >= vexNum)
	{	// 出现异常
		cout << "v1不合法!" << endl;				// 提示信息
		exit(9);									// 退出程序
	}
	if (v2 < 0 || v2 >= vexNum)
	{	// 出现异常
		cout << "v2不合法!" << endl;				// 提示信息
		exit(10);									// 退出程序
	}
	if (v1 == v2)
	{	// 出现异常
		cout << "v1不能等于v2!" << endl;			// 提示信息
		exit(11);									// 退出程序
	}

	if (matrix[v1][v2] != 0)
	{	// 原有向图存在边<v1, v2>,删除后边数自减1
		edgeNum--;
	}
	matrix[v1][v2] = 0;								// 修改<v1, v2>对应的邻接矩阵元素值
}

template <class ElemType>
bool AdjMatrixDirGraph<ElemType>::GetTag(int v) const
// 操作结果：返回顶点v的标志		 
{
	if (v < 0 || v >= vexNum)
	{	// 出现异常
		cout << "v不合法!" << endl;					// 提示信息
		exit(12);									// 退出程序
	}

	return tag[v];									// 返回顶点v的标志
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::SetTag(int v, bool val) const
// 操作结果：设置顶点v的标志为val		 
{
	if (v < 0 || v >= vexNum)
	{	// 出现异常
		cout << "v不合法!" << endl;					// 提示信息
		exit(13);									// 退出程序
	}

	tag[v] = val;									// 设置顶点v的标志为val
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>::AdjMatrixDirGraph(const AdjMatrixDirGraph<ElemType>& source)
// 操作结果：由有向图的邻接矩阵source构造新有向图的邻接矩阵source——复制构造函数模板
{
	int iPos, jPos;									// 临时变量
	vexNum = source.vexNum;							// 复制顶点数
	edgeNum = source.edgeNum;						// 复制边数

	elems = new ElemType[vexNum];					// 生成顶点数据数组
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// 复制顶点数据数组
		elems[iPos] = source.elems[iPos];
	}

	tag = new bool[vexNum];							// 生成标志数组
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// 复制标志数组
		tag[iPos] = source.tag[iPos];
	}

	matrix = (int**)new int* [vexNum];				// 生成邻接矩阵
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// 生成邻接矩阵的行
		matrix[iPos] = new int[vexNum];
	}

	for (iPos = 0; iPos < vexNum; iPos++)
	{
		for (jPos = 0; jPos < vexNum; jPos++)
		{	// 复制邻接矩阵元素赋值
			matrix[iPos][jPos] = source.matrix[iPos][jPos];
		}
	}
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>& AdjMatrixDirGraph<ElemType>::operator =(const AdjMatrixDirGraph<ElemType>& source)
// 操作结果：将有向图的邻接矩阵source赋值给当前有向图的邻接矩阵——重载赋值运算符
{
	if (&source != this)
	{
		int iPos, jPos;								// 临时变量

		delete[]elems;								// 释放元素
		delete[]tag;								// 释放标志

		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 释放邻接矩阵的行
			delete[]matrix[iPos];
		}
		delete[]matrix;							// 释放邻接矩阵

		vexNum = source.vexNum;						// 复制顶点数
		edgeNum = source.edgeNum;					// 复制边数

		elems = new ElemType[vexNum];				// 生成顶点数据数组
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 复制顶点数据数组
			elems[iPos] = source.elems[iPos];
		}

		tag = new bool[vexNum];						// 生成标志数组
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 复制标志数组
			tag[iPos] = source.tag[iPos];
		}

		matrix = (int**)new int* [vexNum];			// 生成邻接矩阵
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// 生成邻接矩阵的行
			matrix[iPos] = new int[vexNum];
		}

		for (iPos = 0; iPos < vexNum; iPos++)
		{
			for (jPos = 0; jPos < vexNum; jPos++)
			{	// 复制邻接矩阵元素赋值
				matrix[iPos][jPos] = source.matrix[iPos][jPos];
			}
		}
	}
	return *this;
}

template <class ElemType>
void Display(const AdjMatrixDirGraph<ElemType>& g)
// 操作结果: 显示邻接矩阵有向图
{
	int** matrix;									// 邻接矩阵
	matrix = (int**)new int* [g.GetVexNum()];		// 生成邻接矩阵
	int iPos, jPos;									// 临时变量 

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{	// 生成邻接矩阵的行
		matrix[iPos] = new int[g.GetVexNum()];
	}

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{
		for (jPos = 0; jPos < g.GetVexNum(); jPos++)
		{	// 为邻接矩阵元素赋值
			matrix[iPos][jPos] = 0;
		}
	}

	for (int v = 0; v < g.GetVexNum(); v++)
	{	// 查找有向图中v的邻接点
		for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
		{	// u为v的邻接点
			matrix[v][u] = 1;
		}
	}

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{	// 显示行
		ElemType e;						// 数据元素
		g.GetElem(iPos, e);				// 取出元素值
		cout << e;						// 显示元素值

		for (jPos = 0; jPos < g.GetVexNum(); jPos++)
		{	// 显示邻接矩阵元素
			cout << "\t" << matrix[iPos][jPos];
		}
		cout << endl;					// 换行 
	}

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{	// 释放邻接矩阵的行
		delete[]matrix[iPos];
	}
	delete[]matrix;					// 释放邻接矩阵
}
template<class ElemType>
int  AdjMatrixDirGraph<ElemType>::GetIndex(string v)
{
	int index = -1;
	for (int i = 0;i < vexNum;i++)
	{
		if (v==elems[i].courseNo)
		{
			index = i;
			break;
		}
	}
	return index;
}
#endif
