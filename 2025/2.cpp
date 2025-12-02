/*
	t = 26:39 min
	nekoliko lahko
	t ~ 22 min
	nekoliko lahko
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>


struct Range {

	long long zacetek;
	long long konec;

	Range() : zacetek(0), konec(0) {}
	Range(long long zacetek, long long konec) : zacetek(zacetek), konec(konec) {}
};

std::ostream& operator<<(std::ostream& os, const Range& ra) {
	os << ra.zacetek;
	os << '-';
	os << ra.konec;
	return os;
}
std::istream& operator>>(std::istream& is, Range& ra) {
	is >> ra.zacetek;
	is.ignore(1);
	is >> ra.konec;
	return is;
}


bool invalidativen1(const std::string& vrednost) {

	if (vrednost.size() % 2 == 1)
		return false;

	return vrednost.substr(0, vrednost.size() / 2) == vrednost.substr(vrednost.size() / 2, vrednost.size());
}
bool invalidativen2(const std::string& vrednost) {

	std::vector<short> deljitelji;

	for (int i = 1; i <= vrednost.size() / 2; i++)
		if (vrednost.size() % i == 0)
			deljitelji.push_back(i);

	for (const short& del : deljitelji) {

		const std::string& vzorec = vrednost.substr(0, del);

		bool napacen = true;

		for (int i = 1; i < vrednost.size() / del; i++) {
			if (vrednost.substr(i * del, del) != vzorec)
				napacen = false;
		}

		if (napacen)
			return true;
	}

	return false;
}

std::pair<long long, long long> preberiPodatke(const std::string& pot) {

	std::pair<long long, long long> resitev = { 0,0 };

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	Range range;

	while (podatki.peek() != EOF) {

		podatki >> range;
		podatki.ignore(1);

		for (long long i = range.zacetek; i <= range.konec; i++) {
			if (invalidativen1(std::to_string(i))) {
				resitev.first += i;
				resitev.second+= i;
				//std::cout << "1: " << i << '\n';
			}
			else if (invalidativen2(std::to_string(i))) {
				resitev.second += i;
				//std::cout << "2: " << i << '\n';
			}
		}
	}

	podatki.close();

	return resitev;
}


int main() {

	std::cout << std::boolalpha;
	
	std::pair<long long, long long> resitev = preberiPodatke("2025/2.txt");
	
	std::cout << "Resitev1: " << resitev.first << ".\n";
	std::cout << "Resitev2: " << resitev.second << ".\n";


	return 0;
}
