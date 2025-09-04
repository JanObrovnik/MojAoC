/*
	Enostavna
	Enostavna
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <chrono>


enum simboli {
	PRAZNINA = '.',
	GALAKSIJA = '#'
};


class Tocka {
public:
	short x, y;

	Tocka() : x(0), y(0) {}
	Tocka(short x, short y) : x(x), y(y) {}
};

int razdalja(Tocka& t1, Tocka& t2) {
	return abs(t1.x - t2.x) + abs(t1.y - t2.y);
}

unsigned long long razdaljaPlusPlusMK1(Tocka& t1, Tocka& t2, std::set<int>& praznineX, std::set<int>& praznineY, int faktor = 2) {

	faktor--;

	unsigned long long razdaljaX = abs(t1.x - t2.x);
	int minX = std::min(t1.x, t2.x);
	int maxX = std::max(t1.x, t2.x);
	for (int x = minX + 1; x < maxX; x++)
		if (praznineX.find(x) != praznineX.end())
			razdaljaX += faktor;

	unsigned long long razdaljaY = abs(t1.y - t2.y);
	int minY = std::min(t1.y, t2.y);
	int maxY = std::max(t1.y, t2.y);;
	for (int y = minY + 1; y < maxY; y++)
		if (praznineY.find(y) != praznineY.end())
			razdaljaY += faktor;

	return razdaljaX + razdaljaY;
}

unsigned long long razdaljaPlusPlusMK2(Tocka& t1, Tocka& t2, std::set<int>& praznineX, std::set<int>& praznineY, int faktor = 2) {

	faktor--;

	int minX = std::min(t1.x, t2.x);
	int maxX = std::max(t1.x, t2.x);
	unsigned long long razdaljaX = maxX - minX;
	for (int x : praznineX)
		if (x > minX && x < maxX)
			razdaljaX += faktor;

	int minY = std::min(t1.y, t2.y);
	int maxY = std::max(t1.y, t2.y);
	unsigned long long razdaljaY = maxY - minY;
	for (int y : praznineY)
		if (y > minY && y < maxY)
			razdaljaY += faktor;

	return razdaljaX + razdaljaY;
}



class Vesolje {
private:
	std::vector<std::string> slika;
	std::vector<Tocka> seznamGalaksij;
	std::set<int> galaksijeX, galaksijeY;
	std::set<int> praznineX, praznineY;


public:
	Vesolje(std::vector<std::string> slika) : slika(slika) {}


	void zapisiGalaksije() {

		for (int y = 0; y < slika.size(); y++)
			for (int x = 0; x < slika[y].size(); x++)
				if (slika[y][x] == GALAKSIJA) {
					galaksijeX.insert(x);
					galaksijeY.insert(y);
				}
	}

	void zapisiPraznine() {

		zapisiGalaksije();

		for (int y = 0; y < slika.size(); y++)
			if (galaksijeY.find(y) == galaksijeY.end())
				praznineY.insert(y);

		for (int x = 0; x < slika[0].size(); x++)
			if (galaksijeX.find(x) == galaksijeX.end())
				praznineX.insert(x);
	}

	void razsiri(int faktor = 2) {

		zapisiGalaksije();

		faktor--;

		for (int y = slika.size() - 1; y >= 0; y--)
			if (galaksijeY.find(y) == galaksijeY.end())
				for (int i = 0; i < faktor; i++)
					slika.insert(slika.begin() + y, std::string(slika[y].size(), '.'));

		for (int x = slika[0].size() - 1; x >= 0; x--)
			if (galaksijeX.find(x) == galaksijeX.end())
				for (std::string& str : slika)
					for (int i = 0; i < faktor; i++)
						str.insert(str.begin() + x, '.');
	}

	void najdiGalaksije() {

		for (int y = 0; y < slika.size(); y++)
			for (int x = 0; x < slika[y].size(); x++)
				if (slika[y][x] == GALAKSIJA)
					seznamGalaksij.push_back(Tocka(x, y));
	}


	int vsotaRazdalj() {

		int resitev = 0;
		
		for (int i = 0; i < seznamGalaksij.size() - 1; i++)
			for (int j = i + 1; j < seznamGalaksij.size(); j++)
				resitev += razdalja(seznamGalaksij[i], seznamGalaksij[j]);
		
		return resitev;
	}

	unsigned long long vsotaRazdaljPlusPlusMK1(int faktor = 2) {

		unsigned long long resitev = 0;

		for (int i = 0; i < seznamGalaksij.size() - 1; i++)
			for (int j = i + 1; j < seznamGalaksij.size(); j++)
				resitev += razdaljaPlusPlusMK1(seznamGalaksij[i], seznamGalaksij[j], praznineX, praznineY, faktor);

		return resitev;
	}

	unsigned long long vsotaRazdaljPlusPlusMK2(int faktor = 2) {

		unsigned long long resitev = 0;

		for (int i = 0; i < seznamGalaksij.size() - 1; i++)
			for (int j = i + 1; j < seznamGalaksij.size(); j++)
				resitev += razdaljaPlusPlusMK2(seznamGalaksij[i], seznamGalaksij[j], praznineX, praznineY, faktor);

		return resitev;
	}

	void narisi() {
		std::for_each(slika.begin(), slika.end(), [](std::string str) {std::cout << str << '\n'; });
	}
};


std::vector<std::string> preberiDatoteko(std::string pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (podatki.is_open()) {

		while (podatki.peek() != EOF) {

			std::string vrstica;

			podatki >> vrstica;

			resitev.push_back(vrstica);
		}
	}
	else std::cout << "Datoteka ni najdena.\n";

	return resitev;
}


int main() {

	Vesolje vesolje1(preberiDatoteko("PodatkiAdvent23-11.txt"));

	vesolje1.razsiri(2);
	vesolje1.najdiGalaksije();

	int vsota1 = vesolje1.vsotaRazdalj();

	//vesolje1.narisi();
	std::cout << "Prva vsota razdalj med galaksijam je " << vsota1 << ".\n";
	std::cout << std::endl;


	Vesolje vesolje2(preberiDatoteko("PodatkiAdvent23-11.txt"));

	vesolje2.najdiGalaksije();
	vesolje2.zapisiPraznine();

	std::chrono::high_resolution_clock::time_point cas1 = std::chrono::high_resolution_clock::now();
	unsigned long long vsota2 = vesolje2.vsotaRazdaljPlusPlusMK1(1000000);
	std::cout << "Cas 1: " << (std::chrono::high_resolution_clock::now() - cas1).count() / 1000000 << " ms\n";

	std::cout << "Druga vsota razdalj med galaksijam je " << vsota2 << ".\n";
	std::cout << std::endl;

	std::chrono::high_resolution_clock::time_point cas2 = std::chrono::high_resolution_clock::now();
	unsigned long long vsota3 = vesolje2.vsotaRazdaljPlusPlusMK2(1000000);
	std::cout << "Cas 2: " << (std::chrono::high_resolution_clock::now() - cas2).count() / 1000000 << " ms\n";

	std::cout << "Druga vsota razdalj med galaksijam je " << vsota3 << ".\n";
	std::cout << std::endl;

	return 0;
}
