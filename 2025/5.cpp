/*
	t = 24:21 min
	zelo lahko
	t = 23:40 min
	lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>


class Range {

public:
	long long spodnji, zgornji;

	Range() : spodnji(-1), zgornji(-1) {}
	Range(long long spodnjiID, long long zgornjiID) : spodnji(spodnjiID), zgornji(zgornjiID) {}

	bool VeljavenID(const long long& ID) const { return ID >= spodnji && ID <= zgornji; }
};

bool operator<(const Range& ra1, const Range& ra2) {
	if (ra1.spodnji != ra2.spodnji) return ra1.spodnji < ra2.spodnji;
	return ra1.zgornji < ra2.zgornji;
}
bool operator==(const Range& ra1, const Range& ra2) {
	return ra1.spodnji == ra2.spodnji && ra1.zgornji == ra2.zgornji;
}


std::pair<std::vector<Range>, std::vector<long long>> preberiPodatke(const std::string& pot) {

	std::vector<Range> resitev0;
	std::vector<long long> resitev1;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return { resitev0,resitev1 };
	}

	std::string vrstica;
	Range range;

	while (std::getline(podatki, vrstica)) {

		if (vrstica.empty())
			break;

		size_t mestoPreloma;

		range.spodnji = std::stoll(vrstica, &mestoPreloma);
		vrstica = vrstica.substr(mestoPreloma + 1);
		range.zgornji = std::stoll(vrstica);

		resitev0.push_back(range);
	}

	long long vrednost;

	while (podatki >> vrednost) {
		resitev1.push_back(vrednost);
	}

	podatki.close();

	return { resitev0,resitev1 };
}


int najdiSteviloSvezih(const std::vector<Range>& seznamRangov, const std::vector<long long>& seznamID) {

	int resitev = 0;

	for (const long long& ID : seznamID) {
		for (const Range& range : seznamRangov) {
			if (range.VeljavenID(ID)) {
				resitev++;
				break;
			}
		}
	}

	return resitev;
}


std::vector<Range> urediSeznamRangov(std::vector<Range> seznamRangov) {

	std::sort(seznamRangov.begin(), seznamRangov.end());

	std::vector<Range> urejenSeznamRangov;
	urejenSeznamRangov.push_back(seznamRangov.front());

	for (auto it = seznamRangov.begin() + 1; it != seznamRangov.end(); it++) {

		if (it->spodnji <= urejenSeznamRangov.back().zgornji) {
			
			if (it->zgornji <= urejenSeznamRangov.back().zgornji)
				continue;

			urejenSeznamRangov.back().zgornji = it->zgornji;
		}
		else {
			urejenSeznamRangov.push_back(*it);
		}
	}

	return urejenSeznamRangov;
}

long long najdiSteviloMoznihSvezih(const std::vector<Range>& seznamRangov) {

	long long resitev = 0;

	std::vector<Range> urejenSeznamRangov = urediSeznamRangov(seznamRangov);

	for (const Range& range : urejenSeznamRangov) {
		resitev += 1 + range.zgornji - range.spodnji;
	}

	return resitev;
}


int main() {

	std::pair<std::vector<Range>, std::vector<long long>> bazaPodatkov = preberiPodatke("2025/5.txt");
	std::vector<Range>& seznamRangov = bazaPodatkov.first;
	std::vector<long long>& seznamID = bazaPodatkov.second;

	int resitev0 = najdiSteviloSvezih(seznamRangov, seznamID);
	std::cout << "Stevilo svezih je " << resitev0 << ".\n";

	long long resitev1 = najdiSteviloMoznihSvezih(seznamRangov);
	std::cout << "Stevilo vseh svezih je " << resitev1 << ".\n";


	return 0;
}
