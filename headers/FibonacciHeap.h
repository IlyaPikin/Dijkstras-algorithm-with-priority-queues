// Copyright 2023 Pikin Ilya
#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include <vector>

using namespace std;


struct FibNode
{
	// ������
	int name;			// ��� ������� � �����
	int key;			// ������ ���� ����������� ���� �������

	// ��������������� ����
	int degree;			// ���������� �������� ����� ������� ����
	bool mark;			// ����� � ���, ���� �� ������ ����� x �������� �����
						// ������� � �������, ����� x ���� �������� �����
	FibNode* parent;	// ��������� �� ������������ ����
	FibNode* child;		// ��������� �� ���� �� �������� �����
	FibNode* left;		// ��������� �� ����� �������� ����
	FibNode* right;		// ��������� �� ������ �������� ����
};


class FibonacciHeap
{
private:

	int n;				// ����� ����� � ����
	FibNode* min_node;	// ��������� �� ���� � ����������� ������
	static vector<FibNode*> LocationInHeap;	// ������ ��� ��������� � ���� � ���� �� ������ �������

	const double GOLDEN_RATIO = 1.618;

public:
	FibonacciHeap(int max_n);
	FibonacciHeap(const FibonacciHeap& other) = delete;
	FibonacciHeap& operator=(const FibonacciHeap& other) = delete;

	int get_size();
	void get_min(int& min_name, int& min_key) const;
	int get_key(int name);
	bool empty();

	void insert(int name, int key);
	void delete_element(int name);
	void delete_min();
	void extract_min(int& min_name, int& min_key);
	void decrease_key(int name, int new_key);


private:
	void heap_union(FibonacciHeap& h);
	void initializeNode(FibNode* nodePtr, int name, int key);
	FibNode* root_list_union(FibNode* x, FibNode* y);	// ��������� �������� ������
	void consolidate(); // ���������� ������ ������
	void link(FibNode* y, FibNode* x);
	void cut(FibNode* x, FibNode* y);
	void cascading_cut(FibNode* y);
};

#endif // FIBONACCIHEAP_H