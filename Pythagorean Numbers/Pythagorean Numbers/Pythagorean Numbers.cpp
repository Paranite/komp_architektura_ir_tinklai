// Pythagorean Numbers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <stdio.h>
#include <iostream>

int main() {

	__declspec(align(16))float fmas[1000];
	for (int i = 0; i < 1000; i++)
		fmas[i] = i + 1;
	__declspec(align(16))float fgmas[4];
	int imsize = sizeof(fmas) / sizeof(float);
	float * fptr;
	float * fgptr;
	for (int j = 1; j <= imsize; j++) {
		for (int i = 0; i < imsize; i += 4) {
			for (int g = 0; g < 4; g++)
				fgmas[g] = j;
			fptr = fmas + i;
			fgptr = fgmas;
			__asm {
				mov eax, fgptr
				mov ebx, fptr
				movaps xmm0, [eax]
				movaps xmm1, [ebx]
				mulps xmm0, xmm0
				mulps xmm1, xmm1

				movaps xmm2, xmm0
				addps xmm2, xmm1 //kvadratu suma

				movaps xmm3, xmm2
				sqrtps xmm3, xmm3 //saknis kvadratu sumos

				roundps xmm3, xmm3, 3

				mulps xmm3, xmm3

				subps xmm3, xmm2

				//mov eax, fgptr
				movaps[eax], xmm3
			}
			for (int g = 0; g < 4; g++) {
				if (fgmas[g] == 0)
					std::cout << j << " " << fmas[i + g] << " = " << j * j + fmas[i + g] * fmas[i + g] << std::endl;
			}
		}
	}

	system("pause");

}