#include <iostream>
#include<fstream>
#include <string>

using namespace std;

class Faktura
{
	string imie;
	string nazwisko;
	string rejestracja_samochodu;
	string nr_tel;
	string opis_uslugi;
	string cena_uslugi;
	int nr_faktury;
	string termin_platnosci;
public:
	Faktura() = default;
	friend class Baza_danych;
	friend class Interfejs;
};

class Klient
{
	string imie;
	string nazwisko;
	string rejestracja_samochodu;
	string nr_tel;
public:
	Klient(string i, string n, string m, string nr)
	{
		imie = i;
		nazwisko = n;
		rejestracja_samochodu = m;
		nr_tel = nr;
	}
	Klient()
	{
	}
	Klient& operator=(const Klient& other) {
		if (this != &other) {
			imie = other.imie;
			nazwisko = other.nazwisko;
			rejestracja_samochodu = other.rejestracja_samochodu;
			nr_tel = other.nr_tel;
		}
		return *this;
	}
	friend class Baza_danych;
	friend class Interfejs;
};

class Baza_danych
{
	string nazwa_pliku_z_klientami;
	string nazwa_pliku_z_cenami;
	string nazwa_pliku_z_fakturami;
public:
	Baza_danych(string k, string c, string f)
	{
		nazwa_pliku_z_cenami = c;
		nazwa_pliku_z_fakturami = f;
		nazwa_pliku_z_klientami = k;
	}
	bool czy_klient_w_bazie(Klient k)
	{
		ifstream plik(nazwa_pliku_z_klientami);
		string linia;

		while (getline(plik, linia))
		{
			if (linia == k.imie + " " + k.nazwisko + " " + k.nr_tel + " " + k.rejestracja_samochodu)
			{
				plik.close();
				return true;
			}
		}
		plik.close();
		return false;
	}
	void dodaj_klienta(Klient k)
	{
		fstream plik;
		plik.open(nazwa_pliku_z_klientami, ios::app);
		plik << k.imie + " " + k.nazwisko + " " + k.nr_tel + " " + k.rejestracja_samochodu <<endl;
		plik.close();
	}
	void dodaj_fakture(Faktura f)
	{
		fstream plik;
		plik.open(nazwa_pliku_z_fakturami, ios::app);
		plik << f.nr_faktury << endl <<  f.imie << endl<< f.nazwisko << endl<< f.rejestracja_samochodu << endl << f.nr_tel << endl << f.opis_uslugi << endl << f.cena_uslugi << endl << f.termin_platnosci << endl << endl;
		plik.close();
	}
	friend class Interfejs;
};

class Interfejs
{
public:
	Baza_danych bd;
	Faktura f;
	Interfejs(string k, string c, string f): bd(k, c, f)
	{
	}
	Klient podaj_dane_klienta()
	{
		Klient k;
		cout << "Podaj imie: ";
		cin >> k.imie;
		cout << "Podaj nazwisko: ";
		cin >> k.nazwisko;
		cout << "Podaj rejestracje samochodu (7 znakow): ";
		cin >> k.rejestracja_samochodu;
		cout << "Podaj nr_telefonu (9 cyfr): ";
		cin >> k.nr_tel;

		return k;
	}
	int podaj_nr_uslugi()
	{
		ifstream plik(bd.nazwa_pliku_z_cenami);
		string linia;
		int nr;
		cout << "Podaj nr uslugi: " << endl;
		while (getline(plik, linia))
		{
			cout << linia << endl;
		}
		cin >> nr;
		return nr;
	}
	void wype³nij_fakture()
	{
		string pom;
		cout << "Podaj imie: ";
		cin >> pom;
		f.imie = pom;
		cout << "Podaj nazwisko: ";
		cin >> pom;
		f.nazwisko = pom;
		cout << "Podaj rejestracje samochodu (7 znakow): ";
		cin >> pom;
		f.rejestracja_samochodu = pom;
		cout << "Podaj nr_telefonu (9 cyfr): ";
		cin >> pom;
		f.nr_tel = pom;
		cout << "Wprowadz opis uslugi: ";
		cin.ignore();
		getline(cin, pom);
		f.opis_uslugi = pom;
		cout << "Wprowadz cene uslugi: ";
		cin >> pom;
		f.cena_uslugi = pom;
		cout << "Wprowadz termin platnosci (DDMMRRR): ";
		cin >> pom;
		f.termin_platnosci = pom;
		ifstream plik(bd.nazwa_pliku_z_fakturami);
		string linia;
		int licznik = 1;
		while (getline(plik, linia))
		{
			if (linia.empty())
			{
				licznik++;
			}
		}
		plik.close();
		f.nr_faktury = licznik;
	}
	void drukuj_fakture()
	{
		cout << "Dane faktury: " << endl;
		cout << "Numer faktury: " << f.nr_faktury << endl;
		cout << "Imie: " << f.imie << endl;
		cout << "Nazwisko: " << f.nazwisko <<endl;
		cout << "Rejestracja samochodu: " << f.rejestracja_samochodu << endl;
		cout << "Numer telefonu: " << f.nr_tel << endl;
		cout << "Opis uslugi: " << f.opis_uslugi << endl;
		cout << "Cena uslugi: " << f.cena_uslugi << endl;
		cout << "Termin platnosci: " << f.termin_platnosci << endl<<endl;
	}
};

int main()
{
	int wybor = 0;
	int nr_uslugi;
	Klient k;
	Interfejs interface("Klienci.txt", "Ceny.txt", "Faktury.txt");
	while(wybor != 3)
	{
		cout << "1. Utworz zgloszenie" << endl;
		cout << "2. Drukuj fakture" << endl;
		cout << "3. Wyjdz" << endl;
		cin >> wybor;
		switch (wybor)
		{
		case 1:
			k = interface.podaj_dane_klienta();
			if (interface.bd.czy_klient_w_bazie(k))
			{
				cout << "Klient jest juz w bazie." << endl;
			}
			else
			{
				interface.bd.dodaj_klienta(k);
				cout << "Klient dodany do bazy." << endl;
			}
			nr_uslugi = interface.podaj_nr_uslugi();
			cout << "Dziekujemy za wypelnienie zgloszenia." << endl;
			break;
		case 2:
			interface.wype³nij_fakture();
			interface.drukuj_fakture();
			interface.bd.dodaj_fakture(interface.f);
			break;
		}
	}
	return 0;
}