#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include "morse.h"

/**
 * @brief Repräsentiert eine Zuordnung von Klartextzeichen zu Morsecode.
 *
 * Diese Struktur wird verwendet, um ein alphanumerisches Zeichen (oder Symbol)
 * einem zugehörigen Morsecode-String zuzuordnen.
 */
typedef struct {
    const char* character;  /**< Das Klartextzeichen (einzelnes Zeichen als String). */
    const char* morse;      /**< Der entsprechende Morsecode (z. B. ".-" für A). */
} MorseMapping;

/**
 * @brief Statische Tabelle mit allen unterstützten Morsecode-Zuordnungen.
 *
 * Diese Tabelle enthält die vollständige Mapping-Tabelle für Buchstaben A–Z,
 * Ziffern 0–9 sowie zugelassene Satz- und Sonderzeichen.
 */
static MorseMapping morse_map[] = {
    {"A", ".-"}, {"B", "-..."}, {"C", "-.-."}, {"D", "-.."}, {"E", "."},
    {"F", "..-."}, {"G", "--."}, {"H", "...."}, {"I", ".."}, {"J", ".---"},
    {"K", "-.-"}, {"L", ".-.."}, {"M", "--"}, {"N", "-."}, {"O", "---"},
    {"P", ".--."}, {"Q", "--.-"}, {"R", ".-."}, {"S", "..."}, {"T", "-"},
    {"U", "..-"}, {"V", "...-"}, {"W", ".--"}, {"X", "-..-"}, {"Y", "-.--"},
    {"Z", "--.."},
    {"0", "-----"}, {"1", ".----"}, {"2", "..---"}, {"3", "...--"},
    {"4", "....-"}, {"5", "....."}, {"6", "-...."}, {"7", "--..."},
    {"8", "---.."}, {"9", "----."},
    {".", ".-.-.-"}, {",", "--..--"}, {":", "---..."}, {";", "-.-.-."},
    {"?", "..--.."}, {"=", "-...-"}, {"-", "-....-"}, {"+", ".-.-."},
    {"_", "..--.-"}, {"(", "-.--."}, {")", "-.--.-"}, {"/", "-..-."},
    {"@", ".--.-."}
};

/**
 * @brief Länge der Morsecode-Mapping-Tabelle.
 *
 * Wird zur Iteration über das `morse_map`-Array verwendet.
 */
static const size_t map_len = sizeof(morse_map) / sizeof(MorseMapping);

/**
 * @brief Konvertiert ein einzelnes Zeichen in den zugehörigen Morsecode.
 *
 * Das übergebene Zeichen wird in Großbuchstaben umgewandelt und mit der internen
 * Morsecode-Tabelle verglichen. Wird keine passende Zuordnung gefunden, gibt die
 * Funktion einen Stern ('*') zurück.
 *
 * @param c Das zu konvertierende Zeichen.
 * @return Zeiger auf den Morsecode-String oder "*" bei unbekanntem Zeichen.
 */
const char* encode_char(char c) {
    char upper = toupper((unsigned char)c);
    char str[2] = {upper, '\0'};
    for (size_t i = 0; i < map_len; ++i) {
        if (strcmp(str, morse_map[i].character) == 0) {
            return morse_map[i].morse;
        }
    }
    return "*";
}

/**
 * @brief Wandelt einen Morsecode-String in ein einzelnes Zeichen zurück.
 *
 * Die Funktion vergleicht den übergebenen Morsecode mit der Mapping-Tabelle
 * und gibt das zugehörige Zeichen zurück. Bei einem unbekannten Code wird
 * ein Stern ('*') zurückgegeben.
 *
 * @param code Nullterminierter String mit einem Morsecode (z. B. ".-" für A).
 * @return Der Klartextbuchstabe (Großbuchstabe, Ziffer oder Symbol) oder '*' bei Fehler.
 */
char decode_morse(const char* code) {
    for (size_t i = 0; i < map_len; ++i) {
        if (strcmp(code, morse_map[i].morse) == 0) {
            return morse_map[i].character[0];
        }
    }
    return '*';
}

/**
 * @brief Codiert einen vollständigen Text in Morsecode und gibt ihn auf stdout aus.
 *
 * Die Funktion wandelt Zeichen des Eingabetexts entsprechend der Mapping-Tabelle
 * in Morsecode um. Nicht unterstützte Zeichen werden als Stern ('*') dargestellt.
 *
 * Buchstaben werden durch ein Leerzeichen getrennt, Wörter wahlweise durch drei Leerzeichen
 * oder (wenn @p slash_mode aktiviert ist) durch den Separator " / ".
 *
 * @param text Nullterminierter C-String mit dem Eingabetext.
 * @param slash_mode true = Worttrennung mit Slash; false = drei Leerzeichen.
 */
void encode_text(const char* text, bool slash_mode) {
    bool first_word = true;
    bool first_char = true;
    while (*text) {
        if (*text == '\n' || *text == '\r') {
            text++;
            continue;
        } else if (*text == ' ') {
            if (!first_char) {
                printf(slash_mode ? " / " : "   ");
            }
            text++;
            first_char = true;
        } else {
            if (!first_word && !first_char) printf(" ");
            const char* code = encode_char(*text);
            printf("%s", code);
            text++;
            first_word = false;
            first_char = false;
        }
    }
    putchar('\n');
}

