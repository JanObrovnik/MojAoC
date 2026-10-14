/*
	t = 47:51 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>


void preberiPodatke(const std::string& pot, std::map<std::string, std::vector<std::string>>& zamenjave, std::string& molekula) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	std::string vrstica;

	std::string vhod, izhod;

	std::getline(podatki, vrstica);

	while (vrstica != "") {

		std::stringstream ss(vrstica);

		ss >> vhod;
		ss.ignore(4);
		ss >> izhod;

		zamenjave[vhod].push_back(izhod);

		std::getline(podatki, vrstica);
	}

	podatki >> molekula;

	podatki.close();

	return;
}


int steviloMoznihZamenjav(const std::map<std::string, std::vector<std::string>>& zamenjave, const std::string& molekula) {

	std::set<std::string> vseMolekule;

	for (auto it = zamenjave.begin(); it != zamenjave.end(); it++) {
		for (const std::string zamenjava : it->second) {
			
			std::string iskanje;
			for (const char& c : it->first)
				iskanje.push_back('x');

			for (int i = 0; i < molekula.size(); i++) {
				
				iskanje.erase(iskanje.begin());
				iskanje.push_back(molekula[i]);

				if (iskanje == it->first) {

					int mesto = i - (it->first.size() - 1);

					std::string novaMolekula = molekula;

					novaMolekula.erase(novaMolekula.begin() + mesto, novaMolekula.begin() + mesto + it->first.size());
					novaMolekula.insert(novaMolekula.begin() + mesto, zamenjava.begin(), zamenjava.end());

					vseMolekule.insert(novaMolekula);
				}
			}
		}
	}

	return vseMolekule.size();
}


int main() {

	std::map<std::string, std::vector<std::string>> zamenjave;
	std::string molekula;

	preberiPodatke("2015/19.txt", zamenjave, molekula);

	//for (auto par : zamenjave) {
	//	std::cout << par.first << ": ";
	//	for (auto str : par.second)
	//		std::cout << str << ", ";
	//	std::cout << '\n';
	//} std::cout << '\n' << molekula << '\n';


	int resitev1 = steviloMoznihZamenjav(zamenjave, molekula);
	std::cout << "Lahko dobimo " << resitev1 << " razlicnih molekul.\n";



	return 0;
}
