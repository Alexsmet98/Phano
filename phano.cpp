#include "Phano.h"

void Phano::load(const std::string & filename)
{
	std::ifstream encod;
	encod.open(filename, std::fstream::binary | std::fstream::in);
	if (!encod.is_open())
		throw std::exception("Файл не найден.");
	while (!encod.eof())
	{
		unsigned char c = 0;
		encod.read((char*)&c, sizeof(c));
		str.push_back(c);
		sym_map[c]++;
	}
	init_tree();
	make_tree();
	root = codes_tree.front();
	std::vector <bool> code;
	make_codes_table(root, code);
	encod.close();
}

void Phano::decoding(const std::string & filename1, const std::string & filename2)
{
	std::ofstream final;
	std::ifstream decod;
	decod.open(filename1, std::fstream::binary | std::fstream::in); 
	final.open(filename2, std::fstream::binary | std::fstream::out); 
	unsigned int s = 0;
	unsigned char next = 0;
	unsigned int count = 0;
	decod >> s;
	for (unsigned int i = 0; i < s; i++)
	{
		unsigned char c = 0;
		decod.read((char*)&c, sizeof(c));
		decod >> sym_map[c];
		count += sym_map[c];
		decod.read((char*)&next, sizeof(next));
	}
	init_tree();
	make_tree();
	root = codes_tree.front();
	std::vector <bool> code;
	make_codes_table(root, code);
	Node *r = root;
	int count_n = 0; unsigned char byte;
	unsigned int i = 0;
	decod.read((char*)&byte, sizeof(char));
	while (i < count - 1)
	{
		bool b = byte & (1 << (7 - count_n));
		if (b)
			r = r->get_r();
		else
			r = r->get_l();
		if (r->get_l() == NULL && r->get_r() == NULL)
		{
			final << r->get_sym();
			r = root;
			i++;
		}
		count_n++;
		if (count_n == 8)
		{
			count_n = 0;
			decod.read((char*)&byte, sizeof(char));
		}
	}
	final.close();
	decod.close();
}



void Phano::make_codes_table(Node *root, std::vector <bool> &code) 
{
	if (root->get_l() != NULL)
	{
		code.push_back(0);
		make_codes_table(root->get_l(), code);
	}
	if (root->get_r() != NULL)
	{
		code.push_back(1);
		make_codes_table(root->get_r(), code);
	}
	if (root->get_l() == NULL && root->get_r() == NULL)
		codes_table[root->get_sym()] = code;
	if (code.size() > 0)
		code.pop_back();
}

void Phano::encoding(const std::string & filename)
{
	std::ofstream wr;
	wr.open(filename, std::fstream::binary | std::fstream::out);
	int count = 0;
	unsigned char buff = 0;
	wr << sym_map.size();
	for (std::map<unsigned char, int>::iterator itr = sym_map.begin(); itr != sym_map.end(); ++itr)
	{
		wr.write((char*)&itr->first, sizeof(unsigned char));
		wr << itr->second << ' ';
	}
	for (size_t i = 0; i < str.size(); i++)
	{
		std::vector<bool> cur = codes_table[str[i]];
		for (size_t n = 0; n < cur.size(); n++)
		{
			buff = buff | cur[n] << (7 - count);
			count++;
			if (count == 8)
			{
				count = 0;
				wr.write((char*)&buff, sizeof(unsigned char));
				buff = 0;
			}
		}
		if (i == str.size() - 1 && count != 8 && count != 0)
		{
			count = 0;
			wr.write((char*)&buff, sizeof(unsigned char));
			buff = 0;
		}
	}
	wr.close();
}
	
void Phano::make_tree()
{
	while (codes_tree.size() != 1) 
	{
		codes_tree.sort(Compare()); 
		Node *L = codes_tree.front();
		codes_tree.pop_front();
		Node *R = codes_tree.front();
		codes_tree.pop_front();
		Node *parent = new Node(L, R); 
		codes_tree.push_back(parent);
		}
}

void Phano::init_tree()
{
	for (std::map<unsigned char, int>::iterator itr = sym_map.begin(); itr != sym_map.end(); ++itr)
	{
		Node *n = new Node;
		n->set_sym(itr->first);
		n->set_freq(itr->second);
		codes_tree.push_back(n);
	}
}
