// Parallel Turing Machine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <Windows.h>
#include <conio.h>

using namespace std;

map<string, string> message;

bool stopMessage(string failas) {
	if (message[failas] == "") return false;
	else {
		cout << message[failas] << endl; //jei message[failas] nera tuscias isvedamas msg apie sustabdyta juosta
		return true;
	}
}

void isvedimas(int stime, string juosta, int GalvPoz, int iteracija, string failas)
{
	//Isvedimas 
	if (!stopMessage(failas)) { // jei stopMessage tuscias rodo iteracija ir kurioj vietoj galvute
		cout << failas << ".txt " << endl;
		for (int i = 0; i < juosta.length(); i++)
		{
			if (GalvPoz == i)
				cout << "V"; //galvutes poz
			else
				cout << " ";
		}
		cout << "    " << iteracija << endl;
		cout << juosta << endl; //isvedama juosta
		Sleep(stime); //sleep time
	}

}

bool nuskaitymas(string failas, int GalvPoz[], string juosta[], map<string, map<string, map<char, string[3]>>> &K, int i)
{
	message[failas] = ""; // padaro tuscia messsaga[failas] kad butu galima is naujo ivesti ten duomenis
	ifstream df(failas + ".txt");
	if (df.fail()) { //jei failo nera
		cerr << "File doesn't exist" << endl;
		df.close();
		return false;
	}
	if (df.peek() == EOF) //jei failas tuscias
	{
		cerr << "This file is empty" << endl;
		df.close();
		return false;
	}
	df >> GalvPoz[i] >> juosta[i];
	GalvPoz[i]--; //-1 kad pritaikyt masyvui
	while (!df.eof()) // failo nuskaitymas
	{
		string esamaBusena; //naujaBusena;
		char esamasSimbolis; //naujasSimbolis, Kryptis;
		df >> esamaBusena;
		while (esamaBusena == "") // kad neskaitytu tusciu eiluciu ar simboliu
		{
			df >> esamaBusena;
			if (df.eof()) break;
		}
		df >> esamasSimbolis; //>> naujasSimbolis >> Kryptis >> naujaBusena;
		df >> K[failas][esamaBusena][esamasSimbolis][0]; //naujas simbolis
		df >> K[failas][esamaBusena][esamasSimbolis][1]; // kryptis
		df >> K[failas][esamaBusena][esamasSimbolis][2]; //nauja busena
		cout << esamaBusena << " " << esamasSimbolis << " " << K[failas][esamaBusena][esamasSimbolis][0] << " " << K[failas][esamaBusena][esamasSimbolis][1] << " " << K[failas][esamaBusena][esamasSimbolis][2] << endl; //instrukciju isvedimas
	}
	df.close();
	return true;
}



