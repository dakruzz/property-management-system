#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cctype>
#include <string>
#include <time.h>
#include <ctime>
using namespace std;



/**********************************************************************
***********************        BAZADANYCH             ******************
***********************************************************************/

class BazaDanych
{
    public:
        BazaDanych();
        virtual ~BazaDanych();
        int * rozmiarPliku(string);
        string getWhereRow(string, int, int); // (fileName, which row, which column)
        string * getRow(string, int); // (fileName, which column)
        //void dodajDoPliku(string,...)
};

BazaDanych::BazaDanych()
{
    //ctor
}

BazaDanych::~BazaDanych()
{
    //dtor
}

int * BazaDanych::rozmiarPliku(string nazwaPliku)
{
    fstream plik;
    plik.open(nazwaPliku.c_str(), ios::in);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    string word;
    int *fileInfo = new int[2];
    int wiersz = 0;
    int kolumna = 0;

    while(plik >> word ) {
        wiersz++;
        if(word == "$") kolumna++;
    }

    if(kolumna == 0) {
        fileInfo[0] = 0;
        fileInfo[1] = 0;
    } else {
        fileInfo[0] = wiersz/kolumna;
        fileInfo[1] = kolumna;
    }

    plik.close();
    return fileInfo;
}

string BazaDanych::getWhereRow(string nazwaPliku, int row, int col)
{
    fstream plik;
    plik.open(nazwaPliku.c_str(), ios::in);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    //INT to STRING
    ostringstream str1;
    str1 << row;
    string stringID = str1.str();

    string word;
    int i=0 ;
    while(plik >> word) {                 // Wybranie kolumny
        if(word == stringID) {
            while(i != col) {
                plik >> word;
                i++;
            }
            plik.close();
            return word;
        }
    }

    plik.close();
    return "";
}

string * BazaDanych::getRow(string nazwaPliku, int col)
{
    fstream plik;
    plik.open(nazwaPliku.c_str(), ios::in);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    int *fileSize;
    fileSize = (rozmiarPliku("uzytkonicy.txt") + 1);

    string emails[*fileSize];
    string word;

    int i=0, j=0;
    while(plik >> word) {                 // Wybranie kolumny
        if(i == col) {
            emails[j] = word;
            i = 0;
            j++;
        }
        i++;
    }

    plik.close();
    return emails;

}



/**********************************************************************
***********************        UZYTKOWNIK             ****************
***********************************************************************/

class Uzytkownik
{
    public:
        Uzytkownik();
        virtual ~Uzytkownik();

        void setImie();
        void setPassword();
        void setEmail();
        string getImie();
        string getPassword();
        string getEmail();

        void rejestracja();
        string * logowanie();

    private:
        string imie;
        string password;
        string email;

};

Uzytkownik::Uzytkownik()
{
    //
}

Uzytkownik::~Uzytkownik()
{
    this->imie = "";
    this->password = "";
    this->email = "";
}

void Uzytkownik::setImie()
{ cin >> this->imie; }

void Uzytkownik::setPassword()
{ cin >> this->password; }

void Uzytkownik::setEmail()
{
    BazaDanych b;

    bool exist;
    int *fileSize;
    fileSize = (b.rozmiarPliku("uzytkownicy.txt") + 1);

    while(1) {
        exist = false;
        cin >> this->email;
        for(int i=0; i < *fileSize; i++) {
            if(this->email == b.getWhereRow("uzytkownicy.txt", i, 3)) {
                exist = true;
            }
        }
        if(exist) {
            cout << "Podany email juz istnieje!" << endl;
            exit(0);
        } else break;
    }
}

string Uzytkownik::getImie()
{ return this->imie; }

string Uzytkownik::getPassword()
{ return this->password; }

string Uzytkownik::getEmail()
{ return this->email; }


void Uzytkownik::rejestracja()
{
    cout << "Podaj imie: ";
    setImie();
    cout << "\nPodaj haslo: ";
    setPassword();
    cout << "\nPodaj email: ";
    setEmail();

    fstream plik;
    plik.open("uzytkownicy.txt", ios::out | ios:: app);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    BazaDanych dane;
    int *id;
    id = ( dane.rozmiarPliku("uzytkownicy.txt") + 1 );

    //ADD USER
    plik << *id << " " << imie << " " << password << " " << email << " $" << endl;

    cout << "Twoj ID: " << *id << "\nTwoje haslo: " << password << endl;
    system("pause");

    plik.close();
}

