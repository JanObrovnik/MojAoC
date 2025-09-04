/*
	precej lahko
	res tezko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>


struct Razmernik {
	int stOd;
	int stDo;
	int razmik;

	Razmernik(int stOd, int stDo, int razmik) : stOd(stOd), stDo(stDo), razmik(razmik) {}
};

class SeznamRazmernikov {
	std::vector<Razmernik> seznam;

public:

};


int steviloStevk(unsigned long long& stevilo);
unsigned long long levaPolovica(unsigned long long& stevilo);
unsigned long long desnaPolovica(unsigned long long& stevilo);


std::list<unsigned long long> preberiPodatke(std::string pot);

void pomizikni(std::list<unsigned long long>& kamni);

std::pair<std::list<int>, int> pretvorba(std::list<unsigned long long>& enojniKamen);
void optimalnoPomizikni(std::list<std::pair<unsigned long long, unsigned long long>>& seznam);
unsigned long long sestej(std::list<std::pair<unsigned long long, unsigned long long>>& seznam);




int main() {

	std::list<unsigned long long> kamni(preberiPodatke("Advent24-11.txt"));
	std::list<std::pair<unsigned long long, unsigned long long>> seznam;
	for (unsigned long long i : kamni) seznam.push_back({ i,1 });
	//std::cout << kamni.size() << "| "; for (auto i : kamni) std::cout << i << ' '; std::cout << '\n';


	int stPonovitev = 25;
	for (int i = 0; i < stPonovitev; i++) {
		pomizikni(kamni);
		//std::cout << kamni.size() << "| "; /**/for (auto i : kamni) std::cout << i << ' ';/**/ std::cout << '\n';
	}
	
	std::cout << "Stevilo kamnov je " << kamni.size() << ".\n";



	stPonovitev = 75;
	for (int i = 0; i < stPonovitev; i++) {
		optimalnoPomizikni(seznam);
		//std::cout << seznam.size() << "| "; /**for (auto i : seznam) std::cout << i.first << ' ';/**/ std::cout << '\n';
		std::cout << i << '\n';
	}

	std::cout << "Stevilo kamnov je " << sestej(seznam) << ".\n";





	return 0;
}



int steviloStevk(unsigned long long& stevilo) {
	return std::to_string(stevilo).size();
}

unsigned long long levaPolovica(unsigned long long& stevilo) {
	std::string str = std::to_string(stevilo);
	str = std::string(str.begin(), str.begin() + str.size() / 2);
	return std::stoull(str);
}

unsigned long long desnaPolovica(unsigned long long& stevilo) {
	std::string str = std::to_string(stevilo);
	str = std::string(str.begin() + str.size() / 2, str.end());
	return std::stoull(str);
}


std::list<unsigned long long> preberiPodatke(std::string pot) {

	std::list<unsigned long long> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena.\n";
		return resitev;
	}

	unsigned long long i;
	while (podatki >> i)
		resitev.push_back(i);

	podatki.close();


	return resitev;
}


void pomizikni(std::list<unsigned long long>& kamni) {

	for (auto it = kamni.begin(); it != kamni.end(); it++) {

		if (*it == 0) {
			*it = 1;
			continue;
		}

		if (steviloStevk(*it) % 2 == 0) {
			kamni.insert(it, levaPolovica(*it));
			kamni.insert(it, desnaPolovica(*it));
			it = --kamni.erase(it);
			if (it == kamni.end()) break;
			continue;
		}

		*it *= 2024;
	}
}

std::pair<std::list<int>, int> pretvorba(std::list<unsigned long long>& enojniKamen) {
	
	std::list<int> seznamResitev;
	int resitev = 0;


	while (!enojniKamen.empty()) {

		resitev += enojniKamen.size();
		
		pomizikni(enojniKamen);
		

		auto konec = std::remove_if(enojniKamen.begin(), enojniKamen.end(), [](auto i) {return steviloStevk(i) == 1; });
		for (auto it = konec; it != enojniKamen.end(); it++)
			seznamResitev.push_back(*it);
		enojniKamen.erase(konec, enojniKamen.end());


		if (resitev > 20) break;
	}

	return std::pair<std::list<int>, int>(seznamResitev, resitev);
}


void optimalnoPomizikni(std::list<std::pair<unsigned long long, unsigned long long>>& seznam) {

	std::list<std::pair<unsigned long long, unsigned long long>> novSeznam;
	novSeznam.push_back(seznam.front());
	
	for (auto it = ++seznam.begin(); it != seznam.end(); it++) {
		
		auto novIt = std::find(novSeznam.begin(), novSeznam.end(), *it);
		
		if (novIt != novSeznam.end())
			novIt->second += it->second;
		else
			novSeznam.push_back(*it);
	}

	seznam = novSeznam;


	for (auto it = seznam.begin(); it != seznam.end(); it++) {

		if (it->first == 0) {
			it->first = 1;
			continue;
		}

		if (steviloStevk(it->first) % 2 == 0) {
			seznam.insert(it, { levaPolovica(it->first),it->second });
			seznam.insert(it, { desnaPolovica(it->first),it->second });
			it = --seznam.erase(it);
			if (it == seznam.end()) break;
			continue;
		}

		it->first *= 2024;
	}
}

unsigned long long sestej(std::list<std::pair<unsigned long long, unsigned long long>>& seznam) {
	
	unsigned long long resitev = 0;

	for (auto& par : seznam)
		resitev += par.second;

	return resitev;
}
