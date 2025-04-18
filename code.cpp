#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define WORD_LEN 50

void normalize(char *word) {
    int i, j = 0;
    char temp[WORD_LEN];
    for (i = 0; word[i]; i++) {
        if (isalpha(word[i])) {
            temp[j++] = tolower(word[i]);
        }
    }
    temp[j] = '\0';
    strcpy(word, temp);
}
int is_in_list(char words[][WORD_LEN], int size, const char *word) {
    for (int i = 0; i < size; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}
int extract_words(const char *filename, char words[][WORD_LEN]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file: %s\n", filename);
        return -1;
    }
    char word[WORD_LEN];
    int count = 0;

    while (fscanf(file, "%49s", word) != EOF) {
        normalize(word);
        if (strlen(word) > 0 && !is_in_list(words, count, word)) {
            strcpy(words[count++], word);
        }
    }
    fclose(file);
    return count;
}
float jaccard_similarity(char words1[][WORD_LEN], int count1, char words2[][WORD_LEN], int count2) {
    int intersection = 0;

    for (int i = 0; i < count1; i++) {
        if (is_in_list(words2, count2, words1[i])) {
            intersection++;
        }
    }

    int union_count = count1 + count2 - intersection;

    if (union_count == 0)
        return 0.0;

    return ((float)intersection / union_count) * 100.0;
}

int main() {
    char words1[MAX_WORDS][WORD_LEN];
    char words2[MAX_WORDS][WORD_LEN];

    int count1 = extract_words("file1.txt", words1);
    int count2 = extract_words("file2.txt", words2);

    if (count1 == -1 || count2 == -1) {
        return 1;
    }

    float similarity = jaccard_similarity(words1, count1, words2, count2);

    printf("Plagiarism Similarity: %.2f%%\n", similarity);

    return 0;
}
