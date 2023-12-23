#include<iostream>
#include<ctime>
#include<cstdlib>
#define n 5
#define pi 3.1416

using namespace std;
void riempi_vett(int v[n])
{

	srand(time(0));
	for (int n1 = 0; n1 < n; n1++) {
		v[n1] = rand() % 10 + 1;
	}
}
void stampa_vett(int v[n]) {
	for (int n1 = 0; n1 < n; n1++) {
		cout << " " << v[n1];

	}
}

int massimo(int v[n]) {
	int max = 0;
	for (int n1 = 0; n1 < n; n++) {
		if (v[n] > max) {
			max = v[n];
		}
	}
	return max;
}

int main() {
	int v[n];
	riempi_vett(v);
	stampa_vett(v);
	cout << "il valore massimo è di:" << massimo(v);
}