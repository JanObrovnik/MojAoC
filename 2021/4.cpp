/*
	t = 54:18
	nekoliko lahko
	t = 8:30
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>


class BingoKarta {

	std::array<std::array<short, 5>, 5> karta;

public:
	BingoKarta() : karta() {}
	BingoKarta(std::array<std::array<short, 5>, 5> karta) : karta(karta) {}


	bool dodajZadetek(const short& zadetek) {

		bool resitev = false;

		for (std::array<short, 5>& arr : karta)
			for (short& i : arr)
				if (i == zadetek) {
					i = -1;
					resitev = true;
				}

		return resitev;
	}

	bool zmaga() const {

		for (int y = 0; y < 5; y++) {

			int vsota1 = 0, vsota2 = 0;

			for (int x = 0; x < 5; x++) {
				vsota1 += karta[y][x];
				vsota2 += karta[x][y];
			}

			if (vsota1 == -5 || vsota2 == -5)
				return true;
		}

		return false;
	}

	int vrniPreostanek() {

		int resitev = 0;

		for (std::array<short, 5>&arr : karta)
			for (short& i : arr)
				if (i != -1)
					resitev += i;

		return resitev;
	}


	void print() const {
		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				std::cout << karta[y][x] << '\t';
			}
			std::cout << '\n';
		}
	}
};


std::pair<std::vector<BingoKarta>, std::vector<short>> preberiPodatke(const std::string& pot) {

	std::vector<BingoKarta> resitev1;
	std::vector<short> resitev2;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	std::string vrstica;

	std::getline(podatki, vrstica);

	size_t sz = 0;

	while (sz != vrstica.size()) {
		vrstica = vrstica.substr(sz);
		vrstica = vrstica.substr(vrstica.find_first_of("0123456789"));
		resitev2.push_back((short)std::stoi(vrstica, &sz));
	}

	while (podatki.peek() != EOF) {

		std::array<std::array<short, 5>, 5> karta;

		for (int y = 0; y < 5; y++) {
			for (int x = 0; x < 5; x++) {
				podatki >> karta[y][x];
			}
		}

		resitev1.push_back(BingoKarta(karta));
	}

	podatki.close();

	//for (const int& i : resitev2)
	//	std::cout << i << ' ';
	//std::cout << '\n';
	//std::cout << '\n';
	//
	//for (const BingoKarta& bk : resitev1) {
	//	bk.print();
	//	std::cout << '\n';
	//}
	//std::cout << '\n';

	return { resitev1,resitev2 };
}


int najdiPrvoZmago(std::vector<BingoKarta>& seznamBingoKart, const std::vector<short>& seznamZadetkov) {

	for (const short& i : seznamZadetkov)
		for (BingoKarta& bingoKarta : seznamBingoKart) {

			bingoKarta.dodajZadetek(i);

			if (bingoKarta.zmaga() == true)
				return bingoKarta.vrniPreostanek() * i;
		}

	return -1;
}

int najdiZadnjoZmago(std::vector<BingoKarta>& seznamBingoKart, const std::vector<short>& seznamZadetkov) {

	for (const short& zadetek : seznamZadetkov)
		for (int i = 0; i < seznamBingoKart.size(); i++) {

			seznamBingoKart[i].dodajZadetek(zadetek);

			if (seznamBingoKart[i].zmaga() == true) {
				
				if (seznamBingoKart.size() == 1)
					return seznamBingoKart[i].vrniPreostanek() * zadetek;

				seznamBingoKart.erase(seznamBingoKart.begin() + i);
				i--;
			}
		}

	return -1;
}


int main() {

	std::pair<std::vector<BingoKarta>, std::vector<short>> podatki(preberiPodatke("2021/4.txt"));
	std::vector<BingoKarta> seznamBingoKart1 = podatki.first;
	std::vector<BingoKarta> seznamBingoKart2 = podatki.first;
	std::vector<short>& seznamZadetkov = podatki.second;

	int resitev1 = najdiPrvoZmago(seznamBingoKart1, seznamZadetkov);

	std::cout << "Najbolsa bingo karta ima " << resitev1 << " tock.\n";


	int resitev2 = najdiZadnjoZmago(seznamBingoKart2, seznamZadetkov);

	std::cout << "Najslabsa bingo karta ima " << resitev2 << " tock.\n";


	return 0;
}
