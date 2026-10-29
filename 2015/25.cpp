/*
	t = 1:16:11 ur
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>


std::pair<int, int> preberiPodatke(const std::string& pot) {

	std::pair<int, int> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki.ignore(80);
	podatki >> resitev.first;
	podatki.ignore(8);
	podatki >> resitev.second;

	podatki.close();

	return resitev;
}


unsigned int najdiLokacijo(const int& row, const int& col) {

	float x = (1. + col) / 2. * col;
	float y = (col + (row + col - 2.)) / 2. * (row - 1.);

	return static_cast<unsigned int>(std::round(x + y));
}

unsigned long long dolociKodo(const int& row, const int& col) {

	unsigned long long vrednost = 20151125ull;
	const unsigned long long mnozitelj = 252533ull;
	const unsigned long long deljitelj = 33554393ull;

	unsigned int lokacija = najdiLokacijo(row, col);

	for (unsigned int i = 1; i < lokacija; i++) {
		vrednost *= mnozitelj;
		vrednost %= deljitelj;
	}

	return vrednost;
}


int main() {

	std::pair<int, int> koordinate = preberiPodatke("2015/25.txt");
	//for (int row = 1; row <= 10; row++)
	//	for (int col = 1; col <= 10; col++)
	//		najdiLokacijo(row, col);


	unsigned long long resitev1 = dolociKodo(koordinate.first, koordinate.second);
	std::cout << "Nasa koda je " << resitev1 << ".\n";


	return 0;
}
