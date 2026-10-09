/*
	t = 48:43 min
	precej lahko
	t ~ 1 min
	neverjetno lahko (enaka koda)
*/

#include <iostream>
#include <fstream>
#include <string>


std::string operator++(std::string& str) {
	int vel = str.size() - 1;
	str[vel]++;
	while (str[vel] > 'z') {
		str[vel] = 'a';
		vel--;
		if (vel < 0)
			return str;
		str[vel]++;
	}
	return str;
}


std::string preberiPodatke(const std::string& pot) {

	std::string resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	podatki >> resitev;

	podatki.close();

	return resitev;
}


bool pravilo1(const std::string& geslo) {

	int predhodnjaRazlika = 0;

	for (int i = 1; i < geslo.size(); i++) {
		
		int trenutnaRazlika = geslo[i] - geslo[i - 1];
		
		if (trenutnaRazlika == 1 && predhodnjaRazlika == 1)
			return true;
		
		predhodnjaRazlika = trenutnaRazlika;
	}

	return false;
}
bool pravilo2(const std::string& geslo) { // za optimizacijo se lahko implementira v inkrementiranju gesla

	for (const char& c : geslo) {
		if (c == 'i' || c == 'l' || c == 'o')
			return false;
	}

	return true;
}
bool pravilo3(const std::string& geslo) {

	int steviloPonovitev = 0;

	for (int i = 1; i < geslo.size(); i++) {
		if (geslo[i - 1] == geslo[i]) {
			steviloPonovitev++;
			i++;
		}
	}

	if (steviloPonovitev > 1)
		return true;
	else
		return false;
}

bool pravilnostGesla(const std::string& geslo) {
	//std::cout << pravilo1(geslo) << " | " << pravilo2(geslo) << " | " << pravilo3(geslo) << '\n';
	return pravilo1(geslo) && pravilo2(geslo) && pravilo3(geslo);
}

std::string ustvariNovoGesnlo(std::string staroGeslo) {

	std::string& novoGeslo = staroGeslo;
	++novoGeslo;

	while (!pravilnostGesla(novoGeslo))
		++novoGeslo;

	return novoGeslo;
}


int main() {

	std::string staroGeslo = preberiPodatke("2015/11.txt");

	std::string resitev1 = ustvariNovoGesnlo(staroGeslo);
	std::cout << "Novo geslo je \"" << resitev1 << "\".\n";

	std::string resitev2 = ustvariNovoGesnlo(resitev1);
	std::cout << "Novo novo geslo je \"" << resitev2 << "\".\n";


	return 0;
}
