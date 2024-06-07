#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct lista {
    string imie;
    string nazwisko;
    string PESEL;
    int wiek;
    lista *pop;
    lista *nast;
};

void dodajElement(lista *&head, string imie, string nazwisko, string PESEL, int wiek);
void dodajElementUnikalny(lista *&head, string imie, string nazwisko, string PESEL, int wiek);
void usunPierwszyElement(lista *&head, string klucz);
void usunWszystkieElementy(lista *&head, string klucz);
void edytujElement(lista *head, string PESEL);
void zapiszDoPliku(lista *head, const string &nazwaPliku, string kryterium);
void wyswietlListe(lista *head);
void wyswietlPlik(const string &nazwaPliku);

int main() {
    lista *head = nullptr;
    int wybor;
    string imie, nazwisko, PESEL, kryterium;
    int wiek;
    string nazwaPliku = "lista.txt";

    do {
        cout << "Menu:" << endl;
        cout << "1. Dodaj element do listy" << endl;
        cout << "2. Dodaj element do listy (unikalne)" << endl;
        cout << "3. Usun pierwszy element o podanej wartosci klucza (nazwisko)" << endl;
        cout << "4. Usun wszystkie elementy o podanej wartosci klucza (nazwisko)" << endl;
        cout << "5. Edytuj element" << endl;
        cout << "6. Zapisz do pliku" << endl;
        cout << "7. Wyswietl liste" << endl;
        cout << "8. Wyswietl zawartosc pliku" << endl;
        cout << "9. Wyjdz" << endl;
        cout << "Wybierz opcje: ";
        cin >> wybor;
        cin.ignore();

        switch (wybor) {
            case 1:
                cout << "Podaj imie: ";
                getline(cin, imie);
                cout << "Podaj nazwisko: ";
                getline(cin, nazwisko);
                cout << "Podaj PESEL: ";
                getline(cin, PESEL);
                cout << "Podaj wiek: ";
                cin >> wiek;
                dodajElement(head, imie, nazwisko, PESEL, wiek);
                break;
            case 2:
                cout << "Podaj imie: ";
                getline(cin, imie);
                cout << "Podaj nazwisko: ";
                getline(cin, nazwisko);
                cout << "Podaj PESEL: ";
                getline(cin, PESEL);
                cout << "Podaj wiek: ";
                cin >> wiek;
                dodajElementUnikalny(head, imie, nazwisko, PESEL, wiek);
                break;
            case 3:
                cout << "Podaj nazwisko do usuniecia: ";
                getline(cin, nazwisko);
                usunPierwszyElement(head, nazwisko);
                break;
            case 4:
                cout << "Podaj nazwisko do usuniecia: ";
                getline(cin, nazwisko);
                usunWszystkieElementy(head, nazwisko);
                break;
            case 5:
                cout << "Podaj PESEL elementu do edycji: ";
                getline(cin, PESEL);
                edytujElement(head, PESEL);
                break;
            case 6:
                cout << "Podaj kryterium (wszystkie / nazwisko / pelnoletni): ";
                getline(cin, kryterium);
                zapiszDoPliku(head, nazwaPliku, kryterium);
                break;
            case 7:
                wyswietlListe(head);
                break;
            case 8:
                wyswietlPlik(nazwaPliku);
                break;
            case 9:
                cout << "Koniec programu." << endl;
                break;
            default:
                cout << "Nieprawidlowy wybor. Sprobuj ponownie." << endl;
        }
    } while (wybor != 9);

    return 0;
}


void dodajElement(lista *&head, string imie, string nazwisko, string PESEL, int wiek) {
    lista *nowy = new lista{imie, nazwisko, PESEL, wiek, nullptr, nullptr};

    if (!head || head->nazwisko > nazwisko) {
        nowy->nast = head;
        if (head) head->pop = nowy;
        head = nowy;
    } else {
        lista *temp = head;
        while (temp->nast && temp->nast->nazwisko <= nazwisko) {
            temp = temp->nast;
        }
        nowy->nast = temp->nast;
        if (temp->nast) temp->nast->pop = nowy;
        temp->nast = nowy;
        nowy->pop = temp;
    }
}

void dodajElementUnikalny(lista *&head, string imie, string nazwisko, string PESEL, int wiek) {
    lista *temp = head;
    while (temp) {
        if (temp->nazwisko == nazwisko && temp->PESEL == PESEL) {
            cout << "Element o podanym nazwisku i PESEL już istnieje." << endl;
            return;
        }
        temp = temp->nast;
    }
    dodajElement(head, imie, nazwisko, PESEL, wiek);
}

void usunPierwszyElement(lista *&head, string klucz) {
    lista *temp = head;
    while (temp) {
        if (temp->nazwisko == klucz) {
            if (temp->pop) temp->pop->nast = temp->nast;
            if (temp->nast) temp->nast->pop = temp->pop;
            if (temp == head) head = temp->nast;
            delete temp;
            return;
        }
        temp = temp->nast;
    }
    cout << "Element o podanym kluczu nie został znaleziony." << endl;
}

void usunWszystkieElementy(lista *&head, string klucz) {
    lista *temp = head;
    while (temp) {
        if (temp->nazwisko == klucz) {
            lista *doUsuniecia = temp;
            if (temp->pop) temp->pop->nast = temp->nast;
            if (temp->nast) temp->nast->pop = temp->pop;
            if (temp == head) head = temp->nast;
            temp = temp->nast;
            delete doUsuniecia;
        } else {
            temp = temp->nast;
        }
    }
}

void edytujElement(lista *head, string PESEL) {
    lista *temp = head;
    while (temp) {
        if (temp->PESEL == PESEL) {
            cout << "Podaj nowe imie: ";
            getline(cin, temp->imie);
            cout << "Podaj nowe nazwisko: ";
            getline(cin, temp->nazwisko);
            cout << "Podaj nowy wiek: ";
            cin >> temp->wiek;
            cin.ignore();
            return;
        }
        temp = temp->nast;
    }
    cout << "Element o podanym PESEL nie został znaleziony." << endl;
}

void zapiszDoPliku(lista *head, const string &nazwaPliku, string kryterium) {
    ofstream plik(nazwaPliku);
    if (!plik) {
        cout << "Nie można otworzyć pliku." << endl;
        return;
    }

    lista *temp = head;
    while (temp) {
        bool zapisz = false;
        if (kryterium == "wszystkie") {
            zapisz = true;
        } else if (kryterium == "nazwisko" && temp->nazwisko == kryterium) {
            zapisz = true;
        } else if (kryterium == "pelnoletni" && temp->wiek >= 18) {
            zapisz = true;
        }

        if (zapisz) {
            plik << temp->imie << " " << temp->nazwisko << " " << temp->PESEL << " " << temp->wiek << endl;
        }
        temp = temp->nast;
    }

    plik.close();
    cout << "Dane zapisane do pliku." << endl;
}

void wyswietlListe(lista *head) {
    lista *temp = head;
    while (temp) {
        cout << temp->imie << " " << temp->nazwisko << " " << temp->PESEL << " " << temp->wiek << endl;
        temp = temp->nast;
    }
}

void wyswietlPlik(const string &nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik) {
        cout << "Nie można otworzyć pliku." << endl;
        return;
    }

    string linia;
    while (getline(plik, linia)) {
        cout << linia << endl;
    }

    plik.close();
}
