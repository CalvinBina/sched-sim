// This program will generate a file that contains
// test data for the scheduler
//
// File will be named data.txt
//
// User will specify how many jobs to generate
// 
// File will have one task per line, in the following format:
// task_id processing_time start_time due_time priority opp1start opp1end opp2start opp2end ... opp5start opp5end
// 
// Calvin Bina
// 11-3-2014
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "mt19937ar.h"
#include <boost/math/distributions.hpp>

#define NUM_TASK_TYPES 8
using namespace boost::math;
// Local configs
struct config {
    float freqs[NUM_TASK_TYPES];
    int start_choice;
    int due_choice;
};

// This will check if there exists a data.txt in the 
// current directory, deleting it if it exists
//
int fileCheck(char const str[])
{
    FILE *fp;
    if (fp = fopen(str, "r"))
    {
        fclose(fp);
        if (strcmp(str, "data.txt") == 0) {
            remove("data.txt");
        }
        return 1;
    }
    return 0;
}

// This will return the task duration/processing time
// for a given taskID t
//
int getTaskDuration(int t)
{
    switch (t)
    {
        case 1:
        case 2:
        case 3:
            return (genrand_int32() % 3) + 2;
        case 4:
            return (genrand_int32() % 4) + 3;
        case 5:
            return (genrand_int32() % 4) + 4;
        case 6:
            return 6 + (genrand_int32() % 4);
        case 7:
            return 1200 + (genrand_int32() % 450);
        case 8:
            return 12 + (genrand_int32() % 6);
    }
}

// This will return the task priority for the given
// taskID t
//
int getTaskPriority(int t)
{
    switch (t)
    {
        case 1:
        case 2:
        case 3:
        case 4:
            return 2;
        case 5:
            return 5;
        case 6:
            return 4;
        case 7:
            return 9;
        case 8:
            return 7;
    }
}

// This will populate the given file
//
int generateData(FILE * fp, int numtasks, struct config * s)
{
    normal_distribution<> norm(2.8, 1.0);
    chi_squared_distribution<> chi(2);
    gamma_distribution<> gamma(2, 2);
    double dist_walker = 0.0;
    double dist_incr = 5.0 / numtasks;
    int c;
    int d;
    int randID;
    int randNum;
    int taskDur;
    int startT;
    int dueT;
    int lastStartT = 0;
    int lastDueT = 0;
    int lastTaskDur = 0;
    int taskPriority;
    double norm_prob;
    double chi_prob;
    float upperBounds[8];
    float lastBound;
    for (c=0; c<numtasks; c++)
    {
        lastBound = 0;
        // Compute the rand num ranges for the task types
        // based off the current task probability
        for (d=0; d<NUM_TASK_TYPES; d++) {
            if (s->freqs[d] == -1.0) {
                lastBound = lastBound + (pdf(norm, dist_walker) * 10000);
                upperBounds[d] = lastBound;
            } else if (s->freqs[d] == -2.0) {
                lastBound = lastBound + (pdf(chi, dist_walker) * 10000);
                upperBounds[d] = lastBound;
            } else if (s->freqs[d] == -3.0) {
                lastBound = lastBound + (pdf(gamma, dist_walker) * 10000);
                upperBounds[d] = lastBound;
            } else {
                lastBound = lastBound + s->freqs[d] * 100;
                upperBounds[d] = lastBound;
            }
        }
        dist_walker = dist_walker + dist_incr;

        // Generate a task
        randNum = (genrand_int32() % int(lastBound));
        if (randNum < upperBounds[0]) {
            randID = 1;
        } else if (randNum < upperBounds[1]) {
            randID = 2;
        } else if (randNum < upperBounds[2]) {
            randID = 3;
        } else if (randNum < upperBounds[3]) {
            randID = 4;
        } else if (randNum < upperBounds[4]) {
            randID = 5;
        } else if (randNum < upperBounds[5]) {
            randID = 6;
        } else if (randNum < upperBounds[6]) {
            randID = 7;
        } else if (randNum < upperBounds[7]) {
            randID = 8;
        }
        taskDur = getTaskDuration(randID);
        // Generate task start time
        if (s->start_choice == 1) {
            startT = lastStartT + lastTaskDur;
        } else {
            if ((lastTaskDur/3) == 0) {
                startT = lastStartT + lastTaskDur;
            } else {
                startT = lastStartT + lastTaskDur - (genrand_int32() % (lastTaskDur/3));
            }
        }
        // Generate task due time
        if (s->due_choice == 1) {
            dueT = lastDueT + taskDur * 3;
        } else {
            dueT = lastDueT + taskDur + (taskDur/4) + (genrand_int32() % taskDur);
        }
        taskPriority = getTaskPriority(randID);
        fprintf(fp, "%d %d %d %d %d 0 0 0 0 0 0 0 0 0 0\n", randID, taskDur, startT, dueT, taskPriority);
        lastTaskDur = taskDur;
        lastStartT = startT;
        lastDueT = dueT;
    }
    return 0;
}

// Creates a default config if a config file DNE
int createDefConfig()
{
    FILE *fp;
    fp = fopen("config.cfg", "w");
    int i;
    for (i=0; i<8; i++) {
        fprintf(fp, "%d 12.5\n", i+1);
    }
    fclose(fp);
    return 0;
}

