// Copyright 2023 Pikin Ilya
#ifndef BINOMIALHEAP_H
#define BINOMIALHEAP_H

#include <vector>
#include <stack>

using namespace std;


struct Node
{
	// Данные
	int name;			// Имя вершины в графе
	int key;			// Оценка веса кратчайшего пути вершины
	
	// Вспомогательные поля
	int degree;			// Количество дочерних узлов данного узла
	Node* p;			// Указатель на родительский узел
	Node* child;		// Указатель на дочерний узел
	Node* sibling;		// Указатель на братский узел
};


class BinomialHeap
{
private:

	int n;
	Node* head;
	static vector<Node*> LocationInHeap;

public:

	BinomialHeap(int max_n);

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

	BinomialHeap();
	BinomialHeap(const BinomialHeap& other) = delete;
	BinomialHeap& operator=(const BinomialHeap& other) = delete;
	void heap_union(BinomialHeap& h1);
	static void link_trees(Node* node1, Node* node2);
	void initializeNode(Node* nodePtr, int name, int key);
	void swap_data(Node* node1, Node* node2);
	void heap_merge(BinomialHeap& h1);

};

#endif // BINOMIALHEAP_H