// ID[0] Imie[1] Haslo[2] Email[3]
string * Uzytkownik::logowanie()
{
    int IDToLogin;
    string passwordToLogin;

    cout << "\nPodaj ID: ";
    cin >> IDToLogin;
    cout << "\nPodaj haslo: ";
    cin >> passwordToLogin;

    // [0] - Imie, [1] - Email, [2] - ID
    string * daneUzytkownika = new string[3];
    BazaDanych dane;
    daneUzytkownika[0] = dane.getWhereRow("uzytkownicy.txt", IDToLogin, 1);
    daneUzytkownika[1] = dane.getWhereRow("uzytkownicy.txt", IDToLogin, 3);
    daneUzytkownika[2] = dane.getWhereRow("uzytkownicy.txt", IDToLogin, 0);

    return daneUzytkownika;
}


/**********************************************************************
***********************        NIERUCHOMOSC             ****************
***********************************************************************/

class Nieruchomosc
{
    public:
        Nieruchomosc(float, string, string, string, float);
        virtual ~Nieruchomosc();

        float cena;
        string lokacja;
        string sprzedawca;
        string kontakt;
        float rozmiar;
        bool dostepnosc;

        void zapisz();
        float getCena();
        string getLokacja();
        string getSprzedawca();
        string getKontakt();
        float getRozmiar();

        bool getDostepnosc();
        void setDostepnosc(bool);


    protected:

    private:
};

Nieruchomosc::Nieruchomosc(float cena, string lokacja, string sprzedawca, string kontakt, float rozmiar)
{
    this->cena = cena;
    this->lokacja = lokacja;
    this->sprzedawca = sprzedawca;
    this->kontakt = kontakt;
    this->rozmiar = rozmiar;
    this->dostepnosc = true;
}


Nieruchomosc::~Nieruchomosc()
{
    this->cena = 0;
    this->lokacja = "";
    this->sprzedawca = "";
    this->kontakt = "";
    this->rozmiar = 0;
    this->dostepnosc = false;
}


float Nieruchomosc::getCena()
{
    return this->cena;
}

string Nieruchomosc::getLokacja()
{
    return this->lokacja;
}

string Nieruchomosc::getSprzedawca()
{
    return this->sprzedawca;
}

string Nieruchomosc::getKontakt()
{
    return this->kontakt;
}

float Nieruchomosc::getRozmiar()
{
    return this->rozmiar;
}

bool Nieruchomosc::getDostepnosc()
{
    return this->dostepnosc;
}

void Nieruchomosc::setDostepnosc(bool x)
{
    this->dostepnosc = x;
}


/**********************************************************************
***********************        LISTA             ***********************
***********************************************************************/

class Lista
{
    public:
        Lista();
        virtual ~Lista();

        string kryteria[4];

        void wyswietlListe();
        void dodajDoListy(Nieruchomosc&);
        void aktualizujListe(int);
        void uwzglednijKryteria();


    protected:

    private:
};

Lista::Lista()
{

}

Lista::~Lista()
{
    //dtor
}

void Lista::wyswietlListe()
{
    fstream plik;
    plik.open("nieruchomosci.txt", ios::in);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    string word;
    int wiersz = 0;
    int kolumna = 0;

        int i = 0;
        while(plik >> word) {
            switch (i)
            {
                case 0:
                    cout << "ID: ";
                    cout << word << " | ";
                    i++;
                    break;
                case 1:
                    cout << "Cena: ";
                    cout << word << " | ";
                    i++;
                    break;
                case 2:
                    cout << "Lokacja: ";
                    cout << word << " | ";
                    i++;
                    break;
                case 3:
                    cout << "Sprzedawca: ";
                    cout << word << " | ";
                    i++;
                    break;
                case 4:
                    cout << "Kontakt: ";
                    cout << word << " | ";
                    i++;
                    break;
                case 5:
                    cout << "Rozmiar(m^2): ";
                    cout << word << " | ";
                    i++;
                    break;
                case 6:
                    cout << "Dostepnosc: ";
                    cout << word << " | ";
                    i++;
                    break;
                case 7:
                    i=0;
                    cout << endl;
                    continue;
            }
        }
    cout << endl;
    plik.close();
}


