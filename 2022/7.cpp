/*
	Tezka
	Zelo lahka
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


struct Datoteka {
	std::string naziv;
	int velikost;

	Datoteka(std::string naziv, int velikost) : naziv(naziv), velikost(velikost) {}
};

std::ostream& operator<<(std::ostream& os, const Datoteka& da) {
	return os << da.naziv << " (dat, velikost=" << da.velikost << ")";
}

class Mapa {
public:
	std::string naziv;
	std::vector<Datoteka> seznamDatotek;
	std::vector<Mapa> seznamMap;
	long long velikost;

	Mapa(std::string naziv) : naziv(naziv), seznamDatotek({}), seznamMap({}), velikost(0) {}

	Mapa* podMapa(std::string pot) {
		auto it = std::find_if(seznamMap.begin(), seznamMap.end(), [&](Mapa& mapa) {return mapa.naziv == pot; });
		return &*it;
	}

	long long vsotaVelikostiDatotek() const {
		return std::accumulate(seznamDatotek.begin(), seznamDatotek.end(), 0LL, [](long long sum, Datoteka da) {return sum + da.velikost; });
	}

	void izracunajVelikosti() {
		velikost = vsotaVelikostiDatotek();
		for (Mapa& mapa : seznamMap) {
			mapa.izracunajVelikosti();
			velikost += mapa.velikost;
		}
	}

	void zapolniSeznamVelikosti(std::vector<std::pair<std::string, long long>>& seznamVelikosti) {
		seznamVelikosti.push_back({ naziv,velikost });
		for (Mapa& mapa : seznamMap) {
			mapa.zapolniSeznamVelikosti(seznamVelikosti);
		}
	}
};

std::ostream& operator<<(std::ostream& os, const Mapa& ma) {
	os << ma.naziv << " (dir) {" << '\n';
	for (const Datoteka& dat : ma.seznamDatotek)
		os << "  " << dat << '\n';
	for (const Mapa& ma2 : ma.seznamMap)
		os << ma2;
	return os << "}\n";
}


Mapa preberiPodatke(std::string pot) {

	Mapa resitev("/");
	std::vector<std::string> seznamPodMap{};


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteki ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	std::getline(podatki, vrstica);

	while (std::getline(podatki, vrstica)) {

		Mapa* trenutnaMapa = &resitev;
		for (const std::string& podMapa : seznamPodMap) {
			trenutnaMapa = trenutnaMapa->podMapa(podMapa);
		}

		if (vrstica[0] == '$') {

			if (std::string(vrstica.begin() + 2, vrstica.begin() + 4) == "cd") {

				if (std::string(vrstica.begin() + 5, vrstica.end()) == "/") {
					seznamPodMap.clear();
				}
				else if (std::string(vrstica.begin() + 5, vrstica.end()) == "..") {
					seznamPodMap.pop_back();
				}
				else {
					seznamPodMap.push_back(std::string(vrstica.begin() + 5, vrstica.end()));
				}

				continue;
			}
			if (std::string(vrstica.begin() + 2, vrstica.begin() + 4) == "ls") {

				continue;
			}
		}


		if (std::string(vrstica.begin() + 0, vrstica.begin() + 3) == "dir") {
			
			trenutnaMapa->seznamMap.push_back(Mapa(std::string(vrstica.begin() + 4, vrstica.end())));

			continue;
		}


		auto it(std::find(vrstica.begin(), vrstica.end(), ' '));

		int velikost(std::stoi(std::string(vrstica.begin(), it)));
		std::string naziv(it + 1, vrstica.end());
		
		trenutnaMapa->seznamDatotek.push_back(Datoteka(naziv, velikost));
	}
	resitev.izracunajVelikosti();

	podatki.close();


	return resitev;
}


long long vsotaManjKot100000(std::vector<std::pair<std::string, long long>>& seznamVelikosti, int max = 100000) {

	return std::accumulate(seznamVelikosti.begin(), seznamVelikosti.end(), 0LL, [&](long long sum, std::pair<std::string, long long> par) {
		if (par.second <= max) return sum + par.second;
		else return sum;
		});
}


int main() {

	Mapa spomin(preberiPodatke("Advent22/7.txt"));
	//std::cout << spomin << "\n\n";

	std::vector<std::pair<std::string, long long>> seznamVelikosti;
	spomin.zapolniSeznamVelikosti(seznamVelikosti);
	//for (const auto& par : seznamVelikosti)
	//	std::cout << par.first << '\t' << par.second << '\n';

	long long resitev1(vsotaManjKot100000(seznamVelikosti));
	std::cout << "Vsota vsega spomin, ki je manjsi od " << resitev1 << ".\n";


	int celotenSpomin(70000000);
	int potrebenProstSpomin(30000000);
	int maxZasedenSpomin(celotenSpomin - potrebenProstSpomin);
	int trenutnoZasedenSpomin(spomin.velikost);
	int minPotrebnoZbrisati(trenutnoZasedenSpomin - maxZasedenSpomin);
	//std::cout << maxZasedenSpomin << '\n' << trenutnoZasedenSpomin << '\n' << minPotrebnoZbrisati << '\n';

	int resitev2(celotenSpomin);
	std::for_each(seznamVelikosti.begin(), seznamVelikosti.end(), [&](std::pair<std::string, long long> par) {
		if (par.second >= minPotrebnoZbrisati && par.second < resitev2)
			resitev2 = par.second;
		});
	std::cout << "Najmanjsa zadostna velikostj z brisanjem je " << resitev2 << ".\n";


	return 0;
}