#ifndef __ADJ_MATRIX_DIR_GRAPH_H__
#define __ADJ_MATRIX_DIR_GRAPH_H__

#include <iostream>								// ����Ԥ��������
#include <cstdlib>								// ��C����exit()��������stdlib.h��cstdlib��C��ͷ�ļ���
using namespace std;							// ʹ�������ռ�std 
#include "lk_queue.h"							// ������

#ifndef DEFAULT_SIZE
#define DEFAULT_SIZE 10000						// ȱʡ��С
#endif

// ����ͼ���ڽӾ�����ģ��
template <class ElemType>
class AdjMatrixDirGraph 
{
protected:
// ���ݳ�Ա:
	int vexNum, edgeNum;						// ��������ͱ���
	int **matrix;								// �ڽӾ���
	ElemType *elems;							// ����Ԫ��
	mutable bool *tag;							// �����־

// ��������ģ��:
	void DFS(int v, void (*visit)(const ElemType &e)) const;	// �Ӷ���v�������������������ͼ
	void BFS(int v, void (*visit)(const ElemType &)) const;		// �ӵڶ���v�������й����������ͼ

public:
// �����������ͷ������������ر���ϵͳĬ�Ϸ�������:
	AdjMatrixDirGraph(ElemType es[], int vertexNum = DEFAULT_SIZE);	
		// ��������Ԫ��Ϊes[]���������ΪvertexNum������Ϊ0������ͼ
	virtual ~AdjMatrixDirGraph();				// ��������ģ��
	void DFSTraverse(void (*visit)(const ElemType &e)) const;	// ��ͼ����������ȱ���
	void BFSTraverse(void (*visit)(const ElemType &)) const;	// ��ͼ���й�����ȱ���
	bool GetElem(int v, ElemType &e) const;		// ��e���ض���v��Ԫ��ֵ	
	bool SetElem(int v, const ElemType &e);		// ���ö���v��Ԫ��ֵΪe
	int GetVexNum() const;						// ���ض������			 
	int GetEdgeNum() const;						// ���ر�������			 
	int FirstAdjVex(int v) const;				// ���ض���v�ĵ�һ���ڽӵ�			 
	int NextAdjVex(int v1, int v2) const;		// ���ض���v1�������v2����һ���ڽӵ�			 
	void InsertEdge(int v1, int v2);			// ���붥��Ϊv1��v2�ı�			 
	void DeleteEdge(int v1, int v2);			// ɾ������Ϊv1��v2�ı�			 
	bool GetTag(int v) const;					// ���ض���v�ı�־		 
	void SetTag(int v, bool val) const;			// ���ö���v�ı�־Ϊval		 
	AdjMatrixDirGraph(const AdjMatrixDirGraph<ElemType> &source);	// ���ƹ��캯��ģ��
	AdjMatrixDirGraph<ElemType> &operator =(const AdjMatrixDirGraph<ElemType> &source); 
		// ���ظ�ֵ�����
}; 

template <class ElemType>
void Display(const AdjMatrixDirGraph<ElemType> &g);						// ��ʾ�ڽӾ�������ͼ


