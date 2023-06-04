// Copyright 2023 Pikin Ilya
#include "../headers/BinomialHeap.h"

BinomialHeap::BinomialHeap()
{
	n = 0;
	head = nullptr;
}

int BinomialHeap::get_size()
{
	return n;
}

void BinomialHeap::get_min(int& min_name, int& min_key) const
{
	if (head == nullptr)
	{
		min_name = -1;
		min_key = -1;
		return;
	}

	Node* curr_el = head;
	int curr_min_name = head->name;
	int curr_min_key = head->key;
	while (curr_el != nullptr)
	{
		if (curr_el->key < curr_min_key)
		{
			curr_min_name = curr_el->name;
			curr_min_key = curr_el->key;
		}
		curr_el = curr_el->sibling;
	}
	min_name = curr_min_name;
	min_key = curr_min_key;
}

int BinomialHeap::get_key(int name)
{
	Node* nodePtr = LocationInHeap[name];
	if (nodePtr == nullptr)
		return -1;
	return nodePtr->key;
}

bool BinomialHeap::empty()
{
	return n == 0;
}


void BinomialHeap::insert(int name, int key)
{
	if (key < 0)
		return;

	BinomialHeap h;
	Node* nodePtr = new Node;
	initializeNode(nodePtr, name, key);

	h.head = nodePtr;
	h.n++;
	h.LocationInHeap[name] = nodePtr;

	heap_union(h);
}

void BinomialHeap::delete_element(int name)
{
	Node* nodePtr = LocationInHeap[name];
	nodePtr->key = INT_MIN;

	Node* node1 = nodePtr;
	Node* node2 = node1->p;

	// "Всплытие" меньшего узла
	while (node2 != nullptr && node1->key < node2->key)
	{
		swap_data(node1, node2);

		// Обновление информации о размещении данных в куче
		LocationInHeap[node1->name] = node1;
		LocationInHeap[node2->name] = node2;

		node1 = node2;
		node2 = node1->p;
	}
	delete_min();
}

void BinomialHeap::delete_min()
{
	if (head == nullptr)
		return;

	Node* curr_el = head;			// Указатель на текущий узел
	Node* prev_el = nullptr;		// Указатель на предыдущий узел
	Node* min_el = head;			// Указатель на узел с минимальным key
	Node* prev_min = nullptr;		// Указатель на предущий для min_el узел 

	int curr_min_name = head->name;
	int curr_min_key = head->key;

	// Поиск минимума
	while (curr_el != nullptr)
	{
		if (curr_el->key < curr_min_key)
		{
			min_el = curr_el;
			prev_min = prev_el;
			curr_min_name = curr_el->name;
			curr_min_key = curr_el->key;
		}
		prev_el = curr_el;
		curr_el = curr_el->sibling;
	}
	int min_name = curr_min_name;

	// Удаление минимального узла из братского списка старой кучи
	if (prev_min != nullptr && min_el->sibling != nullptr)
	{
		prev_min->sibling = min_el->sibling;
	}
	else if (prev_min != nullptr && min_el->sibling == nullptr)
	{
		prev_min->sibling = nullptr;
	}
	else if (prev_min == nullptr and min_el->sibling != nullptr)
	{
		head = min_el->sibling;
	}
	else if (prev_min == nullptr and min_el->sibling == nullptr)
	{
		head = nullptr;
	}

	// Подсчет числа узлов в дереве c корнем min_el
	int k = min_el->degree;
	k = 1 << k;

	// Удаление мин узла
	LocationInHeap.erase(min_name);
	n -= k;

	// Удаление указателей дочерних узлов из LocationInHeap не требуется,
	// так как они вернутся в старую кучу после слияния

	// Если нет дочерних узлов, то сливать нечего. 
	if (min_el->child == nullptr)
	{
		delete min_el;
		return;
	}
	//Иначе, производим слияние...

	// Установка поля p каждого дочернего узла равным nullptr
	Node* child = min_el->child;
	while (child != nullptr) {
		child->p = nullptr;
		child = child->sibling;
	}

	// Обращение порядка связанного списка дочерних узлов min_el
	stack<Node*> s;
	child = min_el->child;
	while (child != nullptr) {
		s.push(child);
		child = child->sibling;
	}

	curr_el = s.top();
	Node* temp = curr_el;
	s.pop();

	while (!s.empty()) {
		curr_el->sibling = s.top();
		s.pop();
		curr_el = curr_el->sibling;
	}

	curr_el->sibling = nullptr;

	BinomialHeap h;
	h.n = k - 1;
	h.head = temp;
	// Добавление указателей в LocationInHeap кучи h не требуется

	// Очистка памяти
	delete min_el;

	// Слияние старой и новой куч
	heap_union(h);
}

