#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *input, *output;

    if ((input = fopen("infant_mortality.txt", "rb")) == NULL) {
        printf("The file is not exist in the directory.\n");
        exit(1);
    }

    if ((output = fopen("../Files/infant_mortality.txt", "wb")) == NULL) {
        printf("Can't open output file.\n");
        exit(1);
    }

    while(!feof(input)) {
        if (fgetc(input) == 'v' && fgetc(input) == '1') {
            int r = 2;
            while(r--) fgetc(input);
            float death;
            fscanf(input, "%f", &death);
            fprintf(output, "%0.3f\n", death);
            printf("%.3f\n", death);
        }
    }

    return 0;
}
