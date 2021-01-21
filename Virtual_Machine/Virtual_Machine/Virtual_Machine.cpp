// Virtual_Machine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream df("q1_encr.txt"); // encrypted failas

struct buffer {
	unsigned char komanda;
	unsigned char skaicius;
};

void decrypRead(vector<buffer> &buf) {
	ifstream input("decryptor.bin", std::ios::binary); //nuskaitymas decryptor failo || ios::binary atidaro faila binary mode
	vector<unsigned char>  A(std::istreambuf_iterator<char>(input), {});
	for (int i = 0; i < A.size(); i += 2) {
		buffer B;
		B.komanda = A[i];
		B.skaicius = A[i + 1];
		buf.push_back(B);
	}
}

bool fzeroFlag(int Rx, unsigned char regs[]) { if (regs[Rx] == 0) return true; else return false; }

void INC(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F; //paimami 4 pirmi bitai, kiti nutrinami ir gaunamas skaicius
								//    1110 0101
								// &  0000 1111
								//    0000 1111
	regs[Rx]++; //registra padidinam vienetu
	zeroFlag = fzeroFlag(Rx, regs);
}

void DEC(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F; //paimami 4 pirmi bitai, kiti nutrinami ir gaunamas skaicius
	regs[Rx]--; //registra sumazinam vienetu
	zeroFlag = fzeroFlag(Rx, regs);
}

void MOV(vector<buffer> buf, int it, unsigned char regs[]) {
	int Rx = buf[it].skaicius & 0x0F;
	int Ry = (buf[it].skaicius & 0xF0) >> 4;
	//    1110 0101
	// &  1111 0000
	//    1110 0000
	regs[Rx] = regs[Ry];
}

void MOVC(vector<buffer> buf, int it, unsigned char regs[]) {
	regs[0] = buf[it].skaicius; //i nulini registra kopijuojama konstanta
}

void LSL(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F; //paimami 4 pirmi bitai, kiti nutrinami ir gaunamas skaicius
	regs[Rx] = regs[Rx] << 1; //perstumiami bitai i kaire
	zeroFlag = fzeroFlag(Rx, regs);
}

void LSR(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F; //paimami 4 pirmi bitai, kiti nutrinami ir gaunamas skaicius
	regs[Rx] = regs[Rx] >> 1;  // perstumiami bitai i desine
	zeroFlag = fzeroFlag(Rx, regs);
}
void JMP(vector<buffer> buf, int &it, unsigned char regs[]) {
	signed char a = buf[it].skaicius; //baito konstanta nusako kiek persokti nuo esamos pozicijos
	it = it + (a / 2) - 1;
}

void JZ(vector<buffer> buf, int &it, unsigned char regs[], bool &zeroFlag) {
	if (zeroFlag) JMP(buf, it, regs);
}

void JNZ(vector<buffer> buf, int &it, unsigned char regs[], bool &zeroFlag) {
	if (!zeroFlag) JMP(buf, it, regs);
}

void JFE(vector<buffer> buf, int &it, unsigned char regs[], bool isEOF) {
	if (isEOF) JMP(buf, it, regs); //persokti jei failo pabaiga
}

void RET() {
	exit(0); //end prog
}

void ADD(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F;
	int Ry = (buf[it].skaicius & 0xF0) >> 4; //paimami nuo 7 iki 4 bito ir pastumiami per 4 i desine tam kad butu galima gauti skaiciu
	regs[Rx] = regs[Rx] + regs[Ry];
	zeroFlag = fzeroFlag(Rx, regs);
}

void SUB(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F;
	int Ry = (buf[it].skaicius & 0xF0) >> 4; //paimami nuo 7 iki 4 bito ir pastumiami per 4 i desine tam kad butu galima gauti skaiciu
	regs[Rx] = regs[Rx] - regs[Ry];
	zeroFlag = fzeroFlag(Rx, regs);
}

void XOR(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F;
	int Ry = (buf[it].skaicius & 0xF0) >> 4; //paimami nuo 7 iki 4 bito ir pastumiami per 4 i desine tam kad butu galima gauti skaiciu
	regs[Rx] = regs[Rx] ^ regs[Ry];
	zeroFlag = fzeroFlag(Rx, regs);
}

void OR(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag) {
	int Rx = buf[it].skaicius & 0x0F;
	int Ry = (buf[it].skaicius & 0xF0) >> 4; //paimami nuo 7 iki 4 bito ir pastumiami per 4 i desine tam kad butu galima gauti skaiciu
	regs[Rx] = regs[Rx] | regs[Ry];
	zeroFlag = fzeroFlag(Rx, regs);
}

void IN(vector<buffer> buf, int it, unsigned char regs[], bool &zeroFlag, bool &isEOF) {
	int Rx = buf[it].skaicius & 0x0F;
	char read;
	df >> read;  //nuskaitomas simbolis
	regs[Rx] = read;  //perkeliamas i atitinkama registra
	if (df.eof()) isEOF = true;  //jei failo pabaiga
	zeroFlag = fzeroFlag(Rx, regs);
}

void OUT(vector<buffer> buf, int it, unsigned char regs[]) {
	int Rx = buf[it].skaicius & 0x0F;
	cout << regs[Rx];
}


void useCommand(vector<buffer> buf, int &it, unsigned char regs[], bool &zeroFlag, bool &isEOF) {
	if (buf[it].komanda == 0x01)			INC(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x02)	DEC(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x03)	MOV(buf, it, regs);
	else if (buf[it].komanda == 0x04)	MOVC(buf, it, regs);
	else if (buf[it].komanda == 0x05)	LSL(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x06)	LSR(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x07)	JMP(buf, it, regs);
	else if (buf[it].komanda == 0x08)	JZ(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x09)	JNZ(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x0A)	JFE(buf, it, regs, isEOF);
	else if (buf[it].komanda == 0x0B)	RET();
	else if (buf[it].komanda == 0x0C)	ADD(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x0D)	SUB(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x0E)	XOR(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x0F)	OR(buf, it, regs, zeroFlag);
	else if (buf[it].komanda == 0x10)	IN(buf, it, regs, zeroFlag, isEOF);
	else if (buf[it].komanda == 0x11)	OUT(buf, it, regs);
}

int main()
{
	unsigned char regs[16];
	vector<buffer> buf; // nuskaito faila ir viska ikelia i buffer'i
	decrypRead(buf);
	int it = 0; // buffer pozicija
	bool isEOF = false; // end of file flag
	bool zeroFlag = false;

	while (1) {
		useCommand(buf, it, regs, zeroFlag, isEOF);
		it += 1;
	}
	return 0;
}

/*
It would appear that we have reached the limits of what
it is possible to achieve with computer technology,
although one should be careful with such statements,
as they tend to sound pretty silly in 5 years.
(John Von Neumann, circa 1949)
*/