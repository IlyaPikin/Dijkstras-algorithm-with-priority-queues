// Copyright 2023 Pikin Ilya
#include "../headers/DHeap.h"

DHeap::DHeap(int n_, int d_, vector<int>& name_, vector<int>& key_, vector<int>& index_) : n(n_), d(d_),
name(name_), key(key_), index(index_)
{
    make_heap();
}

void DHeap::make_heap()     // O(n)
{
    for (int i = 1; i <= n; i++)
        diving(n - i);
}

int DHeap::parent(int i) const // O(1)
{
    if (i == 0)
        return -1;        // Если i-ый узел является корнем
    else
        return (i - 1) / d;
}

int DHeap::left_child(int i) const // O(1)
{
    int j = i * d + 1;
    if (j >= n)
        return -1;        // Если i-ый узел является листом
    else
        return j;
}

int DHeap::right_child(int i) const // O(1)
{
    int j = left_child(i);
    if (j == -1)
        return -1;           // Если i-ый узел является листом
    j = j + d - 1;
    return j < (n - 1) ? j : (n - 1); // возврат минимума
}

int DHeap::min_child(int i) const // O(d)
{
    int leftСhild = left_child(i);
    if (leftСhild == -1)
        return -1;         // Если i-ый узел является листом
    int rightСhild = right_child(i);
    int minСhild = leftСhild;

    for (int j = leftСhild + 1; j <= rightСhild; j++)
        if (key[j] < key[minСhild])
            minСhild = j;
    return minСhild;
}

void DHeap::diving(int i) // O(log(n))
{   // Алгоримт работает без использования swap() в цикле, так как замену
    // key0 и name0 можно сделать один раз после цикла в самом конце.
    // В цикле последовательно идут присваивания.
    int key0 = key[i], name0 = name[i];
    int child = min_child(i);
    while (child != -1 && key0 > key[child])
    {
        key[i] = key[child];
        name[i] = name[child];
        // Сейчас index[name[i]] == child. Исправим это.
        index[name[i]] = i;
        // Погружаемся глубже
        i = child;
        child = min_child(i);
    }
    key[i] = key0;
    name[i] = name0;
    index[name[i]] = i;
}

void DHeap::emersion(int i) // Операция, симметричная diving() - O(log(n)
{
    int key0 = key[i], name0 = name[i];
    int father = parent(i);
    while (father != -1 && key0 < key[father])
    {
        key[i] = key[father];
        name[i] = name[father];
        index[name[i]] = i;
        i = father;
        father = parent(i);
    }
    key[i] = key0;
    name[i] = name0;
    index[name[i]] = i;
}

void DHeap::get_min(int& min_name, int& min_key) const
{
    min_name = name[0];
    min_key = key[0];
}

int DHeap::get_size()
{
    return n;
}

int DHeap::get_d()
{
    return d;
}

bool DHeap::empty()
{
    return n == 0;
}

void DHeap::decrease_key(int i, int delta) // O(log(n))
{
    key[i] -= abs(delta);
    emersion(i);
}

void DHeap::delete_min() // O(log(n))
{
    int name0 = name[0], key0 = key[0];
    name[0] = name[n - 1];
    key[0] = key[n - 1];
    name[n - 1] = name0;
    key[n - 1] = key0;
    index[name0] = n - 1;
    n--;                // Физического удаления не происходит
    if (n > 1) {
        diving(0);
    }
}

void DHeap::delete_element(int i) // O(log(n))
{
    key[i] = INT_MIN;
    emersion(i);
    delete_min();
}