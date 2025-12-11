/*
	t ~ 25 min
	zelo lahko
	t ~ 3 ure
	tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>


std::map<std::string, int> nivoji;
std::map<int, std::vector<std::string>> urejeniNivoji;


std::map<std::string, std::vector<std::string>> preberiPodatke(const std::string& pot) {

	std::map<std::string, std::vector<std::string>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	std::string vhod;
	std::string izhod;
	std::vector<std::string> seznamIzhodov;

	while (std::getline(podatki, vrstica)) {

		std::stringstream ss(vrstica);

		ss >> vhod;
		vhod.erase(vhod.end() - 1);

		while (ss >> izhod)
			seznamIzhodov.push_back(izhod);

		resitev[vhod] = seznamIzhodov;

		seznamIzhodov.clear();
	}

	podatki.close();

	return resitev;
}

std::map<std::string, std::vector<std::string>> obrniPovezave(const std::map<std::string, std::vector<std::string>>& povezave) {

	std::map<std::string, std::vector<std::string>> resitev;

	for (auto it = povezave.begin(); it != povezave.end(); it++) {

		for (const std::string& str : it->second) {
			resitev[str].push_back(it->first);
		}
	}

	return resitev;
}


int poisciSteviloPotiOdDo(const std::map<std::string, std::vector<std::string>>& povezave, const std::string& zacetek, const std::string& konec) {

	int resitev = 0;

	for (const std::string& naslednja : povezave.at(zacetek)) {
		if (naslednja == konec)
			resitev++;
		else
			resitev += poisciSteviloPotiOdDo(povezave, naslednja, konec);
	}

	return resitev;
}


void ustvariNivoje(const std::map<std::string, std::vector<std::string>>& povezave, const std::set<std::string>& preglej, const int& trenutniNivo) {
	
	if (preglej.empty())
		return;
	
	std::set<std::string> novPreglej;

	for (const std::string& str : preglej) {

		nivoji[str] = trenutniNivo;

		if (str == "out")
			continue;

		for (const std::string& naslednja : povezave.at(str)) {
			novPreglej.insert(naslednja);
		}
	}

	ustvariNivoje(povezave, novPreglej, trenutniNivo + 1);
}
void ustvariNavadneNivoje(const std::map<std::string, std::vector<std::string>>& povezave, const std::set<std::string>& preglej, const int& trenutniNivo) {

	ustvariNivoje(povezave, preglej, trenutniNivo);

	for (auto it = nivoji.begin(); it != nivoji.end(); it++) {
		urejeniNivoji[it->second].push_back(it->first);
	}
}


std::map<std::string, std::vector<std::string>> skrajsajPovezave(std::map<std::string, std::vector<std::string>> povezave, const std::vector<std::string>& filter) {

	for (const std::string& str : filter) {
	
		int mesto = nivoji[str];

		for (const std::string& str2 : urejeniNivoji[mesto]) {
			
			if (str2 == str)
				continue;

			povezave[str2].clear();
		}
	}

	return povezave;
}


int main() {

	std::map<std::string, std::vector<std::string>> povezave = preberiPodatke("2025/11.txt");
	std::map<std::string, std::vector<std::string>> povezaveObratne = obrniPovezave(povezave);
	//for (auto it = povezave.begin(); it != povezave.end(); it++) {
	//	std::cout << it->first << " | ";
	//	for (std::string str : it->second)
	//		std::cout << str << ' ';
	//	std::cout << '\n';
	//}

	int resitev1 = poisciSteviloPotiOdDo(povezave, "you", "out");
	std::cout << "Obstaja " << resitev1 << " razlicnih poti.\n";


	ustvariNavadneNivoje(povezave, { "svr" }, 0);
	std::map<std::string, std::vector<std::string>> novePovezave = skrajsajPovezave(povezave, { "dac","fft" });

	std::string vmesni0, vmesni1;
	if (nivoji["dac"] < nivoji["fft"]) { vmesni0 = "dac"; vmesni1 = "fft"; }
	else { vmesni0 = "fft"; vmesni1 = "dac"; }

	long long resitev2 = 1;
	resitev2 *= poisciSteviloPotiOdDo(novePovezave, "svr", vmesni0);
	resitev2 *= poisciSteviloPotiOdDo(novePovezave, vmesni0, vmesni1);
	resitev2 *= poisciSteviloPotiOdDo(novePovezave, vmesni1, "out");
	std::cout << "Obstaja " << resitev2 << " razlicnih poti, ki potujejo med: " << vmesni1 << " in " << vmesni1 << ".\n";



	return 0;
}
