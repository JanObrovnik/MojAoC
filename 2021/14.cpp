/*
	t = 41:43 min
	relativno lahko
	t = 30:00 min (priblizno)
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>


void preberiPodatke(const std::string& pot, std::string& resitev1, std::map<std::pair<char, char>, char>& resitev2) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return;
	}

	podatki >> resitev1;

	char c1, c2, c3;

	while (podatki.peek() != EOF) {

		podatki >> c1 >> c2;
		podatki.ignore(4);
		podatki >> c3;

		resitev2[{c1, c2}] = c3;
	}

	podatki.close();

	return;


}


int simuliraj1(std::string element, const std::map<std::pair<char, char>, char>& recept, const int& stIteracij) {

	int resitev = 0;

	for (int i = 0; i < stIteracij; i++) {
		for (int i = 0; i < element.size() - 1; i++) {

			char c = recept.at({ element[i], element[i + 1] });

			i++;

			element.insert(element.begin() + i, c);
		}
		//std::cout << element << '\n';
	}


	std::set<char> seznamRazlicnihElementov;
	for (auto it = recept.begin(); it != recept.end(); it++)
		seznamRazlicnihElementov.insert(it->second);

	std::vector<int> seznamPonavljanja;
	for (const char& c : seznamRazlicnihElementov)
		seznamPonavljanja.push_back(std::count(element.begin(), element.end(), c));

	auto itPar = std::minmax_element(seznamPonavljanja.begin(), seznamPonavljanja.end());


	return *itPar.second - *itPar.first;
}

long long simuliraj2(std::string element, const std::map<std::pair<char, char>, char>& recept, const int& stIteracij) {

	std::map<std::pair<char, char>, long long> steviloZnakov;

	for (auto it = recept.begin(); it != recept.end(); it++)
		steviloZnakov[it->first] = 0;

	for (int i = 0; i < element.size() - 1; i++)
		steviloZnakov[{element[i], element[i + 1]}]++;


	std::map<std::pair<char, char>, long long> vmesnoSteviloZnakov = steviloZnakov;
	for (auto it = vmesnoSteviloZnakov.begin(); it != vmesnoSteviloZnakov.end(); it++)
		it->second = 0;

	for (int i = 0; i < stIteracij; i++) {
		
		for (auto it = steviloZnakov.begin(); it != steviloZnakov.end(); it++) {

			const long long& kolicina = it->second;
			if (kolicina == 0)
				continue;

			const char& cPrvi = it->first.first;
			const char& cVmesni = recept.at(it->first);
			const char& cDrugi = it->first.second;

			vmesnoSteviloZnakov[{cPrvi, cDrugi}] -= kolicina;
			vmesnoSteviloZnakov[{cPrvi, cVmesni}] += kolicina;
			vmesnoSteviloZnakov[{cVmesni, cDrugi}] += kolicina;
		}

		for (auto it = vmesnoSteviloZnakov.begin(); it != vmesnoSteviloZnakov.end(); it++) {
			steviloZnakov[it->first] += it->second;
			it->second = 0;
		}
	}


	std::map<char, long long> seznamPosameznih;
	for (auto it = recept.begin(); it != recept.end(); it++)
		seznamPosameznih[it->second] = 0;
	seznamPosameznih[element.front()]++;
	seznamPosameznih[element.back()]++;

	for (auto it = steviloZnakov.begin(); it != steviloZnakov.end(); it++) {
		seznamPosameznih[it->first.first] += it->second;
		seznamPosameznih[it->first.second] += it->second;
	}


	long long min = seznamPosameznih.begin()->second / 2;
	long long max = seznamPosameznih.begin()->second / 2;

	for (auto it = seznamPosameznih.begin(); it != seznamPosameznih.end(); it++) {
		
		it->second /= 2;

		if (it->second < min)
			min = it->second;
		else if (it->second > max)
			max = it->second;
	}

	
	return max - min;
}


int main() {

	std::string zacetniElement;
	std::map<std::pair<char, char>, char> recept;

	preberiPodatke("2021/14.txt", zacetniElement, recept);

	
	int resitev1 = simuliraj1(zacetniElement, recept, 10);

	std::cout << "Razlika med najvecjo in najmanjso po 10-ih iteracijah je " << resitev1 << ".\n";


	long long resitev2 = simuliraj2(zacetniElement, recept, 40);

	std::cout << "Razlika med najvecjo in najmanjso po 40-ih iteracijah je " << resitev2 << ".\n";


	return 0;
}
