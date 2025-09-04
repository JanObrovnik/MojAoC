/*
	lahko
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cmath>



enum Smer {
	LEVO = 'L',
	GOR = 'U',
	DESNO = 'R',
	DOL = 'D'
};


struct Tocka {
	short x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(short x, short y) : x(x), y(y) {}

	void premaktni(const Smer& smer) {
		if		(smer == LEVO)  x--;
		else if (smer == GOR)   y--;
		else if (smer == DESNO) x++;
		else if (smer == DOL)   y++;
	}

	void sledi(const Tocka& tocka) {
		if (razdalja(*this, tocka) < 2)
			return;
		if (x > tocka.x) x--;
		if (x < tocka.x) x++;
		if (y > tocka.y) y--;
		if (y < tocka.y) y++;
	}

	friend short razdalja(const Tocka& to1, const Tocka& to2);
};

short razdalja(const Tocka& to1, const Tocka& to2) {
	short x(abs(to1.x - to2.x));
	short y(abs(to1.y - to2.y));
	return (x > y) ? x : y;
}

bool operator==(Tocka to1, Tocka to2) {
	return to1.x == to2.x && to1.y == to2.y;
}
bool operator<(Tocka to1, Tocka to2) {
	if (to1.x != to2.x) return to1.x < to2.x;
	return to1.y < to2.y;
}
bool operator>(Tocka to1, Tocka to2) {
	if (to1.x != to2.x) return to1.x > to2.x;
	return to1.y > to2.y;
}


struct Ukaz {
	Smer smer;
	short krat;

	Ukaz() : smer(DESNO), krat(0) {}
	Ukaz(Smer smer, short krat) : smer(smer), krat(krat) {}
	Ukaz(char smer, short krat) : smer((Smer)smer), krat(krat) {}
};

std::istream& operator>>(std::istream& is, Ukaz& uk) {
	char c;
	short i;
	is >> c >> i;
	uk = Ukaz(c, i);
	return is;
}


void simulirajKratkoVrv(const Ukaz& ukaz, Tocka& glava, Tocka& rep, std::set<Tocka>& seznamTockRepa) {

	for (int i = 0; i < ukaz.krat; i++) {
		glava.premaktni(ukaz.smer);
		rep.sledi(glava);
		seznamTockRepa.insert(rep);
	}
}

void simulirajDolgoVrv(const Ukaz& ukaz, Tocka& glavaDolg, std::vector<Tocka>& repDolg, std::set<Tocka>& seznamTockRepaDolg) {

	for (int i = 0; i < ukaz.krat; i++) {
		glavaDolg.premaktni(ukaz.smer);
		repDolg.front().sledi(glavaDolg);
		for (int i = 1; i < repDolg.size(); i++) {
			repDolg[i].sledi(repDolg[i - 1]);
		}
		seznamTockRepaDolg.insert(repDolg.back());
	}
}


std::pair<int,int> preberiPodatke(std::string pot) {

	int resitev1(0);
	int resitev2(0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);


	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}


	Tocka glava(0, 0);
	Tocka rep(0, 0);
	std::set<Tocka> seznamTockRepa{ Tocka(0,0) };

	Tocka glavaDolg(0, 0);
	std::vector<Tocka> repDolg(9, Tocka(0, 0));
	std::set<Tocka> seznamTockRepaDolg{ Tocka(0,0) };

	Ukaz ukaz;

	while (podatki >> ukaz) {
		simulirajKratkoVrv(ukaz, glava, rep, seznamTockRepa);
		simulirajDolgoVrv(ukaz, glavaDolg, repDolg, seznamTockRepaDolg);
	}
	resitev1 = seznamTockRepa.size();
	resitev2 = seznamTockRepaDolg.size();


	podatki.close();


	return { resitev1,resitev2 };
}



int main(int argc, char* argv[]) {

	std::pair<int, int> resitev(preberiPodatke("Advent22/9.txt"));
	std::cout << "rep na kratki vrvi se je pojavil na " << resitev.first << " razlicnih mestih.\n";
	std::cout << "rep na dolgi vrvi se je pojavil na " << resitev.second << " razlicnih mestih.\n";


	return 0;
}