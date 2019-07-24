// List_Pointers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <fstream>

using namespace std;

struct Node {
	int data; //elemento duomenys
	Node* next; //sekantis elementas
};

void Skaitymas(Node *&pirmas);
void Isvedimas(Node *pirmas);
bool Tikrinimas(Node *pirmas);
void Salinimas(Node *&pirmas);
void Iterpimas(Node *&pirmas);
void Rasymas(Node *pirmas);

int main()
{
	Node* pirmas = NULL;
	Skaitymas(pirmas);
	while (1)
	{
		Isvedimas(pirmas);
		cout << "1. Tikrinti ar sarasas tuscias" << endl;
		cout << "2. Prideti elementa i nurodyta saraso vieta" << endl;
		cout << "3. Pasalinti elementa is saraso" << endl;
		cout << "4. Baigti programos darba" << endl;
		int parinkt;
		cin >> parinkt;
		switch (parinkt)
		{
		case 1:
		{
			if (Tikrinimas(pirmas)) cout << "Sarasas tuscias" << endl;
			else {
				cout << "Sarasas nera tuscias" << endl;
				Isvedimas(pirmas);
			}
			system("PAUSE");
			system("cls");
			break;
		}
		case 2:
		{
			Iterpimas(pirmas);
			system("PAUSE");
			system("cls");
			break;
		}
		case 3:
		{
			Salinimas(pirmas);
			system("PAUSE");
			system("cls");
			break;
		}
		case 4:
		{
			Rasymas(pirmas);
			cout << "Programa baigta" << endl;
			return 0;
		}
		}
	}
	return 0;
}

void Skaitymas(Node *&pirmas)
{
	Node* paskutinis = NULL; //issaugoma paskutinio elemento reiksme
	int n;
	ifstream df;
	df.open("input.txt"); //bando atidaryti faila
	if (df.fail()) //tikrina ar failas yra
	{
		cerr << "Nera failo" << endl;
		df.close();
		exit(1);
	}
	if (df.peek() == EOF)
	{
		cout << "Failas tuscias. Automatiskai priskiriama, kad 0 reiksmiu." << endl;
		df.close();
	}
	else
	{
		df >> n; //nuskaito is failo kiek bus elementu
		int kint;
		Node* naujas;
		naujas = new Node; //sukuriamas naujas node/elementas
		pirmas = naujas;
		paskutinis = naujas;

		df >> kint; //nuskaitomas elementas
		naujas->data = kint; //priskiriama naujo node elemento reiksme
		naujas->next = NULL;  // kito node dar nezinom tai kolkas point'eris yra NULL

		for (int i = 1; i < n; i++) //skaitysim elementus ir kursim sarasa
		{
			naujas = new Node;

			df >> kint;
			naujas->data = kint; //priskiria duomenis node'ui
			naujas->next = NULL; //point'eris NULL nes neturim toliau einanciu node kolkas

			paskutinis->next = naujas; //point'inam paskutini node i nauja
			paskutinis = naujas; //is paskutinio node pereinam i dabartini

		}
		df.close();
	}
}
void Isvedimas(Node *pirmas)
{
	if (Tikrinimas(pirmas)) cout << "Sarasas tuscias" << endl;
	else
	{
		Node* dabartinis = pirmas;
		while (dabartinis != NULL)
		{
			cout << dabartinis->data << " ";
			dabartinis = dabartinis->next;
		}
		cout << endl;
	}

}
bool Tikrinimas(Node *pirmas)
{
	if (pirmas == NULL) return true;
	else	return false;
}
void Salinimas(Node *&pirmas)
{
	if (Tikrinimas(pirmas)) cout << "Sarasas tuscias" << endl;
	else
	{
		int nr;
		int k = 1;
		cout << "Nurodykite pozicija skaiciaus kuri noreti pasalinti" << endl;
		cin >> nr;

		Node* dabartinis = pirmas; //kint kuris eis per node'u pozicijas
		if (nr == 1)
		{
			pirmas = dabartinis->next;
		}
		else
		{
			while (k != nr - 1)
			{
				if (dabartinis->next != NULL)
				{
					dabartinis = dabartinis->next; //pereinama i kita adresa;
					k++;
				}
				else break;
			}

			if (k == nr - 1) // jei esame node esanciam pries ta noda kuri reikia istrinti
			{
				Node* temp;
				temp = dabartinis->next; //su temp point'inam i noda kuri reikia istrinti
				dabartinis->next = temp->next; //su dabartiniu (kuris yra siuo metu nodas pries ta noda kuri reikia istrinti) point'inam i noda po tuo kuri reikia istrinti
				delete temp; // atlaisvinam "temp"
			}
			else cout << "Netinkama vieta" << endl;
		}
	}
}
void Iterpimas(Node *&pirmas)
{
	int sk, vieta;
	cout << "Iveskite skaiciu ir nurodykite jo vieta" << endl;
	cin >> sk >> vieta;
	if (vieta == 1)
	{
		Node* naujas; //sukuriamas pointeris
		naujas = new Node;  //sukuriamas naujas node i kuri point'ina point'eris.
		naujas->next = pirmas; //naujo node next point'ina i pirmas point'inima node
		pirmas = naujas; //padarom kad pirmas point'intu i nauja node
		naujas->data = sk;
	}
	else
	{
		int k = 1;
		Node* dabartinis = pirmas;
		while (k != vieta - 1)
		{
			if (dabartinis->next != NULL)
			{
				dabartinis = dabartinis->next;
				k++;
			}
			else break;
		}
		if (k == vieta - 1)
		{
			Node* naujas;
			naujas = new Node;
			naujas->next = dabartinis->next;
			dabartinis->next = naujas;
			naujas->data = sk;
		}
		else cout << "Netinkama vieta" << endl;
	}
	Isvedimas(pirmas);
}
void Rasymas(Node *pirmas)
{
	ofstream rf("output.txt");
	if (Tikrinimas(pirmas)) rf << "Sarasas tuscias" << endl;
	else
	{
		while (pirmas != NULL)
		{
			rf << pirmas->data << " ";
			pirmas = pirmas->next;
		}
		rf << endl;
	}
	rf.close();
}