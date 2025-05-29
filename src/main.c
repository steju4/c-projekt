#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>

#include "morse.h"

typedef struct {
    const char* character;
    const char* morse;
} MorseMapping;

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
static const size_t map_len = sizeof(morse_map) / sizeof(MorseMapping);

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

char decode_morse(const char* code) {
    for (size_t i = 0; i < map_len; ++i) {
        if (strcmp(code, morse_map[i].morse) == 0) {
            return morse_map[i].character[0];
        }
    }
    return '*';
}

void encode_text(const char* text, bool slash_mode) {
    bool first_word = true;
    while (*text) {
        if (*text == '\n' || *text == '\r') {
            text++;
            continue;
        } else if (*text == ' ') {
            printf(slash_mode ? " / " : "   ");
            text++;
            first_word = true;
        } else {
            if (!first_word) printf(" ");
            const char* code = encode_char(*text);
            printf("%s", code);
            text++;
            first_word = false;
        }
    }
    putchar('\n');
}

void decode_text(const char* morse_input) { // Renamed to avoid conflict
    char buffer[16] = {0};
    int idx = 0;
    int space_count = 0; // Zählt aufeinanderfolgende Leerzeichen/Slashes

    while (*morse_input) {
        if (*morse_input == '.' || *morse_input == '-') {
            if (idx < sizeof(buffer) - 1) {
                buffer[idx++] = *morse_input;
            }
            space_count = 0; // Morsezeichen unterbricht Leerzeichenzählung
        } else if (*morse_input == ' ' || *morse_input == '/') {
            if (idx > 0) { // Vorherige Morse-Sequenz abschließen
                buffer[idx] = '\0';
                char decoded = decode_morse(buffer);
                printf("%c", decoded);
                idx = 0;
            }
            space_count++;
            if (space_count == 3 || (*morse_input == '/' && space_count >= 1) ) {
                // Worttrenner: 3 Spaces ODER ein Slash (umgeben von optionalen Spaces,
                // die bereits als Buchstabentrenner verarbeitet wurden oder den space_count erhöhen)
                printf(" ");
                space_count = 0; // Worttrenner setzt Zählung zurück
            } else if (space_count > 0 && space_count < 3 && *morse_input == ' ') {
                // Dies ist ein Buchstabentrenner (1 oder 2 Spaces)
                // Wird implizit durch das Dekodieren des nächsten Zeichens behandelt
                // oder durch den Worttrenner bei 3 Spaces.
                // Wir tun hier nichts, da der Buchstabentrenner durch das Dekodieren
                // des nächsten Zeichens und das Fehlen eines Leerzeichens davor entsteht.
            }
        } else if (*morse_input == '\n' || *morse_input == '\r') {
            // Ignorieren, aber vorherige Sequenz abschließen
            if (idx > 0) {
                buffer[idx] = '\0';
                char decoded = decode_morse(buffer);
                printf("%c", decoded);
                idx = 0;
            }
            space_count = 0; // Newline resettet auch
        }
        // Andere Zeichen werden ignoriert
        morse_input++;
    }

    // Letztes Zeichen decodieren, falls vorhanden
    if (idx > 0) {
        buffer[idx] = '\0';
        char decoded = decode_morse(buffer);
        printf("%c", decoded);
    }
    printf("\n"); // Newline am Ende der gesamten Ausgabe
}

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

void print_programmer_info() {
    printf("{\n");
    printf("  \"firstname\": \"Julian\",\n");
    printf("  \"surname\": \"Stengele\",\n");
    printf("  \"branch_of_study\": \"TIK24\",\n");
    printf("  \"contact\": \"julian.stengele@gmail.com\"\n");
    printf("}\n");
}

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
        fprintf(stderr, "Warte auf Eingabe von stdin... (Strg+Z/Strg+D + Enter zum Beenden)\n");
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
