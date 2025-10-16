/*
	t ~ 8 h
	nekoliko tezko
	t = 1:44 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <algorithm>


enum Koordinate {
	x = 0,
	y = 1,
	z = 2,
	a = 3
};

template <size_t vel>
class Tocka {
public:
	
	std::array<short, vel> seznamKoordinat;

	Tocka(std::vector<short> seznam) {
		for (int i = 0; i < vel; i++)
			seznamKoordinat[i] = seznam[i];
	}
	Tocka() {
		for (int i = 0; i < vel; i++)
			seznamKoordinat[i] = 0;
	}

	

	size_t razseznost() const {
		return vel;
	}


	Tocka& operator+=(const Tocka& to) {
		for (short i = 0; i < vel; i++) {
			seznamKoordinat[i] += to.seznamKoordinat[i];
		}
		return *this;
	}
	Tocka& operator+=(const short& num) {
		for (short i = 0; i < vel; i++) {
			seznamKoordinat[i] += num;
		}
		return *this;
	}

	Tocka& operator++(int) {
		for (int i = 0; i < vel; i++) {
			seznamKoordinat[i]++;
			if (seznamKoordinat[i] > 1) {
				seznamKoordinat[i] = -1;
				continue;
			}
			break;
		}
		return *this;
	}

	short& operator[](const size_t& it) {
		return seznamKoordinat[it];
	}
	short operator[](const size_t& it) const {
		return seznamKoordinat[it];
	}
	short& operator[](const Koordinate& it) {
		return seznamKoordinat[it];
	}
	short operator[](const Koordinate& it) const {
		return seznamKoordinat[it];
	}

	bool operator==(const short& num) const {
		for (int i = 0; i < vel; i++)
			if (seznamKoordinat[i] != num)
				return false;
		return true;
	}
	bool operator==(const Tocka& to) const {
		for (int i = 0; i < vel; i++)
			if (seznamKoordinat[i] != to.seznamKoordinat[i])
				return false;
		return true;
	}
	bool operator<(const Tocka& to) const {
		for (int i = 0; i < vel; i++)
			if (seznamKoordinat[i] != to.seznamKoordinat[i])
				return seznamKoordinat[i] < to.seznamKoordinat[i];
		return false;
	}
	bool operator>(const Tocka& to) const {
		for (int i = 0; i < vel; i++)
			if (seznamKoordinat[i] != to.seznamKoordinat[i])
				return seznamKoordinat[i] > to.seznamKoordinat[i];
		return false;
	}
};
template <size_t vel>
std::ostream& operator<<(std::ostream& os, const Tocka<vel>& to) {
	for (const short& i : to.seznamKoordinat)
		os << i << '\t';
	return os;
}

template <std::size_t vel>
struct std::hash<Tocka<vel>> {
	auto operator()(const Tocka<vel>& key) const {
		std::hash<short> hasher;
		std::size_t result = 0;
		for (std::size_t i = 0; i < vel; ++i)
			result = result * 31 + hasher(key[i]);
		return result;
	}
};


std::vector<Tocka<2>> PreberiPodatke(const std::string& pot) {

	std::vector<Tocka<2>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	short y = 0;
	while (podatki >> vrstica) {
		short x = 0;
		for (const char& c : vrstica) {
			if (c == '#')
				resitev.push_back(Tocka<2>({ x,y }));
			x++;
		}
		y++;
	}

	podatki.close();

	return resitev;
}


template<size_t nov, size_t star>
std::vector<Tocka<nov>> razsiri(const std::vector<Tocka<star>>& seznamTock) {

	auto min = std::min(nov, star);

	std::vector<Tocka<nov>> resitev;
	Tocka<nov> novaTocka;
	
	for (const Tocka<star>& staraTocka : seznamTock) {
		for (int i = 0; i < min; i++)
			novaTocka[i] = staraTocka[i];
		resitev.push_back(novaTocka);
	}

	return resitev;
}


template<size_t vel>
std::vector<Tocka<vel>> zapisSosed() {

	std::vector<Tocka<vel>> resitev;

	Tocka<vel> soseda;
	soseda += -1;

	for (int i = 0; i < pow(3, vel); i++) {
		if (soseda == 0) {
			soseda++;
			continue;
		}
		resitev.push_back(soseda);
		soseda++;
	}

	return resitev;
}

template<size_t vel>
long long steviloAktivnih(std::vector<Tocka<vel>> seznamTock, short koraki) {

	std::unordered_map<Tocka<vel>, short> poljeGostote;

	std::unordered_set<Tocka<vel>> aktivneTocke;

	std::vector<Tocka<vel>> seznamSosed = zapisSosed<vel>();

	for (short i = 0; i < koraki; i++) {

		poljeGostote.clear();
		aktivneTocke.clear();

		for (const Tocka<vel>& tocka : seznamTock)
			aktivneTocke.insert(tocka);
		seznamTock.clear();

		for (const Tocka<vel>& tocka : aktivneTocke) {

			for (Tocka<vel> soseda : seznamSosed) {

				soseda += tocka;

				poljeGostote[soseda] += 1;
			}
		}

		for (const auto& it : poljeGostote) {

			if (it.second == 3) {
				seznamTock.push_back(it.first);
				continue;
			}
			if (it.second == 2 && aktivneTocke.find(it.first) != aktivneTocke.end()) {
				seznamTock.push_back(it.first);
				continue;
			}
		}
	}

	return seznamTock.size();
}


int main() {

	std::vector<Tocka<2>> podatki = PreberiPodatke("2020/17.txt");
	//for (auto to : podatki)
	//	std::cout << to << '\n';

	std::vector<Tocka<3>> podatki3D = razsiri<3,2>(podatki);
	long long resitev1 = steviloAktivnih(podatki3D, 6);
	std::cout << "Po 6 korakih je stevilo 3D aktivnih kock " << resitev1 << ".\n";
	
	std::vector<Tocka<4>> podatki4D = razsiri<4, 2>(podatki);
	long long resitev2 = steviloAktivnih(podatki4D, 6);
	std::cout << "Po 6 korakih je stevilo 4D aktivnih kock " << resitev2 << ".\n";


	return 0;
}
