#include<iostream>
#include<windows.h>
#include<vector>
#include<cmath>
#include<fstream>
#include<string>
#include<set>
using namespace std;

struct tree
{
	string Char;
	tree*left;
	tree*right;
};
class ShannonFano
{
public:
	struct tabl
	{
		string charLetter;
		float kol;
	};
	struct Binkod
	{
		string letter;
		string kod;
	};
	void InsertFile(string primer);
	void InsertOfFile();
	void ReplacementInputStream(string primer);

	ShannonFano();
	~ShannonFano();
	void Menu(string primer, struct tree*&node);
	void SplitingElement(string primer, struct tree*&node);
	void SplitingGroups(int&razmer, vector<tabl>&groupsP1, vector<tabl>&groupsP2, vector<tabl>&Groups);
	void InsertElementTree(int razmer, struct tree*&node);
	void ObhodTree(struct tree*&node);
	void NeueTree(string ptimer, struct tree*&node);
	void SorrtingLittle(vector<tabl>&Element); // вспомогательная
	void ObhodTreeBinKod(struct tree*&node, int &x);
	void MergerBinaryKod();
	void ClearTree(struct tree*&node);
private:
	vector<tabl>TablElement; // хранятся первоначальные данные таблицы с вероятностью и его символом
	vector<Binkod>binaryKod; // хранятся единичнеы коды каждой разделенной пары включая до одного символа
	vector<Binkod>binaryKodNey; // хранится вся информация об нормальном коде букв
	int differentSymbol;
	int kolGroup; // общее количество узлов в дереве невключая корень
	string primerNeu;
	fstream file;
	string *InputStreemKodBin; // для записи кусков данных для передачи в бинарный файл
	boolean OneSymbol;
};

