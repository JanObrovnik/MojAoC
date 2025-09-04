/*
	Enostavno (zlo lustna naloga)
	Nekoliko tezko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>


enum Usmerjenost
{
	SEVER = '^',
	VZHOD = '>',
	JUG   = 'v',
	ZAHOD = '<',
};

enum Simboli
{
	POT = 'X',
	OVIRA = '#',
	PRAZNOST = '.',
	GORDOL = '|',
	LEVODESNO = '-',
	KOT = '+'
};


class Strazar {
public:
	int x, y;
	Usmerjenost smer;

	Strazar() : x(0), y(0), smer(SEVER) {}
	Strazar(int x, int y, Usmerjenost smer) : x(x), y(y), smer(smer) {}

	void pomik() {
		if (smer == SEVER) y--;
		else if (smer == VZHOD) x++;
		else if (smer == JUG) y++;
		else if (smer == ZAHOD) x--;
	}

	void pomikNazaj() {
		if (smer == SEVER) y++;
		else if (smer == VZHOD) x--;
		else if (smer == JUG) y--;
		else if (smer == ZAHOD) x++;
	}

	Strazar& obrni() {
		if (smer == SEVER) smer = VZHOD;
		else if (smer == VZHOD) smer = JUG;
		else if (smer == JUG) smer = ZAHOD;
		else if (smer == ZAHOD) smer = SEVER;
		return *this;
	}
};

bool obratnoEnakaSmer(Strazar st1, Strazar st2) {
	st2.obrni().obrni();
	return st1.smer == st2.smer;
}

bool operator==(Strazar& st1, Strazar& st2) {
	return st1.x == st2.x && st1.y == st2.y;
}


class Laboratorij {
	Strazar strazar;
	std::vector<std::string> mapa;

	void dodajVrstica(std::string vrstica) {
		mapa.push_back(vrstica);
	}

	void nastaviStrazarja(Strazar strazar) {
		this->strazar = strazar;
	}

	void pocisti() {
		strazar = Strazar();
		mapa.clear();
	}


	bool znotrajLaboratorija(Strazar& strazar) {
		return strazar.x >= 0 && strazar.x < mapa[0].size() &&
			   strazar.y >= 0 && strazar.y < mapa.size();
	}
	

	void pomik(Strazar& strazar, std::vector<std::string>& mapa) {

		while (true) {

			mapa[strazar.y][strazar.x] = POT;

			strazar.pomik();

			if (!znotrajLaboratorija(strazar)) break;

			if (mapa[strazar.y][strazar.x] == OVIRA) {
				strazar.pomikNazaj();
				strazar.obrni();
			}
		}
	}

	bool postaviOviro(Strazar& strazar, std::vector<std::string>& mapa, int steviloKorakov, std::list<Strazar>& seznamNeveljavnihMest) {

		for (int i = 0; i < steviloKorakov; i++) {

			strazar.pomik();
			
			if (!znotrajLaboratorija(strazar)) return false;

			if (mapa[strazar.y][strazar.x] == OVIRA) {
				strazar.pomikNazaj();
				strazar.obrni();
				steviloKorakov++;
			}
		}

		auto it = std::find_if(seznamNeveljavnihMest.begin(), seznamNeveljavnihMest.end(), [&](Strazar ovira) {return ovira == strazar; });

		if (it == seznamNeveljavnihMest.end()) {
			mapa[strazar.y][strazar.x] = OVIRA;
			seznamNeveljavnihMest.push_back(strazar);
		}

		return true;
	}

	bool pomikPoKrogu(Strazar& strazar, std::vector<std::string>& mapa) {

		std::list<Strazar> seznamKotov;

		while (true) {

			strazar.pomik();

			if (!znotrajLaboratorija(strazar))
				break;

			if (mapa[strazar.y][strazar.x] == KOT)
				for (Strazar kot : seznamKotov)
					if (kot == strazar)
						if (!obratnoEnakaSmer(strazar, kot))
							return true;

			if (mapa[strazar.y][strazar.x] == OVIRA) {
				strazar.pomikNazaj();
				strazar.obrni();
				mapa[strazar.y][strazar.x] = KOT;
				seznamKotov.push_back(strazar);
			}
			else if (strazar.smer == SEVER || strazar.smer == JUG)
				mapa[strazar.y][strazar.x] = GORDOL;
			else if (strazar.smer == ZAHOD || strazar.smer == VZHOD)
				mapa[strazar.y][strazar.x] = LEVODESNO;
		}

		return false;
	}


public:
	Laboratorij() : strazar(Strazar()), mapa({}) {}


	int dolzinaPoti() {

		pomik(strazar, mapa);

		int resitev = 0;

		std::for_each(mapa.begin(), mapa.end(), [&](std::string str) {
			resitev += std::count(str.begin(), str.end(), POT);
			});

		return resitev;
	}

	int steviloKrogov() {

		int resitev = 0;

		Strazar ovira(strazar);
		std::vector<std::string> novaMapa(mapa);
		const int steviloKorakov = 1;
		std::list<Strazar> seznamNeveljavnihMest{strazar};

		while (postaviOviro(ovira, novaMapa, steviloKorakov, seznamNeveljavnihMest)) {

			Strazar novStrazar(strazar);

			resitev += pomikPoKrogu(novStrazar, novaMapa);

			novaMapa = mapa;
		}


		return resitev;
	}


	void izpisMape(std::vector<std::string>& mapa) const {
		std::for_each(mapa.begin(), mapa.end(), [](std::string str) {std::cout << str << '\n'; });
	}


	friend void preberiPodatke(std::string pot, Laboratorij& lab);
};


void preberiPodatke(std::string pot, Laboratorij& lab) {


	lab.pocisti();


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return;
	}

	int i = 0;
	std::string vrstica;
	while (podatki >> vrstica) {

		lab.dodajVrstica(vrstica);

		auto it = std::find(vrstica.begin(), vrstica.end(), (char)SEVER);
		if (it != vrstica.end())
			lab.nastaviStrazarja(Strazar(it - vrstica.begin(), i, SEVER));

		i++;
	}

	podatki.close();


	return;
}


int main() {

	Laboratorij laboratorij;


	/// 1. del:
	preberiPodatke("Advent24-6.txt", laboratorij);
	//laboratorij.izpisMape(mapa); std::cout << "\n\n";

	std::cout << "Pot je dolga " << laboratorij.dolzinaPoti() << ".\n";


	/// 2. del:
	preberiPodatke("Advent24-6.txt", laboratorij);
	//laboratorij.izpisMape(mapa); std::cout << "\n\n";

	std::cout << "Stevilo razlicnih krogov je " << laboratorij.steviloKrogov() << ".\n";

	//std::cout << obratnoEnakaSmer(Strazar(0, 0, SEVER), Strazar(0, 0, SEVER)) << '\n';
	//std::cout << obratnoEnakaSmer(Strazar(0, 0, SEVER), Strazar(0, 0, JUG)) << '\n';
	//std::cout << obratnoEnakaSmer(Strazar(0, 0, ZAHOD), Strazar(0, 0, SEVER)) << '\n';
	//std::cout << obratnoEnakaSmer(Strazar(0, 0, ZAHOD), Strazar(0, 0, VZHOD)) << '\n';



	return 0;
}