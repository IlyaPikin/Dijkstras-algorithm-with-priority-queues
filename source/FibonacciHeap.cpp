// Copyright 2023 Pikin Ilya
#include "../headers/FibonacciHeap.h"


vector<FibNode*> FibonacciHeap::LocationInHeap;

FibonacciHeap::FibonacciHeap(int max_n)
{
	n = 0;
	min_node = nullptr;
	LocationInHeap.resize(max_n, nullptr);
}


int FibonacciHeap::get_size()
{
	return n;
}

void FibonacciHeap::get_min(int& min_name, int& min_key) const
{
	if (min_node == nullptr)
	{
		min_name = -1;
		min_key = -1;
		return;
	}

	min_name = min_node->name;
	min_key = min_node->key;
	return;
}

int FibonacciHeap::get_key(int name)
{
	if (name < 0 || name >= LocationInHeap.size())
		return -1;
	FibNode* nodePtr = LocationInHeap[name];
	if (nodePtr == nullptr)
		return -1;
	return nodePtr->key;
}

bool FibonacciHeap::empty()
{
	return n == 0;
}


void FibonacciHeap::insert(int name, int key)
{
	if (name < 0 || name >= LocationInHeap.size())
		return;
	if (key < 0)
		return;

	FibNode* nodePtr = new FibNode;
	initializeNode(nodePtr, name, key);
	min_node = root_list_union(min_node, nodePtr);

	n++;
	LocationInHeap[name] = nodePtr;
}

void FibonacciHeap::delete_element(int name) {
	// Уменьшение ключа до значения INT_MIN:
	// START
	// Обращение к узлу по номеру вершины
	if (name < 0 || name >= LocationInHeap.size())
		return;
	FibNode* x = LocationInHeap[name];

	// Проверка
	if (x == nullptr)
		return;

	// Уменьшение ключа x
	x->key = INT_MIN;
	FibNode* y = x->parent;

	// Если свойство кучи не выполняется, делаем каскадное вырезание
	if (y != nullptr)
	{
		cut(x, y);
		cascading_cut(y);
	}

	// Новый минимум
	min_node = x;
	// END

	delete_min();
}

void FibonacciHeap::delete_min() {
	int min_name, min_key;
	extract_min(min_name, min_key);
}

void FibonacciHeap::extract_min(int& min_name, int& min_key)
{
	if (min_node == nullptr)
	{
		min_name = -1;
		min_key = -1;
		return;
	}

	FibNode* z = min_node;
	FibNode* x = z->child;

	// Добавление всех дочерних узлов z список корней кучи
	if (x != nullptr)
	{
		FibNode* tmp = x;

		do {
			tmp->parent = nullptr;
			tmp = tmp->right;
		} while (tmp != x);

		root_list_union(z, x);
	}

	// Удаление z из списка корней кучи
	z->right->left = z->left;
	z->left->right = z->right;

	if (z == z->right)
	{
		min_node = nullptr;
	}
	else
	{
		min_node = z->right;
		consolidate();
	}

	n--;
	min_name = z->name;
	min_key = z->key;
	LocationInHeap[min_name] = nullptr;
	delete z;
}

void FibonacciHeap::decrease_key(int name, int new_key)
{
	// Обращение к узлу по номеру вершины
	if (name < 0 || name >= LocationInHeap.size())
		return;
	FibNode* x = LocationInHeap[name];

	// Проверки
	if (x == nullptr || new_key < 0 || new_key >= x->key)
		return;

	// Уменьшение ключа x
	x->key = new_key;
	FibNode* y = x->parent;

	// Если свойство кучи не выполняется, делаем каскадное вырезание
	if (y != nullptr && x->key < y->key)
	{
		cut(x, y);
		cascading_cut(y);
	}

	// Новый минимум?
	if (x->key < min_node->key)
		min_node = x;
}



void FibonacciHeap::heap_union(FibonacciHeap& h)
{
	min_node = root_list_union(min_node, h.min_node);
	n += h.n;
}

void FibonacciHeap::initializeNode(FibNode* nodePtr, int name, int key)
{
	nodePtr->name = name;
	nodePtr->key = key;
	
	nodePtr->degree = 0;
	nodePtr->mark = false;
	
	nodePtr->parent = nullptr;
	nodePtr->child = nullptr;
	nodePtr->left = nodePtr;
	nodePtr->right = nodePtr;
}

FibNode* FibonacciHeap::root_list_union(FibNode* x, FibNode* y)
{
	if (x == nullptr)
		return y;
	else if (y == nullptr)
		return x;
	else if (x->key > y->key)
		swap(x, y);

	FibNode* x_list_end = x->left;
	FibNode* y_list_end = y->left;

	x->left = y_list_end;
	y_list_end->right = x;

	y->left = x_list_end;
	x_list_end->right = y;

	return x;
}

void FibonacciHeap::consolidate()
{
	// Оценка на максимальную степень узла кучи D(n)
	int D = static_cast<int>(log(n) / log(GOLDEN_RATIO)) + 1;

	vector<FibNode*> A(D, nullptr);

	FibNode* end = min_node->left;
	FibNode* w = nullptr;
	FibNode* next = min_node;
	// Цикл по всем узлам w исходного корневого списка
	do {
		w = next;
		next = next->right;

		FibNode* x = w;
		int d = x->degree;

		while (A.at(d) != nullptr)
		{
			FibNode* y = A.at(d);
			if (x->key > y->key)
				swap(x, y);

			link(y, x);

			A.at(d) = nullptr;
			d++;
		}
		A[d] = x;
	} while (w != end);

	min_node = nullptr;
	// Цикл для формирования нового корневого списка
	for (int i = 0; i < A.size(); i++)
	{
		if (A.at(i) != nullptr)
		{
			A.at(i)->right = A.at(i);
			A.at(i)->left = A.at(i);
			min_node = root_list_union(min_node, A.at(i));
		}
	}
}

void FibonacciHeap::link(FibNode* y, FibNode* x)
{
	// Удаление y из корневого списка
	y->left->right = y->right;
	y->right->left = y->left;

	// Делаем y потомком x
	if (x->child == nullptr)
	{
		x->child = y;
		y->right = y;
		y->left = y;
	}
	else
	{
		FibNode* child = x->child;
		y->right = child;
		y->left = child->left;
		child->left->right = y;
		child->left = y;
	}

	y->parent = x;
	x->degree++;
	y->mark = false;
}

void FibonacciHeap::cut(FibNode* x, FibNode* y)
{
	// Удаление x из списка дочерних узлов y
	x->right->left = x->left;
	x->left->right = x->right;

	if (y->child == x) {
		if (x->right == x) {
			y->child = nullptr;
		}
		else {
			y->child = x->right;
		}
	}

	y->degree--;
	x->left = x;
	x->right = x;
	x->parent = nullptr;
	x->mark = false;

	// Добавление x в список корней
	min_node = root_list_union(min_node, x);
}

void FibonacciHeap::cascading_cut(FibNode* y)
{
	FibNode* z = y->parent;

	if (z != nullptr) {
		if (y->mark == false) {
			y->mark = true;
		}
		else {
			cut(y, z);
			cascading_cut(z);
		}
	}
}