ShannonFano::ShannonFano()
{
	OneSymbol = false;
	differentSymbol = 0;
	kolGroup = 0;
}
void ShannonFano::Menu(string primer, struct tree*&node)
{
	SplitingElement(primer, node);
	cout << endl;
	for (int j = 0;j < TablElement.size();j++)
	{
		if (j % 5 == 0 && j != 0)
		{
			cout << endl;
		}
		cout << TablElement[j].charLetter << " =\t";
		cout << TablElement[j].kol << "     ";
	}
	cout << endl;
	NeueTree(primer, node);
	InsertElementTree(TablElement.size(), node);
	for (int i = 0;i < differentSymbol;i++)
		binaryKodNey.push_back(Binkod());
	for (int i = 0;i < kolGroup;i++)
		binaryKod.push_back(Binkod());
	int x = -1;
	ObhodTree(node);
	ObhodTreeBinKod(node, x);
	cout << endl;
	MergerBinaryKod();
	if (OneSymbol) // если символ только один(по разновидности)
	{
		binaryKodNey[0].letter = primer[0];
		binaryKodNey[0].kod = '0';
	}
	InputStreemKodBin = new string[primer.size()];
	ReplacementInputStream(primer);
	InsertFile(primer);
	InsertOfFile();
}
void ShannonFano::SplitingElement(string primer, struct tree*&node)
{
	set<char>MySet;
	multiset<char>MyMyltiSet;
	for (int i = 0;i < primer.size();i++)
	{
		MySet.insert(primer[i]);
		MyMyltiSet.insert(primer[i]);
	}
	differentSymbol = MySet.size();
	vector<float>kolElement(differentSymbol);
	set<char>::iterator ik;
	int i = 0;
	for (ik = MySet.begin();i < kolElement.size();ik++, i++)
	{
		primerNeu += *ik;
		kolElement[i] = MyMyltiSet.count(*ik);
	}
	i = 0;
	for (ik = MySet.begin(); ik != MySet.end(); ik++, i++)
	{
		TablElement.push_back(tabl());
		TablElement[i].charLetter = *ik;
		TablElement[i].kol = round((kolElement[i] / primer.size()) * 100) / 100;
	}
	SorrtingLittle(TablElement);
}
void ShannonFano::NeueTree(string primer, struct tree*&node)
{
	node = new tree;
	node->Char = primerNeu;
	node->left = NULL;
	node->right = NULL;
}
void ShannonFano::InsertElementTree(int razmer, struct tree*&node)
{
	vector<tabl>groupsP1;
	vector<tabl>groupsP2;
	SplitingGroups(razmer, groupsP1, groupsP2, TablElement);
	TablElement = groupsP1;
	if (node->left == NULL)
	{
		kolGroup++;
		node->left = new tree;
		node->left->left = node->left->right = NULL;
		for (int i = 0;i < groupsP1.size();i++)
		{
			node->left->Char += groupsP1[i].charLetter;
		}
	}
	if (groupsP2.size() == 0) // если символ один
	{
		OneSymbol = true;
		return;
	}
	if (node->right == NULL)
	{
		kolGroup++;
		node->right = new tree;
		node->right->left = node->right->right = NULL;
		for (int i = 0;i < groupsP2.size();i++)
		{
			node->right->Char += groupsP2[i].charLetter;
		}
	}
	if (groupsP1.size() == 1 && groupsP2.size() == 1)
	{
		return;
	}
	if (groupsP1.size() != 1)
	{
		InsertElementTree(groupsP1.size(), node->left);
	}
	if (groupsP2.size() != 1)
	{
		TablElement = groupsP2;
		InsertElementTree(groupsP2.size(), node->right);
	}
}
void ShannonFano::SplitingGroups(int&razmer, vector<tabl>&groupsP1, vector<tabl>&groupsP2, vector<tabl>&Groups)
{
	float p1 = 0, p2 = 0;
	int indexP1 = 0, indexP2 = 0;
	groupsP1.clear();
	groupsP2.clear();
	for (int j = 0, t = razmer - 1;j != t + 1;)
	{
		if (p1 <= p2)
		{
			p1 += Groups[j].kol;
			groupsP1.push_back(tabl());
			groupsP1[indexP1] = TablElement[j];
			j++;
			indexP1++;
		}
		else
		{
			p2 += Groups[t].kol;
			groupsP2.push_back(tabl());
			groupsP2[indexP2] = TablElement[t];
			t--;
			indexP2++;
		}
	}
	SorrtingLittle(groupsP1);
	SorrtingLittle(groupsP2);
}
void ShannonFano::SorrtingLittle(vector<tabl>&Element)
{
	boolean otvet = true; // сортировка по уменьшению
	for (int i = 0; i < Element.size(); i++)
	{
		otvet = true;
		for (int t = 0; t < Element.size() - 1; t++)
		{
			if (Element[t].kol < Element[t + 1].kol)
			{
				tabl tmp = Element[t];
				Element[t] = Element[t + 1];
				Element[t + 1] = tmp;
				otvet = false;
			}
		}
		if (otvet)break;
	}
}
void ShannonFano::ObhodTree(struct tree*&node)
{
	if (node != NULL)
	{
		cout << node->Char;
		if (node->left != NULL && node->right != NULL)
		{
			cout << " - 0 - ";
		}
		ObhodTree(node->left);
		if (node->left != NULL && node->right != NULL)
		{
			cout << endl;
			cout << node->Char;
			cout << " - 1 - ";
		}
		ObhodTree(node->right);
	}
}
void ShannonFano::ObhodTreeBinKod(struct tree*&node, int &x)
{
	if (node != NULL)
	{
		if (node->left != NULL && node->right != NULL)
		{
			x++;
			binaryKod[x].letter += node->left->Char;
			binaryKod[x].kod += '0';
		}
		ObhodTreeBinKod(node->left, x);
		if (node->left != NULL && node->right != NULL)
		{
			x++;
			binaryKod[x].letter += node->right->Char;
			binaryKod[x].kod += '1';
		}
		ObhodTreeBinKod(node->right, x);
	}
}
void ShannonFano::MergerBinaryKod()
{
	for (int i = 0, t = 0;i < binaryKod.size();i++)
	{
		if (binaryKod[i].letter.size() == 1)
		{
			binaryKodNey[t].letter = binaryKod[i].letter;
			for (int j = 0;j < binaryKod.size();j++)
			{
				if (binaryKod[j].letter != binaryKod[i].letter)
				{
					if (binaryKod[j].letter.find(binaryKod[i].letter) != string::npos) // если есть элемент
					{
						binaryKodNey[t].kod += binaryKod[j].kod;
					}
				}
			}
			binaryKodNey[t].kod += binaryKod[i].kod;
			t++;
		}
	}
}
void ShannonFano::ReplacementInputStream(string primer)
{
	boolean otvet = false;
	for (int i = 0;i < primer.size();i++)
	{
		for (int j = 0;j < binaryKodNey.size();j++)
		{
			string s;
			s += primer[i];
			if (s == binaryKodNey[j].letter)
			{
				for (int r = 0;r < binaryKodNey[j].kod.size();r++)
				{
					InputStreemKodBin[i] += binaryKodNey[j].kod[r];
				}
				otvet = true;
			}
			if (otvet)
			{
				otvet = false;
				break;
			}
		}
	}
}
void ShannonFano::InsertFile(string primer)
{
	file.open("ant", ios::out | ios::trunc | ios::binary);
	for (int i = 0;i<primer.size();i++)
	{
		file.write((char*)&InputStreemKodBin[i], sizeof(InputStreemKodBin[i]));
	}
	cout << "Данные из структуры записаны в файл\n";
	file.close();
}
void ShannonFano::InsertOfFile()
{
	string tmp;
	file.open("ant", ios::in, ios::binary);
	file.seekg(0, ios_base::end);
	int n = file.tellg();
	int kol_elemnt = n / (1 * sizeof(string));
	cout << "kol элементов: " << kol_elemnt << endl;
	file.clear();
	file.seekg(0, ios::beg);
	if (!file.is_open())
	{
		cout << "файл не открыт\n\n" << endl;
		return;
	}
	string slovo; 
	for (int i = 0;i<kol_elemnt;i++)
	{
		file.read((char*)&tmp, sizeof(tmp));
		for (int i = 0;i < binaryKodNey.size();i++)
		{
			if (tmp == binaryKodNey[i].kod)
			{
				slovo += binaryKodNey[i].letter;
			}
		}
		cout << tmp << " ";
	}
	cout << endl;
	cout << slovo;
	cout << endl;
	file.close();
}
void ShannonFano::ClearTree(struct tree*&node)
{
	if (node != NULL)
	{
		ClearTree(node->left);
		ClearTree(node->right);
		delete node;
		node = NULL;
	}
}
ShannonFano::~ShannonFano()
{
	TablElement.clear();
	binaryKod.clear();
	binaryKodNey.clear();
}
void main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	ShannonFano MyCoding;
	tree*node = NULL;
	string primer;
	cout << "Введите слова: ";
	cin >> primer;
	cout << endl;
	MyCoding.Menu(primer, node);
	system("pause");
	return;
}

