/*
	Zelo lahka
	Precej lahka
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>


class ScratchCard {
private:
	short id;
	std::list<short> seznamZadetkov;
	std::list<short> seznamMojihSt;
	int kolicina;

public:
	ScratchCard() : id(0), kolicina(1) {}
	ScratchCard(short kartaID) : id(kartaID), kolicina(1) {}

	short dobiID() const { return id; }

	void dodajZadetek(short zadetek) { seznamZadetkov.push_back(zadetek); }
	void dodajZadetek(std::list<short> seznamZadetkov) { this->seznamZadetkov = seznamZadetkov; }

	void dodajMojo(short mojaSt) { seznamMojihSt.push_back(mojaSt); }
	void dodajMojo(std::list<short> seznamMojihSt) { this->seznamMojihSt = seznamMojihSt; }

	void povecajKolicino() { kolicina++; }
	void povecajKolicino(int kolicina) { this->kolicina += kolicina; }


	int dobicek() const;
	int dobicekNovihKart() const;

	int dobiKolicina() const { return kolicina; }
};


int ScratchCard::dobicek() const {
	
	int resitev = 0;
	int stZadetkov = 0;

	std::for_each(seznamZadetkov.begin(), seznamZadetkov.end(), [&](int i) {
		if (std::find(seznamMojihSt.begin(), seznamMojihSt.end(), i) != seznamMojihSt.end()) stZadetkov++;
		});

	resitev = pow(2, --stZadetkov);


	return resitev;
}

int ScratchCard::dobicekNovihKart() const {

	int resitev = 0;

	std::for_each(seznamZadetkov.begin(), seznamZadetkov.end(), [&](int i) {
		if (std::find(seznamMojihSt.begin(), seznamMojihSt.end(), i) != seznamMojihSt.end()) resitev++;
		});

	return resitev;
}


std::vector<ScratchCard> vnesiPodatke(std::string pot) {
	
	std::vector<ScratchCard> resitev;


	std::ifstream podatki;
	podatki.open(pot, std::ios::in);

	if (podatki.is_open()) {

		while (podatki.peek() != EOF) {

			std::string stavek = "";
			std::getline(podatki, stavek);
			std::stringstream ss(stavek);

			std::string str;
			int vrednost;
			char ch;

			ss >> str;
			ss >> vrednost;

			ScratchCard karta(vrednost);

			ss >> ch;


			std::string strDel1, strDel2;

			std::getline(ss, strDel1, '|');
			std::getline(ss, strDel2);

			std::stringstream ssDel1(strDel1);
			std::stringstream ssDel2(strDel2);


			int vrednost1;
			while (ssDel1 >> vrednost1)
				karta.dodajMojo(vrednost1);

			int vrednost2;
			while (ssDel2 >> vrednost2)
				karta.dodajZadetek(vrednost2);


			resitev.push_back(karta);
		}
	}
	else std::cout << "Datoteka ni najdena";


	return resitev;
}



int main() {

	std::vector<ScratchCard> seznamKart = vnesiPodatke("Advent23/4.txt");

	int skupniDobicek = 0;
	std::for_each(seznamKart.begin(), seznamKart.end(), [&skupniDobicek](ScratchCard karta) {
		int dobicek = karta.dobicek();
		skupniDobicek += dobicek;
		//std::cout << "Karta " << karta.dobiID() << " je prinesla " << dobicek << " tock.\n";
		});
	std::cout << "Skupni prinesek je " << skupniDobicek << " tock.";
	std::cout << std::endl;

	int skupnoSteviloKart = 0;
	for (int i = 0; i < seznamKart.size(); i++) {
		int steviloKart = seznamKart[i].dobiKolicina();
		int steviloZadetkov = seznamKart[i].dobicekNovihKart();
		for (int j = i + 1; j <= i + steviloZadetkov; j++)
			seznamKart[j].povecajKolicino(steviloKart);
		skupnoSteviloKart += steviloKart;
		//std::cout << "Karta " << seznamKart[i].dobiID() << " ima " << steviloKart << " kart.\n";
	}
	std::cout << "Stevilo vseh kart je " << skupnoSteviloKart << '.';
	std::cout << std::endl;


	return 0;
}