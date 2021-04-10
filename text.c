#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "util/task_helper.h"

#define IS_ALPHA(C) (IS_UPPER(C) || IS_LOWER(C)) //macro pentru alphanumerice
#define IS_UPPER(C) (C >= 'A' && C <= 'Z') // macro pentru majuscule
#define IS_LOWER(C) (C >= 'a' && C <= 'z') // macro pentru minuscule

// Aceasta functie face ca primul caracter din fiecare linie sa fie majusucla
void uppercase(char *buffer) {
	
	int primul = 1;// primul caracter citit trebuie sa fie majuscula
	while (*buffer != 0) { // iteram prin tot buffer-ul
		// daca avem caracter-ul \n atunci urmatorul caracter citit trebuie sa
		// fie majuscula
		if (*buffer == '\n') {
			primul = 1;
		} else {
			// daca e primul caracter si e minisucula trebuie convertit
			if (primul == 1 && IS_LOWER(*buffer)) {
				*buffer-=32;
			// daca nu e primul caracter si e majuscula trebuie convertit
			} else if (primul == 0 && IS_UPPER(*buffer)) {
				*buffer+=32;
			}
			// daca s-a ajuns aici rezulta ca nu e primul character
			primul = 0;
		}
		buffer++;
	}
	return;
}

// Aceasta functie scoate fiecare caracter nedorit din buffer
void trimming(char *buffer) {
	int spatii = 0; // counter pentru numarul de spatii
	char *citeste = buffer; // citim cu "citeste" pentru a pastra "buffer"
	while (*buffer != 0) {
		// verificam daca caracterul curent este valid
		if (strchr(" .,;!?:", *buffer) == NULL) { // este valid
			*citeste = *buffer; // scrie caracterul valid in acelasi buffer
			citeste++;
			spatii = 0; // nr de spatii trebui de resetat
			// verificam daca e spatiu si se poate de scris
		} else if (*buffer == ' ' && spatii == 0) { 
			// scriem un spatiu
			*citeste = *buffer;
			citeste++;
			spatii++; // nr de spatii respectiv creste
		}
		buffer++;
	}
	*citeste = 0; // punem un caracter nul la capat pentru afisare corecta
	return;
}

// Schimba aleator din majuscula in minuscula in baza unei probabilitati
void silly(char *buffer, float p) {
	while (*buffer != 0) {
		if (IS_UPPER(*buffer)) { // daca e majuscula
			// verificam daca putem schimba din majuscula in minuscula
			if ((rand() % 100 / 99.0) < p) {
				*buffer+=32;
			}
		} else if (IS_LOWER(*buffer)) { // daca e minuscula
			// verificam daca putem schimba din minuscula in majuscula
			if ((rand() % 100 / 99.0) < p) {
				*buffer-=32;
			}
		}
		buffer++;
	}
	return;
}

// Functie aditionala pentru a verifica daca doua cuvinte rimeaza
// (se considera ca rimeaza cuvintele care au acelasi caracter de sfarsit)
int rimeaza(char *str1, char *str2) {
	// obtinem ultimele caractere din cuvant
	char c1 = str1[strlen(str1) - 1], c2 = str2[strlen(str2) - 1];
	if (IS_UPPER(c1)) { // avem nevoie de minuscule
		c1+=32;
	}
	if (IS_UPPER(c2)) { // avem nevoie de minuscule
		c2+=32;
	}
	return c1 == c2; // intoarcem daca ultimele caractere rimeaza
}

// Functie aditionala pentru a obtine un cuvant dintr-un tablou (table)
// care rimeaza cu un cuvant (ref)
char *obtine_rima(char **table, int length, char *ref) {
	char c = ref[strlen(ref) - 1];// obtinem ultimul caracter
	char *min = NULL; // e cuvant cul mai mic lexicografic
	if (IS_UPPER(c)) { // avem nevoie de minuscule
		c += 32;
	}
	for (int i = 0; i < length; i++) { // pentru fiecare cuvant din caracter
		if (table[i][strlen(table[i]) - 1] == c) {
			if (min == NULL || strcmp(min, table[i]) > 0) { // gasim minim
				min = table[i]; // setam minim
			}
		}
	}
	return min; // returnam cuvantul sau null
}