bool skaiciavimai(string esamaBusena[], string failas, int GalvPoz[], string juosta[], map<string, map<string, map<char, string[3]>>> K, int i, int iteracija, int &stop) {

	if (GalvPoz[i] == -1 || GalvPoz[i] == juosta[i].length()) return false;

	char esamasSimbolis = juosta[i][GalvPoz[i]]; //issaugomas simbolis ant kurio juostoje yra galvute
	if (K[failas][esamaBusena[i]][esamasSimbolis][0] == "" || K[failas][esamaBusena[i]][esamasSimbolis][1] == "" || K[failas][esamaBusena[i]][esamasSimbolis][2] == "") { //jei baigiasi instrukcijos
		message[failas] = failas + ") Stopped on step " + to_string(iteracija) + " - No State: " + esamaBusena[i] + " for symbol: " + esamasSimbolis + " in position: " + to_string(GalvPoz[i]) + "\n" + juosta[i];

		GalvPoz[i] = -1;
		stop++;
		return false;
	}
	else {
		juosta[i][GalvPoz[i]] = K[failas][esamaBusena[i]][esamasSimbolis][0][0];     //pakeiciamas simbolis juostoje

																																		   //Galvutes judejimas
		if (K[failas][esamaBusena[i]][esamasSimbolis][1] == "R") //pagal atitinkama krypti keiciama galvutes poz
			GalvPoz[i]++;
		else
			GalvPoz[i]--;
		if (K[failas][esamaBusena[i]][esamasSimbolis][2] != esamaBusena[i]) esamaBusena[i] = K[failas][esamaBusena[i]][esamasSimbolis][2]; //pagal instrukc keiciam busena

		if (GalvPoz[i] == -1 || GalvPoz[i] == juosta[i].length()) { // jei iseinama is juostos ribu sustabdomas instrukciju vykdymas
			stop++;
			message[failas] = to_string(i) + ".txt ) Ended on step: " + to_string(iteracija) + "\n" + juosta[i];
		}
	}

	return true;
}
int main()
{
	while (1)
	{
		int GalvPoz[5];
		int stime = 0;
		int sk = 0;
		string juosta[5], name, failas[5];
		map<string, map<string, map<char, string[3]>>> K;

		while (name != "start") //kol neparasoma 'start' neprasideda instrukciju vykdymas
		{
			cout << "Enter names of files or type 'start' to launch the program. To change refresh rate type 'time'. To close the program type 'quit'" << endl;
			cin >> name;
			if (name == "start") break;
			failas[sk] = name;
			if (name == "quit") return 0; // jei paraso quit baigia darba (exit(1))
			else if (name == "time") { cout << "Enter refresh time in ms" << endl; cin >> stime; sk--; } //keicia refresh rate
			else if (!nuskaitymas(failas[sk], GalvPoz, juosta, K, sk)) sk--;
			sk++;
		}

		string esamaBusena[5]; //pradine kyptis/busena
		for (int i = 0; i < 5; i++) esamaBusena[i] = "0";
		int iteracija = 1; //veiksmu kiekis
		int stop = 0;
		while (stop != sk) {
			system("cls");
			for (int i = 0; i < sk; i++) {
				skaiciavimai(esamaBusena, failas[i], GalvPoz, juosta, K, i, iteracija, stop);
				isvedimas(stime, juosta[i], GalvPoz[i], iteracija, failas[i]);

			}
			iteracija++;


			if (_kbhit()) {
				char ch = _getch();
				switch (ch)
				{
				case 49:               // Numpad 1 stop
					if (GalvPoz[0] != -1) {
						GalvPoz[0] = -1;
						stop++;
						message[failas[0]] = failas[0] + ".txt ) Stopped on step: " + to_string(iteracija) + "\n" + juosta[0];
					}
					continue;
				case 50:               // Numpad 2 stop
					if (GalvPoz[1] != -1) {
						GalvPoz[1] = -1;
						stop++;
						message[failas[1]] = failas[1] + ".txt ) Stopped on step: " + to_string(iteracija) + "\n" + juosta[1];
					}
					continue;
				case 51:               // Numpad 3 stop
					if (GalvPoz[2] != -1) {
						GalvPoz[2] = -1;
						stop++;
						message[failas[2]] = failas[2] + ".txt ) Stopped on step: " + to_string(iteracija) + "\n" + juosta[2];
					}
					continue;
				case 52:               // Numpad 4 stop
					if (GalvPoz[3] != -1) {
						GalvPoz[3] = -1;
						stop++;
						message[failas[3]] = failas[3] + ".txt ) Stopped on step: " + to_string(iteracija) + "\n" + juosta[3];
					}
					continue;
				}
			}
		}
		//pasibaigus ciklui viskas isspausdinama is naujo
		system("cls");
		for (int i = 0; i < sk; i++) {
			skaiciavimai(esamaBusena, failas[i], GalvPoz, juosta, K, i, iteracija, stop);
			isvedimas(stime, juosta[i], GalvPoz[i], iteracija, failas[i]);

		}
	}
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