// ����ͼ���ڽӾ�����ģ���ʵ�ֲ���
template <class ElemType>
void AdjMatrixDirGraph<ElemType>::DFS(int v, void (*visit)(const ElemType &e)) const
// ��ʼ����������ͼ
// ����������Ӷ���v�������������������ͼ
{	
	SetTag(v, true);							// ���÷��ʱ�־
	ElemType e;									// ��ʱ����
	GetElem(v, e);								// ȡ����v������Ԫ��
	(*visit)(e);								// ���ʶ���v������Ԫ��
	for (int w = FirstAdjVex(v); w != -1; w = NextAdjVex(v, w))
	{	// ��v����δ���ʹ����ڽӶ���w�ݹ����DFS
		if (!GetTag(w))	DFS(w , visit);	
	}
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::DFSTraverse(void (*visit)(const ElemType &e)) const
// ��ʼ����������ͼ
// �����������ͼ����������ȱ���
{
	int v;
	for (v = 0; v < GetVexNum(); v++)
	{	// ��ÿ���������÷��ʱ�־
		SetTag(v, false);
	}

	for (v = 0; v < GetVexNum(); v++)
	{	// ����δ���ʵĶ��㰴DFS���������������
		if (!GetTag(v))	DFS(v, visit);
	}
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::BFS(int v, void (*visit)(const ElemType &)) const
// ��ʼ����������ͼ
// ����������ӵڶ���v�������й����������ͼ
{	
	SetTag(v, true);						// ���÷��ʱ�־
	ElemType e;								// ��ʱ����
	GetElem(v, e);							// ȡ����v������Ԫ��
	(*visit)(e);							// ���ʶ���v������Ԫ��
	LinkQueue<int> q;						// �������
	q.InQueue(v);							// v���
	while (!q.Empty())
	{	// ����q�ǿ�, ����ѭ��
		int u, w;							// ��ʱ����
		q.OutQueue(u);						// ����
		for (w = FirstAdjVex(u); w >= 0; w = NextAdjVex(u, w))
		{	// ��u��δ���ʹ����ڽӶ���w���з���
			if (!GetTag(w))
			{	// ��w���з���
				SetTag(w, true);			// ���÷��ʱ�־
				GetElem(w, e);				// ȡ����w������Ԫ��
				(*visit)(e);				// ���ʶ���w������Ԫ��
				q.InQueue(w);				// w���
			}
		}
	}
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::BFSTraverse(void (*visit)(const ElemType &)) const
// ��ʼ����������ͼ
// �����������ͼ���й�����ȱ���
{
	int v;
	for (v = 0; v < GetVexNum(); v++)
	{	// ��ÿ���������÷��ʱ�־
		SetTag(v, false);
	}

	for (v = 0; v < GetVexNum(); v++)
	{	// ����δ���ʵĶ��㰴BFS���������������
		if (!GetTag(v)) BFS(v , visit); 
	}
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>::AdjMatrixDirGraph(ElemType es[], int vertexNum)
// �����������������Ԫ��Ϊes[]���������ΪvertexNum������Ϊ0������ͼ
{
	if (vertexNum < 0)
	{	// �����쳣
		cout << "�����������Ϊ��!" << endl;	// ��ʾ��Ϣ
		exit(1);								// �˳�����
	}

	vexNum = vertexNum;							// ������ΪvertexNum
	edgeNum = 0;								// ����Ϊ0

	elems = new ElemType[vexNum];				// ���ɶ���Ԫ�ر�����
	
	int u, v;									// ��ʱ���� 
	for (v = 0; v < vexNum; v++)
	{	// ��ʼ������Ԫ��
		elems[v] = es[v];
	}

	tag = new bool[vexNum];						// ���ɱ�־����
	for (v = 0; v < vexNum; v++)
	{	// ��ʼ����־����
		tag[v] = false;
	}

	matrix = (int **)new int *[vexNum];			// �����ڽӾ���
	for (v = 0; v < vexNum; v++)
	{	// �����ڽӾ������
		matrix[v] = new int[vexNum];	
	}

	for (u = 0; u < vexNum; u++)
	{
		for (v = 0; v < vexNum; v++)
		{	// Ϊ�ڽӾ���Ԫ�ظ�ֵ
			matrix[u][v] = 0;
		}
	}
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>::~AdjMatrixDirGraph()
// ����������ͷŶ�����ռ�ÿռ�
{
	delete []elems;								// �ͷ�Ԫ��
	delete []tag;								// �ͷű�־

	for (int iPos = 0; iPos < vexNum; iPos++)
	{	// �ͷ��ڽӾ������
		delete []matrix[iPos];
	}
	delete []matrix;							// �ͷ��ڽӾ���
}

template <class ElemType>
bool AdjMatrixDirGraph<ElemType>::GetElem(int v, ElemType &e) const
// �����������e���ض���v��Ԫ��ֵ, v��ȡֵ��ΧΪ0 �� v �� vexNum, v�Ϸ�ʱ����
//	true, ���򷵻�false
{
	if (v < 0 || v >= vexNum)
	{	// v��Χ��
		return false;	// Ԫ�ز�����
	}
	else
	{	// v�Ϸ�
		e = elems[v];	// ������v��Ԫ��ֵ����e
		return true;	// Ԫ�ش���
	}
}	

template <class ElemType>
bool AdjMatrixDirGraph<ElemType>::SetElem(int v, const ElemType &e)
// ������������ö���v��Ԫ��ֵΪe��v��ȡֵ��ΧΪ0 �� v �� vexNum, v�Ϸ�ʱ����
//	true, ���򷵻�false
{
	if (v < 0 || v >= vexNum)
	{	// v��Χ��
		return false;		// λ�ô�
	}
	else
	{	// v�Ϸ�
		elems[v] = e;		// ����Ԫ��
		return true;		// �ɹ�
	}
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::GetVexNum() const
// ������������ض������			 
{
	return vexNum;
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::GetEdgeNum() const
// ������������ر�������
{
	return edgeNum;
}		 

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::FirstAdjVex(int v) const
// ������������ض���v�ĵ�1���ڽӵ�			 
{
	if (v < 0 || v >= vexNum) 
	{	// �����쳣
		cout << "v���Ϸ�!" << endl;					// ��ʾ��Ϣ
		exit(2);									// �˳�����
	}
		
	for (int cur = 0; cur < vexNum; cur++)
	{	// �����ڽӵ�
		if (matrix[v][cur] != 0) return cur;
	}

	return -1;										// ����-1��ʾ���ڽӵ�
}

template <class ElemType>
int AdjMatrixDirGraph<ElemType>::NextAdjVex(int v1, int v2) const
// ������������ض���v1�������v2����1���ڽӵ�			 
{
	if (v1 < 0 || v1 >= vexNum)
	{	// �����쳣
		cout << "v1���Ϸ�!" << endl;				// ��ʾ��Ϣ
		exit(3);									// �˳�����
	}
	if (v2 < 0 || v2 >= vexNum)
	{	// �����쳣
		cout << "v2���Ϸ�!" << endl;				// ��ʾ��Ϣ
		exit(4);									// �˳�����
	}
	if (v1 == v2)
	{	// �����쳣
		cout << "v1���ܵ���v2!" << endl;			// ��ʾ��Ϣ
		exit(5);									// �˳�����
	}

	for (int cur = v2 + 1; cur < vexNum; cur++)
	{	// �����ڽӵ�
		if (matrix[v1][cur] != 0) return cur;
	}

	return -1;										// ����-1��ʾ���ڽӵ�
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::InsertEdge(int v1, int v2)
// ������������붥��Ϊv1��v2��ȨΪw�ı�			 
{
	if (v1 < 0 || v1 >= vexNum)
	{	// �����쳣
		cout << "v1���Ϸ�!" << endl;				// ��ʾ��Ϣ
		exit(6);									// �˳�����
	}
	if (v2 < 0 || v2 >= vexNum)
	{	// �����쳣
		cout << "v2���Ϸ�!" << endl;				// ��ʾ��Ϣ
		exit(7);									// �˳�����
	}
	if (v1 == v2)
	{	// �����쳣
		cout << "v1���ܵ���v2!" << endl;			// ��ʾ��Ϣ
		exit(8);									// �˳�����
	}

	if (matrix[v1][v2] == 0)
	{	// ԭ����ͼ�ޱ�<v1, v2>��������������1
		edgeNum++;
	}
	matrix[v1][v2] = 1;								// �޸�<v1, v2>��Ӧ���ڽӾ���Ԫ��ֵ
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::DeleteEdge(int v1, int v2)
// ���������ɾ������Ϊv1��v2�ı�			 
{
	if (v1 < 0 || v1 >= vexNum)
	{	// �����쳣
		cout << "v1���Ϸ�!" << endl;				// ��ʾ��Ϣ
		exit(9);									// �˳�����
	}
	if (v2 < 0 || v2 >= vexNum)
	{	// �����쳣
		cout << "v2���Ϸ�!" << endl;				// ��ʾ��Ϣ
		exit(10);									// �˳�����
	}
	if (v1 == v2)
	{	// �����쳣
		cout << "v1���ܵ���v2!" << endl;			// ��ʾ��Ϣ
		exit(11);									// �˳�����
	}

	if (matrix[v1][v2] != 0)
	{	// ԭ����ͼ���ڱ�<v1, v2>,ɾ��������Լ�1
		edgeNum--;
	}
	matrix[v1][v2] = 0;								// �޸�<v1, v2>��Ӧ���ڽӾ���Ԫ��ֵ
}

template <class ElemType>
bool AdjMatrixDirGraph<ElemType>::GetTag(int v) const
// ������������ض���v�ı�־		 
{
	if (v < 0 || v >= vexNum)
	{	// �����쳣
		cout << "v���Ϸ�!" << endl;					// ��ʾ��Ϣ
		exit(12);									// �˳�����
	}

	return tag[v];									// ���ض���v�ı�־
}

template <class ElemType>
void AdjMatrixDirGraph<ElemType>::SetTag(int v, bool val) const 
// ������������ö���v�ı�־Ϊval		 
{
	if (v < 0 || v >= vexNum)
	{	// �����쳣
		cout << "v���Ϸ�!" << endl;					// ��ʾ��Ϣ
		exit(13);									// �˳�����
	}

	tag[v] = val;									// ���ö���v�ı�־Ϊval
}

template <class ElemType>
AdjMatrixDirGraph<ElemType>::AdjMatrixDirGraph(const AdjMatrixDirGraph<ElemType> &source)
// ���������������ͼ���ڽӾ���source����������ͼ���ڽӾ���source�������ƹ��캯��ģ��
{
	int iPos, jPos;									// ��ʱ����
	vexNum = source.vexNum;							// ���ƶ�����
	edgeNum = source.edgeNum;						// ���Ʊ���

	elems = new ElemType[vexNum];					// ���ɶ�����������
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// ���ƶ�����������
		elems[iPos] = source.elems[iPos];
	}

	tag = new bool[vexNum];							// ���ɱ�־����
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// ���Ʊ�־����
		tag[iPos] = source.tag[iPos];
	}

	matrix = (int **)new int *[vexNum];				// �����ڽӾ���
	for (iPos = 0; iPos < vexNum; iPos++)
	{	// �����ڽӾ������
		matrix[iPos] = new int[vexNum];	
	}  

	for (iPos = 0; iPos < vexNum; iPos++)
	{
		for (jPos = 0; jPos < vexNum; jPos++)
		{	// �����ڽӾ���Ԫ�ظ�ֵ
			matrix[iPos][jPos] = source.matrix[iPos][jPos];
		}
	}
}

template <class ElemType>
AdjMatrixDirGraph<ElemType> &AdjMatrixDirGraph<ElemType>::operator =(const AdjMatrixDirGraph<ElemType> &source)
// ���������������ͼ���ڽӾ���source��ֵ����ǰ����ͼ���ڽӾ��󡪡����ظ�ֵ�����
{
	if (&source != this)
	{
		int iPos, jPos;								// ��ʱ����

		delete []elems;								// �ͷ�Ԫ��
		delete []tag;								// �ͷű�־

		for (iPos = 0; iPos < vexNum; iPos++)
		{	// �ͷ��ڽӾ������
			delete []matrix[iPos];
		}
		delete []matrix;							// �ͷ��ڽӾ���

		vexNum = source.vexNum;						// ���ƶ�����
		edgeNum = source.edgeNum;					// ���Ʊ���

		elems = new ElemType[vexNum];				// ���ɶ�����������
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// ���ƶ�����������
			elems[iPos] = source.elems[iPos];
		}

		tag = new bool[vexNum];						// ���ɱ�־����
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// ���Ʊ�־����
			tag[iPos] = source.tag[iPos];
		}

		matrix = (int **)new int*[vexNum];			// �����ڽӾ���
		for (iPos = 0; iPos < vexNum; iPos++)
		{	// �����ڽӾ������
			matrix[iPos] = new int[vexNum];	
		}

		for (iPos = 0; iPos < vexNum; iPos++)
		{
			for (jPos = 0; jPos < vexNum; jPos++)
			{	// �����ڽӾ���Ԫ�ظ�ֵ
				matrix[iPos][jPos] = source.matrix[iPos][jPos];
			}
		}
	}
	return *this;
}

template <class ElemType>
void Display(const AdjMatrixDirGraph<ElemType> &g)
// �������: ��ʾ�ڽӾ�������ͼ
{
	int **matrix;									// �ڽӾ���
	matrix = (int **)new int *[g.GetVexNum()];		// �����ڽӾ���
	int iPos, jPos;									// ��ʱ���� 

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{	// �����ڽӾ������
		matrix[iPos] = new int[g.GetVexNum()];	
	}

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{
		for (jPos = 0; jPos < g.GetVexNum(); jPos++)
		{	// Ϊ�ڽӾ���Ԫ�ظ�ֵ
			matrix[iPos][jPos] = 0;
		}
	}

	for (int v = 0; v < g.GetVexNum(); v++)
	{	// ��������ͼ��v���ڽӵ�
		for (int u = g.FirstAdjVex(v); u != -1; u = g.NextAdjVex(v, u))
		{	// uΪv���ڽӵ�
			matrix[v][u] = 1;
		}
	}

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{	// ��ʾ��
		ElemType e;						// ����Ԫ��
		g.GetElem(iPos, e);				// ȡ��Ԫ��ֵ
		cout << e;						// ��ʾԪ��ֵ

		for (jPos = 0; jPos < g.GetVexNum(); jPos++)
		{	// ��ʾ�ڽӾ���Ԫ��
 			cout << "\t" << matrix[iPos][jPos];
		}
		cout << endl;					// ���� 
	}

	for (iPos = 0; iPos < g.GetVexNum(); iPos++)
	{	// �ͷ��ڽӾ������
		delete []matrix[iPos];
	}
	delete []matrix;					// �ͷ��ڽӾ���
}

#endif