void BinomialHeap::extract_min(int& min_name, int& min_key)
{
	if (head == nullptr)
	{
		min_name = -1;
		min_key = -1;
		return;
	}

	Node* curr_el = head;			// Указатель на текущий узел
	Node* prev_el = nullptr;		// Указатель на предыдущий узел
	Node* min_el = head;			// Указатель на узел с минимальным key
	Node* prev_min = nullptr;		// Указатель на предущий для min_el узел 

	int curr_min_name = head->name;
	int curr_min_key = head->key;

	// Поиск минимума
	while (curr_el != nullptr)
	{
		if (curr_el->key < curr_min_key)
		{
			min_el = curr_el;
			prev_min = prev_el;
			curr_min_name = curr_el->name;
			curr_min_key = curr_el->key;
		}
		prev_el = curr_el;
		curr_el = curr_el->sibling;
	}
	min_name = curr_min_name;
	min_key = curr_min_key;

	// Удаление минимального узла из братского списка старой кучи
	if (prev_min != nullptr && min_el->sibling != nullptr)
	{
		prev_min->sibling = min_el->sibling;
	}
	else if (prev_min != nullptr && min_el->sibling == nullptr)
	{
		prev_min->sibling = nullptr;
	}
	else if (prev_min == nullptr and min_el->sibling != nullptr)
	{
		head = min_el->sibling;
	}
	else if (prev_min == nullptr and min_el->sibling == nullptr)
	{
		head = nullptr;
	}

	// Подсчет числа узлов в дереве c корнем min_el
	int k = min_el->degree;
	k = 1 << k;

	// Удаление мин узла
	LocationInHeap.erase(min_name);
	n -= k;

	// Удаление указателей дочерних узлов из LocationInHeap не требуется,
	// так как они вернутся в старую кучу после слияния

	// Если нет дочерних узлов, то сливать нечего. 
	if (min_el->child == nullptr)
	{
		delete min_el;
		return;
	}
	//Иначе, производим слияние...

	// Установка поля p каждого дочернего узла равным nullptr
	Node* child = min_el->child;
	while (child != nullptr) {
		child->p = nullptr;
		child = child->sibling;
	}

	// Обращение порядка связанного списка дочерних узлов min_el
	stack<Node*> s;
	child = min_el->child;
	while (child != nullptr) {
		s.push(child);
		child = child->sibling;
	}

	curr_el = s.top();
	Node* temp = curr_el;
	s.pop();

	while (!s.empty()) {
		curr_el->sibling = s.top();
		s.pop();
		curr_el = curr_el->sibling;
	}

	curr_el->sibling = nullptr;

	BinomialHeap h;
	h.n = k - 1;
	h.head = temp;
	// Добавление указателей в LocationInHeap кучи h не требуется

	// Очистка памяти
	delete min_el;

	// Слияние старой и новой куч
	heap_union(h);
}

void BinomialHeap::decrease_key(int name, int new_key)
{
	Node* nodePtr = LocationInHeap[name];
	if (nodePtr == nullptr || new_key >= nodePtr->key || new_key < 0)
		return;
	nodePtr->key = new_key;

	Node* node1 = nodePtr;
	Node* node2 = node1->p;

	// "Всплытие" меньшего узла
	while (node2 != nullptr && node1->key < node2->key)
	{
		swap_data(node1, node2);

		// Обновление информации о размещении данных в куче
		LocationInHeap[node1->name] = node1;
		LocationInHeap[node2->name] = node2;

		node1 = node2;
		node2 = node1->p;
	}
}

