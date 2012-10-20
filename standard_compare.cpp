//Program sprawdzajacy, czy dwa pliki (output) sa identyczne.
//Dopuszczalne roznice: 
//    * nadmiarowe znaki konca lini na koncu pliku oraz
//    * nadmiarowe biale znaki na koncu wiersza (tj. spacja, tabulator
//      lub znak konca lini)
//
//Przyjmuje trzy argumenty:
//  argv[1] - plik wejsciowy (nie uzywany)
//  argv[2] - wyjscie usera
//  argv[3] - oczekiwany plik wyjsciowy
//(kolejnosc dwoch ostatnich argumentow ma znaczenie dla poprawnego generowania
//komentarzy)
//
//W przypadku bledu konczy dzialanie z niezerowym kodem wyjscia.
//W przeciwnym wypadku w pierszym wierszu znajduje sie liczba 0 lub 100,
//oznaczajaca odpowiednio bledna lub poprawna odpowiedz.
//W kolejnych wierszach moze znajdowac sie komentarz.
#include <cstdio>
#include <cstdlib>
#include <ctype.h>

int usun_biale_znaki(char * buff, int pos) {
	// Usun ciag bialych znakow konczacy sie na pozycji pos w buforze buff
	// Zwraca liczbe znakow, ktore pozostaly w buforze (przed pozycja pos)
	while (pos>=0 && isspace(buff[pos])) {
		buff[pos] = 0;
		pos--;
	}
	return pos+1;
}

int main(int argc, char * argv[]) {
	FILE * user_output, *orig_output;
	char * user_buff = NULL, * orig_buff = NULL;
	size_t user_buff_len = 0, orig_buff_len = 0;
	int user_buff_read, orig_buff_read;
	int line;
	int pos;

	if (argc != 4) return 1;			// Bledna liczba argumentow

	user_output = fopen(argv[2], "r");
	if (user_output == NULL) return 2;	// Nie otwarto odpowiedzi usera

	orig_output = fopen(argv[3], "r");
	if (orig_output == NULL) {			// Nie otwarto odpowiedzi wzorcowej
		fclose(user_output);
		return 3;	
	}

	line = 1;
	do {
		user_buff_read = getline(&user_buff, &user_buff_len, user_output);
		orig_buff_read = getline(&orig_buff, &orig_buff_len, orig_output);

		if (user_buff_read != -1)
			user_buff_read = usun_biale_znaki(user_buff, user_buff_read-1);

		if (orig_buff_read != -1)
			orig_buff_read = usun_biale_znaki(orig_buff, orig_buff_read-1);

		// Rozna dlugosc wierszy po usunieciu bialych znakow na koncach
		if (user_buff_read != orig_buff_read && user_buff_read+orig_buff_read != -1) {
			printf("0\nLinia %d: Wczytano %d znakow, oczekiwano %d (nie liczac bialych znakow na koncu wiersza)\n", line, user_buff_read, orig_buff_read);
			free(user_buff);
			free(orig_buff);
			fclose(user_output);
			fclose(orig_output);
			return 0;
		}

		for (pos=0; pos<user_buff_read && user_buff[pos]==orig_buff[pos]; pos++) ;
		if (pos<user_buff_read) {
			printf("0\nLinia %d, znak %d: Wczytano '%c', oczekiwano '%c'\n", line, pos+1, user_buff[pos], orig_buff[pos]);
			free(user_buff);
			free(orig_buff);
			fclose(user_output);
			fclose(orig_output);
			return 0;
		}
		line++;
	} while (user_buff_read != -1 || orig_buff_read != -1);

	printf("100\n");
	free(user_buff);
	free(orig_buff);
	fclose(user_output);
	fclose(orig_output);
	return 0;
}
