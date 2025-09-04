/*
	Lahko
	Lahko
*/

#include <iostream>
#include <fstream>
#include <string>


enum KamenSkarnjePapir {
	kamen = 1,
	skarje = 3,
	papir = 2
};

enum IzidIgre {
	poraz = 0,
	remi = 3,
	zmaga = 6
};


IzidIgre ovrednosti(KamenSkarnjePapir& nasprotnik, KamenSkarnjePapir& jaz) {
	if (jaz == kamen) {
		if (nasprotnik == kamen) return remi;
		if (nasprotnik == skarje) return zmaga;
		if (nasprotnik == papir) return poraz;
	}
	if (jaz == skarje) {
		if (nasprotnik == kamen) return poraz;
		if (nasprotnik == skarje) return remi;
		if (nasprotnik == papir) return zmaga;
	}
	if (jaz == papir) {
		if (nasprotnik == kamen) return zmaga;
		if (nasprotnik == skarje) return poraz;
		if (nasprotnik == papir) return remi;
	}
}

KamenSkarnjePapir poisciPotrebniKamenSkarjePapir(KamenSkarnjePapir& nasprotnik, IzidIgre& izid) {
	if (izid == poraz) {
		if (nasprotnik == kamen) return skarje;
		if (nasprotnik == skarje) return papir;
		if (nasprotnik == papir) return kamen;
	}
	if (izid == remi) {
		if (nasprotnik == kamen) return kamen;
		if (nasprotnik == skarje) return skarje;
		if (nasprotnik == papir) return papir;
	}
	if (izid == zmaga) {
		if (nasprotnik == kamen) return papir;
		if (nasprotnik == skarje) return kamen;
		if (nasprotnik == papir) return skarje;
	}
}


int preberiPodatke1(std::string pot) {

	int resitev = 0;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return 0;
	}

	char nasprotnikC, jazC;
	KamenSkarnjePapir nasprotnik, jaz;

	while (podatki.peek() != EOF) {

		podatki >> nasprotnikC >> jazC;

		if (nasprotnikC == 'A') nasprotnik = kamen;
		else if (nasprotnikC == 'B') nasprotnik = papir;
		else if (nasprotnikC == 'C') nasprotnik = skarje;
		
		if (jazC == 'X') jaz = kamen;
		else if (jazC == 'Y') jaz = papir;
		else if (jazC == 'Z') jaz = skarje;

		resitev += jaz;
		resitev += ovrednosti(nasprotnik, jaz);
	}

	podatki.close();


	return resitev;
}


int preberiPodatke2(std::string pot) {

	int resitev = 0;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return 0;
	}

	char nasprotnikC, izidC;
	KamenSkarnjePapir nasprotnik;
	IzidIgre izid;

	while (podatki.peek() != EOF) {

		podatki >> nasprotnikC >> izidC;

		if (nasprotnikC == 'A') nasprotnik = kamen;
		else if (nasprotnikC == 'B') nasprotnik = papir;
		else if (nasprotnikC == 'C') nasprotnik = skarje;

		if (izidC == 'X') izid = poraz;
		else if (izidC == 'Y') izid = remi;
		else if (izidC == 'Z') izid = zmaga;

		resitev += poisciPotrebniKamenSkarjePapir(nasprotnik, izid);
		resitev += izid;
	}

	podatki.close();


	return resitev;
}


int main(int argc, char* argv[]) {

	int resitev1 = preberiPodatke1("Advent22/2.txt");
	std::cout << "Koncni rezultat vseh iger po ugibanju je " << resitev1 << ".\n";

	int resitev2 = preberiPodatke2("Advent22/2.txt");
	std::cout << "Pravi koncni rezultat vseh iger je " << resitev2 << ".\n";


	return 0;
}