/*
	t = 57:56 min
	precej lahko
	t = 16:21 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


enum MATH {
	Plus = -1,
	Krat = -2,
	Oklepaj = -3,
	Zaklepaj = -4
};

void printPodatek(const std::vector<long long>& podatek) {
	for (const long long& i : podatek) {
		if (i == MATH::Oklepaj) std::cout << '(';
		else if (i == MATH::Zaklepaj) std::cout << ')';
		else if (i == MATH::Plus) std::cout << '+';
		else if (i == MATH::Krat) std::cout << '*';
		else std::cout << i;
	}
}
void printPodatke(const std::vector<std::vector<long long>>& bazaPodatkov) {
	for (const std::vector<long long>& podatek : bazaPodatkov) {
		printPodatek(podatek);
		std::cout << '\n';
	}
}

std::vector<std::vector<long long>> preberiPodatke(const std::string& pot) {

	std::vector<std::vector<long long>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (std::getline(podatki, vrstica)) {

		resitev.push_back({});

		for (const char& c : vrstica) {

			if (c == ' ')
				continue;

			if (c == '(')
				resitev.back().push_back(MATH::Oklepaj);
			else if (c == ')')
				resitev.back().push_back(MATH::Zaklepaj);
			else if (c == '+')
				resitev.back().push_back(MATH::Plus);
			else if (c == '*')
				resitev.back().push_back(MATH::Krat);
			else
				resitev.back().push_back(c - '0');
		}
	}

	podatki.close();

	return resitev;
}


std::pair<std::vector<long long>::const_iterator, std::vector<long long>::const_iterator> najdiOkepaj(const std::vector<long long>& vec) {

	std::vector<long long>::const_iterator it1 = vec.end();
	std::vector<long long>::const_iterator it2 = vec.end();

	for (std::vector<long long>::const_iterator it = vec.begin(); it != vec.end(); it++) {
		if (*it == MATH::Oklepaj) {
			it1 = it;
		}
		else if (*it == MATH::Zaklepaj) {
			it2 = it;
			break;
		}
	}

	return { it1,it2 };
}
std::vector<long long>::const_iterator najdiPlus(const std::vector<long long>& vec) {

	for (std::vector<long long>::const_iterator it = vec.begin(); it != vec.end(); it++)
		if (*it == MATH::Plus)
			return it - 1;

	return vec.cend();
}
long long izracunajNovoVrednost(const std::vector<long long>::const_iterator& it) {
	if (*(it + 1) == MATH::Plus)
		return *it + *(it + 2);
	if (*(it + 1) == MATH::Krat)
		return *it * *(it + 2);
	std::cout << "Napaka - izracunajNovoVrednost()\n";
	return -1;
}

long long izracunajPodatek(std::vector<long long> podatek, const int& del) {

	while (podatek.size() > 1) {

		std::pair<std::vector<long long>::const_iterator, std::vector<long long>::const_iterator> it = najdiOkepaj(podatek);

		if (it.first != podatek.end()) {

			long long novaVrednost;
			
			if (it.second - it.first == 4) {
				novaVrednost = izracunajNovoVrednost(it.first + 1);
			}
			else if (it.second - it.first > 4) {
				std::vector<long long> podPodatek(it.first + 1, it.second);
				novaVrednost = izracunajPodatek(podPodatek, del);
			}
			else {
				std::cout << "Napaka - izracunajPodatek()\n";
			}

			int mesto = it.first - podatek.begin();
			
			podatek.erase(it.first, it.second + 1);
			podatek.insert(podatek.begin() + mesto, novaVrednost);
		}
		else if (del == 1) {

			long long novaVrednost = izracunajNovoVrednost(podatek.begin());

			podatek.erase(podatek.begin(), podatek.begin() + 3);
			podatek.insert(podatek.begin(), novaVrednost);
		}
		else if (del == 2) {

			std::vector<long long>::const_iterator itP = najdiPlus(podatek);

			if (itP != podatek.end()) {

				long long novaVrednost = izracunajNovoVrednost(itP);

				int mesto = itP - podatek.begin();

				podatek.erase(itP, itP + 3);
				podatek.insert(podatek.begin() + mesto, novaVrednost);
			}
			else {

				long long novaVrednost = izracunajNovoVrednost(podatek.begin());

				podatek.erase(podatek.begin(), podatek.begin() + 3);
				podatek.insert(podatek.begin(), novaVrednost);
			}
		}
	}

	return podatek.front();
}

long long izracunajPodatke(const std::vector<std::vector<long long>>& bazaPodatkov, int del = 1) {

	long long resitev = 0;

	for (const std::vector<long long>& podatek : bazaPodatkov) {
		resitev += izracunajPodatek(podatek, del);
		//std::cout << izracunajPodatek(podatek, del) << '\n';
	}

	return resitev;
}


int main() {

	std::vector<std::vector<long long>> bazaPodatkov(preberiPodatke("2020/18.txt"));
	//printPodatke(bazaPodatkov);

	long long resitev1 = izracunajPodatke(bazaPodatkov, 1);
	std::cout << "Vsota vseh izracunov je " << resitev1 << ".\n";

	long long resitev2 = izracunajPodatke(bazaPodatkov, 2);
	std::cout << "Vsota vseh izracunov je " << resitev2 << ".\n";


	return 0;
}
