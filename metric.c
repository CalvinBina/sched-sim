// Calvin Bina
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Struct for a task
//
typedef struct
{
    int Priority;           // lower is higher priority
    int ProcessingTime;     // in seconds
    int DueDateTime;        // seconds from start of execution
    double CriticalRatio;
    double SRO;
    int task_id;
    int start_time;
    int opp_times[10];      // even indexes are start opportunity times
} Task;                          // odd indexes are end opportunity times

// Critical Ratio-based scheduling
//
void criticalRatio ( Task * eligible, Task * schedule, int num_tasks )
{
    int currentTime = 0;
    int counter = 0;
    int lowestTaskIndx = 0;
    double lowestRatio = INT_MAX;

    while (counter < num_tasks)
    {
        lowestTaskIndx = 0;
        lowestRatio = INT_MAX;
        int i;
        for ( i = 0; i < num_tasks; i++)
        {
            if (eligible[i].start_time > currentTime) continue;

            eligible[i].CriticalRatio = (double)(eligible[i].DueDateTime - currentTime) / (double)eligible[i].ProcessingTime;

            if ( eligible[i].CriticalRatio < lowestRatio )
            {
                if (eligible[i].opp_times[1] != 0)
                {
                    int bo;
                    for (bo=0; bo < 10; bo+2)
                    {
                        if (eligible[i].opp_times[bo+1] != 0 && eligible[i].opp_times[bo] <= currentTime && eligible[i].opp_times[bo+1] >= currentTime)
                        {
                            lowestRatio = eligible[i].CriticalRatio;
                            lowestTaskIndx = i;
                            break;
                        }
                    }
                } else
                {
                    lowestRatio = eligible[i].CriticalRatio;
                    lowestTaskIndx = i;
                }
            }
        }
        currentTime = currentTime + eligible[lowestTaskIndx].ProcessingTime;
        schedule[counter] = eligible[lowestTaskIndx];
        eligible[lowestTaskIndx].DueDateTime = INT_MAX;
        counter++;
    }
}

// Earliest Due Date-based scheduling
//
void edd ( Task * eligible, Task * schedule, int num_tasks )
{
    int currentTime = 0;
    int counter = 0;
    int eddTime = INT_MAX;
    int eddIndx = 0;
    
    while (counter < num_tasks)
    {
        eddTime = INT_MAX;
        eddIndx = 0;

        int i;
        for ( i=0; i<num_tasks; i++)
        {
            if (eligible[i].start_time > currentTime) continue;

            if (eligible[i].DueDateTime < eddTime)
            {
                if (eligible[i].opp_times[1] != 0)
                {
                    int bo;
                    for (bo=0; bo < 10; bo+2)
                    {
                        if (eligible[i].opp_times[bo+1] != 0 && eligible[i].opp_times[bo] <= currentTime && eligible[i].opp_times[bo+1] >= currentTime)
                        {
                            eddTime = eligible[i].DueDateTime;
                            eddIndx = i;
                            break;
                        }
                    }
                } else
                {
                    eddTime = eligible[i].DueDateTime;
                    eddIndx = i;
                }
            }
        }

        schedule[counter] = eligible[eddIndx];
        currentTime = currentTime + eligible[eddIndx].ProcessingTime;
        eligible[eddIndx].DueDateTime    = INT_MAX;
        counter++;
    }
}

// First Come First Served-based scheduling
// 
void fcfs ( Task * eligible, Task * schedule, int num_tasks )
{
    int currentTime = 0;
    int counter = 0;
    int i;
    while ( counter < num_tasks )
    {
        for (i=0; i < num_tasks; i++)
        {
            if ( eligible[i].start_time > currentTime ) continue;

            if (eligible[i].opp_times[1] != 0)
            {
                int bo;
                for (bo=0; bo < 10; bo+2)
                {
                    if (eligible[i].opp_times[bo+1] != 0 && eligible[i].opp_times[bo] <= currentTime && eligible[i].opp_times[bo+1] >= currentTime)
                    {
                        schedule[i]= eligible[i];
                        eligible[i].start_time = INT_MAX;
                        currentTime = currentTime + eligible[i].ProcessingTime;
                        counter++;
                        break;
                    }
                }
            } else
            {
                schedule[i] = eligible[i];
                eligible[i].start_time = INT_MAX;
                currentTime = currentTime + eligible[i].ProcessingTime;
                counter++;
            }
        }
    }
}

