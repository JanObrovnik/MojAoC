/*
	t = 1:33:58 h
	nekoliko tezko
	t = 38:42 min
	srednja tezavnost
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>


class Torba {
	std::string barva;
	std::vector<std::pair<int, std::string>> drugeTorbe;

public:
	Torba() : barva(), drugeTorbe({}) {}
	Torba(std::string barva) : barva(barva), drugeTorbe({}) {}
	Torba(std::string barva, std::vector<std::pair<int, std::string>>) : barva(barva), drugeTorbe(drugeTorbe) {}

	void dodajTorbo(const std::pair<int, std::string>& torba) {
		drugeTorbe.push_back(torba);
	}

	void print() const {
		std::cout << barva << ": ";
		for (auto& pair : drugeTorbe)
			std::cout << pair.first << ' ' << pair.second << ", ";
		std::cout << '\n';
	}

	bool vsebujeTorbo(const std::string& torba) const {
		for (const auto& vsebuje : drugeTorbe)
			if (torba == vsebuje.second)
				return true;
		return false;
	}

	const std::string vrniBarvo() const {
		return barva;
	}

	const std::vector<std::pair<int, std::string>> vrniDrugeTorbe() const {
		return drugeTorbe;
	}
};


std::vector<Torba> preberiPodatke(const std::string& pot) {

	std::vector<Torba> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo ogoce odpreti.\n";
		return resitev;
	}

	std::string str1, str2, str3, str4;

	while (podatki.peek() != EOF) {

		podatki >> str1 >> str2 >> str3 >> str4;

		resitev.push_back(str1 + " " + str2);

		podatki >> str1;
		if (str1 == "no") {
			podatki >> str2 >> str3;
			continue;
		}

		ZANKA:
		podatki >> str2 >> str3 >> str4;

		resitev.back().dodajTorbo({ std::stoi(str1), str2 + " " + str3 });

		if (str4.back() == ',') {
			podatki >> str1;
			goto ZANKA;
		}
	}

	podatki.close();

	return resitev;
}


int najdiVseTorbeKiVsebujejo(const std::string& barva, const std::vector<Torba>& bazaPodatkov) {

	std::vector<std::string> seznamVsebovanja;

	std::queue<std::string> cakalnaVrsta;
	cakalnaVrsta.push(barva);

	while (!cakalnaVrsta.empty()) {

		for (const auto& torba : bazaPodatkov)
			if (torba.vsebujeTorbo(cakalnaVrsta.front())) {
				
				bool skok = false;
				for (const std::string& zeObstaja : seznamVsebovanja)
					if (torba.vrniBarvo() == zeObstaja) {
						skok = true;
						break;
					}
				if (skok)
					continue;

				cakalnaVrsta.push(torba.vrniBarvo());
				seznamVsebovanja.push_back(torba.vrniBarvo());
			}

		cakalnaVrsta.pop();
	}

	return seznamVsebovanja.size();
}

int najdiSteviloTorbVTorbi(const std::string& barva, const std::vector<Torba>& bazaPodatkov) {

	std::vector<std::pair<int, std::string>> podredje{};

	std::queue<std::pair<int, std::string>> cakalnaVrsta;
	cakalnaVrsta.push({ 1,barva });

	while (!cakalnaVrsta.empty()) {

		for (const auto& torba : bazaPodatkov) {
			if (torba.vrniBarvo() == cakalnaVrsta.front().second) {
				for (const std::pair<int, std::string>& drugeTorbe : torba.vrniDrugeTorbe()) {
					cakalnaVrsta.push({ cakalnaVrsta.front().first * drugeTorbe.first, drugeTorbe.second });
					podredje.push_back({ cakalnaVrsta.front().first * drugeTorbe.first, drugeTorbe.second });
				}
				break;
			}
		}

		cakalnaVrsta.pop();
	}

	int resitev = 0;

	for (const auto& par : podredje) {
		//std::cout << par.first << ' ' << par.second << '\n';
		resitev += par.first;
	}

	return resitev;
}


int main() {

	std::vector<Torba> bazaPodatkov = preberiPodatke("2020/7.txt");
	//for (auto& torba : bazaPodatkov)
	//	torba.print();

	int resitev1 = najdiVseTorbeKiVsebujejo("shiny gold", bazaPodatkov);

	std::cout << "Stevilo torb, ki vsebuje 'shiny gold' je " << resitev1 << ".\n";


	int resitev2 = najdiSteviloTorbVTorbi("shiny gold", bazaPodatkov);

	std::cout << "Stevilo torb, ki so v 'shiny gold' torbi je " << resitev2 << ".\n";



	return 0;
}