void Lista::dodajDoListy(Nieruchomosc& dodaj)
{
    fstream plik;
    plik.open("nieruchomosci.txt", ios::out | ios:: app);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    //ADD NIERUCHOMOSC
    BazaDanych dane;
    int *id;
    id = (dane.rozmiarPliku("nieruchomosci.txt") + 1);


    plik << *id << " " << dodaj.getCena() << " " << dodaj.getLokacja() << " " << dodaj.getSprzedawca() << " " << dodaj.getKontakt() << " "
    << dodaj.getRozmiar() << " " << dodaj.getDostepnosc() << " $" << endl;

    plik.close();
}

void Lista::aktualizujListe(int nieruchomoscID)
{
    fstream plik1;
    fstream plik2;
    plik1.open("nieruchomosci.txt", ios::in);      // Czytanie
    plik2.open("temp.txt", ios::out);   // Pisanie

    string line;
    int i = 0;
    while (!plik1.eof())
    {
        getline(plik1,line);
        if(i == nieruchomoscID)
        {
            line = line.replace(line.end()-3, line.end()-2, "0");
            plik2 << line << endl;
        } else {
            plik2 << line << endl;
        }
        i++;
    }

    fstream plik3;
    fstream plik4;
    plik3.open("nieruchomosci.txt", ios::out);
    plik4.open("temp.txt", ios::in);
    while (!plik4.eof())
    {
        getline(plik4,line);
        plik3 << line << endl;
    }


    if( remove( "temp.txt" ) != 0 )
        perror( "Error deleting file" );
    else
        puts( "File successfully deleted" );

    plik1.close();
    plik2.close();
    plik3.close();
    plik4.close();
}


void Lista::uwzglednijKryteria()
{
    // Szukanie w pliku po podanych kryteriach
}


/**********************************************************************
***********************        SPRZEDAWCA             ******************
***********************************************************************/

class Sprzedawca : public Uzytkownik
{
    public:
        Sprzedawca();
        virtual ~Sprzedawca();
        string imie;
        string email;

        void addNieruchomosc(string, string);

    protected:

    private:
};

Sprzedawca::Sprzedawca()
{
    //ctor
}

Sprzedawca::~Sprzedawca()
{
    //dtor
}

void Sprzedawca::addNieruchomosc(string imieUzytkownika, string emailUzytkownika)
{
    float cena,rozmiar;
    string lokacja, sprzedawca, kontakt;

    cout << "\nPodaj cene(PLN): ";
    cin >> cena;

    cout << "\nPodaj rozmiar(m^2): ";
    cin >> rozmiar;

    cout << "\nPodaj lokacje: ";
    cin >> lokacja;

    sprzedawca = imieUzytkownika;
    kontakt = emailUzytkownika;

    Nieruchomosc dane(cena, lokacja, sprzedawca, kontakt, rozmiar);

    Lista *lista = new Lista;
    lista->dodajDoListy(dane);
}


/**********************************************************************
***********************        PLATNOSC             ***********************
***********************************************************************/

class Platnosc
{
    public:
        Platnosc();
        virtual ~Platnosc();

        void zakup();
        float getCena();
        string getData();
        void setCena(int);
        void setData();
        void transakcje(int);
        void dodajTransakcje(int, int);

    protected:

    private:
        float cena;
        string data;
};

Platnosc::Platnosc()
{
    //
}

Platnosc::~Platnosc()
{
    //dtor
}

float Platnosc::getCena()
{
    return this->cena;
}

string Platnosc::getData()
{
    return this->data;
}

