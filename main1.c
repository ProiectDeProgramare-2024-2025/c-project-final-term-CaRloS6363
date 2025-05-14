#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MAX_CV 100
#define MAX_FIELD 100
#define MAX_PREV_POS 5
#define DATA_FILE "cv_data.txt"

#define COLOR_RESET "\033[0m"
#define COLOR_CYAN "\033[36m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_GREEN "\033[32m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_RED "\033[31m"
#define COLOR_BOLD "\033[1m"

typedef struct {
    char nume[MAX_FIELD];
    char prenume[MAX_FIELD];
    char telefon[20];
    int varsta;
    char facultate[MAX_FIELD];
    char licenta[MAX_FIELD];
    char master[MAX_FIELD];
    char pozitii[MAX_PREV_POS][MAX_FIELD];
    int nrPozitii;
} CV;

int validText(const char *s) {
    if (strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++)
        if (!isalpha((unsigned char)s[i]) && !isspace((unsigned char)s[i]))
            return 0;
    return 1;
}

int validName(const char *s) {
    if (strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++)
        if (!isalpha((unsigned char)s[i]))
            return 0;
    return 1;
}

int validTelefon(const char *s) {
    if (strlen(s) != 10) return 0;
    for (int i = 0; i < 10; i++)
        if (!isdigit((unsigned char)s[i])) return 0;
    return 1;
}

int validVarsta(int v) {
    return (v >= 18 && v <= 100);
}

int validCount(const char *s) {
    if (strlen(s) == 0) return 0;
    for (int i = 0; s[i]; i++)
        if (!isdigit((unsigned char)s[i]))
            return 0;
    int val = atoi(s);
    return (val >= 0 && val <= MAX_PREV_POS);
}

void clearScreen() {
    system("clear");
}

void afiseazaCV(const CV *cv, int idx) {
    printf(COLOR_CYAN "=== CV %d ===\n" COLOR_RESET, idx + 1);
    printf(COLOR_CYAN "Nume complet: " COLOR_RESET);
    printf(COLOR_GREEN "%s " COLOR_MAGENTA "%s\n" COLOR_RESET, cv->nume, cv->prenume);
    printf(COLOR_CYAN "Varsta: " COLOR_RESET);
    printf(COLOR_YELLOW "%d\n" COLOR_RESET, cv->varsta);
    printf(COLOR_CYAN "Telefon: " COLOR_RESET);
    printf(COLOR_YELLOW "%s\n" COLOR_RESET, cv->telefon);
    printf(COLOR_CYAN "Facultate: " COLOR_RESET);
    printf(COLOR_GREEN "%s\n" COLOR_RESET, cv->facultate);
    printf(COLOR_CYAN "Licenta: " COLOR_RESET);
    printf(COLOR_MAGENTA "%s\n" COLOR_RESET, cv->licenta);
    printf(COLOR_CYAN "Master: " COLOR_RESET);
    printf(COLOR_MAGENTA "%s\n" COLOR_RESET, cv->master);
    printf(COLOR_CYAN "Pozitii anterioare:\n" COLOR_RESET);
    for (int j = 0; j < cv->nrPozitii; j++) {
        const char *color = (j % 2 == 0) ? COLOR_YELLOW : COLOR_GREEN;
        printf("  %d. %s%s%s\n", j + 1, color, cv->pozitii[j], COLOR_RESET);
    }
    printf("\n");
}

int contineCuvantCheie(const CV *cv, const char *kw) {
    if (strcasestr(cv->nume, kw) || strcasestr(cv->prenume, kw) ||
        strcasestr(cv->telefon, kw) || strcasestr(cv->facultate, kw) ||
        strcasestr(cv->licenta, kw) || strcasestr(cv->master, kw))
        return 1;
    for (int i = 0; i < cv->nrPozitii; i++)
        if (strcasestr(cv->pozitii[i], kw)) return 1;
    return 0;
}

