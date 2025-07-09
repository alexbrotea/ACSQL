#include "task1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "../include/structuri.h"

#define BUF_SIZE 256
#define NUM_FIELDS 5

static void trim(char *s) {
    while (isspace((unsigned char)*s)) {
        memmove(s, s + 1, strlen(s));
    }
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[--len] = '\0';
    }
}

secretariat *citeste_secretariat(const char *nume_fisier) {
    FILE *fp = fopen(nume_fisier, "r");
    if (!fp) {
        return NULL;
    }

    secretariat *s = malloc(sizeof(secretariat));
    if (!s) {
        fclose(fp);
        return NULL;
    }

    s->studenti = NULL;
    s->nr_studenti = 0;
    s->materii = NULL;
    s->nr_materii = 0;
    s->inrolari = NULL;
    s->nr_inrolari = 0;

    enum {
        STATE_NONE,
        STATE_STUDENTI,
        STATE_MATERII,
        STATE_INROLARI
    } parser_state = STATE_NONE;

    char buffer[BUF_SIZE];

    while (fgets(buffer, sizeof(buffer), fp)) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        char *tmp = buffer;
        while (isspace((unsigned char)*tmp)) {
            tmp++;
        }
        if (*tmp == '\0') {
            continue;
        }

        if (strcmp(tmp, "[STUDENTI]") == 0) {
            parser_state = STATE_STUDENTI;
            continue;
        } else if (strcmp(tmp, "[MATERII]") == 0) {
            parser_state = STATE_MATERII;
            continue;
        } else if (strcmp(tmp, "[INROLARI]") == 0) {
            parser_state = STATE_INROLARI;
            continue;
        }

        if (parser_state == STATE_STUDENTI) {
            int id = 0, an = 0;
            char stat = '\0';
            char nume[BUF_SIZE];

            int fields = sscanf(tmp, " %d , %[^,] , %d , %c",
                                &id, nume, &an, &stat);
            if (fields == NUM_FIELDS - 1) {
                trim(nume);

                student *temp_stud = realloc(s->studenti,
                    (s->nr_studenti + 1) * sizeof(student));
                if (!temp_stud) {
                    fclose(fp);
                    free(s->studenti);
                    free(s);
                    return NULL;
                }
                s->studenti = temp_stud;

                s->studenti[s->nr_studenti].id = id;
                strncpy(s->studenti[s->nr_studenti].nume, nume, MAX_STUDENT_NAME - 1);
                s->studenti[s->nr_studenti].nume[MAX_STUDENT_NAME - 1] = '\0';
                s->studenti[s->nr_studenti].an_studiu = an;
                s->studenti[s->nr_studenti].statut = stat;
                s->studenti[s->nr_studenti].medie_generala = 0.0f;

                s->nr_studenti++;
            }
        } else if (parser_state == STATE_MATERII) {
            int id = 0;
            char denumire[BUF_SIZE], titular[BUF_SIZE];

            int fields = sscanf(tmp, " %d , %[^,] , %[^\n]", &id, denumire, titular);
            if (fields == NUM_FIELDS - 2) {
                trim(denumire);
                trim(titular);

                materie *temp_mat = realloc(s->materii,
                    (s->nr_materii + 1) * sizeof(materie));
                if (!temp_mat) {
                    fclose(fp);
                    free(s->materii);
                    free(s->studenti);
                    free(s);
                    return NULL;
                }
                s->materii = temp_mat;

                s->materii[s->nr_materii].id = id;
                s->materii[s->nr_materii].nume = strdup(denumire);
                s->materii[s->nr_materii].nume_titular = strdup(titular);

                s->nr_materii++;
            }
        } else if (parser_state == STATE_INROLARI) {
            int id_stud = 0, id_mat = 0;
            float n1 = 0.0f, n2 = 0.0f, n3 = 0.0f;
            int fields = sscanf(tmp, " %d , %d , %f %f %f",
                                &id_stud, &id_mat, &n1, &n2, &n3);
            if (fields == NUM_FIELDS) {
                inrolare *temp_inr = realloc(s->inrolari,
                    (s->nr_inrolari + 1) * sizeof(inrolare));
                if (!temp_inr) {
                    fclose(fp);
                    for (int i = 0; i < s->nr_materii; i++) {
                        free(s->materii[i].nume);
                        free(s->materii[i].nume_titular);
                    }
                    free(s->materii);
                    free(s->studenti);
                    free(s);
                    return NULL;
                }
                s->inrolari = temp_inr;

                s->inrolari[s->nr_inrolari].id_student = id_stud;
                s->inrolari[s->nr_inrolari].id_materie = id_mat;
                s->inrolari[s->nr_inrolari].note[0] = n1;
                s->inrolari[s->nr_inrolari].note[1] = n2;
                s->inrolari[s->nr_inrolari].note[2] = n3;

                s->nr_inrolari++;
            }
        }
    }

    fclose(fp);

    for (int i = 0; i < s->nr_studenti; i++) {
        float suma = 0.0f;
        int count = 0;
        for (int j = 0; j < s->nr_inrolari; j++) {
            if (s->inrolari[j].id_student == s->studenti[i].id) {
                float totalNote =
                    s->inrolari[j].note[0] +
                    s->inrolari[j].note[1] +
                    s->inrolari[j].note[2];
                suma += totalNote;
                count++;
            }
        }
        if (count > 0) {
            float medie = (float)suma / (float)count;
            s->studenti[i].medie_generala = medie;
        } else {
            s->studenti[i].medie_generala = 0.0f;
        }
    }

    return s;
}

void adauga_student(secretariat *s, int id, char *nume, int an_studiu, char statut, float medie_generala) {
    if (!s) return;

    s->nr_studenti++;
    s->studenti = (student *)realloc(s->studenti, s->nr_studenti * sizeof(student));
    if (!s->studenti) {
        s->nr_studenti--;
        return;
    }

    student *new_student = &s->studenti[s->nr_studenti - 1];
    new_student->id = id;
    strncpy(new_student->nume, nume, MAX_STUDENT_NAME - 1);
    new_student->nume[MAX_STUDENT_NAME - 1] = '\0';
    new_student->an_studiu = an_studiu;
    new_student->statut = statut;
    new_student->medie_generala = medie_generala;
}

void elibereaza_secretariat(secretariat **s) {
    if (!s || !(*s)) return;

    free((*s)->studenti);

    if ((*s)->materii) {
        for (int i = 0; i < (*s)->nr_materii; i++) {
            free((*s)->materii[i].nume);
            free((*s)->materii[i].nume_titular);
        }
        free((*s)->materii);
    }

    free((*s)->inrolari);
    free(*s);
    *s = NULL;
}
