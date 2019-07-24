// Unicode UTF-8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;
ofstream fr("utf8.txt");
string DecimalToHex(int kodas) {
	string hex;
	int SveikaDalis, Liekana;
	SveikaDalis = kodas;
	Liekana = kodas;
	while (SveikaDalis >= 1)
	{
		Liekana = SveikaDalis % 16;
		SveikaDalis /= 16;
		if (Liekana < 10) hex.insert(0, to_string(Liekana));
		else if (Liekana == 10) hex.insert(0, "A");
		else if (Liekana == 11) hex.insert(0, "B");
		else if (Liekana == 12) hex.insert(0, "C");
		else if (Liekana == 13) hex.insert(0, "D");
		else if (Liekana == 14) hex.insert(0, "E");
		else if (Liekana == 15) hex.insert(0, "F");
	}
	return hex;
}

string HexToBin(string sesioliktainis) {
	string bin;
	for (int i = 0; i < sesioliktainis.length(); i++)
	{
		if (sesioliktainis[i] == '0') bin += "0000";
		else if (sesioliktainis[i] == '1') bin += "0001";
		else if (sesioliktainis[i] == '2') bin += "0010";
		else if (sesioliktainis[i] == '3') bin += "0011";
		else if (sesioliktainis[i] == '4') bin += "0100";
		else if (sesioliktainis[i] == '5') bin += "0101";
		else if (sesioliktainis[i] == '6') bin += "0110";
		else if (sesioliktainis[i] == '7') bin += "0111";
		else if (sesioliktainis[i] == '8') bin += "1000";
		else if (sesioliktainis[i] == '9') bin += "1001";
		else if (sesioliktainis[i] == 'A') bin += "1010";
		else if (sesioliktainis[i] == 'B') bin += "1011";
		else if (sesioliktainis[i] == 'C') bin += "1100";
		else if (sesioliktainis[i] == 'D') bin += "1101";
		else if (sesioliktainis[i] == 'E') bin += "1110";
		else if (sesioliktainis[i] == 'F') bin += "1111";
	}
	return bin;
}

string BinToHex(string dvejetainis) {
	string hex;
	for (int i = 0; i < dvejetainis.length(); i += 4)
	{
		string dal;
		dal = dvejetainis.substr(i, 4);
		if (dal == "0000") hex += "0";
		else if (dal == "0001") hex += "1";
		else if (dal == "0010") hex += "2";
		else if (dal == "0011") hex += "3";
		else if (dal == "0100") hex += "4";
		else if (dal == "0101") hex += "5";
		else if (dal == "0110") hex += "6";
		else if (dal == "0111") hex += "7";
		else if (dal == "1000") hex += "8";
		else if (dal == "1001") hex += "9";
		else if (dal == "1010") hex += "A";
		else if (dal == "1011") hex += "B";
		else if (dal == "1100") hex += "C";
		else if (dal == "1101") hex += "D";
		else if (dal == "1110") hex += "E";
		else if (dal == "1111") hex += "F";
	}
	return hex;
}
string ToUnicode(int nBytes, string code)
{
	string uni;
	while (1)
	{
		if (code.length() > 0)
		{
			uni.insert(0, 1, code.back());
			code = code.substr(0, code.size() - 1);
		}
		else uni.insert(0, "0");

		if (uni.length() == 6 || uni.length() == 14 || uni.length() == 22) uni.insert(0, "10");

		if (uni.length() == 13 && nBytes == 2) {
			uni.insert(0, "110");
			break;
		}

		if (uni.length() == 20 && nBytes == 3) {
			uni.insert(0, "1110");
			break;
		}

		if (uni.length() == 27 && nBytes == 4) {
			uni.insert(0, "11110");
			break;
		}
	}
	return uni;
}
void ReadLibrary(map<int, int> &lib) {
	ifstream ldf("librarynochar.txt");
	string pass; // pirmas failo stulpelis nereikalinga - hex reiksmes ten
	int dec; // antras stulpelis - decimal reiksmes
	int unicode; // trecias stuleplis - unicode reiksmes
	unsigned char ch;
	for (int i = 128; i < 256; i++)
	{
		ldf >> pass >> dec >> unicode;

		lib[dec] = unicode;
	}
}
void HexToChar(string hex) {
	for (int i = 0; i < hex.length(); i += 2) { // hex tdl po 2
		int temp = 0;
		if (hex[i] >= '0' && hex[i] <= '9') {
			temp += (hex[i] - '0') * 16;
		}
		else { temp += (hex[i] - 'A' + 10) * 16; }
		if (hex[i + 1] >= '0' && hex[i + 1] <= '9') {
			temp += (hex[i + 1] - '0');
		}
		else { temp += (hex[i + 1] - 'A' + 10); }
		char nchar = temp;
		fr << nchar;
	}
}
int main()
{
	map<int, int> libr;
	ReadLibrary(libr);
	ifstream df("386intel.txt");
	df >> noskipws; //kad faile skaitytu viska- nepraleistu tarpu
	while (!df.eof())
	{
		unsigned char simb;
		df >> simb;
		int kodas = simb;
		if (kodas < 128) {
			fr << simb;
		}
		else
		{
			kodas = libr[kodas];
			string cValue = DecimalToHex(kodas); // keiciam unicode koda is decimal i hex
			cValue = HexToBin(cValue); // is hex i binary
			string utf;
			if (kodas < 2048)
			{
				utf = BinToHex(ToUnicode(2, cValue));
			}
			else if (kodas < 65536)
			{
				utf = BinToHex(ToUnicode(3, cValue));
			}
			else
			{
				utf = BinToHex(ToUnicode(4, cValue));
			}
			HexToChar(utf);
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
