#include <stdio.h>
#include <stdlib.h>
#define DEBUG 1
#define AGE_GROUP 20

const char FILE_NAME[] = "Files/total.txt";
const int CURRENT_YEAR = 2020;
const float DEATH_RATE = 5.534;
long long int CURRENT_POPULATIONS = 0;
// Here we are assuming that all the people of age
// 20 to 24 years is a married couple.
const int STARTING_COUPLE_YEAR = 20;

void loadPopulation(); // initialize populations from the file
int getYear(); // get the estimated year from user
int getChild(); // get number of estimated child taken by a couple from user
long long int calculatePopulations(int year, int child); // calculate populations and return the updated result
void updatePopulations(int child); // update the populations

struct Populations {
    int popu;
    float percentage;
};

struct Populations populations[AGE_GROUP];

int main()
{
    loadPopulation();
    int year = getYear();
    int child = getChild();

    long long int predictedPopu = calculatePopulations(year - CURRENT_YEAR, child);

    printf("\n\n");

    printf("Current Populations:   %d\n", CURRENT_POPULATIONS);
    printf("Predicted Populations: %lld\n", predictedPopu);
    printf("Populations increases: %lld\n", predictedPopu - CURRENT_POPULATIONS);

    return 0;
}

void loadPopulation()
{
    FILE *fp;

    if ((fp = fopen(FILE_NAME, "rb")) == NULL) {
        printf("The %s file is not in the disk\n", FILE_NAME);
        exit(1);
    }

    // skip the comments contained in the file
    while(fgetc(fp) != '/');

    int popu, i = 0;
    float percentage;
    while(fscanf(fp, "%d", &popu) == 1 && fscanf(fp, "%f", &percentage) == 1) {
        populations[i].popu = popu;
        populations[i].percentage = percentage;
        CURRENT_POPULATIONS += popu;
        i++;
    }
}

int getYear()
{
    int year;

    do {
        printf("Enter an estimated year after %d: ", CURRENT_YEAR);
        scanf("%d", &year);
    } while (year <= CURRENT_YEAR);

    return year;
}

int getChild()
{
    int child;

    do {
        printf("Enter estimated child taken by a couple: ");
        scanf("%d", &child);
    } while (child < 0);

    return child;
}

long long int calculatePopulations(int year, int child)
{
    // Here we are assuming that all the peoples between
    // 20 to 29 years is a married couple.
    long long int newChild = 0;
    int y = year;

    int index = STARTING_COUPLE_YEAR / 5;
    while(y >= 5) {
        // half of the total people
        newChild += (populations[index].popu / 2) * child;
        updatePopulations((populations[index].popu / 2) * child);
        y -= 5;
    }

    // printf("y = %d\n", y);

    // calculate the remaining with percentage if present
    if (y)
        newChild += ((y / 5) * (populations[index].popu / 2)) * child;

    long long int updatedPopu = CURRENT_POPULATIONS + newChild;
    long long int deathPopu = ((updatedPopu * DEATH_RATE) / 1000) * year;

    if (DEBUG) {
        printf("Updated without death: %lld\n", updatedPopu);
        printf("Death:                 %lld\n", deathPopu);
        printf("Updated:               %lld\n", updatedPopu - deathPopu);
        printf("Year = %d\n", year);
    }

    return updatedPopu - deathPopu;
}

void updatePopulations(int child)
{
    int index = STARTING_COUPLE_YEAR / 5;

    while(index) {
        populations[index].popu = populations[index - 1].popu;
        index--;
    }

    populations[index].popu = child;

    if (0) {
        for(int i = 0; i < AGE_GROUP; i++) {
            printf("(%d to %d) ---  %d\n", i * 5, (i + 1) * 5 - 1, populations[i].popu);
        }
        putchar('\n');
    }
}


