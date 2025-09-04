#include <iostream>
#include <fstream>


int main() {

	std::fstream podatki;
	podatki.open("Advent15-1.txt", std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteka ni najdena.\n";
		return 1;
	}

	int resitev1 = 0;
	int stPonovitev = 1;
	int resitev2 = -1;
	char c;

	while (podatki >> c) {
		if (c == '(') resitev1++;
		else resitev1--;

		if (resitev1 == -1 && resitev2 == -1)
			resitev2 = stPonovitev;
		stPonovitev++;
	}
	
	podatki.close();


	std::cout << "Nadstropje: " << resitev1 << ".\n";
	std::cout << "Iterator kleti: " << resitev2 << ".\n";




	return 0;
}