int meniu() {
    int opt;
    printf(COLOR_BOLD COLOR_CYAN "===== Meniu Gestionare CV =====\n" COLOR_RESET);
    printf(COLOR_YELLOW "1. Creare CV nou\n" COLOR_RESET);
    printf(COLOR_YELLOW "2. Vizualizeaza toate CV-urile\n" COLOR_RESET);
    printf(COLOR_YELLOW "3. Cauta CV dupa cuvant cheie\n" COLOR_RESET);
    printf(COLOR_YELLOW "4. Iesire\n" COLOR_RESET);
    printf(COLOR_CYAN "Optiune: " COLOR_RESET);
    if (scanf("%d", &opt) != 1) opt = 0;
    while (getchar() != '\n');
    return opt;
}

CV creazaCV() {
    clearScreen();
    CV cv;
    char buf[MAX_FIELD];
    int n;
    do {
        printf(COLOR_CYAN "Introdu nume (doar litere): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!validName(buf))
            printf(COLOR_RED "Eroare: Numele trebuie sa contina doar litere.\n" COLOR_RESET);
    } while (!validName(buf));
    strcpy(cv.nume, buf);
    do {
        printf(COLOR_CYAN "Introdu prenume (litere si spatii): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!validText(buf))
            printf(COLOR_RED "Eroare: Prenumele invalid.\n" COLOR_RESET);
    } while (!validText(buf));
    strcpy(cv.prenume, buf);
    do {
        printf(COLOR_CYAN "Introdu telefon (10 cifre): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!validTelefon(buf))
            printf(COLOR_RED "Eroare: Telefon invalid.\n" COLOR_RESET);
    } while (!validTelefon(buf));
    strcpy(cv.telefon, buf);
    do {
        printf(COLOR_CYAN "Introdu varsta (18-100): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        cv.varsta = isdigit((unsigned char)buf[0]) ? atoi(buf) : -1;
        if (!validVarsta(cv.varsta))
            printf(COLOR_RED "Eroare: Varsta invalidă.\n" COLOR_RESET);
    } while (!validVarsta(cv.varsta));
    do {
        printf(COLOR_CYAN "Introdu facultate (doar litere): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!validName(buf))
            printf(COLOR_RED "Eroare: Facultatea trebuie sa contina doar litere.\n" COLOR_RESET);
    } while (!validName(buf));
    strcpy(cv.facultate, buf);
    do {
        printf(COLOR_CYAN "Introdu licenta (litere/spatii/N/A): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!(validText(buf) || strcmp(buf, "N/A") == 0))
            printf(COLOR_RED "Eroare: Licenta invalida.\n" COLOR_RESET);
    } while (!(validText(buf) || strcmp(buf, "N/A") == 0));
    strcpy(cv.licenta, buf);
    do {
        printf(COLOR_CYAN "Introdu master (litere/spatii/N/A): " COLOR_RESET);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!(validText(buf) || strcmp(buf, "N/A") == 0))
            printf(COLOR_RED "Eroare: Master invalid.\n" COLOR_RESET);
    } while (!(validText(buf) || strcmp(buf, "N/A") == 0));
    strcpy(cv.master, buf);
    do {
        printf(COLOR_CYAN "Numar pozitii anterioare (0-%d): " COLOR_RESET, MAX_PREV_POS);
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = '\0';
        if (!validCount(buf))
            printf(COLOR_RED "Eroare: Introdu un numar valid (0-%d).\n" COLOR_RESET, MAX_PREV_POS);
        else
            n = atoi(buf);
    } while (!validCount(buf));
    cv.nrPozitii = n;
    for (int i = 0; i < n; i++) {
        do {
            printf(COLOR_CYAN "Pozitia %d (litere/spatii): " COLOR_RESET, i + 1);
            fgets(cv.pozitii[i], sizeof(cv.pozitii[i]), stdin);
            cv.pozitii[i][strcspn(cv.pozitii[i], "\n")] = '\0';
            if (!validText(cv.pozitii[i]))
                printf(COLOR_RED "Eroare: Pozitia invalida.\n" COLOR_RESET);
        } while (!validText(cv.pozitii[i]));
    }
    return cv;
}

void salvareCVappend(const CV *cv) {
    FILE *f = fopen(DATA_FILE, "a");
    if (!f) { perror("Eroare la deschidere"); return; }
    setbuf(f, NULL);
    fprintf(f, "%s\n%s\n%s\n%d\n%s\n%s\n%s\n%d\n",
            cv->nume, cv->prenume, cv->telefon, cv->varsta,
            cv->facultate, cv->licenta, cv->master, cv->nrPozitii);
    for (int i = 0; i < cv->nrPozitii; i++)
        fprintf(f, "%s\n", cv->pozitii[i]);
    fflush(f); fsync(fileno(f)); fclose(f);
}

int incarcaCVuriDinFisier(CV arr[]) {
    FILE *f = fopen(DATA_FILE, "r");
    if (!f) return 0;
    int count = 0;
    char buf[MAX_FIELD];
    while (count < MAX_CV && fgets(buf, sizeof(buf), f)) {
        buf[strcspn(buf, "\n")] = '\0';
        if (!validText(buf)) continue;
        strcpy(arr[count].nume, buf);
        fgets(arr[count].prenume, MAX_FIELD, f); arr[count].prenume[strcspn(arr[count].prenume, "\n")] = '\0';
        fgets(arr[count].telefon, 20, f); arr[count].telefon[strcspn(arr[count].telefon, "\n")] = '\0';
        fgets(buf, MAX_FIELD, f); arr[count].varsta = atoi(buf);
        fgets(arr[count].facultate, MAX_FIELD, f); arr[count].facultate[strcspn(arr[count].facultate, "\n")] = '\0';
        fgets(arr[count].licenta, MAX_FIELD, f); arr[count].licenta[strcspn(arr[count].licenta, "\n")] = '\0';
        fgets(arr[count].master, MAX_FIELD, f); arr[count].master[strcspn(arr[count].master, "\n")] = '\0';
        fgets(buf, MAX_FIELD, f); arr[count].nrPozitii = atoi(buf);
        for (int i = 0; i < arr[count].nrPozitii; i++) {
            fgets(arr[count].pozitii[i], MAX_FIELD, f);
            arr[count].pozitii[i][strcspn(arr[count].pozitii[i], "\n")] = '\0';
        }
        count++;
    }
    fclose(f);
    return count;
}

void vizualizeazaCVuri(const CV arr[], int count) {
    clearScreen();
    if (count == 0) {
        printf(COLOR_RED "Nu exista CV-uri.\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < count; i++)
        afiseazaCV(&arr[i], i);
}

void cautaCV(const CV arr[], int count) {
    clearScreen();
    if (count == 0) {
        printf(COLOR_RED "Nu exista CV-uri.\n" COLOR_RESET);
        return;
    }
    char kw[MAX_FIELD];
    printf(COLOR_CYAN "Cuvant cheie: " COLOR_RESET);
    fgets(kw, MAX_FIELD, stdin); kw[strcspn(kw, "\n")] = '\0';
    clearScreen();
    int gasit = 0;
    for (int i = 0; i < count; i++) {
        if (contineCuvantCheie(&arr[i], kw)) {
            afiseazaCV(&arr[i], i);
            gasit = 1;
        }
    }
    if (!gasit)
        printf(COLOR_RED "Niciun CV cu '%s'.\n" COLOR_RESET, kw);
}

void runApp() {
    CV cvs[MAX_CV];
    int cnt;
    while (1) {
        clearScreen();
        int opt = meniu();
        switch (opt) {
            case 1: {
                CV cv = creazaCV();
                salvareCVappend(&cv);
                printf(COLOR_GREEN "CV salvat!\n" COLOR_RESET);
                printf("Apasa Enter..."); getchar();
                break;
            }
            case 2:
                cnt = incarcaCVuriDinFisier(cvs);
                vizualizeazaCVuri(cvs, cnt);
                printf("Apasa Enter..."); getchar();
                break;
            case 3:
                cnt = incarcaCVuriDinFisier(cvs);
                cautaCV(cvs, cnt);
                printf("Apasa Enter..."); getchar();
                break;
            case 4:
                clearScreen();
                printf(COLOR_CYAN "La revedere!\n" COLOR_RESET);
                return;
            default:
                printf(COLOR_RED "Optiune invalida!\n" COLOR_RESET);
                printf("Apasa Enter..."); getchar();
        }
    }
}

int main() {
    runApp();
    return 0;
}

