/**
 * @file morse.h
 * @brief Header-Datei für das Morsecode-Tool.
 *
 * Diese Datei deklariert alle öffentlich zugänglichen Funktionen
 * und Konstanten, die zum Codieren und Dekodieren von Morsezeichen benötigt werden.
 * Sie wird von der main.c verwendet und trennt Header- und Implementierungscode.
 */

#ifndef MORSE_H
#define MORSE_H

#include <stdbool.h>

/**
 * @brief Konvertiert ein einzelnes alphanumerisches Zeichen in Morsecode.
 *
 * Diese Funktion sucht das übergebene Zeichen in der internen Mapping-Tabelle
 * und gibt den zugehörigen Morsecode-String zurück. Zeichen, die nicht enthalten
 * sind (z. B. Umlaute oder Sonderzeichen außerhalb des definierten Umfangs),
 * werden durch einen Stern ('*') dargestellt.
 *
 * @param c Das zu codierende Zeichen (Groß-/Kleinbuchstabe, Ziffer oder Symbol).
 * @return Zeiger auf den Morsecode als nullterminierten String oder "*" bei Fehler.
 */
const char* encode_char(char c);

/**
 * @brief Wandelt einen Morsecode-String in ein einzelnes Zeichen zurück.
 *
 * Diese Funktion durchsucht die Morsecode-Mapping-Tabelle nach einer exakten Übereinstimmung
 * mit dem übergebenen Morsecode-String und gibt das zugehörige Zeichen zurück.
 * Bei ungültigen oder unbekannten Morsezeichen wird '*' zurückgegeben.
 *
 * @param code Morsecode als nullterminierter String (z. B. "--..--").
 * @return Entsprechendes Klartextzeichen (A–Z, 0–9, Symbol) oder '*' bei unbekanntem Code.
 */
char decode_morse(const char* code);

/**
 * @brief Wandelt einen kompletten Text in Morsecode um und gibt ihn aus.
 *
 * Die Funktion nimmt einen beliebigen C-String entgegen und konvertiert jedes
 * Zeichen in das entsprechende Morsezeichen. Wörter werden je nach Einstellung
 * durch " / " oder drei Leerzeichen getrennt. Nicht abbildbare Zeichen führen
 * zu einem Stern ('*') in der Ausgabe.
 *
 * @param text Eingabetext als nullterminierter C-String.
 * @param slash_mode true, wenn Worttrennung mit "/" erfolgen soll; false für 3* "SPACE".
 */
void encode_text(const char* text, bool slash_mode);

/**
 * @brief Wandelt Morsecode zurück in Klartext und gibt das Ergebnis aus.
 *
 * Diese Funktion verarbeitet eine Morsecode-Zeile, bei der Morsezeichen durch
 * Leerzeichen und Wörter durch drei Leerzeichen oder "/" getrennt sind.
 * Unbekannte Morsezeichen werden durch '*' ersetzt.
 *
 * @param morse_input Nullterminierter String mit Morsecode.
 */
void decode_text(const char* morse_input);

/**
 * @brief Gibt eine strukturierte Hilfeübersicht für den Nutzer auf stdout aus.
 *
 * Die Hilfe listet alle verfügbaren Optionen, Flags und deren Beschreibung auf.
 */
void print_help(void);

/**
 * @brief Gibt Informationen zum Autor des Programms im JSON-Format aus.
 *
 * Enthält Vorname, Nachname, Studiengang und E-Mail-Adresse.
 */
void print_programmer_info(void);

#endif // MORSE_H