// Aceasta functie face ca textul sa rimeze
void rhimy(char *buffer, char *type) {
	char tmp[4096];
	char word_a[16], word_b[16], word_c[16], word_d[16];
	char caracter;
	int length, length_ta, length_tb, length_tc, length_td;
	char **word_table, **word_ta, **word_tb, **word_tc, **word_td;
	word_table = word_ta = word_tb = word_tc = word_td = NULL;
	char *sinonim, *word, *word_pa, *word_pb, *word_pc, *word_pd;
	sinonim = word = word_pa = word_pb = word_pc = word_pd = NULL;
	char *word_one, *word_two, *word_three, *word_four;
	word_one = word_two = word_three = word_four = NULL;
	char *citeste = buffer, *citeste2 = buffer;
	char *scrie = tmp;
	int lines = 0;
	while (*citeste != 0) {
		while (*citeste != '\n' && *citeste != 0) { // citim pana la newline
			if (word == NULL && IS_ALPHA(*citeste)) { // inceputul unui cuvant
				word = citeste; // memoram cuvantul
			}
			// daca ajungem la capat de cuvant
			if (word != NULL && !IS_ALPHA(*(citeste + 1))) {
				caracter = *(citeste + 1);
				*(citeste + 1) = 0; // izolam cuvantul din string
				if (lines % 5 == 0) { // care linie din strofa suntem (linia 1)
					strcpy(word_a, word); // in cuvantul corespunzator liniei
					word_pa = word; // un pointer in string spre inceputul cuv.
				} else if (lines % 5 == 1) { // pentru linia 2
					strcpy(word_b, word);
					word_pb = word;
				} else if (lines % 5 == 2) { // pentru linia 3
					strcpy(word_c, word);
					word_pc = word;
				} else if (lines % 5 == 3) { // pentru linia 4
					strcpy(word_d, word);
					word_pd = word;
				}
				*(citeste + 1) = caracter; // intoarcem caracterul inlocuit
				word = NULL; // cautam un nou cuvant
			}
			citeste++;
		}
		if (lines % 5 == 3) { // ajungem aici cand am terminat cu strofa
			// obtinem tabelele de sinonime pentru fiecare cuvant
			get_synonym(word_a, &length_ta, &word_ta);
			get_synonym(word_b, &length_tb, &word_tb);
			get_synonym(word_c, &length_tc, &word_tc);
			get_synonym(word_d, &length_td, &word_td);
			// "sinonim" este sinonimul cel mai mic lexicografic
			if (strcmp(type, "imperecheata") == 0) { // verificam tipul rimei
				if (!rimeaza(word_a, word_b)) { // verficam daca rimeaza
					sinonim = obtine_rima(word_ta, length_ta, word_b);
					if (sinonim == NULL) { // daca nu rimeaza sinonim cu word_b
						// obtinem cuvantul sinonim care rimeaza cu word_a
						sinonim = obtine_rima(word_tb, length_tb, word_a);
						if (sinonim != NULL) { // daca rimeaza sinonim cu word_a
							// pentru linia doi din strofa
							// vom scrie sinonim
							word_two = sinonim;
						}
					} else {// daca rimeaza sinonim cu word_b
						// pentru linia unu din strofa
						// vom scrie sinonim
						word_one = sinonim; 
					}
				}
				if (!rimeaza(word_c, word_d)) { // daca nu rimeaza
					sinonim = obtine_rima(word_tc, length_tc, word_d); //
					if (sinonim == NULL) { // daca nu rimeaza sinonim cu word_d
						// obtinem cuvantul sinonim care rimeaza cu word_c
						sinonim = obtine_rima(word_td, length_td, word_c);
						if (sinonim != NULL) { // daca rimeaza sinonim cu word_c
							// pentru linia patru din strofa
							// vom scrie sinonim
							word_four = sinonim;
						}
					} else {// daca rimeaza sinonim cu word_b
						// pentru linia trei din strofa
						word_three = sinonim;
					}
				}
			} else if (strcmp(type, "incrucisata") == 0) {
				if (!rimeaza(word_a, word_c)) {
					sinonim = obtine_rima(word_ta, length_ta, word_c);
					if (sinonim == NULL) {
						sinonim = obtine_rima(word_tc, length_tc, word_a);
						if (sinonim != NULL) {
							word_three = sinonim;
						}
					} else {
						word_one = sinonim;
					}
				}
				if (!rimeaza(word_b, word_d)) {
					sinonim = obtine_rima(word_tb, length_tb, word_d);
					if (sinonim == NULL) {
						sinonim = obtine_rima(word_td, length_td, word_b);
						if (sinonim != NULL) {
							word_four = sinonim;
						}
					} else {
						word_two = sinonim;
					}
				}
			} else { // pentru imbratisata
				if (!rimeaza(word_a, word_d)) {
					sinonim = obtine_rima(word_ta, length_ta, word_d);
					if (sinonim == NULL) {
						sinonim = obtine_rima(word_td, length_td, word_a);
						if (sinonim != NULL) {
							word_four = sinonim;
						}
					} else {
						word_one = sinonim;
					}
				}
				if (!rimeaza(word_b, word_c)) {
					sinonim = obtine_rima(word_tb, length_tb, word_c);
					if (sinonim == NULL) {
						sinonim = obtine_rima(word_tc, length_tc, word_b);
						if (sinonim != NULL) {
							word_three = sinonim;
						}
					} else {
						word_two = sinonim;
					}
				}
			}
			while (citeste2 < word_pa) { // scriem in tmp tot pana la aparitia
				// primului cuvant
				*scrie = *citeste2;
				citeste2++;
				scrie++;
			}
			if (word_one != NULL) { // cand ajungem la acest cuvant verificam
				// daca i s-a gasit un sinonim care rimeaza
				strcpy(scrie, word_one); // daca da scriem cuvantul
				citeste2+=strlen(word_a); // sarim peste cuvatul original
				scrie+=strlen(word_one); // sarim peste cuvantul nou scris
			}
			while (citeste2 < word_pb) { // la fel ca mai sus pentru al doilea
				*scrie = *citeste2;
				citeste2++;
				scrie++;
			}
			if (word_two != NULL) {
				strcpy(scrie, word_two);
				citeste2+=strlen(word_b);
				scrie+=strlen(word_two);
			}
			while (citeste2 < word_pc) { // la fel ca mai sus pentru al treilea
				*scrie = *citeste2;
				citeste2++;
				scrie++;
			}
			if (word_three != NULL) {
				strcpy(scrie, word_three);
				citeste2+=strlen(word_c);
				scrie+=strlen(word_three);
			}
			while (citeste2 < word_pd) { // la fel ca mai sus pentru al patrulea
				*scrie = *citeste2;
				citeste2++;
				scrie++;
			}
			if (word_four != NULL) {
				strcpy(scrie, word_four);
				citeste2+=strlen(word_d);
				scrie+=strlen(word_four);
			}
			while (citeste2 < citeste) { // scriem restul caracterelor pana la
				// urmatoarea strofa or capat
				*scrie = *citeste2;
				citeste2++;
				scrie++;
			}
		}
		// resetam valorile
		sinonim = word = word_one = word_two = word_three = word_four = NULL;
		lines++;
		citeste++;
	}
	*scrie = 0; // pentru citire si afisare corecta
	// scriem tot din tmp in buffer-ul original
	for (citeste = tmp; *citeste != 0; citeste++) { 
		*buffer = *citeste;
		buffer++;
	}
	*buffer = 0;
	return;
}