// Saves a config file
int saveConfig(struct config * s)
{
    FILE *fp;
    char fname[20];
    printf("Enter a filename >> ");
    scanf("%s", fname);
    fp = fopen(fname, "w");
    int c;
    for (c=0; c<NUM_TASK_TYPES; c++) {
        fprintf(fp, "%d %f\n", c+1, s->freqs[c]);
    }
    fclose(fp);
    return 0;
}

// Load a config file
int loadConfig(struct config * s, char const str[])
{
    FILE *fp;
    fp = fopen(str, "r");
    int i;
    int id;
    float freq;
    for (i=0; i<NUM_TASK_TYPES; i++) {
        fscanf(fp, "%d %f", &id, &freq);
        s->freqs[i] = freq;
    }
    fclose(fp);
    return 0;
}

// Prints the current configuration loaded
int printConfig(struct config * s)
{
    int c;
    puts("\nCURRENT CONFIGURATION");
    for (c=0; c<NUM_TASK_TYPES; c++) {
        if (s->freqs[c] == -1.0) {
            printf("TaskID: %d Frequency: Normal Distribution\n", c+1);
        } else if (s->freqs[c] == -2.0) {
            printf("TaskID: %d Frequency: Chi-Squared Distribution\n", c+1);
        } else if (s->freqs[c] == -3.0) {
            printf("TaskID: %d Frequency: Gamma Distribution\n", c+1);
        } else {
            printf("TaskID: %d Frequency: %4.2f%%\n", c+1, s->freqs[c]);
        }
    }
}

// Modify the current configuration
int modifyConfig(struct config * s)
{
    puts("Enter the new percentages for each task ID.\nOr, enter '-1' for normal dist, '-2' for chi-squared dist, or '-3' for gamma dist. Ex: ID 1 >> 10.3");
    printf("ID 1 >> ");
    scanf("%f", &s->freqs[0]);
    printf("ID 2 >> ");
    scanf("%f", &s->freqs[1]);
    printf("ID 3 >> ");
    scanf("%f", &s->freqs[2]);
    printf("ID 4 >> ");
    scanf("%f", &s->freqs[3]);
    printf("ID 5 >> ");
    scanf("%f", &s->freqs[4]);
    printf("ID 6 >> ");
    scanf("%f", &s->freqs[5]);
    printf("ID 7 >> ");
    scanf("%f", &s->freqs[6]);
    printf("ID 8 >> ");
    scanf("%f", &s->freqs[7]);
    return 0;
}

// TODO: Finalize config file format
// TODO: Add starting time offset, which would allow a user to specify schedules with optimal solutions - no conflicts in timing
// vs conflicts in timing, with the latter resulting in a more difficult job for a scheduler
int main(int argc, char * argv[])
{
    // Check for config file, create one if it doesn't exist
    if (fileCheck("config.cfg") == 0) {
        createDefConfig();
        puts("Default config file created");
    }

    // Load config file vals into current config
    struct config current_config;
    loadConfig(&current_config, "config.cfg");

    // Print menu to allow user to modify the current config
    int menuChoice;
    while (1) {
        printConfig(&current_config);
        puts("");
        puts("1: Save current config");
        puts("2: Load another config");
        puts("3: Reset config to default");
        puts("4: Generate test data");
        puts("5: Modify current configuration");
        printf("\n>> ");
        scanf("%d", &menuChoice);
        if (menuChoice == 4) {
            break;
        } else if (menuChoice == 1) {
            saveConfig(&current_config);
        } else if (menuChoice == 2) {
            char fname[20];
            printf("Enter a file name >> ");
            scanf("%s", fname);
            loadConfig(&current_config, fname);
        } else if (menuChoice == 3) {
            createDefConfig();
        } else if (menuChoice == 5) {
            modifyConfig(&current_config);
        }
    }

    int s_choice;
    int d_choice;
    // Get start time generation choice
    puts("\nEnter '1' to have start time not conflicting. Enter '2' to have them conflict.\nHaving start times conflict will increase the number of tasks\navailable to be scheduled at any point, thus allowing you to test the scheduling algorithms with more revealing data.");
    printf("\n>> ");
    scanf("%d", &s_choice);

    // Get due time generation choice
    puts("\nEnter '1' to have due times not conflicting. Enter '2' to have them conflict.\nHaving due times conflict will strain the algorithms to choose which task to schedule, as not all of them can finish by their due date, thus allowing you to test the scheduling algorithms with more revealing data.");
    printf("\n>> ");
    scanf("%d", &d_choice);

    current_config.start_choice = s_choice;
    current_config.due_choice = d_choice;
    
    // Stores total number of tasks to generate
    int numTasks = -1;
    if (argc == 2)
    {
        sscanf(argv[1], "%d", &numTasks);
    } else
    {
        printf("\nEnter number of tasks to generate >> ");
        scanf("%d", &numTasks);
    }

    // Seed the rng
    time_t now;
    time(&now);
    init_genrand((int)now);

    // Remove old data.txt, if necessary
    if (fileCheck("data.txt") == 1)
    {
        puts("\nOld test data file deleted.");
    } 

    // Open the file for writing
    FILE *fp;
    fp = fopen("data.txt", "w");
    // generate the data
    int rc = generateData(fp, numTasks, &current_config);

    fclose(fp);

    char in;
    printf("Start simulation on new test data? [Y/n] ");
    scanf(" %c", &in);
    if (in == 'Y' || in == 'y') {
        system("./sim data.txt");
    }
    return 0;
}
