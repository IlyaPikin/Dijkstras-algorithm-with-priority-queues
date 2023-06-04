// Copyright 2023 Pikin Ilya
#include "../headers/Dijkstra.h"

void dijkstra_d_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
                     size_t n, size_t d, int s)
{
    // Так как имена вершин графа начинаются с нуля, изменим имя s
    s--;

    // Массивы для d-кучи
    vector<int> name(n);
    vector<int> index(n);
    vector<int> key(n);

    // Инициализация
    for (int i = 0; i < n; i++)
    {
        pred[i] = inf;
        dist[i] = inf;
        name[i] = i;
        index[i] = i;
        key[i] = inf;
    }
    key[s] = 0;

    DHeap heap(n, d, name, key, index);

    while (!heap.empty())
    {
        int min_name, min_key;
        heap.get_min(min_name, min_key);
        heap.delete_min();
        dist[min_name] = min_key;

        // Просматриваем окрестность вершины min_name
        vtype* curr_el = graph.ADJ[min_name];
        while (curr_el != nullptr)
        {
            int j = curr_el->name - 1;  // Имя текущей вершины в графе - 1
            int j_ind = heap.index[j];

            // Релаксация ребра
            if (key[j_ind] > dist[min_name] + curr_el->w) 
            {
                key[j_ind] = dist[min_name] + curr_el->w;
                heap.emersion(j_ind);
                pred[j] = min_name;
            }
            curr_el = curr_el->next;
        }

    }
}

void dijkstra_binomial_heap(Graph& graph, vector<int>& dist, vector<int>& pred,
    size_t n, int s)
{
    // Так как индексация вершин графа начинается с нуля, а не с 1, уменьшим s
    s--;

    BinomialHeap heap;

    // Инициализация
    for (int i = 0; i < n; i++)
    {
        pred[i] = inf;
        dist[i] = inf;
        heap.insert(i, inf);
    }
    heap.decrease_key(s, 0); // key[s] == 0

    while (!heap.empty())
    {
        int min_name, min_key;
        heap.extract_min(min_name, min_key);
        dist[min_name] = min_key;


        // Просматриваем окрестность вершины min_name
        vtype* curr_el = graph.ADJ[min_name];
        while (curr_el != nullptr)
        {
            int j = curr_el->name - 1;  // Имя текущей вершины в графе - 1

            // Если кратчайший путь до j уже найден, то пропускаем вершину
            if (dist[j] != inf)
            {
                curr_el = curr_el->next;
                continue;
            }

            int key_j = heap.get_key(j);

            // Релаксация ребра
            if (key_j > dist[min_name] + curr_el->w)
            {
                int new_key = dist[min_name] + curr_el->w;
                heap.decrease_key(j, new_key);
                pred[j] = min_name;
            }
            curr_el = curr_el->next;
        }

    }
}