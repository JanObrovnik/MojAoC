/*
	Nekoliko lahko
	Res tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>


enum Smer {
	SEVER = '^',
	VZHOD = '>',
	JUG = 'v',
	ZAHOD = '<'
};

Smer obratnaSmer(Smer smer) {
	if (smer == SEVER) return JUG;
	if (smer == VZHOD) return ZAHOD;
	if (smer == JUG)   return SEVER;
	if (smer == ZAHOD) return VZHOD;
}


class Tocka {
public:
	int x, y;
	Smer smer;

	Tocka() : x(0), y(0), smer(VZHOD) {}
	Tocka(int x, int y) : x(x), y(y), smer(VZHOD) {}
	Tocka(int x, int y, Smer smer) : x(x), y(y), smer(smer) {}
};
const std::list<Tocka> sosede{ Tocka(0,-1,SEVER),Tocka(1,0,VZHOD),Tocka(0,1,JUG),Tocka(-1,0,ZAHOD) };

bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}

bool operator!=(const Tocka& to1, const Tocka& to2) {
	return !(to1 == to2);
}

bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.x != to2.x) return to1.x < to2.x;
	return to1.y < to2.y;
}

std::ostream& operator<<(std::ostream& os, Tocka& to) {
	return os << '(' << (char)to.smer << '|' << to.x << ',' << to.y << ')';
}

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y, to2.smer);
}


class Vozlisce {
public:
	Tocka lokacija;
	int cena;
	Tocka predhodik;
	bool zeObravnavana;

	Vozlisce(Tocka lokacija) : lokacija(lokacija), cena(1000000), predhodik(), zeObravnavana(false) {}
};

std::ostream& operator<<(std::ostream& os, Vozlisce& vo) {
	return os << vo.cena << vo.predhodik << ' ' << vo.zeObravnavana << ' ' << vo.lokacija;
}

bool operator<(const Vozlisce& vo1, const Vozlisce& vo2) {
	if (vo1.zeObravnavana != vo2.zeObravnavana) return vo1.zeObravnavana < vo2.zeObravnavana;
	return vo1.cena < vo2.cena;
}

bool operator==(const Vozlisce& vo, const Tocka& to) {
	return vo.lokacija == to;
}

bool operator!=(const Vozlisce& vo, const Tocka& to) {
	return vo.lokacija != to;
}


class VozliscePlusPlus {
public:
	Tocka lokacija;
	int cena;
	std::list<Tocka> predhodiki;
	bool zeObravnavana;

	VozliscePlusPlus(Tocka lokacija) : lokacija(lokacija), cena(1000000), predhodiki({Tocka()}), zeObravnavana(false) {}
};

std::ostream& operator<<(std::ostream& os, VozliscePlusPlus& vo) {
	return os << vo.cena << vo.predhodiki.front() << ' ' << vo.zeObravnavana << ' ' << vo.lokacija;
}

bool operator<(const VozliscePlusPlus& vo1, const VozliscePlusPlus& vo2) {
	if (vo1.zeObravnavana != vo2.zeObravnavana) return vo1.zeObravnavana < vo2.zeObravnavana;
	return vo1.cena < vo2.cena;
}

bool operator==(const VozliscePlusPlus& vo, const Tocka& to) {
	return vo.lokacija == to;
}

bool operator!=(const VozliscePlusPlus& vo, const Tocka& to) {
	return vo.lokacija != to;
}



class Labirint {
	Tocka zacetek, konec;
	std::list<Tocka> seznamTockPoti;
	int visina, sirina;
	std::list<Vozlisce> najkrajsaPot;
	std::set<Tocka> setTock;


public:
	Labirint() : zacetek(), konec(), seznamTockPoti({}), visina(0), sirina(0), najkrajsaPot({}) {}


	void nastaviZacetek(Tocka zacetek) {
		this->zacetek = zacetek;
	}
	void nastaviKonec(Tocka konec) {
		this->konec = konec;
	}
	void dodajPot(Tocka pot) {
		seznamTockPoti.push_back(pot);
	}
	void nastaviVisinoSirino(int visina, int sirina) {
		this->visina = visina;
		this->sirina = sirina;
	}


	int dijkstraAlgoritem() {

		std::list<Vozlisce> seznamVozlisc(seznamTockPoti.begin(), seznamTockPoti.end());
		seznamVozlisc.push_front(zacetek);
		seznamVozlisc.front().cena = 0;
		seznamVozlisc.push_back(konec);
		seznamVozlisc.back().cena++;


		auto it = std::min_element(seznamVozlisc.begin(), seznamVozlisc.end());

		while (*it != konec) { //////////// mogoc bo to treba spremenit
			//std::cout << *it << '\n';

			for (Tocka soseda : sosede) {

				Tocka novaTocka = it->lokacija + soseda;

				auto it2 = std::find_if(seznamVozlisc.begin(), seznamVozlisc.end(), [&](Vozlisce vozlisce) {
					return vozlisce.lokacija == novaTocka;
					});

				if (it2 == seznamVozlisc.end()) continue;


				int novaCena;
				novaCena = it->cena;
				novaCena++;
				if (novaTocka.smer == it->lokacija.smer);
				else if (novaTocka.smer == obratnaSmer(it->lokacija.smer)) novaCena += 2000;
				else novaCena += 1000;

				if (novaCena < it2->cena) {
					it2->predhodik = it->lokacija;
					it2->cena = novaCena;
					it2->lokacija.smer = soseda.smer;
				}
			}

			it->zeObravnavana = true;
			it = std::min_element(seznamVozlisc.begin(), seznamVozlisc.end());
		}


		najkrajsaPot.clear();
		Vozlisce voz = seznamVozlisc.back();
		
		while (voz.lokacija != seznamVozlisc.front().lokacija) {
		
			//std::cout << voz << '\n';
			najkrajsaPot.push_front(voz);
			voz = *std::find_if(seznamVozlisc.begin(), seznamVozlisc.end(), [&](Vozlisce voz2) {return voz2.lokacija == voz.predhodik; });
		}
		najkrajsaPot.push_front(seznamVozlisc.front());
		std::cout << "1 del." << najkrajsaPot.size() << '\n';
		//std::cout << seznamVozlisc.front() << '\n';


		//std::cout << seznamVozlisc.back() << '\n';
		return seznamVozlisc.back().cena;
	}


	int dijkstraAlgoritemPlusPlus() {

		std::list<VozliscePlusPlus> seznamVozlisc(seznamTockPoti.begin(), seznamTockPoti.end());
		seznamVozlisc.push_front(zacetek);
		seznamVozlisc.front().cena = 0;
		seznamVozlisc.push_back(konec);
		seznamVozlisc.back().cena++;


		auto it = std::min_element(seznamVozlisc.begin(), seznamVozlisc.end());

		while (*it != konec) { //////////// mogoc bo to treba spremenit
			std::cout << *it << '\n';

			for (Tocka soseda : sosede) {

				Tocka novaTocka = it->lokacija + soseda;

				auto it2 = std::find_if(seznamVozlisc.begin(), seznamVozlisc.end(), [&](VozliscePlusPlus vozlisce) {
					return vozlisce.lokacija == novaTocka;
					});

				if (it2 == seznamVozlisc.end()) continue;


				int novaCena;
				novaCena = it->cena;
				novaCena++;
				if (novaTocka.smer == it->lokacija.smer);
				else if (novaTocka.smer == obratnaSmer(it->lokacija.smer)) continue;
				else novaCena += 1000;

				//if (novaTocka == Tocka(3, 11)) std::cout << "dela " << it2->lokacija << novaCena << '\n';

				//std::cout << novaCena << ' ' << it2->cena << '\n';
				if (novaCena < it2->cena) {
					it2->predhodiki.clear();
					it2->predhodiki.push_back(it->lokacija);
					it2->cena = novaCena;
					it2->lokacija.smer = soseda.smer;
				}
				else if (novaCena == it2->cena && novaTocka.smer == obratnaSmer(it2->lokacija.smer)) {
					it2->predhodiki.push_back(it->lokacija);
					//std::cout << it2->predhodiki.size() << ' ' << it->lokacija << " dela\n\n";
				}
				else if (novaCena - it2->cena == 1000 && novaTocka.smer != it2->lokacija.smer) { /////////// napaka nekje tle
					it2->predhodiki.push_back(it->lokacija);
					//std::cout << "dela\n";
				}
			}

			it->zeObravnavana = true;
			it = std::min_element(seznamVozlisc.begin(), seznamVozlisc.end());
		}


		std::queue<Tocka> cakalnaVrsta;
		setTock.clear();
		
		for (Tocka to : seznamVozlisc.back().predhodiki)
			cakalnaVrsta.push(to);
		
		while (!cakalnaVrsta.empty()) {
		
			if (setTock.find(cakalnaVrsta.front()) != setTock.end() || cakalnaVrsta.front() == zacetek) {
			//if (cakalnaVrsta.front() == zacetek) {
				cakalnaVrsta.pop();
				continue;
			}
		
			auto it = std::find_if(seznamVozlisc.begin(), seznamVozlisc.end(), [&](VozliscePlusPlus voz2) {return voz2.lokacija == cakalnaVrsta.front(); });
		
			for (Tocka to : it->predhodiki)
				cakalnaVrsta.push(to);
			
			//std::cout << cakalnaVrsta.front() << '\n';
			setTock.insert(cakalnaVrsta.front());
			std::cout << setTock.size() << '\n';
		
			cakalnaVrsta.pop();
		}
		setTock.insert(zacetek);
		setTock.insert(konec);


		//std::cout << "size: " << setTock.size() << '\n';
		//for (Tocka to : setTock)
		//	std::cout << to << '\n';

		for (auto& to : seznamVozlisc) {
			std::cout << to.predhodiki.size() << ' ' << to.lokacija << '\n';
		}

		prikazi();


		return setTock.size();
	}


	void drugePoti(Vozlisce voz) {

	}


	void prikazi() const {

		std::vector<std::string> mapa(visina, std::string(sirina, '#'));

		for (Tocka pot : seznamTockPoti)
			mapa[pot.y][pot.x] = '.';
		for (Tocka pot : setTock)
			mapa[pot.y][pot.x] = 'O';
		mapa[zacetek.y][zacetek.x] = 'S';
		mapa[konec.y][konec.x] = 'E';

		for (std::string vrstica : mapa)
			std::cout << vrstica << '\n';
	}
};


Labirint preberiPodatke(std::string pot) {
	
	Labirint resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce najti.\n";
		return resitev;
	}

	int x = 0;
	int y = 0;

	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		std::stringstream ss(vrstica);

		char c;

		x = 0;
		while (ss >> c) {

			if (c == '.') resitev.dodajPot(Tocka(x, y));
			else if (c == 'S') resitev.nastaviZacetek(Tocka(x, y));
			else if (c == 'E') resitev.nastaviKonec(Tocka(x, y));

			x++;
		}
		y++;
	}
	resitev.nastaviVisinoSirino(y, x);

	podatki.close();


	return resitev;
}


int main() {

	Labirint labirint(preberiPodatke("Advent24-16.txt"));
	//labirint.prikazi();
	

	int resitev = 0;
	resitev = labirint.dijkstraAlgoritem();

	std::cout << "Najkrajsa vrednost poti je " << resitev << ".\n";


	resitev = labirint.dijkstraAlgoritemPlusPlus();

	std::cout << "Najkrajsa vrednost poti je " << resitev << ".\n";



	return 0;
}