// Shortest Processing Time-based scheduling
//
void spt ( Task * eligible, Task * schedule, int num_tasks )
{
    int currentTime = 0;
    int shortestPtime = INT_MAX;
    int sptIndx = 0;
    int counter = 0;

    while (counter < num_tasks)
    {
        shortestPtime = INT_MAX;
        sptIndx = 0;

        int i;
        for ( i=0; i < num_tasks; i++)
        {
            if (eligible[i].start_time > currentTime) continue;

            if (eligible[i].ProcessingTime < shortestPtime)
            {
                if (eligible[i].opp_times[1] != 0)
                {
                    int bo;
                    for (bo=0; bo < 10; bo+2)
                    {
                        if (eligible[i].opp_times[bo+1] != 0 && eligible[i].opp_times[bo] <= currentTime && eligible[i].opp_times[bo+1] >= currentTime)
                        {
                            shortestPtime = eligible[i].ProcessingTime;
                            sptIndx = i;
                            break;
                        }
                    }
                } else
                {
                    shortestPtime = eligible[i].ProcessingTime;
                    sptIndx = i;
                }
            }
        }
        schedule[counter] = eligible[sptIndx];
        currentTime = currentTime + eligible[sptIndx].ProcessingTime;
        eligible[sptIndx].ProcessingTime = INT_MAX;
        counter++;
    }
}

// Slack per remaining operations-based scheduling
//
void sro ( Task * eligible, Task * schedule, int num_tasks )
{
    int currentTime = 0;
    int lowestSRO   = INT_MAX;
    int lowestIndx  = 0;
    int counter = 0;

    while (counter < num_tasks)
    {
        lowestIndx = 0;
        lowestSRO = INT_MAX;

        int i;
        for (i=0; i < num_tasks; i++)
        {
            if (eligible[i].start_time > currentTime) continue;

            eligible[i].SRO = (double)(eligible[i].DueDateTime - eligible[i].ProcessingTime - currentTime) / (double)(num_tasks - counter);

            if (eligible[i].SRO < lowestSRO)
            {
                if (eligible[i].opp_times[1] != 0)
                {
                    int bo;
                    for (bo=0; bo < 10; bo+2)
                    {
                        if (eligible[i].opp_times[bo+1] != 0 && eligible[i].opp_times[bo] <= currentTime && eligible[i].opp_times[bo+1] >= currentTime)
                        {
                            lowestSRO  = eligible[i].SRO;
                            lowestIndx = i;
                            break;
                        }
                    }
                } else
                {
                    lowestSRO  = eligible[i].SRO;
                    lowestIndx = i;
                }
            }
        }
        currentTime = currentTime + eligible[lowestIndx].ProcessingTime;
        schedule[counter] = eligible[lowestIndx];
        eligible[lowestIndx].DueDateTime = INT_MAX;
        eligible[lowestIndx].ProcessingTime = 0;
        counter++;
    }
}
// TODO: fix file i/o errors
// Read in test data, run desired scheduling method on data,
// and view the results of the test
//
int main (int argc, char* argv[])
{
    // The allowed command line arguments:
    // (optional) FILENAME
    //
    // Initialize variables for measuring performance
    //
    int flowTime = 0;
    int numDelays = 0;
    int totalFlowTime = 0;
    int totalProcTime = 0;
    int totalDelay = 0;

    // Print menu and get algorithm to test
    //
    int sched_choice;
    printf("\n\nChoose the scheduling algorithm to test:\n"
         "1:\tFirst Come First Serve\n"
         "2:\tShortest Processing Time\n"
         "3:\tEarliest Due Date\n"
         "4:\tCritical Ratio\n"
         "5:\tSlack / Remaining Operations\n\n"
         "---> Please choose one of the above:\n> ");
    scanf("%d", &sched_choice);
    if (sched_choice < 1 && sched_choice > 5)
    {
        puts("Invalid input. Exiting...\n");
        return 0;
    }

    // Get a file to read data from
    //
    char filename[128];
    if (argc > 2) { puts("Wrong number of cmdline arguments\n"); return 0; }
    if (argc == 2)
    {
        strcpy(filename, argv[1]);
    } else
    {
        printf("\n---> Please enter the name of the test data file to read:\n> ");
        scanf("%s", filename);
    }

    // Open and read file
    //
    FILE *fp = NULL;
    fp = fopen(filename, "r");
    int c;
    int num_tasks = 0;
    while ( (c = fgetc(fp)) != EOF)
    {
      if (c == '\n')
      {
          num_tasks++;
      }
    }
    fclose(fp);

    // Create task arrays
    //
    Task eligibleTasks[num_tasks];
    //Task      allTasks[num_tasks];
    Task  taskSchedule[num_tasks];

    // Open file and parse tasks
    //
    fp = fopen(filename, "r");
    int counter = 0;
    while ( counter < num_tasks)
    {
        fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\r\n",
              &eligibleTasks[counter].task_id, &eligibleTasks[counter].ProcessingTime, &eligibleTasks[counter].start_time, &eligibleTasks[counter].DueDateTime, &eligibleTasks[counter].Priority, &eligibleTasks[counter].opp_times[0], &eligibleTasks[counter].opp_times[1], &eligibleTasks[counter].opp_times[2], &eligibleTasks[counter].opp_times[3], &eligibleTasks[counter].opp_times[4], &eligibleTasks[counter].opp_times[5], &eligibleTasks[counter].opp_times[6], &eligibleTasks[counter].opp_times[7], &eligibleTasks[counter].opp_times[8], &eligibleTasks[counter].opp_times[9]);
        counter++;
        // test
        printf("READ ID: %d\n", eligibleTasks[counter].task_id);
    }
    fclose(fp);

