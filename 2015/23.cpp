/*
	t ~ 46 min
	neverjetno lahko
	t = 1:07 min
	neverjetno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


enum Instruction {
	HALF,
	TRIPLE,
	INCREMENT,
	JUMP,
	JUMP_EVEN,
	JUMP_ONE
};

struct Navodilo {

	Instruction inst;
	char registerIme;
	unsigned int* registerPtr;
	int kolicina;

	Navodilo(Instruction inst) : inst(inst), registerIme('r'), kolicina(0), registerPtr(nullptr) {}
};


inline Instruction preberiNavodilo(const std::string& navodiloStr) {
	if		(navodiloStr == "hlf") return HALF;
	else if (navodiloStr == "tpl") return TRIPLE;
	else if (navodiloStr == "inc") return INCREMENT;
	else if (navodiloStr == "jmp") return JUMP;
	else if (navodiloStr == "jie") return JUMP_EVEN;
	else if (navodiloStr == "jio") return JUMP_ONE;
}
inline std::string preberiStr(const Instruction& navodiloStr) {
	if		(navodiloStr == HALF)		return"hlf";
	else if (navodiloStr == TRIPLE)		return"tpl";
	else if (navodiloStr == INCREMENT)	return"inc";
	else if (navodiloStr == JUMP)		return"jmp";
	else if (navodiloStr == JUMP_EVEN)	return"jie";
	else if (navodiloStr == JUMP_ONE)	return"jio";
}

std::vector<Navodilo> preberiPodatke(const std::string& pot) {

	std::vector<Navodilo> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string navodiloStr;

	while (podatki.peek() != EOF) {
		
		podatki >> navodiloStr;
		resitev.push_back(Navodilo(preberiNavodilo(navodiloStr)));

		Navodilo& zadnjeNavodilo = resitev.back();

		if (zadnjeNavodilo.inst == HALF || zadnjeNavodilo.inst == TRIPLE || zadnjeNavodilo.inst == INCREMENT) {
			podatki >> zadnjeNavodilo.registerIme;
		}
		else if (zadnjeNavodilo.inst == JUMP) {
			podatki >> zadnjeNavodilo.kolicina;
		}
		else if (zadnjeNavodilo.inst == JUMP_EVEN || zadnjeNavodilo.inst == JUMP_ONE) {
			podatki >> zadnjeNavodilo.registerIme;
			podatki.ignore(1);
			podatki >> zadnjeNavodilo.kolicina;
		}
	}

	podatki.close();

	return resitev;
}


unsigned int simuliraj(std::vector<Navodilo> seznamNavodil, const unsigned int& zacetniA = 0, const unsigned int& zacetniB = 0) {

	unsigned int a = zacetniA;
	unsigned int b = zacetniB;

	for (Navodilo& navodilo : seznamNavodil) {
		if (navodilo.registerIme == 'a')
			navodilo.registerPtr = &a;
		else if (navodilo.registerIme == 'b')
			navodilo.registerPtr = &b;
	}

	int i = 0;

	while (i >= 0 && i < seznamNavodil.size()) {

		if (seznamNavodil[i].inst == HALF) {
			(*seznamNavodil[i].registerPtr) /= 2;
			i++;
		}
		else if (seznamNavodil[i].inst == TRIPLE) {
			(*seznamNavodil[i].registerPtr) *= 3;
			i++;
		}
		else if (seznamNavodil[i].inst == INCREMENT) {
			(*seznamNavodil[i].registerPtr)++;
			i++;
		}
		else if (seznamNavodil[i].inst == JUMP) {
			i += seznamNavodil[i].kolicina;
		}
		else if (seznamNavodil[i].inst == JUMP_EVEN) {
			if ((*seznamNavodil[i].registerPtr) % 2 == 0) i += seznamNavodil[i].kolicina;
			else i++;
		}
		else if (seznamNavodil[i].inst == JUMP_ONE) {
			if ((*seznamNavodil[i].registerPtr) == 1) i += seznamNavodil[i].kolicina;
			else i++;
		}
	}

	return b;
}


int main() {

	std::vector<Navodilo> seznamNavodil = preberiPodatke("2015/23.txt");


	unsigned int resitev1 = simuliraj(seznamNavodil);
	std::cout << "vrednost registra \"b\" je " << resitev1 << ".\n";

	unsigned int resitev2 = simuliraj(seznamNavodil, 1);
	std::cout << "vrednost registra \"b\" je " << resitev2 << ".\n";


	return 0;
}
