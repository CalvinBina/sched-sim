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
#include "mt19937ar.h"

// This will check if there exists a data.txt in the 
// current directory, deleting it if it exists
//
int fileCheck()
{
    FILE *fp;
    if (fp = fopen("data.txt", "r"))
    {
        fclose(fp);
        int rc;
        rc = remove("data.txt");
        if (rc == 1)
        {
            puts("ERROR: Unable to delete old test data file.");
            exit(0);
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
            return 2;
        case 4:
            return 4;
        case 5:
            return 6;
        case 6:
            return 8;
        case 7:
            return 1500;
        case 8:
            return 15;
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
int generateData(FILE * fp, int numtasks)
{
    int c;
    int randID;
    int taskDur;
    int startT;
    int dueT;
    int taskPriority;
    for (c=0; c<numtasks; c++)
    {
        randID  = (genrand_int32() % 8) + 1;
        taskDur = getTaskDuration(randID);
        startT  = genrand_int32() % ((c+1) * 180);
        dueT = -1;
        while (dueT < (startT + taskDur))
            dueT = startT + taskDur + (100 + (genrand_int32() % 900));
        taskPriority = getTaskPriority(randID);
        fprintf(fp, "%d %d %d %d %d 0 0 0 0 0 0 0 0 0 0\n", randID, taskDur, startT, dueT, taskPriority);
    }
    return 0;
}

int main(int argc, char * argv[])
{
    // Remove old data.txt, if necessary
    if (fileCheck() == 1)
    {
        puts("\nOld test data file deleted.");
    }

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
    init_genrand(2947);

    // Open the file for writing
    FILE *fp;
    fp = fopen("data.txt", "w");

    // test
    int rc = generateData(fp, numTasks);

    fclose(fp);
    return 0;
}
