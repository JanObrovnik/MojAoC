/*
	t = 12:00 min
	zelo lahko
	t = 2:21:56 ur
	srednja tezavnost
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>


int sestej(std::istream& podatki) {

	int resitev = 0;

	char c;
	std::string stevilka;

	while (podatki.peek() != EOF) {

		podatki >> c;

		if ((c >= '0' && c <= '9') || c == '-') {
			stevilka.push_back(c);
		}
		else if (!stevilka.empty()) {
			resitev += std::stoi(stevilka);
			stevilka.clear();
		}
	}

	return resitev;
}


int preberiPodatke1(const std::string& pot) {

	int resitev = 0;
	
	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	resitev = sestej(podatki);

	podatki.close();

	return resitev;
}

int preberiPodatke2(const std::string& pot, const std::string& spreglej) {

	int resitev = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}


	char c;
	std::string stevilka;
	std::string barva = "xxx";
	int aktivnaRdeca = 0;
	std::string oklepaji;
	std::vector<int> vmesneVrednosti{0};


	while (podatki.peek() != EOF) {

		podatki >> c;

		if ((c >= '0' && c <= '9') || c == '-') {
			
			stevilka.push_back(c);
		}
		else if (c == '{' || c == '[') {
			
			oklepaji.push_back(c);
			
			if (c == '{') {
				vmesneVrednosti.push_back(0);
				if (aktivnaRdeca) {
					aktivnaRdeca++;
				}
			}
		}
		else if (c == '}' || c == ']') {
			
			oklepaji.pop_back();
			
			if (c == '}') {
				vmesneVrednosti[vmesneVrednosti.size() - 2] += vmesneVrednosti[vmesneVrednosti.size() - 1];
				vmesneVrednosti.pop_back();
				if (aktivnaRdeca) {
					aktivnaRdeca--;
				}
			}
		}
		else {
			
			barva.erase(barva.begin());
			barva.push_back(c);

			if (barva == "red" && !oklepaji.empty() && oklepaji.back() == '{' && !aktivnaRdeca) {
				aktivnaRdeca++;
				vmesneVrednosti.back() = 0;
			}

			if (!stevilka.empty()) {
				
				if (!aktivnaRdeca) {
					vmesneVrednosti.back() += std::stoi(stevilka);
				}

				//std::cout << stevilka << '|' << aktivnaRdeca << ": ";
				//for (int i : vmesneVrednosti) std::cout << i << ", ";
				//std::cout << '\n';

				//std::cout << aktivnaRdeca << "|";
				//for (int i = 0; i < std::count(oklepaji.begin(), oklepaji.end(), '{'); i++) std::cout << '.';
				//std::cout << stevilka << '\n';

				stevilka.clear();
			}
		}
	}

	resitev = vmesneVrednosti.front();


	podatki.close();

	return resitev;
}


int isciMestoPredhodnegaOklepaja(const std::string& str, int index) {

	int stOklepajev = 1;

	while (index >= 0 && stOklepajev != 0) {

		if (str[index] == ']' || str[index] == '}')
			stOklepajev++;
		else if (str[index] == '[' || str[index] == '{')
			stOklepajev--;

		index--;
	}

	return index + 1;
}
int isciMestoNaslednjegaOklepaja(const std::string& str, int index) {

	int stOklepajev = 1;

	while (index < str.size() && stOklepajev != 0) {

		if (str[index] == '{')
			stOklepajev++;
		else if (str[index] == '}')
			stOklepajev--;

		index++;
	}

	return index;
}

void okrajsajOklepaje(std::vector<std::pair<int, int>>& pariOklepajev) {

	for (int i = pariOklepajev.size() - 1; i >= 0; i--) {

		if ((i > 0 && pariOklepajev[i].second <= pariOklepajev[i - 1].second) || 
			(i < pariOklepajev.size() - 1 && pariOklepajev[i].first >= pariOklepajev[i + 1].first)) {
		
			pariOklepajev.erase(pariOklepajev.begin() + i);
		}
	}
}

int preberiPodatke3(const std::string& pot, const std::string& spreglej) {

	int resitev = 0;


	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vse;

	podatki >> vse;

	podatki.close();

	
	std::string barva = "xxx";
	std::vector<std::pair<int, int>> pariOklepajev;

	for (int i = 0; i < vse.size(); i++) {

		barva.erase(barva.begin());
		barva.push_back(vse[i]);

		if (barva == "red") {
			
			int oklepaj = isciMestoPredhodnegaOklepaja(vse, i);

			if (vse[oklepaj] == '[')
				continue;

			int zaklepaj = isciMestoNaslednjegaOklepaja(vse, i);

			pariOklepajev.push_back({ oklepaj, zaklepaj });
		}
	}


	okrajsajOklepaje(pariOklepajev);
	
	//std::cout << pariOklepajev.size() << '\n';
	//for (auto par : pariOklepajev) std::cout << par.first << " - " << par.second << '\n';
	//std::cout << '\n';
	
	auto it = std::remove_if(vse.begin(), vse.end(), [&](const char& c) {
		
		static int i = 0;

		bool zavrzen = false;

		for (const std::pair<int,int> par : pariOklepajev)
			if (i >= par.first && i < par.second)
				zavrzen = true;
		
		i++;

		return zavrzen;
		});

	vse.erase(it, vse.end());


	std::stringstream ss(vse);
	resitev = sestej(ss);
	

	return resitev;
}


int main() {

	int resitev1 = preberiPodatke1("2015/12.txt");
	std::cout << "Vsota vseh stevilk je " << resitev1 << ".\n";


	std::string spreglej = "red";

	//int resitev2 = preberiPodatke2("2015/12.txt", spreglej);
	//std::cout << "S spregledanjem \"" << spreglej << "\" je vsota vseh stevilk " << resitev2 << ".\n";

	int resitev3 = preberiPodatke3("2015/12.txt", spreglej);
	std::cout << "S spregledanjem \"" << spreglej << "\" je vsota vseh stevilk " << resitev3 << ".\n";


	return 0;
}
