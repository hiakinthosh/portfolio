/*
Program parses a file contains relations between people
:: Input file must be named "in.txt"
and be formatted with a rule that one line contains two people
and there is a relation between them

e.g.
    Bob Frank
    Frank Jason
    Martha Jason
    Matt Frank
etc.

:: Output file must be named "out.txt"

:: File with person with the *largest* number of relations
must be named "max.txt"

:: File with person with the *smallest* number of relations
must be named "min.txt"


*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// UWAGA!
// plik wejsciowy musi nazywac sie "in.txt"

void czyszczeniePlikuWyjsciowego() {
    // przyogotowywanie pliku wyjsiowego w wypadku
    // gdyby znajdowaly sie tam jakies smieci
    ofstream ofs;
    ofs.open("out.txt", ios::out | ios::trunc);
    ofs.close();
}


void dopisanieRelacjiDoPliku(string osoba_dopisywana) {
    // upiekszanie zapisu; dodawanie potrzebnych spacji
    size_t zawieraSpacje;
    zawieraSpacje = osoba_dopisywana.find(' ');
    if (zawieraSpacje == string::npos)
        osoba_dopisywana += " ";

    // zapisywanie osoby do pliku
    ofstream ofs;
    ofs.open("out.txt", ios::out | ios::app);
    ofs << osoba_dopisywana;
    ofs.close();
}


void wymazOsobeZLinii(string &linia, string osoba) {
    // uzyskiwanie osoby B w relacji poprzez usuniecie osoby A z linii
    size_t wymazanie = linia.find(osoba);
    if (wymazanie != string::npos) // jesli osoba A sie znalazla
        linia.erase(wymazanie, osoba.length() + 1); // wymaz ja
        // +1 zeby usunac takze spacje spomiedzy imion

    dopisanieRelacjiDoPliku(linia); // zapisz do pliku
}


void wyszukiwanieOsoby(string osoba) {

    cout << "Szukanie wszystkich relacji dla osoby o imieniu: " << osoba << endl;

    // zapisanie na poczatku linii osoby A
    // po niej osoby B, ktore ja znaja
    dopisanieRelacjiDoPliku(osoba);

    ifstream ifs;
    ifs.open("in.txt", ios::in);

    string linia;
    size_t relacja;

    while (getline(ifs, linia)) {
        relacja = linia.find(osoba); // szukanie relacji z osoba
        if (relacja != string::npos) { // jesli znaleziono relacje
            wymazOsobeZLinii(linia, osoba); // uzyskiwanie osoby z ktora tworzy relacje
            cout << "Znaleziono " << osoba
                 << " w relacji z " << linia << endl;
        }
    }

    ifs.close();

    // dodawanie znaku konca linii po zakonczeniu spisywania relacji
    ofstream ofs;
    ofs.open("out.txt", ios::out | ios::app);
    ofs << "\n";
    ofs.close();
}

void zapisywanieMaksa(string osoba, string linia, int liczba_znajomych) {
    ofstream ofs;
    ofs.open("max.txt", ios::out | ios::trunc);

    linia.erase(0, osoba.length());

    ofs << "Osoba o najwiêkszej liczbie znajomych: " << osoba << endl
        << "Liczba znajomych: " << liczba_znajomych-1 << endl // odliczanie samej osoby
        << "Znajomi:" << linia << endl << endl;

    ofs.close();
}

void zapisywanieMin(string osoba, string linia, int liczba_znajomych) {
    ofstream ofs;
    ofs.open("min.txt", ios::out | ios::trunc);

    linia.erase(0, osoba.length());

    ofs << "Osoba o najmniejszej liczbie znajomych: " << osoba << endl
        << "Liczba znajomych: " << liczba_znajomych-1 << endl // odliczanie samej osoby
        << "Znajomi:" << linia << endl << endl;

    ofs.close();
}


void sprawdzanie() { // ew. osoby zliczone jako argument
    ifstream ifs;
    ifs.open("out.txt");

    string linia;
    string osoba;
    size_t pozycja_spacji; // spacje oddzielaja poszczegolne osoby
    int n = 1;
    int liczba_spacji = 0;

    // ustawienie wartosci neutralnych
    int maks = 1;
    int mini = 1;
    string osobaMax;
    string osobaMin;
    string liniaMax;
    string liniaMin;


    while (getline(ifs, linia)) {
        pozycja_spacji = linia.find(" ");
        osoba = linia.substr(0, pozycja_spacji);
        // do testowania, mozna usunac
        /*
        cout << "----- Linia nr " << n << endl
             << "Nalezy do osoby " << osoba << endl
             << "Pozycja pierwszej spacji to "
             << pozycja_spacji << endl;
        */
        n++;
        for (int i = 0; i < linia.length(); i++) // zliczanie spacji w linii
            if (linia[i] == ' ')                 // spacja oznacza kolejna osobe w relacji
                liczba_spacji++;

        if (liczba_spacji > maks) {
            maks = liczba_spacji;
            osobaMax = osoba;
            liniaMax = linia;
            zapisywanieMaksa(osobaMax, liniaMax, maks);
        }
        if (liczba_spacji-1 <= mini || liczba_spacji == 1){
            mini = liczba_spacji;
            osobaMin = osoba;
            liniaMin = linia;
            zapisywanieMin(osobaMin, liniaMin, mini);
        }
        liczba_spacji = 0; // wyzerowanie liczby dla kolejnej linii
    }

    ifs.close();
}

int main() {

    czyszczeniePlikuWyjsciowego();

    ifstream ifs;
    ifs.open("in.txt", ios::in);
    string osoba;
    string osoby_zaliczone;
    size_t znaleziono;

    while (!ifs.eof()) {
        ifs >> osoba; // pobieranie po kolei osoob

        znaleziono = osoby_zaliczone.find(osoba);
        if (znaleziono == string::npos) {
        // jesli nie bylo jeszcze przetwarzania tej osoby
            osoby_zaliczone += osoba + " ";
            wyszukiwanieOsoby(osoba);
            //cout << "Osoby zaliczone: " << osoby_zaliczone << endl; // testing
        }
    }

    ifs.close();

    sprawdzanie();

    return 0;
}