/**
 * @brief Dekodiert einen Morsecode-Text zu Klartext und gibt ihn aus.
 *
 * Die Funktion verarbeitet Morsezeichen, die durch Leerzeichen oder Slashes getrennt sind.
 * Einfache Leerzeichen trennen Buchstaben, drei Leerzeichen oder ein Slash trennen Wörter.
 *
 * Ungültige Morsezeichen werden durch '*' ersetzt. Zeilenumbrüche werden ignoriert.
 *
 * @param morse_input Nullterminierter String mit Morsecode.
 */
void decode_text(const char* morse_input) {
    char buffer[16] = {0};
    int idx = 0;
    int space_count = 0;

    while (*morse_input) {
        if (*morse_input == '.' || *morse_input == '-') {
            if (idx < sizeof(buffer) - 1) {
                buffer[idx++] = *morse_input;
            }
            space_count = 0;
        } else if (*morse_input == ' ' || *morse_input == '/') {
            if (idx > 0) {
                buffer[idx] = '\0';
                char decoded = decode_morse(buffer);
                printf("%c", decoded);
                idx = 0;
            }
            space_count++;
            if (space_count == 3 || (*morse_input == '/' && space_count >= 1)) {
                printf(" ");
                space_count = 0;
            }
        } else if (*morse_input == '\n' || *morse_input == '\r') {
            if (idx > 0) {
                buffer[idx] = '\0';
                char decoded = decode_morse(buffer);
                printf("%c", decoded);
                idx = 0;
            }
            space_count = 0;
        }
        morse_input++;
    }

    if (idx > 0) {
        buffer[idx] = '\0';
        char decoded = decode_morse(buffer);
        printf("%c", decoded);
    }
    printf("\n");
}

/**
 * @brief Gibt eine detaillierte Hilfeübersicht mit allen verfügbaren Optionen aus.
 */
void print_help() {
    printf("Verwendung: morse [Optionen] [Dateien oder Texte]\n");
    printf("Optionen:\n");
    printf("  -e, --encode             Text in Morsecode umwandeln (Standard)\n");
    printf("  -d, --decode             Morsecode in Text umwandeln\n");
    printf("  -o, --out FILE           Ausgabe in Datei\n");
    printf("      --programmer-info    Zeigt Informationen zum Programmierer (JSON)\n");
    printf("      --slash-wordspacer   Trennt Worte mit / (nur bei Encode)\n");
    printf("  -h, --help               Diese Hilfe anzeigen\n");
}

/**
 * @brief Gibt die Programmautoreninformationen im JSON-Format aus.
 */
void print_programmer_info() {
    printf("{\n");
    printf("  \"firstname\": \"Julian\",\n");
    printf("  \"surname\": \"Stengele\",\n");
    printf("  \"branch_of_study\": \"TIK24\",\n");
    printf("  \"contact\": \"julian.stengele@gmail.com\"\n");
    printf("}\n");
}

/**
 * @brief Hauptfunktion: Steuert das Parsen der Kommandozeilenargumente und Programmablauf.
 *
 * Je nach Optionen wird die Eingabe als Datei oder über stdin gelesen und in Morsecode
 * codiert oder dekodiert. Auch Optionen wie Hilfeausgabe, Programmer-Info und Dateiausgabe
 * werden hier behandelt.
 *
 * @param argc Anzahl der übergebenen Argumente.
 * @param argv Array der Argumente.
 * @return 0 bei Erfolg, 1 bei Fehlern.
 */
int main(int argc, char** argv) {
    int opt;
    int option_index = 0;
    bool encode = false;
    bool decode = false;
    bool slash_wordspacer = false;
    char* outfile = NULL;

    static struct option long_options[] = {
        {"encode", no_argument, 0, 'e'},
        {"decode", no_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {"out", required_argument, 0, 'o'},
        {"programmer-info", no_argument, 0, 0},
        {"slash-wordspacer", no_argument, 0, 1},
        {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "edho:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'e': encode = true; break;
            case 'd': decode = true; break;
            case 'o': outfile = optarg; break;
            case 'h': print_help(); return 0;
            case 0:
                if (strcmp("programmer-info", long_options[option_index].name) == 0) {
                    print_programmer_info();
                    return 0;
                }
                break;
            case 1:
                slash_wordspacer = true;
                break;
            default:
                print_help();
                return 1;
        }
    }

    if (!encode && !decode) {
        encode = true;
    }

    if (encode && decode) {
        fprintf(stderr, "Fehler: Sowohl --encode als auch --decode angegeben.\n");
        return 1;
    }

    if (slash_wordspacer && decode) {
        fprintf(stderr, "Fehler: --slash-wordspacer nur mit --encode erlaubt.\n");
        return 1;
    }

    FILE* out = stdout;
    if (outfile) {
        out = freopen(outfile, "w", stdout);
        if (!out) {
            perror("Fehler beim Oeffnen der Ausgabedatei");
            return 1;
        }
    }

    if (optind < argc) {
        for (int i = optind; i < argc; i++) {
            FILE* f = fopen(argv[i], "r");
            if (!f) {
                perror("Fehler beim Oeffnen der Datei");
                continue;
            }
            char buffer[1024];
            while (fgets(buffer, sizeof(buffer), f)) {
                if (decode)
                    decode_text(buffer);
                else
                    encode_text(buffer, slash_wordspacer);
            }
            fclose(f);
        }
    } else {
        char input[1024];
        while (fgets(input, sizeof(input), stdin)) {
            if (decode)
                decode_text(input);
            else
                encode_text(input, slash_wordspacer);
        }
    }

    if (outfile) fclose(out);
    return 0;
}
