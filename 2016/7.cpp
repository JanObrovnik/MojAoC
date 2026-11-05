/*
	t = 42:49 min
	precej lahko
	t = 20:14 min
	precej lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


bool veljavenStrABBA(const std::string& str) {
	for (int i = 0; i < str.size() - 3; i++)
		if ((str[i] != str[i + 1]) && str[i] == str[i + 3] && str[i + 1] == str[i + 2])
			return true;
	return false;
}
bool veljavenVecStrABBA(const std::vector<std::string>& vec) {
	for (const std::string& str : vec)
		if (veljavenStrABBA(str))
			return true;
	return false;
}

bool veljavenStrABA(const std::string& ABA, const std::vector<std::string>& izven) {
	for (const std::string& strIzven : izven)
		for (int i = 0; i < strIzven.size() - 2; i++)
			if (strIzven[i] == ABA[1] && strIzven[i + 1] == ABA[0] && strIzven[i + 2] == ABA[1])
				return true;
	return false;
}
bool veljavenVecStrABA(const std::vector<std::string>& znotraj, const std::vector<std::string>& izven) {
	for (const std::string& strZnotraj : znotraj)
		for (int i = 0; i < strZnotraj.size() - 2; i++)
			if ((strZnotraj[i] != strZnotraj[i + 1]) && (strZnotraj[i] == strZnotraj[i + 2]))
				if (veljavenStrABA(std::string(strZnotraj.begin() + i, strZnotraj.begin() + i + 3), izven))
					return true;
	return false;
}

class Vrstica {
public:
	
	std::vector<std::string> izven, znotraj;

	Vrstica() : izven({}), znotraj({}) {}
	Vrstica(std::string celotnaVrstica) {
		
		std::string str;

		for (const char& c : celotnaVrstica) {

			if (c == '[') {
				izven.push_back(str);
				str.clear();
				continue;
			}
			if (c == ']') {
				znotraj.push_back(str);
				str.clear();
				continue;
			}

			str.push_back(c);
		}

		izven.push_back(str);
	}

	bool pravilnaABBA() const {
		return !veljavenVecStrABBA(znotraj) && veljavenVecStrABBA(izven);
	}
	bool pravilnaABA() const {
		return veljavenVecStrABA(znotraj, izven);
	}
};

std::ostream& operator<<(std::ostream& os, const Vrstica& vrstica) {
	for (const std::string& str : vrstica.izven) os << str << ',';
	os << ' ' << '[';
	for (const std::string& str : vrstica.znotraj) os << str << ',';
	os << ']';
	return os;
}


std::vector<Vrstica> preberiPodatke(const std::string& pot) {

	std::vector<Vrstica> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	std::string vrstica;

	while (podatki.peek() != EOF) {
		
		podatki >> vrstica;

		resitev.push_back(Vrstica(vrstica));
	}

	podatki.close();

	return resitev;
}


int preveriPravilnostABBA(const std::vector<Vrstica>& seznamVrstic) {

	int resitev = 0;

	for (const Vrstica& vrstica : seznamVrstic) {
		resitev += vrstica.pravilnaABBA();
		//if (vrstica.pravilnaABBA()) std::cout << vrstica << '\n';
	}

	return resitev;
}
int preveriPravilnostABA(const std::vector<Vrstica>& seznamVrstic) {

	int resitev = 0;

	for (const Vrstica& vrstica : seznamVrstic) {
		resitev += vrstica.pravilnaABA();
		//if (vrstica.pravilnaABA()) std::cout << vrstica << '\n';
	}

	return resitev;
}


int main() {

	std::vector<Vrstica> seznamVrstic = preberiPodatke("2016/7.txt");


	int resitev1 = preveriPravilnostABBA(seznamVrstic);
	std::cout << "V seznamu je " << resitev1 << " zmoznih ABBA IP-jev.\n";

	int resitev2 = preveriPravilnostABA(seznamVrstic);
	std::cout << "V seznamu je " << resitev2 << " zmoznih ABA IP-jev.\n";


	return 0;
}
