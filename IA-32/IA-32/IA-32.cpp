// IA-32.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <vector>


std::vector <int> bin;
int temp;
void toVector() {
	bin.push_back(temp);
}
int main(int argc, char** argv) {
	if (argc < 2) {
		printf("Error! Missing parameters");
		return 0;
	}
	temp = atoi(argv[1]);
	//int *a = new int[(temp / 16 + 1) * 4];
	//int *a = new int [100];
	//int *b;
	//b = a;
	__asm {
		push eax
		push ebx
		push ecx
		push edx
		//----
		mov edx, 0 // kad veiktu dalyba

		mov eax, temp // number
		cmp eax, edx // jei <=0
		jle FINISH // eina i finish

		mov ebx, eax //ebx bus laikoma sveik dal

		START :   // loop
		mov eax, ebx // irasoma is naujo sveik dal
			mov ecx, 2
			div ecx   // eax dal is 2
			mov ecx, eax
			imul eax, 2 // sveik dal * 2
			sub ebx, eax // gauname liekana
			mov[temp], ebx // perkeliama i temp kad perkeltume i vektoriu
						   //mov a, ebx
			mov ebx, ecx // perkeliame i ebx sveikaja dali			mov a, ebx
						 //inc a
			call toVector //iskviecia funkcija perkelti liekana i vektoriu
						  // nusinulint eax, ecx ir edx nes kitaip neisveda pilnai bin sk.
			mov eax, 0
			mov ecx, 0
			mov edx, 0
			cmp ebx, edx // jei daugiau uz 0, tesiamas ciklas
			jg START

			FINISH :
		pop eax
			pop ebx
			pop ecx
			pop edx
	};
	for (int i = bin.size() - 1; i >= 0; i--)
		std::cout << bin[i];
}