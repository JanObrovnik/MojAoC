/*
	t = 3:11:16 h
	lahko
	t = 8:20 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& ve) {
	for (const T& i : ve) {
		if (i == -1) os << "[ ";
		else if (i == -2) os << "] ";
		else os << i << ' ';
	}
	return os;
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<std::vector<T>>& ve) {
	for (const std::vector<T>& ve : ve) {
		os << ve << '\n';
	}
	return os;
}


std::vector<std::vector<short>> preberiPodatke(const std::string& pot) {

	std::vector<std::vector<short>> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	std::vector<short> stevilskaVrstica;

	while (std::getline(podatki, vrstica)) {
		for (const char& c : vrstica) {

			if (c == ',')
				continue;
			else if (c == '[')
				stevilskaVrstica.push_back(-1);
			else if (c == ']')
				stevilskaVrstica.push_back(-2);
			else
				stevilskaVrstica.push_back(c - '0');
		}

		resitev.push_back(stevilskaVrstica);
		stevilskaVrstica.clear();
	}

	podatki.close();

	return resitev;
}


void reduciraj(std::vector<short>& seznam) {

ZACETEK:

	//- Eksplodiraj
	short stOklepajev = 0;

	for (int i = 0; i < seznam.size(); i++) {

		if (seznam[i] == -1) { stOklepajev++; continue; }
		if (seznam[i] == -2) { stOklepajev--; continue; }

		if (stOklepajev == 5) {
			int mesto = i - 1;

			for (int j = mesto - 1; j > 0; j--)
				if (seznam[j] >= 0) {
					seznam[j] += seznam[mesto + 1];
					break;
				}
			for (int j = mesto + 4; j < seznam.size(); j++)
				if (seznam[j] >= 0) {
					seznam[j] += seznam[mesto + 2];
					break;
				}

			seznam.erase(seznam.begin() + mesto, seznam.begin() + mesto + 4);
			seznam.insert(seznam.begin() + mesto, 0);

			goto ZACETEK;
		}
	}

	//- Razdvoji
	for (int i = 0; i < seznam.size(); i++) {
	
		if (seznam[i] > 9) {
			int st = seznam[i];

			seznam.erase(seznam.begin() + i);
			seznam.insert(seznam.begin() + i, -2);
			seznam.insert(seznam.begin() + i, st/2 + st%2);
			seznam.insert(seznam.begin() + i, st/2);
			seznam.insert(seznam.begin() + i, -1);

			goto ZACETEK;
		}
	}
}

std::vector<short> zdruzi(const std::vector<std::vector<short>>& seznam) {

	std::vector<short> resitev = seznam.front();

	for (int i = 1; i < seznam.size(); i++) {

		resitev.insert(resitev.begin(), -1);
		for (const short& i : seznam[i] )
			resitev.push_back(i);
		resitev.push_back(-2);

		reduciraj(resitev);
	}

	return resitev;
}


int magnituda(const int& i1, const int& i2) {
	return 3 * i1 + 2 * i2;
}

int magnitudaSeznama(const std::vector<short>& ve) {

	std::vector<int> seznam(ve.begin(), ve.end());

	int leva = -1;
	int desna = -1;
	int indeks = -1;

	while (seznam.size() > 1) {
		for (int i = 0; i < seznam.size(); i++) {
			const int& st = seznam[i];

			if (st == -1) {
				leva = -1;
				continue;
			}

			if (leva >= 0) {
				desna = st;
				indeks = i - 2;
				break;
			}
			else {
				leva = st;
				continue;
			}
		}

		int vsota = magnituda(leva, desna);

		seznam.erase(seznam.begin() + indeks, seznam.begin() + indeks + 4);
		seznam.insert(seznam.begin() + indeks, vsota);

		leva = -1;
		desna = -1;
	}

	return seznam.front();
}


int najdiMaxParov(const std::vector<std::vector<short>>& seznam) {

	int resitev = 0;

	for (int i = 0; i < seznam.size(); i++) {
		for (int j = 0; j < seznam.size(); j++) {

			if (i == j)
				continue;

			const int& nova(magnitudaSeznama(zdruzi({ seznam[i],seznam[j] })));

			if (nova > resitev)
				resitev = nova;
		}
	}

	return resitev;
}


int main(int argc, char* argv[]) {

	std::vector<std::vector<short>> podatki(preberiPodatke("2021/18.txt"));
	//std::cout << podatki << '\n';
	//std::cout << zdruzi(podatki) << '\n';


	int resitev1 = magnitudaSeznama(zdruzi(podatki));
	std::cout << "Magnituda domace naloge je " << resitev1 << ".\n";


	int resitev2 = najdiMaxParov(podatki);
	std::cout << "Najvecja magnituda parov je " << resitev2 << ".\n";



	return 0;
}
