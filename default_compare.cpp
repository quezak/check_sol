/**
 * Porównanie dwóch plików wyjściowych dla zadanego pliku wejściowego, z pominięciem znaków końca
 * linii na końcu pliku oraz dodatkowych białych znaków na końcach wierszy.
 *
 * Argumenty wywołania
 * 1. Plik wejściowy testu (nieużywany w tej sprawdzaczce)
 * 2. Sprawdzany plik wyjściowy
 * 3. Wzorcowy plik wyjściowy
 *
 * Gdy praca zakończy się błędem, zwracany jest niezerowy kod wyjścia.
 * Po normalnym zakończeniu wypisywana jest ocena (0 lub 100), a drugi wiersz może zawierać
 * komentarz do błędów.
 *
 * Author: Artur Kozak, na podstawie standard_compare (autor nieznany)
 * v0.8 (20.11.2012), cc-by-nc/beerware quezak@gmail.com
 */

#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
using namespace std;

static const string NL = "\r\n";
static const string WS = "\r\n\t ";
static ifstream chkifs, orgifs;
static unsigned int line_num = 0;

// usunięcie białych znaków na końcu stringa
static void trim_ws(string &str, const string &wsstr = WS)
{
    size_t last_nws = str.find_last_not_of(wsstr);
    if (last_nws != string::npos) str.erase(last_nws+1);
    else str.clear();
}

// zakonczenie: zamkniecie strumieni, wypisanie wyniku
static void koniec(int pts, const char *msg = NULL, ...)
{
    chkifs.close();
    orgifs.close();
    printf("%d\n", pts);
    if (msg) {
        va_list args;
        va_start(args, msg);
        vprintf(msg, args);
        va_end(args);
    }
    exit(0);
}

int main(int argc, char* argv[])
{
    if (argc != 4) return 1; //zła liczba argumentów
    // otwieramy pliki
    chkifs.open(argv[2]);
    if (chkifs.fail()) return 2;
    orgifs.open(argv[3]);
    if (orgifs.fail()) {
        chkifs.close();
        return 3;
    }
    string chkstr, orgstr;
    // rozpoczynamy sprawdzanie
    bool org_ok, chk_ok;
    do {
        org_ok = (bool)(getline(orgifs, orgstr));
        chk_ok = (bool)(getline(chkifs, chkstr));
        ++line_num;
        if (org_ok) trim_ws(orgstr);
        if (chk_ok) trim_ws(chkstr);
        int org_len = org_ok ? orgstr.length() : -1;
        int chk_len = chk_ok ? chkstr.length() : -1;
        if (org_len != chk_len && org_len+chk_len > -1)
            koniec(0, "Linia %u: wczytano %d znaków, oczekiwano %d\n", line_num, chk_len, org_len);
        if (org_ok && org_len == chk_len) {
            pair<string::iterator, string::iterator> fdiff;
            fdiff = mismatch(orgstr.begin(), orgstr.end(), chkstr.begin());
            if (fdiff.first != orgstr.end()) 
                koniec(0, "Linia %u, znak %d: wczytano '%c', oczekiwano '%c'\n",
                        line_num, fdiff.first - orgstr.begin() + 1, *fdiff.second, *fdiff.first);
        }
    } while (org_ok || chk_ok);
    koniec(100);
}
