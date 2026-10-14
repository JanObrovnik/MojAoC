/*
	t = 48:51 min
	zelo lahko
	t = 4:32 min
	neverjetno lahko
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


std::vector<std::string> preberiPodatke(const std::string& pot) {

	std::vector<std::string> resitev;

	std::fstream podatki;
	podatki.open(pot, std::ios::in);

	if (!podatki.is_open()) {
		std::cout << "Datoteke \"" << pot << "\" ni bilo mogoce odpreti.\n";
		return resitev;
	}

	while (podatki.peek() != EOF) {
		resitev.push_back("");
		podatki >> resitev.back();
	}

	podatki.close();

	return resitev;
}


void printVecStr(const std::vector<std::string>& vecStr) {
	for (const std::string str : vecStr)
		std::cout << str << '\n';
	std::cout << '\n';
}


int steviloPrizganih(const std::vector<std::string>& monitor) {
	
	int resitev = 0;

	for (int y = 0; y < monitor.size(); y++)
		for (int x = 0; x < monitor[y].size(); x++) {
			resitev += monitor[y][x] == '#';
		}

	return resitev;
}

int simuliraj(const std::vector<std::string>& monitor, const int& steviloIteracij, int del = 1) {

	std::vector<std::string> monitor1 = monitor;
	std::vector<std::string> monitor2 = monitor;

	std::vector<std::string>* starMonitor = &monitor1;
	std::vector<std::string>* novMonitor = &monitor2;

	const size_t velikost = monitor.size();

	for (int i = 0; i < steviloIteracij; i++) {
		for (int y = 0; y < velikost; y++)
			for (int x = 0; x < velikost; x++) {

				int steviloPrizganihSosed = 0;

				for (int dy : {-1, 0, 1})
					for (int dx : {-1, 0, 1})
						if (dx != 0 || dy != 0) {

							int xx = x + dx;
							int yy = y + dy;

							if (yy == -1 || yy == velikost || xx == -1 || xx == velikost)
								continue;

							steviloPrizganihSosed += (*starMonitor)[yy][xx] == '#';
						}

				bool trenutnoPrizgana = (*starMonitor)[y][x] == '#';

				if (trenutnoPrizgana) {
					if (steviloPrizganihSosed == 2 || steviloPrizganihSosed == 3) {
						(*novMonitor)[y][x] = '#';
					}
					else {
						(*novMonitor)[y][x] = '.';
					}
				}
				else {
					if (steviloPrizganihSosed == 3) {
						(*novMonitor)[y][x] = '#';
					}
					else {
						(*novMonitor)[y][x] = '.';
					}
				}
			}

		if (del == 2) {
			(*novMonitor)[0][0] = '#';
			(*novMonitor)[velikost - 1][0] = '#';
			(*novMonitor)[0][velikost - 1] = '#';
			(*novMonitor)[velikost - 1][velikost - 1] = '#';
		}

		if (starMonitor == &monitor1) {
			starMonitor = &monitor2;
			novMonitor = &monitor1;
		}
		else {
			starMonitor = &monitor1;
			novMonitor = &monitor2;
		}
	}

	return steviloPrizganih(*starMonitor);
}


int main() {

	std::vector<std::string> monitor = preberiPodatke("2015/18.txt");


	int resitev1 = simuliraj(monitor, 100);
	std::cout << "Prizganih je " << resitev1 << " luci.\n";

	int resitev2 = simuliraj(monitor, 100, 2);
	std::cout << "Prizganih je " << resitev2 << " luci.\n";


	return 0;
}
