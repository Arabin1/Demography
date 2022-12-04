#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0
#define V_INPUT 0
#define AGE_GROUP 20
#define ESTIMATED_GROUP 16

const char FILE_NAME[] = "Files/2020_total.txt";
const char DEATH_RATE[] = "Files/death_rate.txt";
const char INFANT_MORTALITY[] = "Files/infant_mortality.txt";
const int CURRENT_YEAR = 2020;
long long int CURRENT_POPULATIONS = 0;
// Here we are assuming that all the people of age
// 20 to 24 years is a married couple.
const int STARTING_COUPLE_YEAR = 20;

void loadData(); // Load necessary data from file
int getYear(); // get the estimated year from user
float getChild(); // get number of estimated child taken by a couple from user
long long int calculatePopulations(int year, float child); // calculate populations and return the updated result
void updatePopulations(int child); // update the populations

struct Populations {
    int popu;
    float percentage;
};

struct Populations populations[AGE_GROUP];
float death_rates[ESTIMATED_GROUP];
float infant_mortality[ESTIMATED_GROUP];

int main()
{
    loadData();
    if (V_INPUT) {
        puts("Populations....");
        for(int i = 0; i < AGE_GROUP; i++) {
            printf("(%d to %d) ---  %d\n", i * 5, (i + 1) * 5 - 1, populations[i].popu);
        }
        putchar('\n');

        puts("Death Rates...");
        for (int i = 0; i < ESTIMATED_GROUP; i++)
            printf("(%d to %d) -- %.3f\n", CURRENT_YEAR + (i * 5) + 1, CURRENT_YEAR + (i + 1) *5, death_rates[i]);
        putchar('\n');

        puts("Infant Mortality...");
        for (int i = 0; i < ESTIMATED_GROUP; i++)
            printf("(%d to %d) -- %.3f\n", CURRENT_YEAR + (i * 5) + 1, CURRENT_YEAR + (i + 1) *5, infant_mortality[i]);
        putchar('\n');
    }
    int year = getYear();
    float child = getChild();

    long long int predictedPopu = calculatePopulations(year - CURRENT_YEAR, child);

    printf("\n\n");

    printf("Current Populations:   %d\n", CURRENT_POPULATIONS);
    printf("Predicted Populations: %lld\n", predictedPopu);
    printf("Populations increases: %lld\n", predictedPopu - CURRENT_POPULATIONS);

    return 0;
}

void loadData()
{
    // first load the populations
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
    fclose(fp);

    // now load death_rates
    if ((fp = fopen(DEATH_RATE, "rb")) == NULL) {
        printf("The %s file is not in the disk\n", DEATH_RATE);
        exit(1);
    }

    float death;
    i = 0;
    while(!feof(fp)) {
        int r = 5;
        float summer = 0;
        while(r-- && fscanf(fp, "%f", &death) == 1) {
            summer += death;
        }
        death_rates[i++] = summer / 5; // average of 5 years
    }
    fclose(fp);

    // now load infant mortality
    if ((fp = fopen(INFANT_MORTALITY, "rb")) == NULL) {
        printf("The %s file is not in the disk\n", INFANT_MORTALITY);
        exit(1);
    }

    float mortality;
    i = 0;
    while(!feof(fp)) {
        int r = 5;
        float summer = 0;
        while(r-- && fscanf(fp, "%f", &mortality) == 1) {
            summer += mortality;
        }
        infant_mortality[i++] = summer / 5; // average of 5 years
    }
    fclose(fp);
}

int getYear()
{
    int year;

    do {
        printf("Enter an estimated year after %d: ", CURRENT_YEAR);
        scanf("%d", &year);
    } while (year <= CURRENT_YEAR || year > 2100);

    return year;
}

float getChild()
{
    float child;

    do {
        printf("Enter estimated child taken by a couple: ");
        scanf("%f", &child);
    } while (child < 0.0);

    return child;
}

long long int calculatePopulations(int year, float child)
{
    // Here we are assuming that all the peoples between
    // 20 to 24 years is a married couple.
    long long int updatedPopu = CURRENT_POPULATIONS, deathPopu = 0;
    int y = year, i = 0, j = 0;

    int index = STARTING_COUPLE_YEAR / 5;
    while(y >= 5) {
        // half of the total people
        float newChild = (populations[index].popu / 2) * child;
        newChild -= (infant_mortality[j++] / 1000.0) * newChild;
        updatePopulations(newChild);
        updatedPopu += newChild;
        deathPopu = ((updatedPopu * DEATH_RATE[i++]) / 1000);
        updatedPopu -= deathPopu;
        y -= 5;
    }

    // calculate the remaining with percentage if present
    if (y){
        int newChild = ((y / 5.0) * (populations[index].popu / 2)) * child;
        if (DEBUG) printf("New child: %d\n", newChild);
        newChild -= (infant_mortality[j++] / 1000.0) * newChild;
        updatePopulations(newChild);
        updatedPopu += newChild;
        deathPopu = ((updatedPopu * DEATH_RATE[i++]) / 1000);
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

    if (DEBUG) {
        for(int i = 0; i < AGE_GROUP; i++) {
            printf("(%d to %d) ---  %d\n", i * 5, (i + 1) * 5 - 1, populations[i].popu);
        }
        putchar('\n');
    }
}


