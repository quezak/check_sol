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
* generowanie samych testów.

Wymagania
---------
* program `time` (najczęściej `/usr/bin/time`, nie mylić z poleceniem powłoki o tejże nazwie),
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
    
    W trzech ostatnich przypadkach w kolejnym wierszu zostanie wypisany opis błędu.

W trybie skróconego wyjścia (patrz `--short-output`) dla każdego testu wypisywany jest tylko jeden
znak wyniku testu, analogicznie jak wyżej.

Po zakończeniu testowania wyświetlane jest podsumowanie -- liczba wyników różnego rodzaju,
maksymalny czas oraz pamięć dla testu.

W pliku `check_sol_example.png` zobaczyć można przykład wyjścia skryptu dla danych spreparowanych
tak, by pokazywać różne możliwe przypadki.

Parametry uruchomienia
----------------------
* `-s | --solution PLIK` kod programu do sprawdzenia. Obsługiwane są pliki `.cpp`, `.cc`, `.c`, `.pas`,
  inne należy skompilować ręcznie i użyć opcji `-S`. Domyślne flagi kompilacji ustawione są w pliku
  `check_sol.cfg`, ale honorowane są ustawienia zmiennych środowiskowych.
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
* `-m | --mem-limit LICZBA` ustawia limit pamięci dla programu na `LICZBA` kilobajtów (lub `LICZBA`
  megabajtów, jeśli `LICZBA` ma dopisek `M`). Uwaga: przy zbyt niskich (kilkanaście MB i mniej) 
  limitach zachowanie programów jest dość nieokreślone.
* `-t | --time-limit LICZBA` ustawia limit czasu wykonania jednego testu na `LICZBA` sekund.
* `-h | --short-output` zamiast jednego wiersza wyniku dla każdego testu wypisuje pojedynczy znak
  statusu sprawdzenia -- przydatne przy testowaniu na dużej liczbie przykładów.
* `-N | --no-delete` nie usuwa plików tymczasowych tworzonych przez program. Przydatne w połączeniu
  z `--no-errors`, gdyż te pliki zawierają dane dot. wykonania ostatniego testu.
* `-E | --check-stderr` porównuje również standardowe wyjście błędu programów. Zakłada, że wzorcowe
  wyjścia błędu mają rozszerzenie `.err` i znajdują się w katalogu z plikami `.out`.
* `-g | --gen-outs` zamiast porównywać plikami `.out` w katalogu podanym w `-o`, używa zadanego 
  programu do jego wygenerowania. Przy `-E` generuje w tymże katalogu również pliki `.err`.
  Uwaga: generowane pliki nadpisują dotychczasowe pliki o tej samej nazwie.
* `-G | --gen-tests PLIK` uznaje plik rozwiązania za generator testów, przyjmujący parametry
  określone w PLIKu konfiguracyjnym. Zobacz sekcję _Generowanie testów_, opisującą format tych
  plików. Opcja generowania wyłącza `-g`, zaś opcje sprawdzenia nie mają znaczenia. Pozostają
  aktywne limity czasu i pamięci. Uwaga: generowane pliki nadpisują dotychczasowe.

#### Przykłady wywołania
`check_sol -s zadanie.cpp -i testy -o testy`: najprostsze sprawdzenie kodu `zadanie.cpp`
na wszystkich plikach `.in` z katalogu testy.

`check_sol -s zadanie.cc -i tests/in -o tests/out -E -t 1 -m 50M -e`: skompilowanie pliku 
`zadanie.cc` i uruchomienie go na wszystkich plikach wejściowych z katalogu `tests/in`, sprawdzając
zgodność z wyjściami `.out` i `.err` w katalogu `tests/out`, z limitem 1 sekundy i 50MiB pamięci
oraz przerwaniem testowania po pierwszym błędzie.

`check_sol -S generator -G gen_cfg -i testy`: wygenerowanie testów do katalogu `testy` programem 
`generator` na podstawie konfiguracji w pliku `gen_cfg`.

Generowanie testów
------------------
Można zamiast pliku rozwiązania podać program generujący testy. Program taki powinien przyjmować
odpowiednią liczbę parametrów z wiersza poleceń. Liczba generowanych testów, ich nazwa i parametry
powinny zostać zapisane w skrypcie konfiguracyjnym. Skrypt powinien ustawiać następujące zmienne:
* `TEST_NAME` -- prefiks nazwy generowanych plików wejściowych (`nazwa01.in`, ...)
* `TEST_COUNT` -- liczba testów do wygenerowania
* `PARAMS` -- lista parametrów generatora
* `param1, ...` -- tablice o nazwach wskazywanych przez `$PARAMS`, zawierające przynajmniej
  `$TEST_COUNT` elementów
* zamiast `PARAMS` oraz osobnych tablic można zdefiniować tablicę `GEN_TESTS_PARAMS`, zawierającą
  przynajmniej `$TEST_COUNT` elementów -- wierszy gotowych do przekazania generatorowi.

Zaleca się, żeby jako jeden z parametrów podać ziarno generatora liczb losowych.
Można wtedy, zamiast wysyłać duże pliki wejściowe, wysłać sam generator i plik konfiguracyjny,
uzyskując na dowolnej maszynie taki sam zestaw testów. Ponadto, skrypt _może_ zawierać inne komendy,
zostaną one wykonane przed rozpoczęciem generowania (nawet wymienione zmienne mogą być generowane,
a nie podane wprost). Parametry powinny mieć unikalne nazwy.

Przykładowy prosty generator, wypisujący `n` losowych liczb z zakresu `[0..n)`:
```C++
sscanf(argv[1], "%d", &seed);
sscanf(argv[2], "%d", &n);
srand(seed);
for (int i=0; i<n; ++i) printf("%d ", rand()%n);
```

Przykładowy plik konfiguracyjny powyższego generatora:
```sh
TEST_NAME=nazwa
TEST_COUNT=5
PARAMS="P_SEED P_ILE"
P_SEED=(123 950 1209 5392 2)
P_ILE=(4 10 50 123 500000)
```

Konfiguracja tych samych testów, przy użyciu drugiego sposobu definiowania zmiennych:
```sh
TEST_NAME=nazwa
TEST_COUNT=5
GEN_TESTS_PARAMS=("123 4" "950 10" "1209 50" "5392 123" "2 500000")
```


Znane błędy
-----------
* W czasie sprawdzeń nie powinien być uruchomiony inny program o nazwie identycznej ze sprawdzanym.
* Program (z zamierzenia) nie jest całkowicie idiotoodporny -- argumenty wykonania powinny być użyte
  zgodnie z przeznaczeniem.

Znalezione błędy można zgłaszać przez GitHub: `https://github.com/quezak/check_sol/issues`

Changelog
---------
* v2.0: generowanie testów, selftest przy instalacji
* v1.9: opcja generowania plików wyjściowych
* v1.8-1.89: **liczne** poprawki związane z czytelnością i stylem kodu.
* v1.7.1: dodane sprawdzenie wyjścia błędów.
* v1.7: wersja z momentu założenia repozytorium. Liczyła kilka ładnych lat i nie była napisana
  zbyt ładnie.
