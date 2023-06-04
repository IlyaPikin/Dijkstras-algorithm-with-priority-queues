// Copyright 2023 Pikin Ilya
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>

#include "../headers/Graph.h"
#include "../headers/DHeap.h"
#include "../headers/BinomialHeap.h"
#include "../headers/Dijkstra.h"

using namespace std;


// Класс, необходимый для вывода вещественных чисел с символом запятой
class comma : public numpunct<char> {
public:
	comma() : numpunct<char>() {}
protected:
	char do_decimal_point() const {
		return ',';
	}
};

bool compare(int n, vector<int>& dist1, vector<int>& dist2, vector<int>& up1, vector<int>& up2)
{
	for (int i = 0; i < n; i++)
	{
		if ((dist1[i] != dist2[i]))
		{
			cout << "Distances are not equel!" << endl;
			cout << i + 1 << ": " << dist1[i] << " " << dist2[i];
			cout << " parent = " << up1[i] + 1 << " " << up2[i] + 1 << endl;
			return false;
		}
	}
	cout << "Correct!" << endl;
	return true;
}

void test_binomial() {
	BinomialHeap bheap(5);
	bheap.insert(0, 10);
	bheap.insert(1, 18);
	bheap.insert(2, 25);
	bheap.insert(3, 12);
	bheap.insert(4, 9);

	bheap.decrease_key(2, 5);

	int min_name, min_key;
	bheap.extract_min(min_name, min_key);

	bheap.get_min(min_name, min_key);
}

void ex1()
{
	// ---------------- 1st experiment ----------------

	ofstream ex1("C:\\Users\\Илья\\source\\repos\\deeplom\\deeplom\\experiments\\ex1\\ex1_2.csv");
	//ostream& ex1 = cout;

	ex1 << fixed;


	for (uint i = 1; i <= 5001; i += 100)
	{
		uint n = i;
		uint m = n * (n - 1);
		uint q = 1;
		uint r = 1000;

		ex1 << n << ";" << m << ";" << q << ";" << r;

		Graph graph(n, q, r); // generate full graph

		// Dijkstra's algorithm with d-heap
		vector<int> dist1(n);
		vector<int> pred1(n);

		auto start_d = std::chrono::steady_clock::now();
		dijkstra_d_heap(graph, dist1, pred1, n, 2, 1);
		auto end_d = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_d = end_d - start_d;

		ex1 << ";" << elapsed_seconds_d.count();
		//ex1 << ";" << elapsed_seconds_d.count() << endl;


		// Dijkstra's algorithm with binomial heap

		vector<int> dist2(n);
		vector<int> pred2(n);

		auto start_fb = std::chrono::steady_clock::now();
		dijkstra_binomial_heap(graph, dist2, pred2, n, 1);
		auto end_fb = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed_seconds_fb = end_fb - start_fb;

		ex1 << ";" << elapsed_seconds_fb.count() << endl;

		if (!compare(n, dist1, dist2, pred1, pred2))
		{
			system("pause");
		}
	}

	ex1.close();
}

void cake_test()
{
	//Graph cake("./input.txt");
	Graph cake(100, 1, 10);
	cake.print_to_console();
	cout << endl;

	int n = cake.get_n();

	vector<int> dist1(n);
	vector<int> pred1(n);

	dijkstra_d_heap(cake, dist1, pred1, n, 2, 1);

	//for (int i = 0; i < n; i++) {
	//	cout << dist[i] << " ";
	//}
	//cout << endl;
	//for (int i = 0; i < n; i++) {
	//	cout << pred[i] << " ";
	//}

	//cout << endl << endl;

	vector<int> dist2(n);
	vector<int> pred2(n);

	dijkstra_binomial_heap(cake, dist2, pred2, n, 1);

	//for (int i = 0; i < n; i++) {
	//	cout << dist1[i] << " ";
	//}
	//cout << endl;
	//for (int i = 0; i < n; i++) {
	//	cout << pred1[i] << " ";
	//}

	compare(n, dist1, dist2, pred1, pred2);
}

int main() {
	// Настройка вывода вещественный чисел с символом запятой
	locale loccomma(cout.getloc(), new comma);
	locale::global(loccomma);

	//test_binomial();
	//cake_test();

	ex1();
	

	return 0;
}