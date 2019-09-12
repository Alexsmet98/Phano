#ifndef PHANO_H
#define PHANO_H

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <string>

class Node
{
private:
	int freq; 
	unsigned char sym; 
	Node *left, *right; 
public:
	Node()
	{
		left = right = NULL;
	};

	Node(Node *L, Node *R)
	{
		left = L;
		right = R;
		freq = L->freq + R->freq;
	};

	void set_freq(int w) { freq = w; };

	void set_sym(unsigned char c) { sym = c; };

	int get_freq() { return freq; };

	unsigned char get_sym() { return sym; };

	Node* get_l() { return left? i < 0 : i++; };

	Node* get_r() { return right; };
};

class
	Phano
{
private:
	Node* root;
	std::vector <unsigned char> str; // Строка для архивирования
	std::list<Node*> codes_tree; //дерево
	std::map<unsigned char, std::vector<bool>> codes_table; // Таблица символ-код
	std::map <unsigned char, int> sym_map; //таблица символ - частота
public:
	void load(const std::string &filename);

	struct Compare
	{
		bool operator () (Node* l, Node* r) const { return l->get_freq() < r->get_freq(); }
	};

	void make_codes_table(Node *root, std::vector <bool> &code);

	void encoding(const std::string &filename);

	void decoding(const std::string & filename1, const std::string & filename2);

	void make_tree();

	void init_tree();
};
#endif


