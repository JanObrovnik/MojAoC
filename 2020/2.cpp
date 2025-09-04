/*
	t = 24:55 min
	precej lahko
	t = 7:28 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>


class Geslo {
private:
	char znakPolitika;
	std::pair<short, short> omejitvePolitika;
	std::string geslo;

public:
	Geslo(std::istream& is) {
		is >> omejitvePolitika.first;
		is.ignore(1);
		is >> omejitvePolitika.second;
		is >> znakPolitika;
		is.ignore(1);
		is >> geslo;
		//std::cout << omejitvePolitika.first << '-' << omejitvePolitika.second << ' ' << znakPolitika << ": " << geslo << '\n';
	}

	bool pravilno1() const {
		const short& steviloZnakov = std::count(geslo.begin(), geslo.end(), znakPolitika);
		return steviloZnakov >= omejitvePolitika.first && steviloZnakov <= omejitvePolitika.second;
	}

	bool pravilno2() const {
		const short& steviloZnakov = (geslo[omejitvePolitika.first - 1] == znakPolitika) +
									 (geslo[omejitvePolitika.second - 1] == znakPolitika);
		return steviloZnakov == 1;
	}
};


std::pair<int,int> preberiPodatke(const std::string& pot) {

	std::pair<int, int> resitev = { 0,0 };

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { -1,-1 };
	}

	while (podatki.peek() != EOF) {
		Geslo geslo(podatki);
		resitev.first += geslo.pravilno1();
		resitev.second += geslo.pravilno2();
	}

	podatki.close();

	return resitev;
}


int main() {

	std::pair<int, int> resitev = preberiPodatke("2020/2.txt");

	std::cout << "Stevilo pravilnih gesl je " << resitev.first << ".\n";
	std::cout << "Stevilo pravilnih gesl je " << resitev.second << ".\n";


	return 0;
}
