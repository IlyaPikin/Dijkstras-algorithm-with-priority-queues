// Copyright 2023 Pikin Ilya
#ifndef DHEAP_H
#define DHEAP_H

#include <vector>

using namespace std;

const int inf = INT_MAX;

struct DHeap
{
    int n;                 // ����� ����� � ����
    int d;                 // ������� d-����
    vector<int>& name;     // ����� ������ � �����
    vector<int>& key;      // ������ ���� ����������� ���� ��� ������� � i-�� ������
    vector<int>& index;    // ������� ������ � d-����, index[name[i]]=i

    DHeap(int n_, int d_, vector<int>& name_, vector<int>& key_, vector<int>& index_);

    void make_heap();               // O(n)
    int parent(int i) const;        // O(1)
    int left_child(int i) const;    // O(1)
    int right_child(int i) const;   // O(1)
    int min_child(int i) const;     // O(d)

    void diving(int i);             // O(log(n))
    void emersion(int i);           // ��������, ������������ diving() - O(log(n)

    void get_min(int& min_name, int& min_key) const;
    int get_size();
    int get_d();
    bool empty();

    void decrease_key(int i, int delta); // O(log(n))
    void delete_min();                   // O(log(n))
    void delete_element(int i);          // O(log(n))
};

#endif  // DHEAP_H