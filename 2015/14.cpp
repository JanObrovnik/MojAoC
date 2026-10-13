/*
	t = 38:14 min
	zelo lahko
	t = 29::22 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


struct SeverniJelen {
	std::string ime;
	int hitrost; // km/s
	int casZalogEnergije; // s
	int casPocitka; // s

	SeverniJelen() : ime(""), hitrost(-1), casZalogEnergije(-1), casPocitka(-1) {}
	SeverniJelen(std::string ime, int hitrost, int casZalogEnergije, int casPocitka)
		: ime(ime), hitrost(hitrost), casZalogEnergije(casZalogEnergije), casPocitka(casPocitka) {}
};

std::ostream& operator<<(std::ostream& os, const SeverniJelen& sj) {
	return os << sj.ime << ": v = " << sj.hitrost << " km/s, t = " << sj.casZalogEnergije << " s, p = " << sj.casPocitka << " s";
}


std::vector<SeverniJelen> preberiPodatke(const std::string& pot) {

	std::vector<SeverniJelen> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {

		resitev.push_back(SeverniJelen());
		SeverniJelen& sj = resitev.back();
		
		podatki >> sj.ime;
		podatki.ignore(9);
		podatki >> sj.hitrost;
		podatki.ignore(10);
		podatki >> sj.casZalogEnergije;
		podatki.ignore(33);
		podatki >> sj.casPocitka;
		podatki.ignore(10);
	}

	podatki.close();

	return resitev;
}


std::pair<std::vector<std::string>, int> odcitajZmagovalcaPoPoti(const std::vector<SeverniJelen>& seznamSevernihJelenov, const int& casTekmovanja) {

	int resitevPot = 0;
	std::vector<std::string> resitevIme = {};

	std::map<std::string, int> zbirkaPodatkov;

	for (const SeverniJelen& sj : seznamSevernihJelenov) {

		//std::cout << sj << '\n';

		int casSkupni = sj.casZalogEnergije + sj.casPocitka;
		int stCiklov = casTekmovanja / casSkupni;
		int casOstanek = casTekmovanja % casSkupni;

		int opravljenaPot = (sj.hitrost * sj.casZalogEnergije) * stCiklov;

		if (casOstanek > sj.casZalogEnergije)
			opravljenaPot += sj.hitrost * sj.casZalogEnergije;
		else
			opravljenaPot += sj.hitrost * casOstanek;

		zbirkaPodatkov[sj.ime] = opravljenaPot;

		if (opravljenaPot >= resitevPot) {
			resitevPot = opravljenaPot;
		}
	}

	for (const std::pair<std::string, int>& par : zbirkaPodatkov)
		if (par.second == resitevPot)
			resitevIme.push_back(par.first);

	return { resitevIme, resitevPot };
}

std::pair<std::string, int> odcitajZmagovalcaPoTockah(const std::vector<SeverniJelen>& seznamSevernihJelenov, const int& casTekmovanja) {

	int resitevPot = 0;
	std::string resitevIme;

	std::map<std::string, int> lestvica;

	for (int t = 1; t <= casTekmovanja; t++) {
		
		std::pair<std::vector<std::string>, int> vodilni = odcitajZmagovalcaPoPoti(seznamSevernihJelenov, t);
		for (const std::string& ime : vodilni.first)
			lestvica[ime]++;
	}

	auto it = std::max_element(lestvica.begin(), lestvica.end(), [&](std::pair<std::string, int> parL, std::pair<std::string, int> parR) {
		return parL.second < parR.second;
		});

	//for (auto it = lestvica.begin(); it != lestvica.end(); it++) {
	//	std::cout << it->first << ": " << it->second << '\n';
	//}

	return *it;
}


int main() {

	std::vector<SeverniJelen> seznamSevernihJelenov = preberiPodatke("2015/14.txt");


	std::pair<std::vector<std::string>, int> resitev1 = odcitajZmagovalcaPoPoti(seznamSevernihJelenov, 2503);

	std::cout << "Najhitrejsi severni jelen je " << resitev1.first.front() << " z opravljeno potjo " << resitev1.second << " km.\n";


	std::pair<std::string, int> resitev2 = odcitajZmagovalcaPoTockah(seznamSevernihJelenov, 2503);

	std::cout << "Zmagovalec Olimpiskih iger je " << resitev2.first << ", ki je dosegel " << resitev2.second << " tock.\n";


	return 0;
}
