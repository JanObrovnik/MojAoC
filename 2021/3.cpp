/*
	t = 31:00
	nekoliko lahko
	t = ?
	nekoliko lahko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>


std::ostream& operator<<(std::ostream& os, const std::vector<short>& sz) {
	for (const short& i : sz) os << i << ' ';
	return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& sz) {
	for (const std::string& str : sz) os << str << ' ';
	return os;
}


std::multiset<int> preberiPodatke1(const std::string& pot, int& velikostPodatkov) {

	std::multiset<int> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	int x = 0, y = 0;
	char vrednost;
	std::string vrstica;

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);

		std::stringstream ss(vrstica);
		
		while (ss.peek() != EOF) {

			ss >> vrednost;
			
			if (vrednost == '1') {
				resitev.insert(x);
				//std::cout << x << ' ';
			}

			x++;
		}
		//std::cout << vrstica << '\n';
		
		x = 0;
		y++;
	}

	podatki.close();

	velikostPodatkov = y;
	return resitev;
}


std::vector<short> preberiPodatke2(const std::string& pot, std::vector<std::string>& seznamBinarnihStevil) {

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return {};
	}

	std::string vrstica;

	std::getline(podatki, vrstica);
	std::vector<short> resitev(vrstica.size(), 0);

	seznamBinarnihStevil.push_back(vrstica);

	for (int i = 0; i < vrstica.size(); i++) {
		if (vrstica[i] == '1') resitev[i]++;
		else resitev[i]--;
	}

	while (podatki.peek() != EOF) {

		std::getline(podatki, vrstica);

		seznamBinarnihStevil.push_back(vrstica);

		for (int i = 0; i < vrstica.size(); i++) {
			if (vrstica[i] == '1') resitev[i]++;
			else resitev[i]--;
		}
	}
	
	podatki.close();

	return resitev;
}


std::string filter(std::vector<std::string> seznamBinarnihStevil, const std::string& rat) {

	for (int i = 0; i < seznamBinarnihStevil.front().size(); i++) {

		//std::cout << seznamBinarnihStevil << '\n';

		short st1 = 0, st0 = 0;
		
		for (const std::string& bin : seznamBinarnihStevil) {
			if (bin[i] == '1') st1++;
			else st0++;
		}
		char vecina = (st1 >= st0) ? '1' : '0';

		std::vector<std::string>::iterator it;

		if (rat == "O2")
			it = std::remove_if(seznamBinarnihStevil.begin(), seznamBinarnihStevil.end(), [&](const std::string& bin) {return vecina == bin[i]; });
		else
			it = std::remove_if(seznamBinarnihStevil.begin(), seznamBinarnihStevil.end(), [&](const std::string& bin) {return vecina != bin[i]; });

		seznamBinarnihStevil.erase(it, seznamBinarnihStevil.end());

		if (seznamBinarnihStevil.size() == 1)
			return seznamBinarnihStevil.front();
	}

	return "Ni najdene koncne stevilke";
}


int main() {

	/// Del 1
	int velikostPodatkov = 0;

	std::multiset<int> podatki(preberiPodatke1("2021/3.txt", velikostPodatkov));

	std::string gamaStr, epsilonStr;

	int dolzina = *podatki.rbegin();

	for (int i = 0; i <= dolzina; i++) {

		int st = podatki.count(i);
		//std::cout << i << ' ';

		if (st > velikostPodatkov - st) {
			gamaStr.push_back('1');
			epsilonStr.push_back('0');
		}
		else {
			gamaStr.push_back('0');
			epsilonStr.push_back('1');
		}
	}
	std::cout << gamaStr << '|' << epsilonStr << '\n';

	int gama = std::stoi(gamaStr, nullptr, 2);
	int epsilon = std::stoi(epsilonStr, nullptr, 2);

	std::cout << "Vrednost je " << gama * epsilon << ".\n";


	/// Del 2
	std::vector<std::string> seznamBinarnihStevil;

	std::vector<short> binarnaVsota(preberiPodatke2("2021/3.txt", seznamBinarnihStevil));

	std::string O2Generator = filter(seznamBinarnihStevil, "O2");
	std::string CO2Cistilec = filter(seznamBinarnihStevil, "CO2");

	std::cout << '\n' << O2Generator << '|' << CO2Cistilec << '\n';

	int O2St = std::stoi(O2Generator, nullptr, 2);
	int CO2St = std::stoi(CO2Cistilec, nullptr, 2);

	std::cout << "Vrednost je " << O2St * CO2St << ".\n";


	return 0;
}