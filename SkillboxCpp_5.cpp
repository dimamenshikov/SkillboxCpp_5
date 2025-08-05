#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <fstream>
//---------------------------------------------------------------------------------------------------------------------
#define ALPHABET_SIZE 26
#define CONSOLE GetStdHandle(STD_OUTPUT_HANDLE)
//---------------------------------------------------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------------------------------------------------
void SetCursor(short x, short y)
{
	COORD position = { x,y };
	SetConsoleCursorPosition(CONSOLE, position);
}
//---------------------------------------------------------------------------------------------------------------------
COORD FindCursor()
{
	CONSOLE_SCREEN_BUFFER_INFO position;
	GetConsoleScreenBufferInfo(CONSOLE, &position);
	return position.dwCursorPosition;
}
//---------------------------------------------------------------------------------------------------------------------
void EraseHint(int quantitySymbol)
{
	for (int border = FindCursor().X, i = border; i < border + quantitySymbol; ++i)
	{
		cout << " ";
	}
	SetCursor(FindCursor().X - quantitySymbol, FindCursor().Y);
}
//---------------------------------------------------------------------------------------------------------------------
class PrefixTree
{
public:
	PrefixTree** _children = nullptr;
	bool _end = false;
	PrefixTree()
	{
		_children = new PrefixTree*[ALPHABET_SIZE];
		for (int i = 0; i < ALPHABET_SIZE; ++i)
		{
			_children[i] = nullptr;
		}
	}
	~PrefixTree()
	{
		DeleteTrie();
	}
	void DeleteTrie()
	{
		for (int i = 0; i < ALPHABET_SIZE; ++i)
		{
			if (_children[i])
			{
				delete _children[i];
			}
		}
		delete[] _children;
	}
	void AddWord(string& word)
	{
		PrefixTree* currentTrie = this;
		for (int i = 0; i < word.length(); ++i)
		{
			int index = word[i] - 'a';
			if (!currentTrie->_children[index] && index < ALPHABET_SIZE && index >= 0)
			{
				PrefixTree* newNode = new PrefixTree;
				currentTrie->_children[index] = newNode;
			}
			currentTrie = currentTrie->_children[index];
			currentTrie->_end = i == word.length() - 1;
		}
	}

	bool FindWord(const string&& word)
	{
		PrefixTree* currentTrie = this;
		for (int i = 0; i < word.length(); ++i)
		{
			int index = word[i] - 'a';
			if (currentTrie->_children[index] && index < ALPHABET_SIZE && index >= 0)
			{
				currentTrie = currentTrie->_children[index];
				continue;
			}
			return false;
		}
		return currentTrie->_end;
	}
	
	bool FindNearWord(const string& word, string& newWord, bool completely = true)
	{
		PrefixTree* currentTrie = this;
		newWord.clear();
		for (int i = 0; i < word.length(); ++i)
		{
			int index = word[i] - 'a';
			if (currentTrie->_children[index] && index < ALPHABET_SIZE && index >= 0)
			{
				if (completely)
				{
					newWord.push_back(word[i]);
				}
				currentTrie = currentTrie->_children[index];
				continue;
			}
			return false;
		}
		if (currentTrie != this)
		{
			while (!currentTrie->_end)
			{
				for (int i = 0; i < ALPHABET_SIZE; ++i)
				{
					if (currentTrie->_children[i])
					{
						currentTrie = currentTrie->_children[i];
						newWord.push_back(char(i + 'a'));
						break;
					}
				}
			}
			return true;
		}
		return false;
	}
};
//---------------------------------------------------------------------------------------------------------------------
int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	fstream fileDictionary;
	fileDictionary.open("Dictionary.txt");
	if (!fileDictionary.is_open())
	{
		cout << "Не удалось найти словарь";
	}

	PrefixTree* dictionary = new PrefixTree;
	string str, word;
	fileDictionary >> str;
	while (str != ".")
	{
		dictionary->AddWord(str);
		fileDictionary >> str;
	}

	str.clear();
	char symbol;
	int quantitySymbol = 0;

	while(true)
	{
		symbol = _getch();
		EraseHint(quantitySymbol);

		switch(symbol)
		{
		case 27:
			goto Quit;

		case 13:
			str.clear();
			quantitySymbol = 0;
			SetCursor(0, FindCursor().Y + 1);
			break;

		case 32:
			SetCursor(FindCursor().X + 1, FindCursor().Y);
			str.clear();
			quantitySymbol = 0;
			break;

		case 9:
			if (dictionary->FindNearWord(str, word, false))
			{
				cout << word;
				str.append(word);
				quantitySymbol = 0;
			}
			break;

		default:
			if (symbol >= 97 && symbol <= 122)
			{
				cout << symbol;
				str.push_back(symbol);
			}
			if (dictionary->FindNearWord(str, word, false))
			{
				SetConsoleTextAttribute(CONSOLE, FOREGROUND_INTENSITY);
				cout << word;
				quantitySymbol = word.length();
				SetCursor(FindCursor().X - quantitySymbol, FindCursor().Y);
				SetConsoleTextAttribute(CONSOLE, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY);
			}
			break;
		}
	}

Quit:
	fileDictionary.close();
	delete dictionary;
	return 0;
}