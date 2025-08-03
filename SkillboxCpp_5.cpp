#include <iostream>
#include <Windows.h>
#include <queue>
#include <conio.h>

#define ALPHABET_SIZE 26

using namespace std;

class Trie
{
public:
	Trie** _children = nullptr;
	bool _end = false;
	Trie()
	{
		_children = new Trie * [ALPHABET_SIZE];
		for (int i = 0; i < ALPHABET_SIZE; ++i)
		{
			_children[i] = nullptr;
		}
	}
	~Trie()
	{

	}

	void AddWord(string& word)
	{
		Trie** currentTrie = _children;
		for (int i = 0; i < word.length(); ++i)
		{
			if (!currentTrie[word[i] - 'a'])
			{
				Trie* newNode = new Trie;
				currentTrie[word[i] - 'a'] = newNode;
			}
			currentTrie = currentTrie[word[i] - 'a']->_children;
		}
	}

	//bool FindWord(const string& word)
	//{
	//	Trie** currentTrie = _children;
	//	for (int i = 0; i < word.length(); ++i)
	//	{
	//		if (currentTrie && currentTrie[word[i] - 'a'])
	//		{
	//			currentTrie = _children[i]->_children;
	//		}
	//	}
	//	return (currentTrie[word[word.length() - 1] - 'a']->_end);
	//}
	
	void ShowTrie()
	{
		Trie** currentTrie = _children;
		queue<int> o;
		
		while (currentTrie)
		{
			o.push(0);
			while (!o.empty())
			{
				o.pop();
				if (currentTrie)
				{
					for (int i = 0; i < ALPHABET_SIZE; ++i)
					{
						if (currentTrie[i])
						{
							cout << char(i + 'a') << " ";
							o.push(i);
						}
					}
					cout << endl;
				}
			} 
			currentTrie = currentTrie[o.front()]->_children;
		}
	}
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	Trie* d = new Trie;
	string s = "qewref";
	d->AddWord(s);
	s = "qewaaa";
	d->AddWord(s);
	d->ShowTrie();

	delete d;
	return 0;
}