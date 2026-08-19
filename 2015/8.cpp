/*
	t = 17:42 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>


void skrajsajVrstico(std::string& vrstica) {

	std::string resitev;

	for (int i = 1; i < vrstica.size() - 1; i++) {

		if (vrstica[i] == '\\') {
			if (vrstica[i + 1] == '\\') {
				resitev.push_back('\\');
				i++;
			}
			else if (vrstica[i + 1] == '\"') {
				resitev.push_back('\"');
				i++;
			}
			else if (vrstica[i + 1] == 'x') {
				resitev.push_back('x');
				i += 3;
			}
		}
		else {
			resitev.push_back(vrstica[i]);
		}
	}

	vrstica = resitev;
}


int preberiPodatke(const std::string& pot) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return -1;
	}

	std::string vrstica;
	int dolzinaZapisa = 0, dolzinaSpomina = 0;

	while (podatki.peek() != EOF) {
		
		std::getline(podatki, vrstica);

		dolzinaZapisa += vrstica.size();
		
		skrajsajVrstico(vrstica);

		dolzinaSpomina += vrstica.size();

		//std::cout << vrstica << ": " << dolzinaZapisa << " | " << dolzinaSpomina << '\n';
	}

	podatki.close();

	return dolzinaZapisa - dolzinaSpomina;
}


int main() {

	int resitev1 = preberiPodatke("2015/8.txt");

	std::cout << "Razlika med dolzino zapisa in dolzino spomina je " << resitev1 << ".\n";


	return 0;
}
