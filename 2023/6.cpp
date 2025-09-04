/*
	Zelo lahka
	Zelo lahka
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>
#include <numeric>


class Dirka {
private:
	long long cas, pot;

public:
	Dirka(long long cas, long long pot) : cas(cas), pot(pot) {}

	long long steviloZmag() const {
		long long resitev = 0;

		for (long long drzanje = 0; drzanje <= cas/2; drzanje++) {
			long long tekmovanje = cas - drzanje;

			if (drzanje * tekmovanje > pot) {
				resitev = tekmovanje - drzanje + 1;
				break;
			}
		}

		return resitev;
	}
};


std::list<Dirka> preberiPodatke(std::string pot) {

	std::list<Dirka> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";
		return resitev;
	}

	std::string casStr, potStr;

	std::getline(podatki, casStr);
	std::getline(podatki, potStr);

	std::stringstream casSS(casStr);
	std::stringstream potSS(potStr);

	casSS.ignore(10);
	potSS.ignore(10);

	int casInt, potInt;

	while (casSS.peek() != EOF) {
		casSS >> casInt;
		potSS >> potInt;

		resitev.push_back(Dirka(casInt, potInt));
	}

	podatki.close();


	return resitev;
}

Dirka preberiPodatke2(std::string pot) {

	Dirka resitev(0, 0);


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti\n";
		return resitev;
	}

	std::string casStr, potStr;

	std::getline(podatki, casStr);
	std::getline(podatki, potStr);

	casStr.erase(casStr.begin(), casStr.begin() + 10);
	potStr.erase(potStr.begin(), potStr.begin() + 10);

	auto casIt = std::remove(casStr.begin(), casStr.end(), ' ');
	auto potIt = std::remove(potStr.begin(), potStr.end(), ' ');

	casStr.erase(casIt, casStr.end());
	potStr.erase(potIt, potStr.end());

	resitev = Dirka(std::stoll(casStr), std::stoll(potStr));

	podatki.close();


	return resitev;
}


int main() {

	std::list<Dirka> seznamDirk(preberiPodatke("Advent23/6.txt"));
	
	std::list<int> seznamZmag;
	std::for_each(seznamDirk.begin(), seznamDirk.end(), [&seznamZmag](Dirka dirka) {seznamZmag.push_back(dirka.steviloZmag()); });

	int mozneZmage = std::accumulate(seznamZmag.begin(), seznamZmag.end(), 1, [](int i, int j) {return i * j; });
	std::cout << "Imamo " << mozneZmage << " moznih zmag.\n";
	std::cout << std::endl;


	Dirka dirka(preberiPodatke2("Advent23/6.txt"));

	long long resitev2 = dirka.steviloZmag();

	std::cout << "Imamo " << resitev2 << " moznih zmag za zdruzeno tekmo.\n";



	return 0;
}