/*
Program parses a file with .tex extension and
collects title of the article contained in file,
its author,
moreover counts images which occurred
and list its bibliography
*/

#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;


void zapisywanieAutora(string linia) {
    ofstream ofs;
    ofs.open("out.txt", ios::out | ios::app);

    int pozycja_poczatkowa, pozycja_koncowa;
    size_t znaleziono;

    znaleziono = linia.find('{');
    if (znaleziono != string::npos)
        pozycja_poczatkowa = znaleziono + 1;

    znaleziono = linia.find('}');
    if (znaleziono != string::npos)
        pozycja_koncowa = znaleziono;

    string autor = linia.substr((pozycja_poczatkowa), (pozycja_koncowa-pozycja_poczatkowa));
    cout << "Autor: " << autor << endl;

    ofs << "Autor artykulu: " << autor << "\n";
    ofs.close();
}

void zapisywanieTytulu(string linia) {
    ofstream ofs;
    ofs.open("out.txt", ios::out | ios::app);

    int pozycja_poczatkowa, pozycja_koncowa;
    size_t znaleziono;

    znaleziono = linia.find('{');
    if (znaleziono != string::npos)
        pozycja_poczatkowa = znaleziono + 1;

    znaleziono = linia.find('}');
    if (znaleziono != string::npos)
        pozycja_koncowa = znaleziono;

    string tytul = linia.substr((pozycja_poczatkowa), (pozycja_koncowa-pozycja_poczatkowa));
    cout << "Tytul: " << tytul << endl;

    ofs << "Tytul artykulu: " << tytul << "\n";
    ofs.close();
}

void zapisywanieObrazow(int ilosc_obrazow) {
    ofstream ofs;
    ofs.open("out.txt", ios::out | ios::app);

    cout << "Liczba ilustracji: " << ilosc_obrazow << "\n";
    ofs << "Liczba ilustracji: " << ilosc_obrazow << "\n";
    ofs.close();
}

void zapisywanieBib(const char *nazwaPliku) {
    ifstream ifs;
    ofstream ofs;
    ifs.open(nazwaPliku, ios::in);
    ofs.open("out.txt", ios::out | ios::app);

    ofs << "Bibliografia: " << endl << endl;

    string linia;
    string bibitem = "\\bibitem";
    size_t znaleziono;
    int n = 1;  // numeracja pozycji bibliograficznych
    while (getline(ifs, linia)) {
        znaleziono = linia.find(bibitem);
        if (znaleziono != string::npos) { // jesli znaleziono slowo kluczowe
            getline(ifs, linia);          // to wyswietl kolejna linijke
            ofs << n << ". " << linia << endl;
            n++;
        }
    }

    ifs.close();
    ofs.close();

    cout << "Dodano bibliografie.\n";
}


int main() {
    ofstream ofs; // czyszczenie pliku wyjsciowego
    ofs.open("out.txt", ios::out | ios::trunc);
    ofs.close();

    ifstream ifs;
    char nazwaPliku[80];
    cout << "Podaj nazwe pliku LaTeX: ";
    cin >> nazwaPliku;
    ifs.open(nazwaPliku, ios::in); // plik wskazany przez usera

    if (!ifs.is_open()) {
        cout << "Plik nie zostal otwarty" << endl;
        exit(1); // zakonczenie programu z bledem
    }

    // slowa kluczowe
    string autor = "\\author";
    string tytul = "\\title";
    string obraz = "\\begin{figure}";

    size_t znaleziono; // do wyszukiwania slow kluczowych
    int ilosc_obrazow = 0;
    string linia;
    while (getline(ifs, linia)) { // pobieranie linia po linii
        znaleziono = linia.find(autor);
        if (znaleziono != string::npos) // jesli znaleziono slowo kluczowe \author
            zapisywanieAutora(linia);
        znaleziono = linia.find(tytul);
        if (znaleziono != string::npos) // jesli znaleziono slowo kluczowe \title
            zapisywanieTytulu(linia);
        znaleziono = linia.find(obraz);
        if (znaleziono != string::npos) // jesli znaleziono slowo kluczowe \begin{figure}
            ilosc_obrazow++;
    }
    ifs.close();

    zapisywanieObrazow(ilosc_obrazow);
    zapisywanieBib(nazwaPliku);

    cout << "\nW pliku 'out.txt' znajduje sie wynik operacji\n";
    return 0;
}
