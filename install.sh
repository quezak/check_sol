#!/bin/bash

source check_sol.cfg

echo "Podaj katalog instalacji (enter = ~/.check_sol):"
read path

### katalog
if [ -z "$path" ]; then
    path="$HOME/.check_sol"
fi
path=${path%/}
if [ -e "$path" ] && [ ! -d "$path" ]; then
    echo "Plik już istnieje i nie jest katalogiem"
    exit 1
fi
mkdir -p "$path" || ( echo "nie można utworzyć katalogu" && exit 2 )

### kopie
cp README.md standard_compare.cpp install.sh "$path" || \
    ( echo "nie można zapisać do podanego katalogu" && exit 3 )

### time
timep="$(which time)"
if [ -z $timep ]; then
    echo "nie można znaleźć programu time"
    exit 4
fi
timep=${timep//\//\\\/}

### sprawdzaczka
$CXX $CXXFLAGS standard_compare.cpp -o "$path/standard_compare" || \
    ( echo "nie udało się skompilować sprawdzaczki" && exit 5 )

### konfig
epath=${path//\//\\\/}
sed -e "s/\(DEFAULT_CHECKER=\).*/\1'$epath\/standard_compare'/;s/\(TIMEPRG=\).*/\1'$timep'/" \
    check_sol.cfg > "$path/check_sol.cfg"

### skrypt
sed -e "s/\(CHECK_SOL_DIR=\).*/\1'$epath'/" check_sol > "$path/check_sol"
chmod +x "$path/check_sol" || ( echo "nie można ustawić praw wykonywania skryptu" && exit 6 )

### przydasie do testowych wywołań
tmpdir="__check_sol_selftest"
rm -rf "$path/$tmpdir"
mkdir "$path/$tmpdir"
cd "$path/$tmpdir"
ln -s `which echo` "echo"
cat > gen_cfg << EOF
#!/bin/bash
TEST_NAME=selftest
TEST_COUNT=5
PARAMS=LICZBA
LICZBA=(2 1 100000000 -1 -2)
EOF
cat > gen_out << EOF
#!/bin/bash
read liczba
echo \$[liczba+1]
EOF
cat > selftest.cpp << EOF
#include<cstdio>
#include<vector>
int main() {
    int n;
    scanf("%d", &n);
    if (n==2) printf("hehe"); // WA
    if (n==1) { int m=n-1; printf("%d\\n", n/m); } // RE
    if (n==-1) while(--n); // TL
    if (n==-2) { std::vector<int> v; v.resize(100000000); printf("%lu\\n", v.size()); } // ML
    printf("%d\\n", n+1);
}
EOF

### testowe wywołania
chmod +x gen_out
mkdir testy
echo
echo "------------ SPRAWDZENIE GENEROWANIA TESTÓW ------------"
../check_sol -S "echo" -i "testy" -G "gen_cfg" || \
    ( echo "sprawdzenie nie powiodło się" && exit 10 )
echo
echo "------------ SPRAWDZENIE GENEROWANIA WYJŚCIA ------------"
../check_sol -S "gen_out" -i "testy" -o "testy" -g || \
    ( echo "sprawdzenie nie powiodło się" && exit 10 )
echo
echo "------------ SPRAWDZENIE TESTOWANIA ROZWIĄZAŃ ------------"
../check_sol -s "selftest.cpp" -i "testy" -o "testy" -m 30M -t 1 || \
    ( echo "sprawdzenie nie powiodło się" && exit 10 )
cd
rm -rf "$path/$tmpdir"

### alles klar
cat << EOF

Jeśli wyniki ostatniego sprawdzenia to kolejno: WA, RE, AC, TL, ML, sprawdzanie działa poprawnie.

CheckSol został zainstalowany do katalogu $path. Możesz go uruchomić przez
$path/check_sol
lub podlinkować ów skrypt do katalogu na zmiennej \$PATH
EOF
