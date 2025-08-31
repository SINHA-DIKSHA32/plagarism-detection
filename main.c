#include <stdio.h>
#include "plagcheck.h"

int main() {
    float similarity = checkPlagiarism("input.txt", "database.txt");
    printf("Plagiarism: %.2f%%\n", similarity);

    FILE *f = fopen("result.txt", "w");
    if (f == NULL) {
        printf("❌ Error: result.txt file could not be created!\n");
        return 1;
    }

    fprintf(f, "Duplication detected: %.2f%%\n", similarity);
    fclose(f);
    return 0;
}
