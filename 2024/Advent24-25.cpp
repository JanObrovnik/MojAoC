#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>



void preberiPodatke(std::string pot, 
	std::vector<std::array<short, 5>>& seznamKljucavnic, std::vector<std::array<short, 5>>& seznamKljucev) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni bila najdena.\n";
		return;
	}

	std::string vrstica;
	char c;

	while (podatki.peek() != EOF) {

		podatki >> vrstica;

		std::array<short, 5> visine{0};

		for (int i = 0; i < 5; i++) 
			for (int i = 0; i < 5; i++) {
				
				podatki >> c;

				if (c == '#')
					visine[i]++;
			}
		
		(vrstica.front() == '#') ? seznamKljucavnic.push_back(visine) : seznamKljucev.push_back(visine);

		podatki >> vrstica;
	}

	podatki.close();
}


inline bool ujemanjeArrayev(std::array<short, 5>& kljucavnica, std::array<short, 5>& kljuc) {

	for (int i = 0; i < 5; i++)
		if (kljuc[i] + kljucavnica[i] > 5)
			return false;

	return true;
}

int steviloUjemanj(std::vector<std::array<short, 5>>& seznamKljucavnic, std::vector<std::array<short, 5>>& seznamKljucev) {

	int resitev = 0;

	for (auto& kljucavnica : seznamKljucavnic) {
		for (auto& kljuc : seznamKljucev) {
			if (ujemanjeArrayev(kljucavnica, kljuc)) resitev++;
		}
	}

	return resitev;
}


int main(int argc, char* argv[]) {

	std::vector<std::array<short, 5>> seznamKljucavnic;
	std::vector<std::array<short, 5>> seznamKljucev;
	seznamKljucavnic.reserve(250);
	seznamKljucev.reserve(250);

	preberiPodatke("Advent24-25.txt", seznamKljucavnic, seznamKljucev);


	std::cout << "seznamKljucavnic:\n";
	for (auto& arr : seznamKljucavnic) {
		for (short& i : arr)
			std::cout << i << ' ';
		std::cout << '\n';
	}
	std::cout << '\n';

	std::cout << "seznamKljucev:\n";
	for (auto& arr : seznamKljucev) {
		for (short& i : arr)
			std::cout << i << ' ';
		std::cout << '\n';
	}
	std::cout << '\n';

	std::cout << "seznamKljucavnic: " << seznamKljucavnic.size() << "   seznamKljucev: " << seznamKljucev.size() << '\n';


	int resitev = steviloUjemanj(seznamKljucavnic, seznamKljucev);
	std::cout << "Stevilo ujemanj je " << resitev << ".\n";



	return 0;
}
