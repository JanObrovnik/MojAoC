/*
	zelo lahko
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>


struct Podmornica {
	int horinzontala;
	int globina;
	int cilj;

	Podmornica() : horinzontala(0), globina(0), cilj(0) {}
	Podmornica(int horinzontala, int globina, int cilj) : horinzontala(horinzontala), globina(globina), cilj(cilj) {}

	int resitev() const { return horinzontala * globina; }
};


Podmornica preberiPodatke(std::string pot, bool del2 = false) {

	Podmornica resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string str;
	int st;

	while (podatki.peek() != EOF) {

		podatki >> str >> st;

		if (del2 == false) {
			if		(str == "down") resitev.globina += st;
			else if (str == "up")   resitev.globina -= st;
			else					resitev.horinzontala += st;
		}
		else {
			if		(str == "down") resitev.cilj += st;
			else if (str == "up")   resitev.cilj -= st;
			else {
				resitev.horinzontala += st;
				resitev.globina += resitev.cilj * st;
			}
		}
	}

	podatki.close();

	return resitev;
}


int main() {

	Podmornica podmornica1(preberiPodatke("2021/2.txt"));

	std::cout << "horiz * glob = " << podmornica1.resitev() << '\n';


	Podmornica podmornica2(preberiPodatke("2021/2.txt", true));

	std::cout << "horiz * glob = " << podmornica2.resitev() << '\n';


	return 0;
}