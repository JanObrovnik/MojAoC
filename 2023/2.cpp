/*
	Precej enostavna
	Precej enostavna
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <algorithm>


const int RDECA_MAX = 12;
const int ZELENA_MAX = 13;
const int MODRA_MAX = 14;


class IgraKock {
private:
	int id = 0;
	std::list<int> seznamRdecih{ 0 };
	std::list<int> seznamZelenih{ 0 };
	std::list<int> seznamModrih{ 0 };

public:
	void dodajRdeco(int stevilo) { seznamRdecih.push_back(stevilo); }
	void dodajZeleno(int stevilo) { seznamZelenih.push_back(stevilo); }
	void dodajModro(int stevilo) { seznamModrih.push_back(stevilo); }
	void zabeleziID(int igraID) { id = igraID; }

	bool dobiVeljavnost() {
		return *std::max_element(seznamRdecih.begin(), seznamRdecih.end()) <= RDECA_MAX &&
			   *std::max_element(seznamZelenih.begin(), seznamZelenih.end()) <= ZELENA_MAX &&
			   *std::max_element(seznamModrih.begin(), seznamModrih.end()) <= MODRA_MAX;
	}
	int dobiMinKock() {
		return *std::max_element(seznamRdecih.begin(), seznamRdecih.end()) *
			   *std::max_element(seznamZelenih.begin(), seznamZelenih.end()) *
			   *std::max_element(seznamModrih.begin(), seznamModrih.end());
	}
	int dobiID() const { return id; }
};


int main() {

	std::list<IgraKock> seznamIger;

	std::ifstream podatki;
	podatki.open("PodatkiAdvent23-2.txt", std::ios::in);

	if (podatki.is_open()) {

		while (podatki.peek() != EOF) {

			IgraKock igra;

			std::string stavek = "";
			std::getline(podatki, stavek);
			std::stringstream ss(stavek);

			std::string str;
			int vrednost;
			char ch;

			ss >> str;
			ss >> vrednost;
			igra.zabeleziID(vrednost);
			ss >> ch;

			while (true) {

				ss >> vrednost;
				ss >> ch;
				ss >> str;

				if (ch == 'r') igra.dodajRdeco(vrednost);
				else if (ch == 'g') igra.dodajZeleno(vrednost);
				else if (ch == 'b') igra.dodajModro(vrednost);

				char c = str[str.size() - 1];
				if (!(c == ',' || c == ';')) break;
			}

			seznamIger.push_back(igra);
		}
	}
	else std::cout << "Datoteka ni najdena";


	int vsotaMoznihIger = 0;
	std::for_each(seznamIger.begin(), seznamIger.end(), [&vsotaMoznihIger](IgraKock igra) {
		std::cout << "Igra " << igra.dobiID() << " je ";
		if (igra.dobiVeljavnost()) {
			vsotaMoznihIger += igra.dobiID();
			std::cout << "mogoca\n";
		}
		else std::cout << "nemogoca\n";
		});
	std::cout << "\nVsota moznih iger je " << vsotaMoznihIger << '\n' << std::endl;



	int vsotaMinKockIger = 0;
	std::for_each(seznamIger.begin(), seznamIger.end(), [&vsotaMinKockIger](IgraKock igra) {
		std::cout << "Igra " << igra.dobiID() << " potrebuje " << igra.dobiMinKock() << '\n';
		vsotaMinKockIger += igra.dobiMinKock();
		});
	std::cout << "\nVsota vseh min kock je " << vsotaMinKockIger << '\n' << std::endl;

	return 0;
}
