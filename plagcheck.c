#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "plagcheck.h"

#define MAX_LINE 500
#define N 2 // N-Gram size

void cleanLine(char *line) {
    int i, j = 0;
    for (i = 0; line[i]; i++) {
        if (isalnum(line[i]) || isspace(line[i])) {
            line[j++] = tolower(line[i]);
        }
    }
    line[j] = '\0';
}

// -------- WORD MATCHING --------
int wordExistsInDB(char *word, const char *dbPath) {
    FILE *db = fopen(dbPath, "r");
    if (!db) return 0;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), db)) {
        cleanLine(line);
        if (strstr(line, word)) {
            fclose(db);
            return 1;
        }
    }
    fclose(db);
    return 0;
}

float wordMatching(const char *inputPath, const char *dbPath) {
    FILE *input = fopen(inputPath, "r");
    if (!input) return 0;

    char line[MAX_LINE], *token;
    int total = 0, matched = 0;

    while (fgets(line, sizeof(line), input)) {
        cleanLine(line);
        token = strtok(line, " ");
        while (token != NULL) {
            total++;
            if (wordExistsInDB(token, dbPath)) {
                matched++;
            }
            token = strtok(NULL, " ");
        }
    }

    fclose(input);
    return (total == 0) ? 0 : ((float)matched / total) * 100;
}

// -------- N-GRAM MATCHING --------
int countMatchingNGrams(char *line, FILE *db) {
    char *words[100];
    int wordCount = 0, matchCount = 0;
    char *token = strtok(line, " ");

    while (token != NULL) {
        words[wordCount++] = token;
        token = strtok(NULL, " ");
    }

    fseek(db, 0, SEEK_SET);
    char dbLine[MAX_LINE];
    while (fgets(dbLine, sizeof(dbLine), db)) {
        cleanLine(dbLine);
        for (int i = 0; i <= wordCount - N; i++) {
            char ngram[300] = "";
            for (int j = 0; j < N; j++) {
                strcat(ngram, words[i + j]);
                if (j < N - 1) strcat(ngram, " ");
            }
            if (strstr(dbLine, ngram)) {
                matchCount++;
                break;
            }
        }
    }
    return matchCount;
}

float ngramMatching(const char *inputPath, const char *dbPath) {
    FILE *input = fopen(inputPath, "r");
    FILE *db = fopen(dbPath, "r");
    if (!input || !db) return 0;

    char inputLine[MAX_LINE];
    int totalNGrams = 0, matchedNGrams = 0;

    while (fgets(inputLine, sizeof(inputLine), input)) {
        cleanLine(inputLine);
        if (strlen(inputLine) == 0) continue;

        char backupLine[MAX_LINE];
        strcpy(backupLine, inputLine);

        int wordCount = 0;
        char *token = strtok(backupLine, " ");
        while (token != NULL) {
            wordCount++;
            token = strtok(NULL, " ");
        }

        int ngrams = wordCount >= N ? wordCount - N + 1 : 0;
        totalNGrams += ngrams;

        if (ngrams > 0) {
            matchedNGrams += countMatchingNGrams(inputLine, db);
        }
    }

    fclose(input);
    fclose(db);
    return (totalNGrams == 0) ? 0 : ((float)matchedNGrams / totalNGrams) * 100;
}

// -------- FINAL HYBRID PLAGIARISM --------
float checkPlagiarism(const char *inputPath, const char *dbPath) {
    float wordScore = wordMatching(inputPath, dbPath);
    float ngramScore = ngramMatching(inputPath, dbPath);
    
    printf("🔍 Word Match Score: %.2f%%\n", wordScore);
    printf("🔍 N-Gram Match Score: %.2f%%\n", ngramScore);

    float finalScore = (wordScore + ngramScore) / 2;
    return finalScore;
}
