#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0
#define AGE_GROUP 20

const char FILE_NAME[] = "Files/2011_total.txt";
const int CURRENT_YEAR = 2011;
const float DEATH_RATE[] = {5.638, 5.553};
long long int CURRENT_POPULATIONS = 0;
// Here we are assuming that all the people of age
// 20 to 24 years is a married couple.
const int STARTING_COUPLE_YEAR = 20;

void loadPopulation(); // initialize populations from the file
int getYear(); // get the estimated year from user
int getChild(); // get number of estimated child taken by a couple from user
float getInfantMorality(); // get infant morality rate per 1000 live births
long long int calculatePopulations(int year, int child, float iF); // calculate populations and return the updated result
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
    float iF = getInfantMorality();

    long long int predictedPopu = calculatePopulations(year - CURRENT_YEAR, child, iF);

    printf("\n\n");

    printf("Current Populations:   %d\n", CURRENT_POPULATIONS);
    printf("Predicted Populations: %lld\n", predictedPopu);
    printf("Populations at 2020:   168220000\n");
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

float getInfantMorality()
{
    float iF;

    do {
        printf("Enter infant morality (Per 1000 live births): ");
        scanf("%f", &iF);
    } while(iF < 0);

    return iF;
}

long long int calculatePopulations(int year, int child, float iF)
{
    // Here we are assuming that all the peoples between
    // 20 to 24 years is a married couple.
    long long int updatedPopu = CURRENT_POPULATIONS, deathPopu = 0;
    int y = year, i = 0;

    int index = STARTING_COUPLE_YEAR / 5;
    while(y >= 5) {
        // half of the total people
        int newChild = (populations[index].popu / 2) * child;
        newChild -= (iF / 1000) * newChild;
        updatePopulations(newChild);
        updatedPopu += newChild;
        deathPopu += ((updatedPopu * DEATH_RATE[i++]) / 1000);
        updatedPopu -= deathPopu;
        y -= 5;
    }

    // calculate the remaining with percentage if present
    if (y){
        int newChild = ((y / 5) * (populations[index].popu / 2)) * child;
        newChild -= (iF / 1000) * newChild;
        updatePopulations(newChild);
        updatedPopu += newChild;
        deathPopu += ((updatedPopu * DEATH_RATE[i++]) / 1000);
        updatedPopu -= deathPopu;
    }

    if (DEBUG) {
        printf("Updated without death: %lld\n", updatedPopu);
        printf("Death:                 %lld\n", deathPopu);
        printf("Updated:               %lld\n", updatedPopu - deathPopu);
        printf("Year = %d\n", year);
    }

    return updatedPopu;
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


