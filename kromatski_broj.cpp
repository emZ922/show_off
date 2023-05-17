#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>
#include <list>
#include <algorithm>
using namespace std;
string *splitString(string str, int n_s)
{ // za podatke iz trece linije
	string *res = new string[n_s];
	for (int i = 0; i < n_s; i++)
	{
		*(res + i) = str.substr(0, str.find(' '));
		str = str.substr(str.find(' ') + 1, str.length() - str.find(' '));
	}
	return res;
}

void read_data(string ime_datoteke, int &n, int &n_s, string &strS)
{
	ifstream inputFile;
	inputFile.open(ime_datoteke);
	if (!inputFile)
	{
		cout << "Nevaljano ime datoteke!";
		exit(1);
	}

	string *lines = new string[3];
	string line;
	int i = 0;
	while (getline(inputFile, line))
	{
		*(lines + i) = line;
		i++;
	}
	n = stoi(*(lines));
	n_s = stoi(*(lines + 1));
	strS = *(lines + 2);
	delete[] lines;
	inputFile.close();
}
class graf
{
private:
	int n;			  // broj vrhova
	int n_S;		  // card(S)
	int *mSusjedstva; // matrica susjedstva
	int *bojeVrhova;  // polje-> indeks = oznaka vrha, vrijednost = boja
	int k;			  // kromatski broj
	int *vrhovi;	  // polje vrhova, potrebno za permutacije

public:
	~graf() {
		delete[] mSusjedstva;
		delete[] bojeVrhova;
		delete[] vrhovi;
	}
	graf(int n, int n_S, int *S)
	{

		this->n = n;
		this->n_S = n_S;
		mSusjedstva = new int[n * n];
		vrhovi = new int[n];
		bojeVrhova = new int[n];
		// popuniti matricu susjedtsva
		for (int i = 0; i < n; i++)
		{
			*(bojeVrhova + i) = 0;
			*(vrhovi + i) = 0;
			for (int j = 0; j < n; j++)
			{
				int razlika = abs(i - j);
				bool susjedi = false;
				for (int k = 0; k < n_S && !susjedi; k++)
				{
					if (*(S + k) == razlika)
					{
						susjedi = true;
					}
				}
				if (susjedi)
					*(mSusjedstva + n * i + j) = 1;
				else
					*(mSusjedstva + n * i + j) = 0;
			}
		}
	}

	bool valjanaBoja(int vrh, int boja)
	{

		for (int j = 0; j < n; j++)
		{
			if (*(mSusjedstva + n * vrh + j) == 1 && *(bojeVrhova + j) == boja)
			{				  // ako su susjedi isto bojani
				return false; // boja ne valja
			}
		}
		return true;
	}
	void print()
	{
		for (int i = 0; i < n; i++)
		{
			cout << *(bojeVrhova + i);
		}
	}
	bool k_obojiv(int vrh, int b0, int boje, int brojac)
	{

		if (vrh == n)
		{ // vrhove oznacujem od 0 do n-1

			return true;
			// gotovo
		}
		if (vrh == 0 && brojac != 0)
		{
			return false; // vratili smo se u prvi vrh
		}
		for (int i = b0; i <= boje; i++)
		{

			if (valjanaBoja(vrh, i))
			{							 // ako postoji odgovorajauce boja u vec koristenim
				*(bojeVrhova + vrh) = i; // oboji vrh tom bojom
				if (k_obojiv(vrh + 1, 1, boje, brojac + 1))
				{ // ispitaj treba li ostalim vrhovima vise boja
					return true;
				}
				// ako smo tu, neki v>vrh nije se uspio obojiti s bojama na raspolaganju
				*(bojeVrhova + vrh) = 0;						 // ponisti bojanje
				return (k_obojiv(vrh, i + 1, boje, brojac + 1)); // pokusaj obojiti trenutni vrh s visom bojom
			}
		}
		return false;
	}
};
int main()
{
	string ime_datoteke;

	cout << "Unesite ime datoteke: ";
	cin >> ime_datoteke;

	int n;
	int n_s;	 // n = broj vrhova, n_s = velicina skupa S
	string strS; // treca linija

	read_data(ime_datoteke, n, n_s, strS);

	string *str = new string[n_s];
	str = splitString(strS, n_s); // str je skup S kao polje stringova S

	int *S = new int[n_s]; // skup S u int polju

	for (int i = 0; i < n_s; i++) // popuniti polje S
	{
		*(S + i) = stoi(*(str + i));
	}

	graf g = graf(n, n_s, S);

	// kromatski broj je najmanji broj k za koji je graf k obojiv
	int k = 1;
	
	while (!g.k_obojiv(0, 1, k, 0))
	{
		k++;
	}
	// g.print();
	cout << "Kromatski broj zadanog grafa je " << k << endl; // kromatski broj = min broj koristenih boja
	delete[] str, S;
	

	return 0;
}
