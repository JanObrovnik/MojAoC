/*
	Srednje tezka
	Zelo zahtevna
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


enum Kompas {
	NIC = -1,
	SEVER,
	S = SEVER,
	JUG,
	J = JUG,
	VZHOD,
	V = VZHOD,
	ZAHOD,
	Z = ZAHOD,
};

Kompas obrni(Kompas smer) {
	if (smer == S) return J;
	if (smer == J) return S;
	if (smer == V) return Z;
	if (smer == Z) return V;
}

enum CevDef {
	START      =  'S',
	PRAZNO     =  '.',
	NAVPICNO   =  '|',
	VODORAVNO  =  '-',
	JUG_VZH    =  'F',
	JUG_ZAH    =  '7',
	SEV_VZH    =  'L',
	SEV_ZAH    =  'J'
};


class Tocka {
public:
	short x, y;

	Tocka() : x(0), y(0) {}
	Tocka(short x, short y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, Tocka tocka) {
	return os << '(' << tocka.x << ", " << tocka.y << ')';
}

bool operator<(Tocka tocka, short i) {
	return tocka.x < i || tocka.y < i;
}

bool operator>(Tocka tocka, short i) {
	return tocka.x > i || tocka.y > i;
}


struct Cev {
	Tocka xy;
	CevDef naziv = START;
	Kompas smer = NIC;

	Cev() = default;
	Cev(Tocka xy, CevDef naziv, Kompas smer) : xy(xy), naziv(naziv), smer(smer) {}
};

Tocka najdiNaslednjoTocko(Tocka tocka, Kompas smer) {
	Tocka resitev(tocka);

	if (smer == S) resitev.y--;
	else if (smer == J) resitev.y++;
	else if (smer == V) resitev.x++;
	else if (smer == Z) resitev.x--;

	return resitev;
}

Kompas drugaOdprtina(char naziv, Kompas smer) {
	if		(naziv == NIC)			 return smer;
	else if (naziv == START)	 return smer;
	else if (naziv == NAVPICNO)  return (smer == S) ? J : S;
	else if (naziv == VODORAVNO) return (smer == V) ? Z : V;
	else if (naziv == JUG_VZH)   return (smer == J) ? V : J;
	else if (naziv == JUG_ZAH)   return (smer == J) ? Z : J;
	else if (naziv == SEV_VZH)   return (smer == S) ? V : S;
	else if (naziv == SEV_ZAH)   return (smer == S) ? Z : S;

	return smer;
}

bool preveriOdprtino(char naziv, Kompas smer) {
	if		(naziv == PRAZNO)	 return false;
	else if (naziv == START)	 return false;
	else if (naziv == NAVPICNO)  return (smer == S || smer == J) ? true : false;
	else if (naziv == VODORAVNO) return (smer == V || smer == Z) ? true : false;
	else if (naziv == JUG_VZH)	 return (smer == J || smer == V) ? true : false;
	else if (naziv == JUG_ZAH)	 return (smer == J || smer == Z) ? true : false;
	else if (naziv == SEV_VZH)	 return (smer == S || smer == V) ? true : false;
	else if (naziv == SEV_ZAH)	 return (smer == S || smer == Z) ? true : false;

	return false;	
}

class Sistem {
private:
	std::vector<std::string> baza;
	std::vector<std::string> obkoljenaBaza;
	Cev cev;
	int dolzinaCevi;

public:
	Sistem(std::string pot) { preberiPodatke(pot); najdiS(); definirajS(); ustvariObkoljenoBazo(); }


	void najdiS() {
		for (short y = 0; y < baza.size(); y++)
			for (short x = 0; x < baza[y].size(); x++)
				if (baza[y][x] == 'S')
					cev.xy = Tocka(x, y);
	}

	void definirajS() {

		for (Kompas smer : { VZHOD, SEVER, ZAHOD, JUG }) {

			Tocka naslednjaTocka = najdiNaslednjoTocko(cev.xy, smer);
			short& x = naslednjaTocka.x;
			short& y = naslednjaTocka.y;

			if (y < 0 || x < 0 || y >= baza.size() || x >= baza[0].size()) continue;

			if (preveriOdprtino(baza[y][x], smer)) {
				cev.smer = smer;
				return;
			}
		}
	}


	int potuj(bool zarisi = false) {

		do {
			if (zarisi) obkoljenaBaza[cev.xy.y][cev.xy.x] = cev.naziv;

			dolzinaCevi++;

			Tocka naslednjaTocka = najdiNaslednjoTocko(cev.xy, cev.smer);
			short& x = naslednjaTocka.x;
			short& y = naslednjaTocka.y;

			cev = Cev(naslednjaTocka, (CevDef)baza[y][x], drugaOdprtina(baza[y][x], obrni(cev.smer)));
		}
		while (cev.naziv != START);

		ustvariObkoljenoBazo();


		return dolzinaCevi/2;
	}



	void ustvariObkoljenoBazo() {
		static bool ze = false;

		if (!ze) {
			obkoljenaBaza.assign(baza.size(), std::string(baza[0].size(), PRAZNO));
			ze = true;
			return;
		}

		std::vector<Kompas> sez;
		for (Kompas kom : {SEVER, JUG, VZHOD, ZAHOD}) {

			Tocka xy = najdiNaslednjoTocko(cev.xy, kom);
			short& x = xy.x;
			short& y = xy.y;

			if (y < 0 || x < 0 || y >= baza.size() || x >= baza[0].size()) continue;

			if (preveriOdprtino(obkoljenaBaza[xy.y][xy.x], obrni(kom)))
				sez.push_back(kom);
		}

		char& naziv = obkoljenaBaza[cev.xy.y][cev.xy.x];

		if (sez.size() == 2) {
			if (sez[0] == S) {
				if (sez[1] == J) naziv = '|';
				else if (sez[1] == V) naziv = 'L';
				else if (sez[1] == Z) naziv = 'J';
			}
			else if (sez[0] == J) {
				if (sez[1] == V) naziv = 'F';
				else if (sez[1] == Z) naziv = '7';
			}
			else if (sez[0] == V) naziv = '-';
		}
	}
	
	std::vector<int> zapisiMeje(Tocka tocka) {

		std::vector<int> resitev(4, 0);

		for (int x = 0; x < tocka.x; x++) if (obkoljenaBaza[tocka.y][x] == 'X') resitev[ZAHOD]++;
		for (int x = tocka.x + 1; x < obkoljenaBaza[tocka.y].size(); x++) if (obkoljenaBaza[tocka.y][x] == 'X') resitev[VZHOD]++;
		for (int y = 0; y < tocka.y; y++) if (obkoljenaBaza[y][tocka.x] == 'X') resitev[SEVER]++;
		for (int y = tocka.y + 1; y < obkoljenaBaza.size(); y++) if (obkoljenaBaza[y][tocka.x] == 'X') resitev[JUG]++;

		return resitev;
	}

	void oznaciObkoljenost() {

		for (int y = 0; y < obkoljenaBaza.size(); y++) {
			bool cezMejo = false;
			Kompas smer = VZHOD;
			
			bool pomoznaMeja = false;
			Kompas pomoznaSmer = NIC;

			for (int x = 0; x < obkoljenaBaza[y].size(); x++) {
				char& naziv = obkoljenaBaza[y][x];
				
				if (naziv != PRAZNO) {

					if (!pomoznaMeja) {

						if (preveriOdprtino(naziv, smer)) {

							pomoznaSmer = drugaOdprtina(naziv, smer);
							pomoznaMeja = true;
						}
						else {

							cezMejo = cezMejo ? false : true;
						}
					}
					else {

						if (!preveriOdprtino(naziv, smer)) {

							if (pomoznaSmer == drugaOdprtina(naziv, obrni(smer))) {

								cezMejo = cezMejo;
							}
							else {

								cezMejo = cezMejo ? false : true;
							}

							pomoznaMeja = false;
						}
					}
				}
				else {

					naziv = cezMejo ? '1' : '0';
				}
			}
		}
	}

	int steviloNotranjihMest() {

		int resitev = 0;

		for (std::string str : obkoljenaBaza) for (char c : str) if (c == '1') resitev++;

		return resitev;
	}


	void izpisiPodatke() {
		std::for_each(baza.begin(), baza.end(), [](std::string str) {std::cout << str << '\n'; });
	}

	void izpisiObkoljenoBazo() {
		std::for_each(obkoljenaBaza.begin(), obkoljenaBaza.end(), [](std::string str) {std::cout << str << '\n'; });
	}

	void preberiPodatke(std::string pot) {

		std::fstream podatki;
		podatki.open(pot, std::ios::in);

		if (podatki.is_open()) {

			while (podatki.peek() != EOF) {

				std::string vrstica;
				std::getline(podatki, vrstica);

				baza.push_back(vrstica);
			}

			podatki.close();
		}
		else std::cout << "Datoteka ni najdena.\n";
	}
};


int main() {

	Sistem sistemCevi("PodatkiAdvent23-10.txt");
	//sistemCevi.izpisiPodatke();
	//std::cout << '\n';

	std::cout << "Najbolj oddaljena cev je " << sistemCevi.potuj() << " korakov stran.\n" << std::endl;


	sistemCevi.ustvariObkoljenoBazo();
	//sistemCevi.izpisiObkoljenoBazo();
	//std::cout << '\n';
	
	sistemCevi.potuj(true);
	//sistemCevi.izpisiObkoljenoBazo();
	//std::cout << '\n';

	sistemCevi.oznaciObkoljenost();
	sistemCevi.izpisiObkoljenoBazo();
	std::cout << '\n';

	std::cout << "Stevilo notranjih cevi je " << sistemCevi.steviloNotranjihMest() << '\n' << std::endl;



	return 0;
}
