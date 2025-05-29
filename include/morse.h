#ifndef MORSE_H
#define MORSE_H

/// @file morse.h
/// @brief Enthält die Morsecode-Mappings und Funktionen zum Codieren und Decodieren

#include <stdbool.h>

const char* encode_char(char c);
char decode_morse(const char* code);
void encode_text(const char* text, bool slash_mode);
void decode_text(const char* morse);
void print_help();
void print_programmer_info();

#endif // MORSE_H
