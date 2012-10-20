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

echo
echo "CheckSol został zainstalowany do katalogu $path. Możesz go uruchomić przez"
echo "$path/check_sol"
echo "lub skopiować ów skrypt do katalogu na zmiennej \$PATH"
