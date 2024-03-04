// Copyright 2023 Pikin Ilya
#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include <vector>

using namespace std;


struct FibNode
{
	// Данные
	int name;			// Имя вершины в графе
	int key;			// Оценка веса кратчайшего пути вершины

	// Вспомогательные поля
	int degree;			// Количество дочерних узлов данного узла
	bool mark;			// Метка о том, были ли потери узлом x дочерних узлов
						// начиная с момента, когда x стал дочерним узлом
	FibNode* parent;	// Указатель на родительский узел
	FibNode* child;		// Указатель на один из дочерних узлов
	FibNode* left;		// Указатель на левый братский узел
	FibNode* right;		// Указатель на правый братский узел
};


class FibonacciHeap
{
private:

	int n;				// Число узлов в куче
	FibNode* min_node;	// Указатель на узел с минимальным ключом
	static vector<FibNode*> LocationInHeap;	// Массив для обращения к узлу в куче по номеру вершины

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
	FibNode* root_list_union(FibNode* x, FibNode* y);	// Связывает корневые списки
	void consolidate(); // Уплотнение списка корней
	void link(FibNode* y, FibNode* x);
	void cut(FibNode* x, FibNode* y);
	void cascading_cut(FibNode* y);
};

#endif // FIBONACCIHEAP_H