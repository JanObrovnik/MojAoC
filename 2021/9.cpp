/*
	t = 27:16 min
	zelo lahko
	t = 1:23:02 h
	srednje tezka ()
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <set>
#include <algorithm>
#include <numeric>


class Tocka {
public:
	int x, y;

	Tocka() : x(-1), y(-1) {}
	Tocka(int x, int y) : x(x), y(y) {}

	bool izvenMape(const std::vector<std::string>& mapa) {
		if (x < 0 || y < 0 || y >= mapa.size() || x >= mapa.front().size())
			return true;
		else
			return false;
	}
};

bool operator==(const Tocka& to1, const Tocka& to2) {
	return to1.x == to2.x && to1.y == to2.y;
}
bool operator<(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y < to2.y;
	return to1.x < to2.x;
}
bool operator>(const Tocka& to1, const Tocka& to2) {
	if (to1.y != to2.y) return to1.y > to2.y;
	return to1.x > to2.x;
}

Tocka operator+(const Tocka& to1, const Tocka& to2) {
	return Tocka(to1.x + to2.x, to1.y + to2.y);
}
Tocka& operator+=(Tocka& to1, const Tocka& to2) {
	to1 = to1 + to2;
	return to1;
}

std::vector<Tocka> seznamNizin;


std::vector<std::string> preberiPodatke(const std::string pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki >> vrstica)
		resitev.push_back(vrstica);

	podatki.close();

	return resitev;
}


bool preveriSosede(const std::vector<std::string>& mapa, const int& y, const int& x) {

	int xNov = x;
	int yNov = y;
	yNov--;

	if (yNov >= 0 && yNov < mapa.size())
		if (mapa[yNov][xNov] <= mapa[y][x])
			return false;

	xNov = x;
	yNov = y;
	xNov--;

	if (xNov >= 0 && xNov < mapa.front().size())
		if (mapa[yNov][xNov] <= mapa[y][x])
			return false;

	xNov = x;
	yNov = y;
	yNov++;

	if (yNov >= 0 && yNov < mapa.size())
		if (mapa[yNov][xNov] <= mapa[y][x])
			return false;

	xNov = x;
	yNov = y;
	xNov++;

	if (xNov >= 0 && xNov < mapa.front().size())
		if (mapa[yNov][xNov] <= mapa[y][x])
			return false;

	//std::cout << x << ',' << y << '\n';
	seznamNizin.push_back(Tocka(x, y));
	return true;
}

int najdiNizine(const std::vector<std::string>& mapa) {

	int resitev = 0;

	for (int y = 0; y < mapa.size(); y++)
		for (int x = 0; x < mapa.front().size(); x++)
			resitev += preveriSosede(mapa, y, x) * (mapa[y][x] - '0' + 1);

	return resitev;
}


int velikostKotanje(const Tocka& tocka, const std::vector<std::string>& mapa) {

	std::set<Tocka> seznamTockKotanje;

	const std::array<const Tocka, 4> seznamSosed{ Tocka(0,-1),Tocka(1,0), Tocka(0,1), Tocka(-1,0) };

	std::queue<Tocka> cakalnaVrsta;
	cakalnaVrsta.push(tocka);

	while (!cakalnaVrsta.empty()) {

		bool neveljavnaSoseda = false;
		
		Tocka& trenutna = cakalnaVrsta.front();

		if (mapa[trenutna.y][trenutna.x] == '9') {
			cakalnaVrsta.pop();
			continue;
		}

		for (Tocka soseda : seznamSosed) {

			soseda += cakalnaVrsta.front();

			if (soseda.izvenMape(mapa))
				continue;

			if (mapa[soseda.y][soseda.x] < mapa[trenutna.y][trenutna.x] &&	seznamTockKotanje.find(soseda) == seznamTockKotanje.end()) {
				neveljavnaSoseda = true;
				break;
				//////////////// ne deluje za primer: "01233321", ampak takega primera ni
				//////////////// pomoznosti brisanje na isti visini
			}
		}

		if (neveljavnaSoseda == false) {

			seznamTockKotanje.insert(trenutna);

			for (Tocka soseda : seznamSosed) {

				soseda += cakalnaVrsta.front();
				Tocka& trenutna = cakalnaVrsta.front();

				if (soseda.izvenMape(mapa))
					continue;

				if (mapa[soseda.y][soseda.x] >= mapa[trenutna.y][trenutna.x] && seznamTockKotanje.find(soseda) == seznamTockKotanje.end())
					cakalnaVrsta.push(soseda);
			}
		}

		cakalnaVrsta.pop();
	}

	return seznamTockKotanje.size();
}

int pregledKotanji(const std::vector<std::string>& mapa) {

	std::array<int, 3> seznamNajvecjihKotanji{ 0,0,0 };

	for (const Tocka& nizina : seznamNizin) {
		
		int novaVelikost(velikostKotanje(nizina, mapa));

		auto it = std::min_element(seznamNajvecjihKotanji.begin(), seznamNajvecjihKotanji.end(), [](int i1, int i2) {return i1 < i2;});
		
		if (novaVelikost > *it)
			*it = novaVelikost;
	}

	return std::accumulate(seznamNajvecjihKotanji.begin(), seznamNajvecjihKotanji.end(), 1, [](int i1, int i2) {return i1 * i2;});
}


int main() {

	std::vector<std::string> mapa(preberiPodatke("2021/9.txt"));


	int resitev1(najdiNizine(mapa));

	std::cout << "Vsota nevarnosti je " << resitev1 << ".\n";
	
	
	int resitev2(pregledKotanji(mapa));
	
	std::cout << "Produkt najvecjih treh kotanji je " << resitev2 << ".\n";	// 1600104


	return 0;
}
