// Copyright 2023 Pikin Ilya
#include "../headers/Graph.h"


// struct vtype

vtype::vtype() { name = 0; w = 0; next = nullptr; }

vtype::vtype(uint name_, uint w_, vtype* next_) : name(name_), w(w_), next(next_) {}

vtype& vtype::operator=(const vtype& other)
{
	if (this != &other)
	{
		name = other.name;
		w = other.w;
		next = nullptr;
	}
	return *this;
}


// class Graph

bool Graph::is_engine_initialized = false;
mt19937 Graph::engine;

void Graph::init_generator()
{
	// Создаём псевдо-устройство для получения случайного зерна.
	std::random_device device;
	// Получаем случайное зерно последовательности
	engine.seed(device());
	is_engine_initialized = true;
}

Graph::Graph() : n(0), m(0) { ADJ = nullptr; }
Graph::Graph(string fileName)
{
	generate_from_file(fileName);
}
Graph::Graph(uint n_, uint q_, uint r_)
{
	generate_full_graph(n_, q_, r_);
}
Graph::Graph(uint n_, uint m_, uint q_, uint r_)
{
	generate_rand_graph(n_, m_, q_, r_);
}
Graph::Graph(const Graph& other)
{
	n = other.n;
	m = other.m;

	ADJ = new vtype * [n];

	for (uint i = 0; i < n; i++)
	{
		vtype* other_el = other.ADJ[i];
		ADJ[i] = nullptr;
		vtype** prev_ptr = &ADJ[i];

		while (other_el != nullptr)
		{
			vtype* new_edge = new vtype(other_el->name, other_el->w, nullptr);
			*prev_ptr = new_edge;
			prev_ptr = &(new_edge->next);
			other_el = other_el->next;
		}
	}
}
Graph::~Graph()
{
	clear();
}

void Graph::generate_from_file(string fileName)
{
	clear();
	// чтение данных из файла
	ifstream file(fileName);
	file >> n >> m;
	string str;
	getline(file, str);

	ADJ = new vtype * [n];

	// заполнение списка смежностей
	for (uint i = 0; i < n; i++)
	{
		ADJ[i] = nullptr;
		vtype** prev_ptr = &ADJ[i];

		string new_str;
		getline(file, new_str);

		size_t pos = 0;
		// парсинг строки
		while ((pos = new_str.find('(')) != std::string::npos)
		{
			new_str.erase(0, pos + 1);
			uint name = stoi(new_str, &pos); // получение номера смежной вершины

			new_str.erase(0, pos + 1); // удаление запятой

			uint w = stoi(new_str, &pos); // получение веса ребра

			vtype* new_edge = new vtype(name, w, nullptr);

			*prev_ptr = new_edge; // добавляем адрес нового элемента в старый

			prev_ptr = &(new_edge->next); // обновляем адрес указателя на следующий элемент

		}

	}
}
void Graph::generate_rand_graph(uint n_, uint m_, uint q, uint r)
{
	if (m_ >= n_ * (n_ - 1))
	{
		cout << "Too many edges!";
		generate_full_graph(n_, q, r);
		return;
	}

	clear();
	n = n_;
	m = m_;

	ADJ = new vtype * [n];
	for (uint i = 0; i < n; i++)
		ADJ[i] = nullptr;

	// Проверяем корректность аргументов
	if (q > r) swap(q, r);

	// Инициализация генератора ПСЧ, если он еще не был инициализирован
	if (!is_engine_initialized)
		init_generator();

	// Создаём равномерное распределение в диапозоне [q,r] для генерации веса ребра
	uniform_int_distribution<uint> weight_distribution(q, r);
	// Создаём равномерное распределение в диапозоне [0,n-1] для генерации индекса вершины
	uniform_int_distribution<uint> index_distribution(0, n - 1);

	uint count = 0;
	while (count != m)
	{
		// Вычисляем псевдослучайное число: вызовем распределение как функцию,
		// передав генератор произвольных целых чисел как аргумент.
		uint weight = weight_distribution(engine);
		uint index = index_distribution(engine);
		uint name = 0;
		do
		{
			name = index_distribution(engine);
		} while (name == index);

		vtype** prev_ptr = &ADJ[index];

		// Вставка в список смежности с проверкой кратности ребра 
		while (true)
		{
			vtype* curr_el = *prev_ptr;

			if (curr_el == nullptr)
			{
				vtype* new_edge = new vtype(name + 1, weight, nullptr);
				*prev_ptr = new_edge; // добавляем адрес нового элемента в старый
				count++;
				break;
			}
			else
			{
				if (curr_el->name == name + 1)
				{
					break;
				}
				else if (curr_el->name < name + 1)
				{
					prev_ptr = &(curr_el->next); // обновляем адрес указателя на следующий элемент

				}
				else // curr_el->name > name + 1
				{
					vtype* new_edge = new vtype(name + 1, weight, curr_el);
					*prev_ptr = new_edge; // добавляем адрес нового элемента в предыдущий
					count++;
					break;
				}
			}
		}
	}
}
void Graph::generate_full_graph(uint n_, uint q, uint r)
{
	clear();
	n = n_;
	m = n * (n - 1);

	ADJ = new vtype * [n];
	for (uint i = 0; i < n; i++)
		ADJ[i] = nullptr;

	// Проверяем корректность аргументов
	if (q > r) swap(q, r);

	// Инициализация генератора ПСЧ, если он еще не был инициализирован
	if (!is_engine_initialized)
		init_generator();

	// Создаём равномерное распределение в диапозоне [q,r] для генерации веса ребра
	uniform_int_distribution<uint> weight_distribution(q, r);


	for (uint index = 0; index < n; index++)
	{
		vtype** prev_ptr = &ADJ[index];
		// Вставка в список смежности
		for (uint j = 0; j < n; j++)
		{
			// Вычисляем псевдослучайное число: вызовем распределение как функцию,
			// передав генератор произвольных целых чисел как аргумент.
			uint weight = weight_distribution(engine);

			if (j != index)
			{
				vtype* new_edge = new vtype(j + 1, weight, nullptr);
				*prev_ptr = new_edge; // добавляем адрес нового элемента в старый
				prev_ptr = &(new_edge->next); // обновляем адрес указателя на следующий элемент
			}
		}
	}
}

uint Graph::get_n()
{
	return n;
}
uint Graph::get_m()
{
	return m;
}

void Graph::clear()
{
	if (ADJ != nullptr)
	{

		for (uint i = 0; i < n; i++)
		{
			vtype* current = ADJ[i];
			vtype* next = nullptr;
			while (current != nullptr)
			{
				next = current->next;
				delete current;
				current = next;
			}
		}
		delete[] ADJ;
		ADJ = nullptr;
	}
	n = 0;
	m = 0;
}
void Graph::print_to_console()
{
	cout << "n = " << n << ", m = " << m << endl;
	for (uint i = 0; i < n; i++)
	{
		cout << i + 1 << ": ";
		vtype* curr_el = ADJ[i];
		while (curr_el)
		{
			cout << "(" << curr_el->name << "," << curr_el->w << ") ";
			curr_el = curr_el->next;
		}
		cout << endl;
	}
	cout << endl;
}
void Graph::print_to_file(string fileName)
{
	ofstream file(fileName);
	file << n << " " << m << endl;
	for (uint i = 0; i < n; i++)
	{
		file << i + 1 << ": ";
		vtype* curr_el = ADJ[i];
		while (curr_el)
		{
			file << "(" << curr_el->name << "," << curr_el->w << ") ";
			curr_el = curr_el->next;
		}
		file << endl;
	}
	file << endl;
}

