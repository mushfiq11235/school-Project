#ifndef __VOWELIZER_H__
#define __VOWELIZER_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h> 

void devoweling_simple(char* text, char* vowels, char* non_vowels);
void envoweling_simple(char* text, char* vowels, char* non_vowels);
void devoweling_advanced(char* text, char* vowels, char* non_vowels);
void envoweling_advanced(char* text, char* vowels, char* non_vowels);

#endif