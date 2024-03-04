// Copyright 2023 Pikin Ilya
#ifndef GRAPH_H
#define GRAPH_H

#define uint unsigned

#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>

using namespace std;

struct vtype
{
	uint name;
	uint w;
	vtype* next;

	vtype();
	vtype(uint name_, uint w_, vtype* next_);
	vtype& operator=(const vtype& other);
	~vtype() {}
};

class Graph
{
private:

	uint n;								// Число вершин в графе
	uint m;								// Число рёбер в графе
	vtype** ADJ;						// Списки смежности графа
	static bool is_engine_initialized;	// Был ли инициализирован engine
	static mt19937 engine;				// Генератор псевдослучайных чисел

private:
	void init_generator();

public:
	Graph();
	Graph(string fileName);
	Graph(uint n_, uint q_, uint r_);
	Graph(uint n_, uint m_, uint q_, uint r_);
	Graph(const Graph& other);
	~Graph();

	void generate_from_file(string fileName);
	void generate_rand_graph(uint n_, uint m_, uint q, uint r);
	void generate_full_graph(uint n_, uint q, uint r);

	uint get_n();
	uint get_m();

	void clear();
	void print_to_console();
	void print_to_file(string fileName);

	friend void dijkstra_d_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
		size_t n, size_t d, int s);
	friend void dijkstra_binomial_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
		size_t n, int s);
	friend void dijkstra_fibonacci_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
		size_t n, int s);
};


#endif  // GRAPH_H