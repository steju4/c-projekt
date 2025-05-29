#ifndef MORSE_H
#define MORSE_H

/**
 * @file morse.h
 * @brief Deklarationen für Morsecode-Funktionen.
 * @author Julian Stengele
 * @date 2023-10-27 // Oder das aktuelle Datum
 *
 * Diese Header-Datei deklariert die Funktionen, die für das Codieren von Text
 * in Morsecode und das Decodieren von Morsecode zurück in Text benötigt werden.
 * Sie enthält auch Deklarationen für Hilfsfunktionen wie die Ausgabe von
 * Programminformationen und Hilfetexten.
 */

#include <stdbool.h> // Für den Typ bool

/**
 * @brief Codiert ein einzelnes Zeichen in seine Morsecode-Repräsentation.
 * @param c Das zu codierende Zeichen.
 * @return Ein konstanter String, der den Morsecode für das Zeichen darstellt,
 *         oder "*" wenn das Zeichen nicht in der Morse-Map definiert ist.
 * @see morse_map
 */
const char* encode_char(char c);

/**
 * @brief Decodiert einen einzelnen Morsecode-String zurück in ein Zeichen.
 * @param code Der zu decodierende Morsecode-String (z.B. ".-").
 * @return Das decodierte Zeichen (z.B. 'A'),
 *         oder '*' wenn der Morsecode nicht in der Morse-Map definiert ist.
 * @see morse_map
 */
char decode_morse(const char* code);

/**
 * @brief Codiert einen kompletten Text in Morsecode.
 *
 * Die Ausgabe erfolgt auf stdout. Buchstaben innerhalb eines Wortes werden durch ein
 * Leerzeichen getrennt. Wörter werden standardmäßig durch drei Leerzeichen getrennt,
 * es sei denn, der slash_mode ist aktiviert. Newlines und Carriage Returns im
 * Eingabetext werden ignoriert. Am Ende der gesamten Ausgabe wird ein Newline-Zeichen gefügt.
 * @param text Der zu codierende Eingabetext.
 * @param slash_mode Wenn `true`, werden Wörter mit " / " (SP/SP) anstelle von
 *                   "   " (SPSPSP) getrennt.
 */
void encode_text(const char* text, bool slash_mode);

/**
 * @brief Decodiert einen kompletten Morsecode-String zurück in Text.
 *
 * Die Eingabe wird als Morsecode interpretiert, wobei '.' und '-' als Morse-Signale
 * und Leerzeichen sowie '/' als Trenner dienen. Die Ausgabe des decodierten Textes
 * erfolgt auf stdout. Ein einzelnes Leerzeichen zwischen Morse-Codes wird als
 * Buchstabentrenner interpretiert (d.h. kein expliziter Output). Drei aufeinanderfolgende
 * Leerzeichen oder ein '/' (optional umgeben von Leerzeichen) werden als Worttrenner
 * interpretiert und als einzelnes Leerzeichen im Ausgabetext dargestellt.
 * Newlines und Carriage Returns im Eingabecode werden ignoriert.
 * Am Ende der gesamten Ausgabe wird ein Newline-Zeichen gefügt.
 * @param morse_input Der zu decodierende Morsecode-String.
 */
void decode_text(const char* morse_input);

/**
 * @brief Gibt den Hilfetext des Programms auf stdout aus.
 *
 * Listet die Verwendung des Programms und alle verfügbaren Kommandozeilenoptionen
 * mit kurzen Erklärungen auf.
 */
void print_help();

/**
 * @brief Gibt Informationen über den Programmierer im JSON-Format auf stdout aus.
 *
 * Enthält Vorname, Nachname, Studiengang und Kontakt-E-Mail-Adresse.
 */
void print_programmer_info();

#endif // MORSE_H