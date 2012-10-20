CheckSol
========

Co to jest?
-----------
check\_sol -- Skrypt do automatycznego sprawdzania poprawności programów, tj. zgodności wyjścia
programu z wyjściem wzorcowym dla zadanych plików wejściowych. Główne funkcje:
* kompilacja kodu najbardziej popularnych języków lub użycie skompilowanego programu,
* automatyczne uruchomienie programu dla zbioru lub całego katalogu plików wejściowych (.in) oraz
  sprawdzenie wyjścia standardowego/błędów,
* porównywanie identycznościowe wyjścia programu z wzorcowym; można jednak dostarczyć własny program
  sprawdzający, dowolnie definiujący zgodność wyjść (patrz opcja `--compare`),
* czytelna, pokolorowana, konfigurowalna postać wyjścia,
* pomiar i limity czasu wykonania oraz zajętej pamięci,

Wymagania
---------
* program `time` (najczęściej znajdujący się w `/usr/bin/time`),
* uprawnienia do wykonania polecenia `ulimit`, jeśli używane są limity czasu lub pamięci,
* odpowiednie kompilatory w przypadku podania pliku z kodem jako argumentu.

Opis wyjścia skryptu
--------------------
W trybie normalnym, dla każdego testu wypisywany jest jeden wiersz wyniku:
* numer i nazwa testu,
* orientacyjna zużyta pamięć, w kiB, jeśli program działał wystarczająco długo żeby ją zmierzyć,
* czas wykonanie programu, w sekundach z dokładnością do setnych,
* wynik testu:
    * ` AC ` -- _Answer Correct_, odpowiedź zgodna z wzorcową,
    * ` TL ` -- _Time Limit (Exceeded)_, przekroczony limit czasu wykonania,
    * ` ML ` -- _Memory Limit (Exceeded)_, przekroczony limit zajmowanej pamięci,
    * ` WA ` -- _Wrong Answer_, odpowiedź niezgodna z wzorcową,
    * ` RE ` -- _Runtime Error_, błąd wykonania
    * `????` -- inny błąd (np. brak plików)
    w trzech ostatnich przypadkach w kolejnym wierszu zostanie wypisany opis błędu.

W trybie skróconego wyjścia (patrz `--short-output`) dla każdego testu wypisywany jest tylko jeden
znak wyniku testu, analogicznie jak wyżej.

Po zakończeniu testowania wyświetlane jest podsumowanie -- liczba wyników różnego rodzaju,
maksymalny czas oraz pamięć dla testu.

Parametry uruchomienia
----------------------
* `-s | --solution PLIK` kod programu do sprawdzenia. Obsługiwane są pliki .cpp, .cc, .c, .pas, inne
  należy skompilować ręcznie i użyć opcji `-S`.
  Podanie tego parametru lub `-S` jest konieczne.
* `-S | --sol-exec PLIK` skompilowany program do sprawdzenia. Powinien mieć prawa do uruchomienia.
  Podanie tego parametru lub `-s` jest konieczne.
* `-i | --input PLIKI` pliki wejściowe, dla których ma zostać uruchomiony program. Argumentem może
  być plik lub lista plików, jak również katalog -- wtedy użyte zostaną wszystkie zawarte w nim
  pliki `.in`. Parametr konieczny.
* `-o | --output KATALOG` katalog zawierający plik[i] wyjściowe. Muszą mieć nazwy identyczne
  z podanymi plikami wejściowymi oraz rozszerzenie `.out`. Parametr konieczny.
* `-c | --compare PLIK` kod programu sprawdzającego zgodność wyjść. Obecnie obsługiwany jest tylko 
  format .cpp. Jeśli nie zostanie podany ten parametr ani `-C`, zostanie użyta domyślna 
  sprawdzaczka, określona w pliku konfiguracyjnym. Aby dowiedzieć się, jak powinien wyglądać 
  program sprawdzający, przeczytaj komentarz na początku `standard_compare.cpp`.
* `-C | --comp-exec PLIK` skompilowany program sprawdzający zgodność wyjść.
* `-e | --no-errors` powoduje zatrzymanie sprawdzania po pierwszym błędnym teście.
* `-m | --mem-limit LICZBA` ustawia limit pamięci dla programu na LICZBA kilobajtów (lub LICZBA
  megabajtów, jeśli LICZBA ma dopisek `M`).
* `-t | --time-limit LICZBA` ustawia limit czasu wykonania jednego testu na LICZBA sekund.
* `-h | --short-output` zamiast jednego wiersza wyniku dla każdego testu wypisuje pojedynczy znak
  statusu sprawdzenia -- przydatne przy testowaniu na dużej liczbie przykładów.
* `-N | --no-delete` nie usuwa plików tymczasowych tworzonych przez program. Przydatne w połączeniu
  z `--no-errors`, gdyż te pliki zawierają dane dot. wykonania ostatniego testu.
* `-E | --check-stderr` porównuje również standardowe wyjście błędu programów. Zakłada, że wzorcowe
  wyjścia błędu mają rozszerzenie `.err` i znajdują się w katalogu z plikami `.out`.

Przykład wywołania:
`check_sol -s zadanie.cc -i tests/in -o tests/out -E -t 1 -m 50M -e`: skompilowanie pliku 
`zadanie.cc` i uruchomienie go na wszystkich plikach wejściowych z katalogu `tests/in`, sprawdzając
zgodność z wyjściami `.out` i `.err` w katalogu `tests/out`, z limitem 1 sekundy i 50MiB pamięci
i przerwaniem po pierwszym błędzie.

Znane błędy
-----------
* W czasie sprawdzeń nie powinien być uruchomiony inny program o nazwie identycznej ze sprawdzanym.
* Program (z zamierzenia) nie jest całkowicie idiotoodporny -- argumenty wykonania powinny być użyte
  zgodnie z przeznaczeniem.
Znalezione błędy można zgłaszać przez GitHub: `https://github.com/quezak/check_sol/issues`

Changelog
---------
* v1.85: *liczne* poprawki związane z czytelnością i stylem kodu.
* v1.7.1: dodane sprawdzenie wyjścia błędów.
* v1.7: wersja z momentu założenia repozytorium. Liczyła kilka ładnych lat i nie była napisana
  zbyt ładnie.