//TODO: Implement task filter
    //Filter eligible jobs
    //filterTasks ( allTasks, eligibleTasks );
    //scheduler ( eligibleTasks, taskSchedule );

    //startTime = clock();
    //

    // Run chosen scheduler
    //

    if (sched_choice == 1)
    {
        fcfs(eligibleTasks, taskSchedule, num_tasks);
    } else if (sched_choice == 2)
    {
        spt(eligibleTasks, taskSchedule, num_tasks);
    } else if (sched_choice == 3)
    {
        edd(eligibleTasks, taskSchedule, num_tasks);
    } else if (sched_choice == 4)
    {
        criticalRatio(eligibleTasks, taskSchedule, num_tasks);
    } else if (sched_choice == 5)
    {
        sro(eligibleTasks, taskSchedule, num_tasks);
    }

    // Run through scheduled list, measure performance
    int i;
    for ( i = 0; i < num_tasks; i++ )
    {
        printf("TASKID: %d\n", taskSchedule[i].task_id);
        totalProcTime = taskSchedule[i].ProcessingTime + totalProcTime;
        flowTime      = flowTime + taskSchedule[i].ProcessingTime;
        totalFlowTime = flowTime + totalFlowTime;

        if ( flowTime > taskSchedule[i].DueDateTime )
        {
            numDelays++;
            totalDelay = totalDelay + (flowTime - taskSchedule[i].DueDateTime);
        }
    }

    // TODO: update performance evaluations
    // Initialize variables for results
    double AvgFinishTime = 0.0;
    double Utilization = 0.0;
    double AvgNumOfTasks = 0.0;
    double AvgJobDelays = 0.0;

    // Calculate performance
    AvgFinishTime = (double)totalFlowTime / (double)num_tasks;
    Utilization   = (double)totalProcTime / (double)totalFlowTime;
    AvgNumOfTasks = (double)totalFlowTime / (double)totalProcTime;
    AvgJobDelays  = (double)totalDelay    / (double)num_tasks;

    // Display results
    printf ("\n\nRESULTS\n------------\n"
            "Average Finish Time: %f\n"
            "        Utilization: %f\n"
            " Average # of Tasks: %f\n"
            "  Average Job Delay: %f\n", AvgFinishTime, Utilization, AvgNumOfTasks, AvgJobDelays);
    return 0;
}
