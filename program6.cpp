// Program 6: Parallel word search in a file (OpenMP)
// Each thread searches a different word, so the searches run in parallel.
// Put some text in words.txt next to the program.
// Compile: g++ -fopenmp program6_word_search.cpp -o p6
// Run:     ./p6
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <omp.h>
using namespace std;

const int COUNT = 5;
string search_words[COUNT] = {"lorem", "ipsum", "dolor", "sit", "amet"};

string toLower(string s) {
	for (char &c : s) c = tolower(c);
	return s;
}

int main() {
	// read the file once into memory
	ifstream file("words.txt");
	if (!file) { cout << "words.txt not found" << endl; return 1; }
	vector<string> text;
	string w;
	while (file >> w) text.push_back(toLower(w));

	int counts[COUNT] = {0};
	double start = omp_get_wtime();

	#pragma omp parallel for   // add num_threads(1/2/4/8) to compare
	for (int i = 0; i < COUNT; i++)
		for (size_t j = 0; j < text.size(); j++)
			if (text[j] == search_words[i]) counts[i]++;

	double time = omp_get_wtime() - start;

	for (int i = 0; i < COUNT; i++)
		cout << search_words[i] << " : " << counts[i] << endl;
	cout << "Time: " << time << " s" << endl;
}
