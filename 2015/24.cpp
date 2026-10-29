/*
	t = 3:15:36 ur
	nekoliko lahko
	t ~ 25 min
	zelo lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>


std::vector<int> preberiPodatke(const std::string& pot) {

	std::vector<int> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	int i;

	while (podatki.peek() != EOF) {
		podatki >> i;
		resitev.push_back(i);
	}

	podatki.close();

	return resitev;
}


unsigned long long izracunajQE(const std::vector<int>& paketUtezi) {
	return std::accumulate(paketUtezi.begin(), paketUtezi.end(), (unsigned long long)1, [](unsigned long long i, unsigned long long j) {return i * j; });
}


void forFunkcija(const std::vector<int>& seznamUtezi, const int& minStevilo, const int& ciljPaketa, int j, std::vector<std::vector<int>>& seznamUporabljenihUtezi, std::vector<int> trenutneUtezi = {}) {

	//for (int i : trenutneUtezi) std::cout << i << ' '; std::cout << "| " << j << '\n';
	//std::cout << trenutneUtezi.size() << '\n';

	for (int i = j; i < seznamUtezi.size(); i++) {
		
		trenutneUtezi.push_back(seznamUtezi[i]);
		
		if (trenutneUtezi.size() == minStevilo) {
			
			//for (int& i : trenutneUtezi) std::cout << i << ' ';
			//std::cout << "| " << std::accumulate(trenutneUtezi.begin(), trenutneUtezi.end(), 0) << '\n';

			if (std::accumulate(trenutneUtezi.begin(), trenutneUtezi.end(), 0) == ciljPaketa) {
				seznamUporabljenihUtezi.push_back(trenutneUtezi);
			}
		}
		
		else if (trenutneUtezi.size() < minStevilo)
			forFunkcija(seznamUtezi, minStevilo, ciljPaketa, i + 1, seznamUporabljenihUtezi, trenutneUtezi);

		trenutneUtezi.pop_back();
	}
}

unsigned long long odcitajQE(const std::vector<int>& seznamUtezi) {

	int vsota = std::accumulate(seznamUtezi.begin(), seznamUtezi.end(), 0);
	int ciljPaketa = vsota / 3;
	int minStevilo = 0;

	int tekocaVsota = 0;
	int index = seznamUtezi.size() - 1;
	while (tekocaVsota < ciljPaketa) {
		tekocaVsota += seznamUtezi[index];
		index--;
		minStevilo++;
	}
	//std::cout << vsota << ' ' << ciljPaketa << '\n';


	std::vector<std::vector<int>> seznamUporabljenihUtezi;
	bool obstajaKombinacija = false;

	while (!obstajaKombinacija) {

		seznamUporabljenihUtezi.clear();

		while (seznamUporabljenihUtezi.empty() && minStevilo < seznamUtezi.size() / 3 + 1) {

			forFunkcija(seznamUtezi, minStevilo, ciljPaketa, 0, seznamUporabljenihUtezi);

			if (seznamUporabljenihUtezi.empty())
				minStevilo++;
		}

		if (seznamUporabljenihUtezi.empty())
			return -2;


		obstajaKombinacija = false;

		for (const std::vector<int>& tekocSeznamUtezi : seznamUporabljenihUtezi) {

			std::vector<int> novSeznamUtezi = seznamUtezi;

			auto it = std::remove_if(novSeznamUtezi.begin(), novSeznamUtezi.end(), [&](int i) {
				return std::find(tekocSeznamUtezi.begin(), tekocSeznamUtezi.end(), i) != tekocSeznamUtezi.end();
				});

			novSeznamUtezi.erase(it, novSeznamUtezi.end());

			int novoMinStevilo = minStevilo;
			std::vector<std::vector<int>> seznamUporabljenihUtezi;
			while (seznamUporabljenihUtezi.empty() && novoMinStevilo < seznamUtezi.size() / 2 + 1) {
				forFunkcija(novSeznamUtezi, novoMinStevilo, ciljPaketa, 0, seznamUporabljenihUtezi);
				if (seznamUporabljenihUtezi.empty())
					novoMinStevilo++;
			}


			if (!seznamUporabljenihUtezi.empty()) {
				obstajaKombinacija = true;
				break;
			}
		}

		if (!obstajaKombinacija) {
			minStevilo++;
		}
	}
	//std::cout << seznamUporabljenihUtezi.size() << '\n' << '\n';
	//for (std::vector<int> vec : seznamUporabljenihUtezi) {
	//	for (int i : vec) {
	//		std::cout << i << ' ';
	//	} std::cout << '\n';
	//} std::cout << '\n';

	if (seznamUporabljenihUtezi.size() == 1) {
		return izracunajQE(seznamUporabljenihUtezi.front());
	}
	else if (seznamUporabljenihUtezi.size() > 1) {
		auto it = std::min_element(seznamUporabljenihUtezi.begin(), seznamUporabljenihUtezi.end(), [](const std::vector<int>& vec1, const std::vector<int>& vec2) {
			return izracunajQE(vec1) < izracunajQE(vec2);
			});
		return izracunajQE(*it);
	}
	return -1;
}
unsigned long long odcitajQE2(const std::vector<int>& seznamUtezi, const int& steviloZalogovnikov) {

	int vsota = std::accumulate(seznamUtezi.begin(), seznamUtezi.end(), 0);
	int ciljPaketa = vsota / steviloZalogovnikov;
	int minStevilo = 0;

	int tekocaVsota = 0;
	int index = seznamUtezi.size() - 1;
	while (tekocaVsota < ciljPaketa) {
		tekocaVsota += seznamUtezi[index];
		index--;
		minStevilo++;
	}


	std::vector<std::vector<int>> seznamUporabljenihUtezi;
	bool obstajaKombinacija = false;

	while (!obstajaKombinacija) {

		seznamUporabljenihUtezi.clear();

		while (seznamUporabljenihUtezi.empty() && minStevilo < seznamUtezi.size() / steviloZalogovnikov + 1) {

			forFunkcija(seznamUtezi, minStevilo, ciljPaketa, 0, seznamUporabljenihUtezi);

			if (seznamUporabljenihUtezi.empty())
				minStevilo++;
		}

		if (seznamUporabljenihUtezi.empty())
			return -1;


		obstajaKombinacija = false;

		for (const std::vector<int>& tekocSeznamUtezi : seznamUporabljenihUtezi) {

			std::vector<int> novSeznamUtezi = seznamUtezi;

			auto it = std::remove_if(novSeznamUtezi.begin(), novSeznamUtezi.end(), [&](int i) {
				return std::find(tekocSeznamUtezi.begin(), tekocSeznamUtezi.end(), i) != tekocSeznamUtezi.end();
				});

			novSeznamUtezi.erase(it, novSeznamUtezi.end());


			unsigned long long veljavno = 0;
			
			if (steviloZalogovnikov > 2)
				veljavno = odcitajQE2(novSeznamUtezi, steviloZalogovnikov - 1);


			if (veljavno != -1) {
				obstajaKombinacija = true;
				break;
			}
		}

		if (!obstajaKombinacija) {
			minStevilo++;
		}
	}
	//std::cout << seznamUporabljenihUtezi.size() << '\n' << '\n';
	//for (std::vector<int> vec : seznamUporabljenihUtezi) {
	//	for (int i : vec) {
	//		std::cout << i << ' ';
	//	} std::cout << '\n';
	//} std::cout << '\n';

	if (seznamUporabljenihUtezi.size() == 1) {
		return izracunajQE(seznamUporabljenihUtezi.front());
	}
	else if (seznamUporabljenihUtezi.size() > 1) {
		auto it = std::min_element(seznamUporabljenihUtezi.begin(), seznamUporabljenihUtezi.end(), [](const std::vector<int>& vec1, const std::vector<int>& vec2) {
			return izracunajQE(vec1) < izracunajQE(vec2);
			});
		return izracunajQE(*it);
	}
	return -1;
}


int main() {

	std::vector<int> seznamUtezi = preberiPodatke("2015/24.txt");


	unsigned long long resitev1 = odcitajQE2(seznamUtezi, 3);
	std::cout << "\"quantum entanglement\" najbolj optimalne postavitve je " << resitev1 << ".\n";

	unsigned long long resitev2 = odcitajQE2(seznamUtezi, 4);
	std::cout << "\"quantum entanglement\" najbolj optimalne postavitve po popravkih je " << resitev2 << ".\n";


	return 0;
}