void Platnosc::setCena(int nieruchomoscID)
{
    BazaDanych b;
    int *row, *col;
    col = b.rozmiarPliku("nieruchomosci.txt");
    row = b.rozmiarPliku("nieruchomosci.txt") + 1;

    fstream plik;
    plik.open("nieruchomosci.txt", ios::in);      // Czytanie
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    string data[*row][*col];
    string word;
    for(int i=0; i<*row; i++) {
        for(int j=0; j<*col; j++) {
            plik >> word;
            data[i][j] = word;
        }
    }

    this->cena = atoi( data[nieruchomoscID][1].c_str() );
    plik.close();
}

void Platnosc::setData()
{
    time_t czas;
    time(&czas );

    string t( ctime( &czas ) );
    t = t.substr( 0, t.length() -1 );
    this->data = t;
}

void Platnosc::transakcje(int uzytkownikID)
{
    fstream plik;
    plik.open("transakcje.txt", ios::in);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    string word;
    string tempTable[9];

    int i = 0;
    while (plik >> word)
    {
        if(i<9) {
            tempTable[i] = word;
            i++;
            continue;
        } else {
            if( atoi( tempTable[2].c_str() ) == uzytkownikID ) {
                cout << "ID zakupionej nieruchomosci: " << tempTable[1]
                    << " | data transakcji: " << tempTable[6] << "." << tempTable[5] << "." << tempTable[8] << " " << tempTable[7]
                    <<" | koszt:" << tempTable[3] << endl;
                i = 0;
            } else {
                i = 0;
            }
        }
    }
}

void Platnosc::dodajTransakcje(int nieruchomoscID, int uzytkownikID)
{
    fstream plik;
    plik.open("transakcje.txt", ios::out | ios:: app);
    if(!plik.good()) {
        cout << "\nERROR 404";
        exit(0);
    }

    BazaDanych dane;
    int *id;
    id = (dane.rozmiarPliku("transakcje.txt") + 1);

    string data = getData();

    plik << *id << " " << nieruchomoscID << " " << uzytkownikID << " " << getCena() << " " << data << " $" << endl;

    plik.close();
}


void Platnosc::zakup()
{
    cout << "\nKwota nieruchomosci to: " << getCena() << " PLN" << endl;
    cout << "Czy aby na pewno chcesz zakupic nieruchomosc?\ny) Tak\nn) Nie" << endl;
    char yn;
    cin >> yn;

    if(yn == 'y') {
        cout << "Dziekujemy za zakup nieruchomosci. Transakcja wyniosla " << getCena() << " PLN, oraz zostala dokonana dnia: " << getData() << endl;
        system("pause");
    } else {
        cout << "Zapraszamy ponownie!" << endl;
        system("pause");
    }
}


/**********************************************************************
***********************        MAIN             ***********************
***********************************************************************/

