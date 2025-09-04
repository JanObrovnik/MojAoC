/*
	t = 24:34 min
	neverjetno lahko
	t = 2:01:17 h
	srednje tezko
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <algorithm>


std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& ve) {
	for (const std::string& str : ve)
		os << str << ' ';
	return os;
}


short najdiUnikatne(const std::vector<std::string>& seznam) {

	short resitev = 0;

	for (const std::string& koda : seznam) {
		const short& velikost = koda.size();
		if (velikost == 2 || velikost == 3 || velikost == 4 || velikost == 7)
			resitev++;
	}

	return resitev;
}


char najdiRazliko(const std::string& str1, const std::string& str2) {

	if (str1.size() < str2.size())
		for (const char& c : str2)
			if (std::find(str1.begin(), str1.end(), c) == str1.end())
				return c;
	if (str1.size() > str2.size())
		for (const char& c : str1)
			if (std::find(str2.begin(), str2.end(), c) == str2.end())
				return c;
	return -1;
}
std::string najdiRazlikoStr(const std::string& str1, const std::string& str2) {
	
	std::string resitev;

	if (str1.size() < str2.size())
		for (const char& c : str2)
			if (std::find(str1.begin(), str1.end(), c) == str1.end())
				resitev.push_back(c);
	if (str1.size() > str2.size())
		for (const char& c : str1)
			if (std::find(str2.begin(), str2.end(), c) == str2.end())
				resitev.push_back(c);

	return resitev;
}

char najdiEnakost(const std::string& str1, const std::string& str2) {

	if (str1.size() < str2.size())
		for (const char& c : str2)
			if (std::find(str1.begin(), str1.end(), c) != str1.end())
				return c;
	if (str1.size() > str2.size())
		for (const char& c : str1)
			if (std::find(str2.begin(), str2.end(), c) != str2.end())
				return c;
	return -1;
}
std::string najdiEnakostStr(const std::string& str1, const std::string& str2) {

	std::string resitev;

	if (str1.size() < str2.size())
		for (const char& c : str2)
			if (std::find(str1.begin(), str1.end(), c) != str1.end())
				resitev.push_back(c);
	if (str1.size() > str2.size())
		for (const char& c : str1)
			if (std::find(str2.begin(), str2.end(), c) != str2.end())
				resitev.push_back(c);

	return resitev;
}


int identificirajStevilo(const std::vector<std::string>& stevilo, const std::map<char, char>& legenda) {

	std::string resitev;

	for (const std::string str : stevilo) {

		std::string preurejenoStevilo;

		for (const char& c : str)
			preurejenoStevilo.push_back(legenda.at(c));

		std::sort(preurejenoStevilo.begin(), preurejenoStevilo.end());

		if		(preurejenoStevilo == "abcefg")  resitev.push_back('0');
		else if (preurejenoStevilo == "cf")		 resitev.push_back('1');
		else if (preurejenoStevilo == "acdeg")   resitev.push_back('2');
		else if (preurejenoStevilo == "acdfg")   resitev.push_back('3');
		else if (preurejenoStevilo == "bcdf")    resitev.push_back('4');
		else if (preurejenoStevilo == "abdfg")   resitev.push_back('5');
		else if (preurejenoStevilo == "abdefg")  resitev.push_back('6');
		else if (preurejenoStevilo == "acf")     resitev.push_back('7');
		else if (preurejenoStevilo == "abcdefg") resitev.push_back('8');
		else if (preurejenoStevilo == "abcdfg")  resitev.push_back('9');
	}
	//std::cout << resitev << '\n';

	return std::stoi(resitev);
}

int identificiraj(std::vector<std::string> unikatniSeznam, const std::vector<std::string>& stevilo) {

	std::map<char, char> legenda;
	legenda['a'] = 'g';
	legenda['b'] = 'g';
	legenda['c'] = 'g';
	legenda['d'] = 'g';
	legenda['e'] = 'g';
	legenda['f'] = 'g';
	legenda['g'] = 'g';


	//std::cout << unikatniSeznam << '\n';
	std::sort(unikatniSeznam.begin(), unikatniSeznam.end(), [](const std::string& str1, const std::string& str2) {
		return str1.size() < str2.size();
		});
	for (std::string& str : unikatniSeznam)
		std::sort(str.begin(), str.end());
	//std::cout << unikatniSeznam << '\n';


	/// a:
	char cA = najdiRazliko(unikatniSeznam[0], unikatniSeznam[1]);
	legenda[cA] = 'a';

	/// c:
	std::string vmesnaKoda;
	vmesnaKoda.push_back(najdiRazliko(unikatniSeznam[9], unikatniSeznam[8]));
	vmesnaKoda.push_back(najdiRazliko(unikatniSeznam[9], unikatniSeznam[7]));
	vmesnaKoda.push_back(najdiRazliko(unikatniSeznam[9], unikatniSeznam[6]));
	char cC = najdiEnakost(vmesnaKoda, unikatniSeznam[0]);
	legenda[cC] = 'c';

	/// f:
	vmesnaKoda.clear();
	vmesnaKoda.push_back(cC);
	char cF = najdiRazliko(unikatniSeznam[0], vmesnaKoda);
	legenda[cF] = 'f';

	/// b,d,e:
	for (int i = 6; i < 9; i++) {
		vmesnaKoda = najdiEnakostStr(unikatniSeznam[2], unikatniSeznam[i]);
		/// b,d:
		if (vmesnaKoda.size() == 3) {
			if (najdiEnakostStr(unikatniSeznam[0], vmesnaKoda).size() == 2) {
				/// d:
				char cD = najdiRazliko(unikatniSeznam[i], unikatniSeznam[9]);
				legenda[cD] = 'd';
				/// b:
				char cB = najdiRazliko(vmesnaKoda, unikatniSeznam[0]);
				legenda[cB] = 'b';
			}
		}
		/// e:
		else if (vmesnaKoda.size() == 4) {
			char cE = najdiRazliko(unikatniSeznam[i], unikatniSeznam[9]);
			legenda[cE] = 'e';
		}
	}

	//for (auto it = legenda.begin(); it != legenda.end(); it++) {
	//	std::cout << it->first << " : " << it->second << '\n';
	//}
	//std::cout << '\n';


	return identificirajStevilo(stevilo, legenda);
}


std::pair<int,int> preberiPodatke(const std::string& pot) {

	int resitev1 = 0;
	int resitev2 = 0;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke ni bilo mogoce odpreti.\n";
		return { resitev1,resitev2 };
	}

	char c;
	std::string koda;

	while (podatki.peek() != EOF) {
		std::vector<std::string> seznamPrvi{};
		std::vector<std::string> seznamDrugi{};

		for (int i = 0; i < 10; i++) {
			podatki >> koda;
			seznamPrvi.push_back(koda);
		}
		podatki >> c;
		for (int i = 0; i < 4; i++) {
			podatki >> koda;
			seznamDrugi.push_back(koda);
		}

		resitev1 += najdiUnikatne(seznamDrugi);
		resitev2 += identificiraj(seznamPrvi, seznamDrugi);
	}

	podatki.close();

	return { resitev1,resitev2 };
}


int main() {
	
	std::pair<int, int> resitev(preberiPodatke("2021/8.txt"));

	std::cout << "Stevilo unikatnih stevil je " << resitev.first << ".\n";

	std::cout << "Vsota vseh stevil je " << resitev.second << ".\n";


	return 0;
}
