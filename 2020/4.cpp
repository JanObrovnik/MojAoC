/*
	t = 34:51 min
	nekoliko lahko
	t = 48:52 min
	enostavno ampak dolgocasno
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


struct PotniList {
	// bolje bi blo z std::map<std::string,std::string>

	std::string rojstnoLeto = "";
	std::string izvrsilnoLeto = "";
	std::string pretecloLeto = "";
	std::string visina = "";
	std::string lasjeBarva = "";
	std::string ociBarva = "";
	std::string potniListID = "";
	std::string drzavniID = "";

	void pocisti() {
		rojstnoLeto.clear();
		izvrsilnoLeto.clear();
		pretecloLeto.clear();;
		visina.clear();
		lasjeBarva.clear();
		ociBarva.clear();
		potniListID.clear();
		drzavniID.clear();
	}

	void dodaj(const std::string& informacija) {
		
		const std::string& infoKljuc = informacija.substr(0, 3);
		const std::string& infoVrednost = informacija.substr(4, informacija.size());
		
		if (infoKljuc == "byr") rojstnoLeto = infoVrednost;
		else if (infoKljuc == "iyr") izvrsilnoLeto = infoVrednost;
		else if (infoKljuc == "eyr") pretecloLeto = infoVrednost;
		else if (infoKljuc == "hgt") visina = infoVrednost;
		else if (infoKljuc == "hcl") lasjeBarva = infoVrednost;
		else if (infoKljuc == "ecl") ociBarva = infoVrednost;
		else if (infoKljuc == "pid") potniListID = infoVrednost;
		else if (infoKljuc == "cid") drzavniID = infoVrednost;
	}

	bool veljaven(const bool& strogaVeljavno = false) const {
		
		bool enostavnaVeljavnost = !(rojstnoLeto.empty() || izvrsilnoLeto.empty() || pretecloLeto.empty() || visina.empty() ||
									lasjeBarva.empty() || ociBarva.empty() || potniListID.empty());

		if (enostavnaVeljavnost == false) return false;
		if (strogaVeljavno == false) return true;


		if ((std::stoi(rojstnoLeto) >= 1920 && std::stoi(rojstnoLeto) <= 2002) == false) return false;
		
		if ((std::stoi(izvrsilnoLeto) >= 2010 && std::stoi(izvrsilnoLeto) <= 2020) == false) return false;
		
		if ((std::stoi(pretecloLeto) >= 2020 && std::stoi(pretecloLeto) <= 2030) == false) return false;

		if (visina.substr(visina.size() - 2, visina.size()) == "cm") {
			if (((std::stoi(visina.substr(0, visina.size() - 2)) >= 150) && (std::stoi(visina.substr(0, visina.size() - 2)) <= 193)) == false)
				return false;
		}
		else if (visina.substr(visina.size() - 2, visina.size()) == "in") {
			if (((std::stoi(visina.substr(0, visina.size() - 2)) >= 59) && (std::stoi(visina.substr(0, visina.size() - 2)) <= 76)) == false)
				return false;
		}
		else return false;

		if (lasjeBarva.size() != 7) return false;
		for (int i = 0; i < lasjeBarva.size(); i++) {
			if (i == 0) {
				if (lasjeBarva[i] != '#') return false;
			}
			else {
				if (!((lasjeBarva[i] >= '0' && lasjeBarva[i] <= '9') || (lasjeBarva[i] >= 'a' && lasjeBarva[i] <= 'f'))) return false;
			}
		}

		if (!(ociBarva == "amb" || ociBarva == "blu" || ociBarva == "brn" || ociBarva == "gry" || ociBarva == "grn" || ociBarva == "hzl" || ociBarva == "oth")) return false;

		if (potniListID.size() != 9) return false;
		for (const char& c : potniListID)
			if (c < '0' || c > '9') return false;


		return true;
	}
};


std::pair<int, int>  preberiPodatke(const std::string pot) {

	std::pair<int, int> resitev = { 0,0 };

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;
	std::string informacija;
	PotniList potniList;

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);

		if (vrstica.empty()) {
			resitev.first += potniList.veljaven(false);
			resitev.second += potniList.veljaven(true);
			potniList.pocisti();
			continue;
		}

		std::stringstream ss(vrstica);

		while (ss >> informacija)
			potniList.dodaj(informacija);
	}
	resitev.first += potniList.veljaven(false);
	resitev.second += potniList.veljaven(true);
	potniList.pocisti();

	podatki.close();

	return resitev;
}


int main() {

	std::pair<int, int> resitev = preberiPodatke("2020/4.txt");

	std::cout << "Stevilo \"veljavnih\" potnih listov je " << resitev.first << ".\n";
	std::cout << "Stevilo \"bolj veljavnih\" potnih listov je " << resitev.second << ".\n";


	return 0;
}
