#include "vowelizer.h"
#define MAX_SIZE_MSG 512
#define VOWELS "aeiou"

// This function split the text into vowels and non-vowels
void devoweling_simple(char* text, char* vowels, char* non_vowels){
    // Split the text into vowels and non-vowels
    int i = 0;
    while (text[i] != '\0'){
        // printf("i = %d\n", i);
        if (strchr(VOWELS, text[i]) != NULL){
            vowels[i] = text[i];
            non_vowels[i] = ' ';
        }
        else{
            non_vowels[i] = text[i];
            vowels[i] = ' ';
        }
        i++;
    }
    // Add the end of string
    vowels[i] = '\0';
    non_vowels[i] = '\0';
}

void envoweling_simple(char* text, char* vowels, char* non_vowels){
    // init text
    fprintf(stdout, "text : %s - vowels : %s - non_vowels : %s\n", text, vowels, non_vowels);
    memset(text, 0, MAX_SIZE_MSG);
    fprintf(stdout, "text : %s - vowels : %s - non_vowels : %s\n", text, vowels, non_vowels);
    // Concatenate the vowels and non-vowels
    int i = 0;
    while (vowels[i] != '\0' || non_vowels[i] != '\0'){
        if (vowels[i] != ' '){
            text[i] = vowels[i];
        }
        else{
            text[i] = non_vowels[i];
        }
        i++;
    }
    text[i] = '\0';
}

void devoweling_advanced(char* text, char* vowels, char* non_vowels){
    // Split the text into vowels and non-vowels
    int i = 0, j = 0, k = 0, s = 0;
    while (text[i] != '\0'){
        if (strchr(VOWELS, text[i]) != NULL){
            sprintf(vowels + j, "%d", s);
            j++;
            vowels[j] = text[i];
            j++;
            s=0;
        }
        else{
            non_vowels[k] = text[i];
            k++;
            s++;
        }
        i++;
    }
    // Add the end of string
    vowels[j] = '\0';
    non_vowels[k] = '\0';
}

void envoweling_advanced(char* text, char* vowels, char* non_vowels){
    // Concatenate the vowels and non-vowels
    int i = 0, j = 0, k = 0, s = 0;
    while (vowels[i] != '\0'){
        if (strchr(VOWELS, vowels[i]) == NULL){
            int n = atoi(vowels + i);
            for (int l = 0; l < n; l++){
                text[j] = non_vowels[k];
                j++;
                k++;
            }
            text[j] = vowels[i+1];
            j++;
        }
        i++;
    }
    j--;
    while (non_vowels[k] != '\0'){
        text[j] = non_vowels[k];
        j++;
        k++;
    }
    text[j] = '\0';
}