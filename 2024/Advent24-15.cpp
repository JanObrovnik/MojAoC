/*
	Nekoliko lahka
	malo tezja od prvega dela
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <ciso646>


enum Smer {
	SEVER = '^',
	VZHOD = '>',
	JUG   = 'v',
	ZAHOD = '<',
	PRAZNI = ' '
};


class Predmet {
public:
	int x, y;

	Predmet() : x(0), y(0) {}
	Predmet(int x, int y) : x(x), y(y) {}

	void pomik(Smer smer) {
		if		(smer == SEVER) y--;
		else if (smer == VZHOD) x++;
		else if (smer == JUG)	y++;
		else if (smer == ZAHOD) x--;
	}

	void pomikNazaj(Smer smer) {
		if		(smer == SEVER) y++;
		else if (smer == VZHOD) x--;
		else if (smer == JUG)	y--;
		else if (smer == ZAHOD) x++;
	}
};

bool operator==(const Predmet& pr1, const Predmet& pr2) {
	return pr1.x == pr2.x && pr1.y == pr2.y;
}

bool operator<(const Predmet& pr1, const Predmet& pr2) {
	if (pr1.x != pr2.x) return pr1.x < pr2.x;
	return pr1.y < pr2.y;
}

bool operator>(const Predmet& pr1, const Predmet& pr2) {
	if (pr1.x != pr2.x) return pr1.x > pr2.x;
	return pr1.y > pr2.y;
}


class Skladisce {
	Predmet robot;
	std::list<Predmet> seznamZabojnikov;
	std::list<Predmet> seznamZidov;
	int visina, sirina;


	bool premikZabojnika(std::list<Predmet>::iterator it, Smer smer) {

		Predmet novZabojnik = *it;
		novZabojnik.pomik(smer);


		if (std::find(seznamZidov.begin(), seznamZidov.end(), novZabojnik) != seznamZidov.end())
			return false;

		auto it2 = std::find(seznamZabojnikov.begin(), seznamZabojnikov.end(), novZabojnik);
		if (it2 != seznamZabojnikov.end())
			if (!premikZabojnika(it2, smer)) return false;
		

		it->pomik(smer);

		return true;
	}


public:
	Skladisce() : robot(), seznamZabojnikov({}), seznamZidov({}), visina(0), sirina(0) {}


	void nastaviRobota(Predmet robot) {
		this->robot = robot;
	}

	void dodajZabojnik(Predmet zabojnik) {
		seznamZabojnikov.push_back(zabojnik);
	}

	void dodajZid(Predmet zid) {
		seznamZidov.push_back(zid);
	}

	void nastaviVisinoSirino(int visina, int sirina) {
		this->visina = visina;
		this->sirina = sirina;
	}


	void simuliraj(Smer smer) {
		
		robot.pomik(smer);

		if (std::find(seznamZidov.begin(), seznamZidov.end(), robot) != seznamZidov.end()) {
			robot.pomikNazaj(smer);
			return;
		}

		auto it = std::find(seznamZabojnikov.begin(), seznamZabojnikov.end(), robot);
		if (it != seznamZabojnikov.end())
			if (!premikZabojnika(it, smer)) {
				robot.pomikNazaj(smer);
				return;
			}
	}


	int vrednostGPS() const {

		int resitev = 0;

		for (Predmet zabojnik : seznamZabojnikov) {
			resitev += zabojnik.x + zabojnik.y * 100;
		}

		return resitev;
	}


	void prikazi() const {

		std::vector<std::string> mapa(visina,std::string(sirina, '.'));

		mapa[robot.y][robot.x] = '@';
		for (Predmet zabojnik : seznamZabojnikov) mapa[zabojnik.y][zabojnik.x] = 'O';
		for (Predmet zid : seznamZidov) mapa[zid.y][zid.x] = '#';

		for (std::string vrstica : mapa) std::cout << vrstica << '\n';
	}
};


int preberiPodatke(std::string pot) {

	Skladisce skladisce;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteko ni bilo mogoce odpreti.\n";
		return -1;
	}

	std::string vrstica;

	int y = 0;
	int x = 0;
	while (std::getline(podatki, vrstica)) {
		if (vrstica.empty()) break;

		std::stringstream ss(vrstica);
		char c;

		x = 0;
		while (ss >> c) {
			if		(c == '#') skladisce.dodajZid(Predmet(x, y));
			else if (c == 'O') skladisce.dodajZabojnik(Predmet(x, y));
			else if (c == '@') skladisce.nastaviRobota(Predmet(x, y));
			x++;
		}
		y++;
	}
	skladisce.nastaviVisinoSirino(y, x);

	std::cout << "Zacetek:\n"; skladisce.prikazi(); std::cout << '\n';
	char c;
	while (podatki >> c) {
		skladisce.simuliraj(Smer(c));
		//std::cout << "Pomik " << c << ":\n"; skladisce.prikazi(); std::cout << '\n';
	}
	std::cout << "Konec:\n"; skladisce.prikazi(); std::cout << '\n';

	podatki.close();




	return skladisce.vrednostGPS();
}

/// 2 del

class Tocka {
public:
	int x, y;

	Tocka() : x(0), y(0) {}
	Tocka(int x, int y) : x(x), y(y) {}

	void pomik(Smer smer) {
		if		(smer == SEVER) y--;
		else if (smer == VZHOD) x++;
		else if (smer == JUG)	y++;
		else if (smer == ZAHOD) x--;
	}
};

bool operator==(const Tocka& pr1, const Tocka& pr2) {
	return pr1.x == pr2.x && pr1.y == pr2.y;
}

bool operator<(const Tocka& pr1, const Tocka& pr2) {
	if (pr1.x != pr2.x) return pr1.x < pr2.x;
	return pr1.y < pr2.y;
}

bool operator>(const Tocka& pr1, const Tocka& pr2) {
	if (pr1.x != pr2.x) return pr1.x > pr2.x;
	return pr1.y > pr2.y;
}

Tocka operator+(Tocka to1, Tocka to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}


class Skatla {
public:
	Tocka leva;
	Tocka desna;

	Skatla() : leva(), desna(Tocka(1,0)) {}
	Skatla(Tocka tocka) : leva(tocka), desna(tocka + Tocka(1,0)) {}

	void pomik(Smer smer) {
		leva.pomik(smer);
		desna.pomik(smer);
	}
};


class Prostor {
	Tocka robot;
	std::vector<Skatla> seznamSkatl;
	std::vector<Tocka> seznamZidov;
	int visina, sirina;


	bool moznostPremikaSkatl(Smer smer, std::vector<Skatla>::iterator it) {

		Skatla skatuh(*it);
		skatuh.pomik(smer);

		if (std::any_of(seznamZidov.begin(), seznamZidov.end(), [&](Tocka& tocka) {
			return skatuh.leva == tocka or skatuh.desna == tocka; }))
			return false;

		

		if (smer == ZAHOD) {
			auto it2 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.leva or skatla.desna == skatuh.leva;
				});
			if (it2 != seznamSkatl.end())
				return moznostPremikaSkatl(smer, it2);
		}
		else if (smer == VZHOD) {
			auto it3 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.desna or skatla.desna == skatuh.desna;
				});
			if (it3 != seznamSkatl.end())
				return moznostPremikaSkatl(smer, it3);
		}
		else {
			auto it2 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.leva or skatla.desna == skatuh.leva;
				});
			if (it2 != seznamSkatl.end())
				if (!moznostPremikaSkatl(smer, it2))
					return false;

			auto it3 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.desna or skatla.desna == skatuh.desna;
				});
			if (it3 != seznamSkatl.end())
				if (!moznostPremikaSkatl(smer, it3))
					return false;
		}

		return true;
	}

	void premikSkatl(Smer smer, std::vector<Skatla>::iterator it) {

		Skatla skatuh(*it);
		skatuh.pomik(smer);


		if (smer == ZAHOD) {
			auto it2 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.leva or skatla.desna == skatuh.leva;
				});
			if (it2 != seznamSkatl.end())
				premikSkatl(smer, it2);
		}
		else if (smer == VZHOD) {
			auto it3 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.desna or skatla.desna == skatuh.desna;
				});
			if (it3 != seznamSkatl.end())
				premikSkatl(smer, it3);
		}
		else {
			auto it2 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.leva or skatla.desna == skatuh.leva;
				});
			if (it2 != seznamSkatl.end())
				premikSkatl(smer, it2);

			auto it3 = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
				return skatla.leva == skatuh.desna or skatla.desna == skatuh.desna;
				});
			if (it3 != seznamSkatl.end())
				premikSkatl(smer, it3);
		}

		it->pomik(smer);
	}


public:
	Prostor() : robot(), seznamSkatl({}), seznamZidov({}), visina(0), sirina(0) {}

	
	void nastaviRobota(Tocka robot) {
		this->robot = robot;
	}
	void dodajSkatlo(Tocka tocka) {
		seznamSkatl.push_back(Skatla(tocka));
	}
	void dodajZid(Tocka tocka) {
		seznamZidov.push_back(tocka);
	}
	void nastaviVisinoSirino(int visina, int sirina) {
		this->visina = visina;
		this->sirina = sirina;
	}


	void simuliraj(Smer smer) {

		Tocka roboDuh(robot);
		roboDuh.pomik(smer);

		if (std::any_of(seznamZidov.begin(), seznamZidov.end(), [&](Tocka& tocka) {return roboDuh == tocka; }))
			return;

		auto it = std::find_if(seznamSkatl.begin(), seznamSkatl.end(), [&](Skatla& skatla) {
			return skatla.leva == roboDuh or skatla.desna == roboDuh;
			});
		if (it != seznamSkatl.end())
			if (!moznostPremikaSkatl(smer, it))
				return;

		robot.pomik(smer);
		
		if (it == seznamSkatl.end())
			return;

		premikSkatl(smer, it);
	}

	int dobiGPS() const {

		int resitev = 0;

		for (Skatla skatla : seznamSkatl)
			resitev += skatla.leva.x + 100 * skatla.leva.y;

		return resitev;
	}


	void prikazi() const {

		std::vector<std::string> mapa(visina, std::string(sirina, '.'));

		mapa[robot.y][robot.x] = '@';
		for (Skatla skatla : seznamSkatl) {
			mapa[skatla.leva.y][skatla.leva.x] = '[';
			mapa[skatla.desna.y][skatla.desna.x] = ']';
		}
		for (Tocka zid : seznamZidov)
			mapa[zid.y][zid.x] = '#';

		for (std::string vrstica : mapa)
			std::cout << vrstica << '\n';
	}
};


int preberiPodatkeDel2(std::string pot) {

	Prostor prostor;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni bila najdena.\n";
		return -1;
	}

	int y = 0;
	int x = 0;

	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		if (vrstica.empty()) break;

		std::stringstream ss(vrstica);

		x = 0;

		char c;
		while (ss >> c) {

			if (c == '#') {
				prostor.dodajZid(Tocka(2*x, y));
				prostor.dodajZid(Tocka(2*x + 1, y));
			}
			else if (c == 'O') {
				prostor.dodajSkatlo(Tocka(2*x, y));
			}
			else if (c == '@')
				prostor.nastaviRobota(Tocka(2*x, y));

			x++;
		}
		y++;
	}
	prostor.nastaviVisinoSirino(y, 2*x);


	prostor.prikazi();


	char c;
	while (podatki >> c)
		prostor.simuliraj(Smer(c));


	prostor.prikazi();



	return prostor.dobiGPS();
}


int main() {

	std::cout << "Skupna vrednost GPS vrednosti je " << preberiPodatke("Advent24-15.txt") << ".\n";
	std::cout << "Skupna siroka vrednost GPS vrednosti je " << preberiPodatkeDel2("Advent24-15.txt") << ".\n";

	return 0;
}
