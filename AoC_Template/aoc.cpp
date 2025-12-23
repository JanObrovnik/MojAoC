#include <iostream>
#include <fstream>
#include <string>


void preberiPodatke(const std::string& pot) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return;
	}

	while (podatki.peek() != EOF) {

	}

	podatki.close();

	return;
}


int main() {

	preberiPodatke("2025/$itemname$.txt");


	return 0;
}
