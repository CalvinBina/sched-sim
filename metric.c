// Calvin Bina
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Struct for a task
// fields are listed in the same order
// as the config.cfg file
//
struct Task
{
    int task_id;
    int ProcessingTime;     // in seconds
    int start_time;
    int DueDateTime;        // seconds from start of execution
    int Priority;           // lower is higher priority
    int opp_times[10];      // even indexes are start opportunity times
                            // odd indexes are end opportunity times


    // These are used to store decision values for the respective 
    // sorting methods
    double CriticalRatio;
    double SRO;
};

// Critical Ratio-based scheduling - tasks with CR < 1 are behind schedule, CR > 1 ahead of schedule
//
void criticalRatio ( struct Task * eligible, struct Task * schedule, int num_tasks )
{
    int currentTime = 0;
    int counter = 0;
    int lowestTaskIndx = 0;
    double lowestRatio = INT_MAX;
    int i;

    // test 
    int n;
    for (n=0; n<num_tasks; n++)
    {
        printf("SENT IN: %d\n", eligible[n].task_id);
    }
    // counter keeps track of number of tasks that have been scheduled
    while (counter < num_tasks)
    {
        // Every time we schedule a task, we reset the variables that keep track of the lowest Critical Ratio
        lowestTaskIndx = -1;
        lowestRatio = INT_MAX;

        // The for loop that runs through each task and determines which task (if any) to schedule next
        for ( i = 0; i < num_tasks; i++)
        {
            // if the current time is not past the start_time of this job, we cannot schedule this task yet 
            if (eligible[i].start_time > currentTime) continue;

            if (eligible[i].DueDateTime == INT_MAX) continue;

            // Calculate CR (this value changes for each task as time goes on)
            eligible[i].CriticalRatio = (double)(eligible[i].DueDateTime - currentTime) / (double)eligible[i].ProcessingTime;

            if ( eligible[i].CriticalRatio < lowestRatio )
            {
                // This is placeholder code for working with opportunity times
                // TODO: Determine a method to incorporate opportunity windows for tasks - quite possibly up the priority of the task and work with higher priorities first
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
                } else  // this will keep track of the lowest CR for this run through, and the task index
                {
                    lowestRatio = eligible[i].CriticalRatio;
                    lowestTaskIndx = i;
                }
            }
        }

        // if no tasks are eligible this run through, but we still haven't scheduled all tasks, we need to increment the time to simulate passage of time
        if (lowestTaskIndx == -1)
        {
            currentTime = currentTime + 20;
            continue;
        }
        // otherwise, we update the current time to simulate real-time scheduling
        currentTime = currentTime + eligible[lowestTaskIndx].ProcessingTime;
        schedule[counter] = eligible[lowestTaskIndx];
        eligible[lowestTaskIndx].DueDateTime = INT_MAX;
        counter++;
    }

    // calculate CR for all tasks that havent been scheduled yet, 
}

// Earliest Due Date-based scheduling
//
void edd ( struct Task * eligible, struct Task * schedule, int num_tasks )
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
void fcfs ( struct Task * eligible, struct Task * schedule, int num_tasks )
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
void spt ( struct Task * eligible, struct Task * schedule, int num_tasks )
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
void sro ( struct Task * eligible, struct Task * schedule, int num_tasks )
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

// This function reads the data file and gets the task list to schedule
//
int readData (FILE * fp, struct Task * tasks) 
{
    int numLines = 0;
    int c;
    char line[512];
    int temp_id;
    int temp_proctime;
    int temp_startTime;
    int temp_duetime;
    int temp_priority;
    int temp_op[10];
    while (!feof(fp)) {
        if (fgets(line, 512, fp)==NULL) break;
        sscanf(line, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", &temp_id, &temp_proctime, &temp_startTime, &temp_duetime, &temp_priority, &temp_op[0], &temp_op[1], &temp_op[2], &temp_op[3], &temp_op[4], &temp_op[5], &temp_op[6], &temp_op[7], &temp_op[8], &temp_op[9]);
        tasks[numLines].task_id = temp_id;
        tasks[numLines].ProcessingTime = temp_proctime;
        tasks[numLines].start_time = temp_startTime;
        tasks[numLines].DueDateTime = temp_duetime;
        tasks[numLines].Priority = temp_priority;
        for (c=0; c<10; c++)
        {
            tasks[numLines].opp_times[c] = temp_op[c];
        }
        numLines++;
    }
    return numLines;
}

// TODO: Make flowchart diagrams of scheduling methods, and make adjustments to fine tune
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
    struct Task tasks[100];
    struct Task taskSchedule[100];
    int numtasks = readData(fp, tasks);

    // Open file and parse tasks
    //

//TODO: Implement task filter
    //Filter eligible jobs
    //filterTasks ( allTasks, taskSchedule );
    //scheduler ( taskSchedule, taskSchedule );

    //startTime = clock();
    //

    // Run chosen scheduler
    //

    if (sched_choice == 1)
    {
        fcfs(tasks, taskSchedule, numtasks);
    } else if (sched_choice == 2)
    {
        spt(tasks, taskSchedule, numtasks);
    } else if (sched_choice == 3)
    {
        edd(tasks, taskSchedule, numtasks);
    } else if (sched_choice == 4)
    {
        criticalRatio(tasks, taskSchedule, numtasks);
    } else if (sched_choice == 5)
    {
        sro(tasks, taskSchedule, numtasks);
    }

    // Run through scheduled list, measure performance
    int i;
    for ( i = 0; i < numtasks; i++ )
    {
        printf("TASKID sorted: %d\n", taskSchedule[i].task_id);
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
    AvgFinishTime = (double)totalFlowTime / (double)numtasks;
    Utilization   = (double)totalProcTime / (double)totalFlowTime;
    AvgNumOfTasks = (double)totalFlowTime / (double)totalProcTime;
    AvgJobDelays  = (double)totalDelay    / (double)numtasks;

    // Display results
    printf ("\n\nRESULTS\n------------\n"
            "Average Finish Time: %f\n"
            "        Utilization: %f\n"
            " Average # of Tasks: %f\n"
            "  Average Job Delay: %f\n", AvgFinishTime, Utilization, AvgNumOfTasks, AvgJobDelays);
    return 0;
}
