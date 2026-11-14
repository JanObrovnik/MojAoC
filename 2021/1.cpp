/*
	zelo lahko
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <queue>


int prebriPodatke1(std::string pot) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	int predhodnjeSt, sedanjeSt;
	podatki >> predhodnjeSt;

	while (podatki.peek() != EOF) {

		podatki >> sedanjeSt;

		if (predhodnjeSt < sedanjeSt)
			resitev++;

		predhodnjeSt = sedanjeSt;
	}

	podatki.close();

	return resitev;
}

int prebriPodatke2(std::string pot) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::queue<int> vrsta;
	int staraVsota(0), novaVsota(0), dodajanjeSt(0);
	
	podatki >> dodajanjeSt;
	vrsta.push(dodajanjeSt);
	staraVsota += dodajanjeSt;

	podatki >> dodajanjeSt;
	vrsta.push(dodajanjeSt);
	staraVsota += dodajanjeSt;

	podatki >> dodajanjeSt;
	vrsta.push(dodajanjeSt);
	staraVsota += dodajanjeSt;

	novaVsota = staraVsota;

	while (podatki.peek() != EOF) {

		novaVsota -= vrsta.front();
		vrsta.pop();

		podatki >> dodajanjeSt;
		vrsta.push(dodajanjeSt);
		novaVsota += dodajanjeSt;

		if (staraVsota < novaVsota)
			resitev++;

		staraVsota = novaVsota;
	}

	podatki.close();

	return resitev;
}


int main(int argc, char* argv[]) {

	int resitev1(prebriPodatke1("2021/1.txt"));

	std::cout << "Globina se poveca za " << resitev1 << " meritev.\n";


	int resitev2(prebriPodatke2("2021/1.txt"));

	std::cout << "Globina se poveca za " << resitev2 << " oknov meritev.\n";


	return 0;
}