int main()
{

    BazaDanych * plik = new BazaDanych;
    Uzytkownik * user = new Uzytkownik;
    Sprzedawca * sprzedawca = new Sprzedawca;
    Lista * lista = new Lista;
    Platnosc * personal = new Platnosc;

    //MENU
    int menu = 1;
    string * daneUzytkownika;
    daneUzytkownika = NULL;
    string imieUzytkownika;
    string emailUzytkownika;
    int uzytkownikID;

    do
    {
        switch(menu)
        {

            //MENU POCZATKOWE
            case 1:
            {
                system("cls");
                cout << "2) Logowanie\n3) Rejestracja\n4) Lista nieruchomosci\n0) Zakoncz program\n" << endl;
                cin >> menu;
                continue;
            }


            //LOGIN
            case 2:
            {
                user->~Uzytkownik();
                system("cls");

                // [0] - Imie, [1] - Email, [2] - ID
                daneUzytkownika = user->logowanie();
                imieUzytkownika = *daneUzytkownika;
                emailUzytkownika = *(daneUzytkownika + 1);
                uzytkownikID = atoi( (*(daneUzytkownika + 2)).c_str() );

                menu = daneUzytkownika ? 9 : 1;
                continue;
            }


            //REJESTRACJA
            case 3:
            {
                system("cls");
                user->rejestracja();
                menu = 1;
                continue;
            }


            //LISTA
            case 4:
            {
                char yn;
                system("cls");
                cout << "\nLista:\n";
                lista->wyswietlListe();
                cout << endl << "Czy jestes zainteresowany kupnem ktorejs nieruchomosci?\ny) Tak\nn) Nie\n";
                cin >> yn;

                if(daneUzytkownika) {
                    if(yn == 'y') menu = 7;
                    else menu = 9;
                    continue;
                } else {
                    if(yn == 'n') menu = 1;
                    else {
                        cout << "\nNajpierw sie zaloguj" << endl;
                        system("pause");
                        system("cls");
                        menu = 1;
                    } continue;
                }
            }

            //DODAWANIE NIERUCHOMOSCI
            case 5:
            {
                system("cls");
                if(daneUzytkownika) {
                    try {
                        sprzedawca->addNieruchomosc(imieUzytkownika, emailUzytkownika);
                        menu = 4;
                        continue;
                    }
                    catch(...)
                    {
                        cout << "Cos poszlo nie tak" << endl;
                        system("pause");
                        system("cls");
                        menu = 1;
                        continue;
                    }
                } else {
                    cout << "\nNajpierw sie zaloguj" << endl;
                    system("pause");
                    system("cls");
                    menu = 1;
                    continue;
                }
            }

            //KUPCA
            case 6:
            {
                system("cls");
                if(daneUzytkownika) {
                    cout << "1) Menu poczatkowe\n4) Lista nieruchomosci\n8) Lista transakcji" << endl;
                    cin >> menu;
                    continue;
                } else {
                    cout << "\nNajpierw sie zaloguj" << endl;
                    system("pause");
                    system("cls");
                    menu = 1;
                    continue;
                }
            }

            //ZAKUP NIERUCHOMOSCI
            case 7:
            {
                int *row, *col;
                col = plik->rozmiarPliku("nieruchomosci.txt");
                row = plik->rozmiarPliku("nieruchomosci.txt") + 1;

                if(daneUzytkownika) {
                    int nieruchomoscID;
                    cout << "Podaj ID nieruchomosci, ktora Cie interesuje: ";
                    cin >> nieruchomoscID;

                    if(nieruchomoscID >= *row) {
                        cout << "\nBledne ID" << endl;
                        system("pause");
                        menu = 4;
                        continue;
                    } else {

                        int dostepnosc = atoi( plik->getWhereRow("nieruchomosci.txt", nieruchomoscID, 6).c_str() );
                        if(dostepnosc == 0) {
                             cout << "\nOferta jest juz wykupiona" << endl;
                             system("pause");
                             menu = 4;
                             continue;
                        } else {
                            personal->setCena(nieruchomoscID);
                            personal->setData();
                            personal->zakup();
                            personal->dodajTransakcje(nieruchomoscID, uzytkownikID);
                            lista->aktualizujListe(nieruchomoscID);
                            menu = 4;
                            continue;
                        }
                    }
                } else {
                    cout << "\nNajpierw sie zaloguj" << endl;
                    system("pause");
                    system("cls");
                    menu = 1;
                    continue;
                }
            }

            //LISTA TRANSAKCJI
            case 8:
            {
                system("cls");
                if(daneUzytkownika) {
                    personal->transakcje(uzytkownikID);
                    menu = 6;
                    system("pause");
                    continue;
                } else {
                    cout << "\nNajpierw sie zaloguj" << endl;
                    system("pause");
                    system("cls");
                    menu = 1;
                    continue;
                }
            }

            //KUPCA LUB SPRZEDAWCA
            case 9:
            {
                system("cls");
                if(daneUzytkownika) {
                    cout << "1) Menu poczatkowe\n4) Lista nieruchomosci\n5) Sprzedawca nieruchomosci\n6) Kupca\n0) Zakoncz program" << endl;
                    cin >> menu;
                    continue;
                } else {
                    cout << "\nNajpierw sie zaloguj" << endl;
                    system("pause");
                    system("cls");
                    menu = 1;
                    continue;
                }
            }

            //ZAKONCZ
            case 0:
            {
                user->~Uzytkownik();

                delete user;
                delete plik;
                delete sprzedawca;
                delete lista;
                delete personal;

                break;
            }

            default:
            {
                cout << "Niepoprawna opcja\nSprobuj jeszcze raz: ";
                cin >> menu;
                continue;
            }
        }

    }while(menu != 0);

    return 0;
}