void friendly(char *buffer) {
	
	char *citeste = buffer;
	char tmp[4096];
	char *word = NULL;
	char *scrie = tmp;
	char *friendly_word;
	while (*citeste != 0) {
		*scrie = *citeste;
		if (word == NULL && IS_ALPHA(*citeste)) { // inceputul unui cuvant
			word = scrie;
		}
		// daca ajungem la capat de cuvant
		if (word != NULL && !IS_ALPHA(*(citeste + 1))) {
			// izolam cuvantul
			*(scrie + 1) = 0;
			get_friendly_word(word, &friendly_word); // obtinem un diminutiv
			if (friendly_word != NULL) { // daca e valid diminutivul
				strcpy(word, friendly_word); // scriem diminutivul
				scrie = word; // sarim peste cuvantul original
				scrie += strlen(friendly_word) - 1; // sarim peste diminutiv
			}
			word = NULL;
		}
		citeste++;
		scrie++;
	}
	// scriem tot ce am obtinum in bufferul original
	*scrie = 0;
	for (scrie = tmp; *scrie != 0; scrie++) {
		*buffer = *scrie;
		buffer++;
	}
	// punem caracterul nul pentru afisare corecta
	*buffer = 0;
	return;
}

int main(void) {
	char command[32]; // commanda citita de la tastatura
	char buffer[4096]; // bufferul pentru transformari
	char *tok; // fiecare argument din comanda
	srand(42);
	// apelarea functiilor in dependenta de comanda
	while (fgets(command, 32, stdin) != NULL) {
		tok = strtok(command, " \n");
		if (!strcmp(tok, "exit")) {
			return 0;
		} else if (!strcmp(tok, "load")) {
			load_file(strtok(NULL, " \n"), buffer);
		} else if (!strcmp(tok, "uppercase")) {
			uppercase(buffer);
		} else if (!strcmp(tok, "print")) {
			printf("%s\n\n", buffer);
		} else if (!strcmp(tok, "trimming")) {
			trimming(buffer);
		} else if (!strcmp(tok, "make_it_silly")) {
			tok = strtok(NULL, " \n");
			silly(buffer, atof(tok));
		} else if (!strcmp(tok, "make_it_friendlier")) {
			friendly(buffer);
		} else if (!strcmp(tok, "make_it_rhyme")) {
			rhimy(buffer, strtok(NULL, " \n"));
		}
	}
	return 1;
}