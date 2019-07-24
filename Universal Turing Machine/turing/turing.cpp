// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <Windows.h>
#include <conio.h>

using namespace std;

int main()
{
	while (1)
	{
		int GalvPoz;
		int stime = 0;
		string juosta, name;
	startagain:
		cout << "Enter file name. To close the program type 'quit'. To change delay type 'time'" << endl;
		cin >> name;
		if (name == "quit") return 0; // jei paraso quit baigia darba (exit(1))
		if (name == "time") { cout << "Enter delay in ms" << endl; cin >> stime; goto startagain; } //keicia refresh rate
		ifstream df(name + ".txt");
		if (df.fail()) { //jei failo nera
			cerr << "File doesn't exist" << endl;
			df.close();
			goto startagain;
		}
		if (df.peek() == EOF) //jei failas tuscias
		{
			cerr << "This file is empty" << endl;
			df.close();
			goto startagain;
		}
		df >> GalvPoz >> juosta;
		map<string, map<char, string[3]>> K; //// sukuriamas toks map, kad pasiekus K[esamaBusena][esamasSimbolis] butu rasti reikiami duomenys testi toliau
		while (!df.eof()) // failo nuskaitymas
		{
			string esamaBusena, naujaBusena;
			char esamasSimbolis, naujasSimbolis, Kryptis;
			df >> esamaBusena;
			if (df.eof()) break;
			df >> esamasSimbolis >> naujasSimbolis >> Kryptis >> naujaBusena;
			K[esamaBusena][esamasSimbolis][0] = naujasSimbolis;
			K[esamaBusena][esamasSimbolis][1] = Kryptis;
			K[esamaBusena][esamasSimbolis][2] = naujaBusena;
			cout << esamaBusena << " " << esamasSimbolis << " " << K[esamaBusena][esamasSimbolis][0] << " " << K[esamaBusena][esamasSimbolis][1] << " " << K[esamaBusena][esamasSimbolis][2] << endl;
		}
		df.close();
		string Kryptis = "R", esamaBusena = "0"; //pradine kyptis/busena
		int iteracija = 1; //veiksmu kiekis
		GalvPoz--; //-1 kad pritaikyt masyvui

		while (GalvPoz != -1 || GalvPoz != juosta.length())
		{
			char esamasSimbolis = juosta[GalvPoz]; //issaugomas simbolis ant kurio juostoje yra galvute
			if (K[esamaBusena][esamasSimbolis][0] == "" || K[esamaBusena][esamasSimbolis][1] == "" || K[esamaBusena][esamasSimbolis][2] == "") { //jei baigiasi instrukcijos
				cout << endl;
				cout << "Stopped on step " << iteracija << " - No State: " << esamaBusena << " for symbol: " << esamasSimbolis << endl;
				break;
			}
			else juosta[GalvPoz] = K[esamaBusena][esamasSimbolis][0][0];     //pakeiciamas simbolis juostoje

			if (K[esamaBusena][esamasSimbolis][1] != Kryptis) Kryptis = K[esamaBusena][esamasSimbolis][1]; //pagal instrukc keiciam krypti
			if (K[esamaBusena][esamasSimbolis][2] != esamaBusena) esamaBusena = K[esamaBusena][esamasSimbolis][2]; //pagal instrukc keiciam busena
																												   //Galvutes judejimas
			if (Kryptis == "R") //pagal atitinkama krypti keiciama galvutes poz
				GalvPoz++;
			else
				GalvPoz--;
			//Isvedimas tikrinimui
			for (int i = 0; i < juosta.length(); i++)
			{
				if (GalvPoz == i)
					cout << "V"; //galvutes poz
				else
					cout << " ";
			}
			cout << "    " << iteracija << endl;
			cout << juosta << endl;
			iteracija++;
			Sleep(stime); //sleep time
			if (_kbhit()) { cout << endl; break; } //paspaudus bet kuri mygtuka sustabdo cikla
		}
	}
	return 0;
}

