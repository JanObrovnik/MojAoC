/*
	t = 17:42 min
	zelo lahko
	t = 12:19 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>



std::string skrajsajVrstico(const std::string& vrstica) {

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

	return resitev;
}

std::string razsiriVrstico(const std::string& vrstica) {

	std::string resitev;

	resitev.push_back('\"');

	for (int i = 0; i < vrstica.size(); i++) {

		if (vrstica[i] == '\\') {
			resitev.push_back('\\');
			resitev.push_back('\\');
		}
		else if (vrstica[i] == '\"') {
			resitev.push_back('\\');
			resitev.push_back('\"');
		}
		else {
			resitev.push_back(vrstica[i]);
		}
	}
	
	resitev.push_back('\"');

	return resitev;
}


std::pair<int, int> preberiPodatke(const std::string& pot) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { -1,-1 };
	}

	std::string vrstica;
	int dolzinaZapisa = 0, dolzinaSpomina = 0, dolzinaRazsiritve = 0;

	while (podatki.peek() != EOF) {
		
		std::getline(podatki, vrstica);

		dolzinaZapisa += vrstica.size();
		
		std::string skrajsanaVrstica = skrajsajVrstico(vrstica);
		dolzinaSpomina += skrajsanaVrstica.size();

		std::string razsirjenaVrstica = razsiriVrstico(vrstica);
		dolzinaRazsiritve += razsirjenaVrstica.size();

		//std::cout << vrstica << " | " << skrajsanaVrstica << " | " << razsirjenaVrstica << '\n';
	}

	podatki.close();

	return { dolzinaZapisa-dolzinaSpomina,dolzinaRazsiritve-dolzinaZapisa };
}


int main() {

	std::pair<int, int> resitev = preberiPodatke("2015/8.txt");

	std::cout << "Razlika med dolzino zapisa in dolzino spomina je " << resitev.first << ".\n";
	std::cout << "Razlika med dolzino zapisa zapisa in dolzino zapisa je " << resitev.second << ".\n";


	return 0;
}
