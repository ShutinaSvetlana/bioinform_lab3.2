#include "stdafx.h"
#include "stdio.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include"algorithm"
using namespace std;

pair<int*, int> ByPass(int* a, int i, int l, int k)
{
	for (int j = i; j > -1; j--)
	{
		if (a[j] < k - 1)
		{
			a[j]++;
			return make_pair(a, j);
		}
	}
	return make_pair(a, -1);
}

pair<int*, int>  NextVertex(int* a, int i, int l, int k)
{
	if (i < l)
	{
		a[i + 1] = 0;
		return make_pair(a, i + 1);
	}
	else
	{
		for (int j = l - 1; j > -1; j--)
		{
			if (a[j] < k - 1)
			{
				a[j]++;
				return make_pair(a, j);
			}
		}
	}
	return make_pair(a, -1);
}
int TotalDistance(int* s, int** str,int n, int t, int u)
{
	int score = 0;
	int* new_s=new int[u];
	int* count = new int[t];
	int m = 0;
	for (int j = 0; j < t; j++)
	{
		int count_in_str = u;
		for (int i = 0; i < n - u + 1; i++)
		{			
			for (int k = 0; k < u; k++)
				new_s[k] = str[j][k+i];
			for (int k = 0; k < u; k++)
				if (new_s[k] != s[k])
					m++;
			if (m <= count_in_str)
				count_in_str = m;
			m = 0;
		}
		count[j] = count_in_str;
		score += count[j];
	}

	delete[] count;

	return score;
}

int** Int(string* str, int t)
{
	int** DNA = new int*[t];
	for (int i = 0; i < t; i++)
		DNA[i] = new int[strlen(str[0].c_str())];
	for (int i = 0; i < t; i++)
	{
		for (int j = 0; j < strlen(str[0].c_str()); j++)
		{
			if (str[i].c_str()[j] == 'A')
				DNA[i][j] = 0;
			else
			{
				if(str[i].c_str()[j] == 'C')
					DNA[i][j] = 1;
				else
				{
					if (str[i].c_str()[j] == 'G')
						DNA[i][j] = 2;
					else
						DNA[i][j] = 3;
				}
			}
		}
	}
	return DNA;
}
char* Char(int* s, int l)
{
	char* bestW = new char[l];
	for (int i = 0; i < l; i++)
	{
		if (s[i] == 0)
			bestW[i] = 'A';
		else
			if (s[i] == 1)
				bestW[i] = 'C';
			else
				if (s[i] == 2)
					bestW[i] = 'G';
				else
					bestW[i] = 'T';
	}
	return bestW;
}
char* SearchMotif(string* str, int t, int l)
{	
	char* bestWord = new char[l];
	int* bestW = new int[l];
	pair<int*, int> p;
	int bestD = INT32_MAX;
	int** DNA = new int*[t];
	for (int i = 0; i < t; i++)
		DNA[i] = new int[strlen(str[0].c_str())];
	DNA= Int(str,t);
	int n = strlen(str[0].c_str());
	int* s = new int[l];
	for (int i = 0; i < t; i++)
		s[i] = 0;
	int* prefix = new int[t];
	for (int i = 0; i < t; i++)
		prefix[i] = 0;
	int* word = new int[t];
	for (int i = 0; i < t; i++)
		word[i] = 0;
	int i = 0;
	int optimisticD = 0;
	while (i > -1)
	{
		if (i < l - 1)
		{
			for (int j = 0; j <= i; j++)
				prefix[j] = s[j];
			optimisticD = TotalDistance(prefix, DNA,n, t, i+1);
			if (optimisticD > bestD)
			{
				p = ByPass(s,i,l,4);
				s = p.first;
				i = p.second;
			}
			else
			{
				p = NextVertex(s, i, l, 4);
				s = p.first;
				i = p.second;
			}
		}
		else
		{
			for (int j = 0; j < l; j++)
				word[j] = s[j];
			if (TotalDistance(word, DNA,n, t, l) <= bestD)
			{
				bestD = TotalDistance(word, DNA,n, t,l);
				for (int j = 0; j < l; j++)
					bestW[j] = word[j];
			}
			p = NextVertex(s, i, l, 4);
			s = p.first;
			i = p.second;
		}
	}
	bestWord = Char(bestW, l);
	return bestWord;
}

int main()
{
	setlocale(LC_ALL, ".1251");

	ifstream input("input.txt"); // открыли файл для чтения	
	string l1; //размер шаблона
	input >> l1; //считали кол-во	

	int l; //размер шаблона
	l = atoi(l1.c_str()); //перевод в int	

	int count = 0;
	string s; //строка		
	while (input)
	{
		getline(input, s); //считали строки из файла		
		count++; //кол-во строк
	}
	input.close(); // закрываем файл

	input.open("input.txt"); // открыли файл для чтения	

	int n = count - 2; //кол-во строк с буквами	

	string* str = new string[n]; //строка
	getline(input, str[0]); //считали первую строку из файла до конца
	for (int i = 0; i < n; i++)
		getline(input, str[i]);

	char* result = new char[l]; //результат
	result = SearchMotif(str, n, l);

	input.close(); // закрываем файл

	ofstream output; //создаем файл для записи
	output.open("output.txt"); //открыли файл для записи		
	for (int i = 0; i < l; i++)
		output << result[i];
	output.close(); // закрываем файл	

	delete[] str;
	delete[] result;
	return 0;
}

