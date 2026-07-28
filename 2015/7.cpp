/*
	t = 2:04:17 ur
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>


const std::vector<std::string> seznamOperatorjev {
	"NOT",
	"AND",
	"OR",
	"LSHIFT",
	"RSHIFT"
};


int identificirajUkaz(const std::vector<std::string>& vrstica) {

	for (int vrst = 0; vrst < vrstica.size(); vrst++)
		for (int oprt = 0; oprt < seznamOperatorjev.size(); oprt++)
			if (vrstica[vrst] == seznamOperatorjev[oprt])
				return vrst;

	return - 1;
}


class Ukaz {

public:

	std::vector<std::string> vhodi;
	std::vector<std::string> vhodiTip;
	std::string izhod;
	std::string ukaz;
	
	Ukaz(const std::vector<std::string>& vrstica) {

		izhod = vrstica.back();

		int index = identificirajUkaz(vrstica);

		if (index == -1) {
			ukaz = "NULL";
			vhodi.push_back(vrstica[0]);
		}
		else {
			ukaz = vrstica[index];
			if (index == 0) {
				vhodi.push_back(vrstica[1]);
			}
			else if (index == 1) {
				vhodi.push_back(vrstica[0]);
				vhodi.push_back(vrstica[2]);
			}
		}

		for (const std::string& vhod : vhodi) {
			
			bool samoStevke = true;
			
			for (const char& c : vhod) 
				if (c < '0' || c > '9') {
					samoStevke = false;
					break;
				}

			if (samoStevke)
				vhodiTip.push_back("short");
			else
				vhodiTip.push_back("string");
		}
	}


	bool znaniVhodi(const std::map<std::string, unsigned short>& mapa) const {
		
		for (int i = 0; i < vhodi.size(); i++)
			if (vhodiTip[i] == "string" && mapa.find(vhodi[i]) == mapa.end())
				return false;

		return true;
	}

	void izracunaj(std::map<std::string, unsigned short>& mapa) const {

		std::vector<unsigned short> vhodiVal;

		for (int i = 0; i < vhodi.size(); i++) {
			if (vhodiTip[i] == "short")
				vhodiVal.push_back(static_cast<unsigned short>(std::stoul(vhodi[i])));
			else if (vhodiTip[i] == "string")
				vhodiVal.push_back(mapa[vhodi[i]]);
		}


		if (ukaz == "NOT") {
			mapa[izhod] = ~vhodiVal.front();
		}
		else if (ukaz == "AND") {
			mapa[izhod] = vhodiVal.front() & vhodiVal.back();
		}
		else if (ukaz == "OR") {
			mapa[izhod] = vhodiVal.front() | vhodiVal.back();
		}
		else if (ukaz == "LSHIFT") {
			mapa[izhod] = vhodiVal.front() << vhodiVal.back();
		}
		else if (ukaz == "RSHIFT") {
			mapa[izhod] = vhodiVal.front() >> vhodiVal.back();
		}
		else {
			mapa[izhod] = vhodiVal.front();
		}
	}


	void izpisi() const {

		for (int i = 0; i < vhodi.size(); i++)
			std::cout << vhodi[i] << '(' << vhodiTip[i] << ") ";
		std::cout << "| ";
		std::cout << ukaz << " | ";
		std::cout << izhod;
	}
};


std::vector<Ukaz> preberiPodatke(const std::string& pot) {

	std::vector<Ukaz> resitev;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}


	std::vector<std::string> ukaz;
	std::string vrstica, znak;

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);
		std::stringstream ss(vrstica);

		while (ss.peek() != EOF) {
			ss >> znak;
			ukaz.push_back(znak);
		}

		resitev.push_back(Ukaz(ukaz));



		ukaz.clear();
	}


	podatki.close();

	return resitev;
}


void zacetniVpis(std::map<std::string, unsigned short>& mapa, std::vector<Ukaz>& seznamUkazov) {

	auto it = std::remove_if(seznamUkazov.begin(), seznamUkazov.end(), [&](const Ukaz& ukaz) {
		if (ukaz.ukaz == "NULL" && ukaz.vhodiTip.front() == "short") {
			mapa[ukaz.izhod] = static_cast<unsigned short>(std::stoul(ukaz.vhodi.front()));
			return true;
		}
		return false;
		});

	seznamUkazov.erase(it, seznamUkazov.end());
}

void iteriraj(std::map<std::string, unsigned short>& mapa, std::vector<Ukaz>& seznamUkazov) {

	auto it = std::remove_if(seznamUkazov.begin(), seznamUkazov.end(), [&](const Ukaz& ukaz) {
		if (ukaz.znaniVhodi(mapa)) {
			ukaz.izracunaj(mapa);
			return true;
		}
		return false;
		});

	seznamUkazov.erase(it, seznamUkazov.end());
}

void simuliraj(std::map<std::string, unsigned short>& mapa, std::vector<Ukaz> seznamUkazov) {

	zacetniVpis(mapa, seznamUkazov);

	while (!seznamUkazov.empty())
		iteriraj(mapa, seznamUkazov);
}


int main() {

	std::vector<Ukaz> seznamUkazov = preberiPodatke("2015/7.txt");

	std::map<std::string, unsigned short> mapa;


	simuliraj(mapa, seznamUkazov);


	std::cout << "Vrednost na zici \"a\" je " << mapa["a"] << ".\n";




	return 0;
}
