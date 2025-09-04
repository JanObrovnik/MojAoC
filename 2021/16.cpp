/*
	t = 1:12:34 h
	srednje tezko
	t = 4:43:19 h
	nekoliko tezko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <numeric>


enum class OP {
	SUM = 0,
	PROD = 1,
	MIN = 2,
	MAX = 3,
	ERROR = 4,
	MORE = 5,
	LESS = 6,
	EQ = 7
};


long long operiraj(const OP& op, const std::vector<long long>& seznamVrednosti) {

	long long resitev = 0;


	switch (op) {
	case OP::SUM:
		resitev = std::accumulate(seznamVrednosti.begin(), seznamVrednosti.end(), 0ll);
		break;

	case OP::PROD:
		resitev = std::accumulate(seznamVrednosti.begin(), seznamVrednosti.end(), 1ll, [](long long i1, long long i2) {return i1 * i2;});
		break;

	case OP::MIN:
		resitev = *std::min_element(seznamVrednosti.begin(), seznamVrednosti.end());
		break;

	case OP::MAX:
		resitev = *std::max_element(seznamVrednosti.begin(), seznamVrednosti.end());
		break;

	case OP::ERROR:
		std::cout << "PRVI\n";
		break;

	case OP::MORE:
		resitev = (seznamVrednosti.front() > seznamVrednosti.back());
		break;

	case OP::LESS:
		resitev = (seznamVrednosti.front() < seznamVrednosti.back());
		break;

	case OP::EQ:
		resitev = (seznamVrednosti.front() == seznamVrednosti.back());
		break;

	default:
		std::cout << "NAPAKA DEFAULT\n";
		break;
	}

	return resitev;
}


std::string preberiPodatke(const std::string& pot) {

	std::string resitev{};

	std::map<char, std::string> hex;
	hex['0'] = "0000"; hex['4'] = "0100"; hex['8'] = "1000"; hex['C'] = "1100";
	hex['1'] = "0001"; hex['5'] = "0101"; hex['9'] = "1001"; hex['D'] = "1101";
	hex['2'] = "0010"; hex['6'] = "0110"; hex['A'] = "1010"; hex['E'] = "1110";
	hex['3'] = "0011"; hex['7'] = "0111"; hex['B'] = "1011"; hex['F'] = "1111";

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return resitev;
	}

	char c;
	while (podatki >> c) 
		resitev += hex.at(c);

	podatki.close();

	return resitev;
}


long long resitev = 0;

void preberiPaket(std::string podatki) {

VRNITEV:

	long long verzija = std::stoll(podatki.substr(0, 3), nullptr, 2);
	int tip = std::stoll(podatki.substr(3, 3), nullptr, 2);
	//std::cout << verzija << '\n' << tip << '\n';

	podatki.erase(podatki.begin(), podatki.begin() + 6);

	resitev += verzija;


	if (tip == 4) {

		std::string vrednost{};
	
		std::string podStr = podatki.substr(0, 5);
		podatki.erase(podatki.begin(), podatki.begin() + 5);

		while (podStr[0] == '1') {

			vrednost += podStr.substr(1, 4);

			podStr = podatki.substr(0, 5);
			podatki.erase(podatki.begin(), podatki.begin() + 5);
		}

		vrednost += podStr.substr(1, 4);
	
		//mojOperator.dodajVrednost(std::stoll(vrednost, nullptr, 2));
		//std::cout << "=4 " << vrednost << ": " << std::stoll(vrednost, nullptr, 2) << '\n';
		//std::cout << podatki << "\n\n";
	}
	else {

		//mojOperator.dodajOperator(tip);
		//std::cout << "!=4 " << tip << '\n';

		bool dolzinskiTip = podatki.front() - '0';
		podatki.erase(podatki.begin());

		if (dolzinskiTip) { // 11

			long long steviloPodPaketov = std::stoll(podatki.substr(0, 11), nullptr, 2);
			podatki.erase(podatki.begin(), podatki.begin() + 11);

			preberiPaket(podatki);
			podatki.clear();
		}
		else { // 15

			long long dolzinaPodPaketa = std::stoll(podatki.substr(0, 15), nullptr, 2);
			podatki.erase(podatki.begin(), podatki.begin() + 15);

			//std::cout << dolzinaPodPaketa << '\n';

			preberiPaket(podatki.substr(0,dolzinaPodPaketa));
			podatki.erase(podatki.begin(), podatki.begin() + dolzinaPodPaketa);
		}

		//std::cout << dolzinskiTip << '\n';
	}

	if (std::any_of(podatki.begin(), podatki.end(), [](const char& c) {return c == '1';}))
		goto VRNITEV;
}

struct Delaj {
	void preberi1Paket11(std::string& podatki, long long steviloPodPaketov, std::string& strPodatkov) {

		for (int i = 0; i < steviloPodPaketov; i++) {

			preberi1Paket(podatki, strPodatkov);

		}
	}
	void preberi1Paket15(std::string& podatki, long long dolzinaPodPaketa, std::string& strPodatkov) {

		size_t zacetna = podatki.size();
		size_t naslednja = podatki.size();

		int pon = 0;

		while (zacetna - naslednja < dolzinaPodPaketa) {
			
			preberi1Paket(podatki, strPodatkov);

			naslednja = podatki.size();			
		}
	}

	void preberi1Paket(std::string& podatki, std::string& strPodatkov) {

		int verzija = std::stoi(podatki.substr(0, 3), nullptr, 2);
		int tip = std::stoi(podatki.substr(3, 3), nullptr, 2);

		podatki.erase(podatki.begin(), podatki.begin() + 6);


		if (tip == 4) {

			std::string vrednostStr{};

			std::string podStr = podatki.substr(0, 5);
			podatki.erase(podatki.begin(), podatki.begin() + 5);

			while (podStr[0] == '1') {

				vrednostStr += podStr.substr(1, 4);

				podStr = podatki.substr(0, 5);
				podatki.erase(podatki.begin(), podatki.begin() + 5);
			}

			vrednostStr += podStr.substr(1, 4);

			long long vrednost = std::stoll(vrednostStr, nullptr, 2);

			//std::cout << vrednost << ' ';
			strPodatkov += std::to_string(vrednost) + ' ';
		}
		else {

			bool dolzinskiTip = podatki.front() - '0';
			podatki.erase(podatki.begin());
			
			//std::cout << tip << '(';
			strPodatkov += std::to_string(tip) + '(';

			if (dolzinskiTip) { // 11

				long long steviloPodPaketov = std::stoll(podatki.substr(0, 11), nullptr, 2);
				podatki.erase(podatki.begin(), podatki.begin() + 11);

				preberi1Paket11(podatki, steviloPodPaketov, strPodatkov);
			}
			else { // 15

				long long dolzinaPodPaketa = std::stoll(podatki.substr(0, 15), nullptr, 2);
				podatki.erase(podatki.begin(), podatki.begin() + 15);

				preberi1Paket15(podatki, dolzinaPodPaketa, strPodatkov);
			}

			//std::cout << ')';
			strPodatkov += ')';

		}
	}
};


void test(std::string& strPodatkov) {

	//auto itKonec = std::make_reverse_iterator(std::find(strPodatkov.begin(), strPodatkov.end(), ')'));
	//auto itZacetek = std::find(itKonec, strPodatkov.rend(), '(');
	//
	//for (auto it = itKonec; it != itZacetek; it++)
	//	std::cout << *it;
	//std::cout << '\n';


	int konec = strPodatkov.find_first_of(')');
	int zacetek = strPodatkov.find_last_of('(', konec);
	OP operacija = (OP)std::stoi(strPodatkov.substr(zacetek - 1, 1));
	//std::cout << "op: " << (int)operacija << '\n';

	//for (int i = zacetek + 1; i < konec; i++)
	//	std::cout << strPodatkov[i];
	//std::cout << '\n' << strPodatkov[zacetek - 1] << '\n';


	std::string podStr = strPodatkov.substr(zacetek + 1, konec - zacetek - 2);

	std::vector<long long> seznamVrednosti;
	size_t pozicija;
	long long vrednost;

	while (!podStr.empty()) {
		seznamVrednosti.push_back(std::stoll(podStr, &pozicija, 10));
		podStr = podStr.substr(pozicija);
	}

	//for (const long long& i : seznamVrednosti)
	//	std::cout << i << '\n';
	//std::cout << '\n';


	std::string nadomestek = std::to_string(operiraj(operacija, seznamVrednosti)) + " ";

	//std::cout << "nad: " << nadomestek << '\n';


	strPodatkov.replace(zacetek - 1, konec - zacetek + 2, nadomestek);

	//std::cout << strPodatkov << '\n';
}


int main() {

	std::string podatki(preberiPodatke("2021/16.txt"));
	std::string kopija = podatki;
	//std::cout << podatki << '\n';


	preberiPaket(podatki);

	std::cout << "Vsota vseh verzij je " << resitev << ".\n";


	std::string strPodatkov;

	Delaj delaj;
	delaj.preberi1Paket(podatki, strPodatkov);

	while (strPodatkov.find('(') != std::string::npos) {
		test(strPodatkov);
	}

	std::cout << "Operacije vrnejo resitev " << strPodatkov << ".\n";





	return 0;
}
