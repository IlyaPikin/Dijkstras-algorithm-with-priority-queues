// Copyright 2023 Pikin Ilya
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include "Graph.h"
#include "DHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"

using namespace std;

void dijkstra_d_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
    size_t n, size_t d, int s);

void dijkstra_binomial_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
    size_t n, int s);

void dijkstra_fibonacci_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
    size_t n, int s);

#endif // DIJKSTRA_H