void BinomialHeap::heap_union(BinomialHeap h1)
{
	// Обработка случаев с пустыми кучами
	if (head == nullptr && h1.head != nullptr)
	{
		n = h1.n;
		head = h1.head;
		LocationInHeap.insert(h1.LocationInHeap.begin(), h1.LocationInHeap.end());
		return;
	}
	else if (h1.head == nullptr)
	{
		return;
	}

	heap_merge(h1);

	// Проход по братскому списко и объединение деревьев с одинаковыми степенями
	Node* curr = head;
	Node* prev = nullptr;
	Node* next = curr->sibling;

	while (next != nullptr)
	{
		// Если 2 соседних корня имеют разную степень или 3 корня подряд имеют
		// одинаковую степень, то переходим к следующему дереву.
		if ((curr->degree != next->degree) ||
			(next->sibling != nullptr && curr->degree == next->sibling->degree))
		{
			prev = curr;
			curr = next;
		}
		else
		{
			// Иначе сливаем деревья с одинаковой степенью
			if (curr->key <= next->key)
			{
				curr->sibling = next->sibling;
				BinomialHeap::link_trees(next, curr);
			}
			else
			{
				if (prev == nullptr)
				{
					head = next;
				}
				else
				{
					prev->sibling = next;
				}
				BinomialHeap::link_trees(curr, next);
				curr = next;
			}
		}
		next = curr->sibling;
	}
}


void BinomialHeap::link_trees(Node* node1, Node* node2)
{
	node1->p = node2;
	node1->sibling = node2->child;
	node2->child = node1;
	node2->degree++;
}

void BinomialHeap::initializeNode(Node* nodePtr, int name, int key)
{
	nodePtr->name = name;
	nodePtr->key = key;

	nodePtr->degree = 0;
	nodePtr->p = nullptr;
	nodePtr->child = nullptr;
	nodePtr->sibling = nullptr;
}

void BinomialHeap::swap_data(Node* node1, Node* node2)
{
	int temp_name = node1->name;
	node1->name = node2->name;
	node2->name = temp_name;

	int temp_key = node1->key;
	node1->key = node2->key;
	node2->key = temp_key;
}

void BinomialHeap::heap_merge(BinomialHeap& h1)
{
	Node* curr1 = head;			// Указатель на узлы братского списка первой кучи
	Node* curr2 = h1.head;		// Указатель на узлы братского списка второй кучи
	Node* curr3 = nullptr;		// Указатель на узлы братского списка итоговой кучи

	// Определение первого узла в итоговой куче
	if (curr1->degree <= curr2->degree)
	{
		curr3 = curr1;
		curr1 = curr1->sibling;
	}
	else
	{
		curr3 = curr2;
		curr2 = curr2->sibling;
	}

	head = curr3;

	// Слияние двух куч в единый связанный список, отсортированный
	// по степеням в монотонно возрастающем порядке
	while (curr1 != nullptr && curr2 != nullptr)
	{
		if (curr1->degree <= curr2->degree)
		{
			curr3->sibling = curr1;
			curr1 = curr1->sibling;
		}
		else
		{
			curr3->sibling = curr2;
			curr2 = curr2->sibling;
		}
		curr3 = curr3->sibling;
	}

	// Копирование оставшихся в первой куче деревьев
	if (curr1 != nullptr)
	{
		while (curr1 != nullptr)
		{
			curr3->sibling = curr1;
			curr1 = curr1->sibling;
			curr3 = curr3->sibling;
		}
	}

	// Копирование оставшихся во второй куче деревьев
	if (curr2 != nullptr)
	{
		while (curr2 != nullptr)
		{
			curr3->sibling = curr2;
			curr2 = curr2->sibling;
			curr3 = curr3->sibling;
		}
	}

	// Слияние имен и указателей в LocationInHeap, увеличение кол-ва узлов итоговой кучи
	LocationInHeap.insert(h1.LocationInHeap.begin(), h1.LocationInHeap.end());
	n += h1.n;
}
