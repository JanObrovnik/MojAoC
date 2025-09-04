/*
	Lahko
	Lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <map>



char najdiPonovitev(const std::string& seznam) {

	std::string del1(seznam.begin(), seznam.end() - seznam.size() / 2);
	std::string del2(seznam.end() - seznam.size() / 2, seznam.end());

	std::map<char, short> mapa;
	for (short i = 0; i < del2.size(); i++)
		mapa[del2[i]] = i;

	for (const char& c : del1)
		if (mapa.find(c) != mapa.end())
			return c;
	
	std::cout << "Napaka - najdiPonovitev()\n";
}

char najdiZnacko(const std::string& seznam1, const std::string& seznam2, const std::string& seznam3) {
	
	std::map<char, short> mapa2;
	for (short i = 0; i < seznam2.size(); i++)
		mapa2[seznam2[i]] = i;
	std::map<char, short> mapa3;
	for (short i = 0; i < seznam3.size(); i++)
		mapa3[seznam3[i]] = i;

	for (const char& c : seznam1) {
		if (mapa2.find(c) != mapa2.end() && mapa3.find(c) != mapa3.end())
			return c;
	}
	
	std::cout << "Napaka - najdiZnacko()\n";
}

int vrednostKarakterja(const char& c) {

	if ('a' <= c && c <= 'z') return c - 'a' + 1;
	if ('A' <= c && c <= 'Z') return c - 'A' + 27;

	std::cout << "Napaka - vrednostKarakterja()\n";
}


int preberiPodatke1(std::string pot) {

	int resitev = 0;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return 0;
	}

	std::string vrstica;

	while (podatki >> vrstica)
		resitev += vrednostKarakterja(najdiPonovitev(vrstica));

	podatki.close();


	return resitev;
}


int preberiPodatke2(std::string pot) {

	int resitev = 0;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return 0;
	}

	std::string vrstica1, vrstica2, vrstica3;

	while (podatki >> vrstica1 >> vrstica2 >> vrstica3)
		resitev += vrednostKarakterja(najdiZnacko(vrstica1, vrstica2, vrstica3));

	podatki.close();


	return resitev;
}


int main() {

	int resitev(preberiPodatke1("Advent22/3.txt"));
	std::cout << "Vsota ponovitev je " << resitev << ".\n";

	int resitev2(preberiPodatke2("Advent22/3.txt"));
	std::cout << "Vsota znack skupin je " << resitev2 << ".\n";


	return 0;
}