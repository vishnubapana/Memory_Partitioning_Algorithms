/***************************************************************************
 * File: main.c
 * Author: Venkata Bapanapalli
 * Procedures:
 * PROCESS - struct with task info stream elements.
 * rePROCESS - resets info stream's elements to allow them to be processed again.
 * allProcessQue - set un-queued tasks to false while queued tasks are set to true.
 * searchArr - search array used for first-fit and next-fit algorithms.
 * addToMem - Place tasks into memory.
 * incrementTime - Scheduling algorithm to time algorithms.
 * biggest - find the biggest memory size, used for worst-fit algorithm.
 * smallest - find the smalled memory size, used for best-fit algorithm.
 * bestFit - implements the best fit memory partitioning algorithm.
 * firstFit - implements the first fit memory partitioning algorithm.
 * nextFit - implements the next fit memory partitioning algorithm.
 * worstFit - implements the worst fit memory partitioning algorithm.
 * uniform - uniform random number generator.
 * normal - normal random number generator.
 * main - populates the stream struct with random values for size and time,
         then run 1000 experiments and print average for each algorithm.
***************************************************************************/
#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

int MEM_FREE = 56;
int NUM_PROCESSES = 1000;
/***************************************************************************
 * struct PROCESS
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: struct that defines the info stream.
 * Parameters:
 * N/A
**************************************************************************/
struct PROCESS
{
    int size;                                         // holds the size of each task
    int time;                                         // holds the time of each task
    int tTime = 0;                                    // holds the time taken to process the task
    int oTime = time;                                 // holds the original time of each task
    int location = -1;                                // location in memory
    bool isComplete = false;                          // checks if the task is completed
    bool queued = false;                              // checks if a task is queued
};
/***************************************************************************
 * void rePROCESS(PROCESS* arr[])
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: resets info stream's elements to allow them to be processed again.
 * Parameters:
 * arr[]  I/P  PROCESS  passes the dynamic array of the PROCESS struct
**************************************************************************/
void rePROCESS(PROCESS* arr[])
{
    for(int i=0; i<NUM_PROCESSES; i++)                  		// resets 1000 tasks to be reprocessed
    {
        arr[i]->tTime = 0;
        arr[i]->location = -1;
        arr[i]->isComplete = false;
        arr[i]->queued = false;
        arr[i]->time = arr[i]->oTime;
    }
}
/***************************************************************************
 * bool allProcessesQue(PROCESS* arr[])
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: queued tasks to true, allowing them to be distinguished from un-queued tasks.
 * Parameters:
 * arr[]  I/P  PROCESS  passes the dynamic array of the PROCESS struct
**************************************************************************/
bool allProcessesQue(PROCESS* arr[])
{
    bool current = true;
    for(int i=0; i<NUM_PROCESSES; i++)                    		// sets all queued tasks to true
    {
        if (!arr[i]->queued)
        {
            current = false;
        }
    }
    return current;
}
/***************************************************************************
 * int searchArr(PROCESS* mem[], int startPos,  int tSize)
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: search array used to search for specific tasks to place in memory.
 * Parameters:
 * mem       I/P    PROCESS *[]  The mem dynamic array struct
 * startPos  I/P    int          Holds the start position of the dynamic array
 * tSize     I/P    int          Holds the size of the task
**************************************************************************/
int searchArr(PROCESS* mem[], int startPos,  int tSize)
{
    for(int i=startPos; i<MEM_FREE; i++)
    {
        bool toPlace = true;
        for(int j = i; j < tSize + i; j++ )                 		// search from current index
        {
            if (j >= MEM_FREE || mem[j] != nullptr )
            {
                toPlace = false;
                break;
            }
        }
        if (toPlace)
        {
            return i;
        }
    }
    for(int i=0; i<startPos; i++)                       		// returns to the beginning
    {
        bool toPlace = true;
        for(int j = i; j < tSize + i; j++ )             		// Search starting from index i
        {                                               
            if (j >= MEM_FREE || mem[j] != nullptr )
            {
                toPlace = false;
                break;
            }
        }
        if (toPlace)
        {
            return i;
        }
    }
    return -1;
}
/***************************************************************************
 * void addToMem(PROCESS* mem[], PROCESS*& placeItem, int put)
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: Adds tasks to the memory
 * Parameters:
 * mem         I/P   PROCESS*[]   holds the memory array
 * placeItem   I/P   PROCESS*&    holds the item that needs to be placed in memory
 * put         I/P   int          index of the value that needs to be placed in memory
**************************************************************************/
void addToMem(PROCESS* mem[], PROCESS*& placeItem, int put)
{
    for(int i=0; i<placeItem->size; i++)            			// adds specific tasks to memory
    {
        mem[put + i] = placeItem;
    }
    placeItem->queued = true;
    placeItem->location = put;
}
/***************************************************************************
 * void incrementTime(PROCESS* mem[], PROCESS* arr[], int& totalTime, int current)
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: This is the scheduling function which will output total time taken for each algorithm.
 * Parameters:
 * mem        I/P       PROCESS*[]   memory array
 * arr        I/P       PROCESS*[]   info stream
 * totalTime  O/P       int&         total time value
 * current    I/P       int          index of the current position
**************************************************************************/
void incrementTime(PROCESS* mem[], PROCESS* arr[], int& totalTime, int current)
{
    for(int i = 0; i<current; i++)                          		// Decrease total time for items on stream
    {
        arr[i]->time--;
        arr[i]->tTime++;
        if (arr[i]->time == 0)                              		// Task completed: Remove from memory
        {
            arr[i]->isComplete = true;
            for( int k = arr[i]->location; k < arr[i]->location + arr[i]->size; k++)
            {
                mem[k] = nullptr;
            }
        }
    }
    totalTime++;
}
/***************************************************************************
 * void biggest(PROCESS* mem[], int& index, int& indexSize, int sizeReq)
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: finds the biggest memory block, this is used for the worst case algorithm.
 * Parameters:
 * mem         I/P  PROCESS*[]   memory array
 * index       O/P  int&         will hold the value of the largest memory block
 * indexSize   I/P  int&         holds the value of the largest memory block
 * sizeReq     I/P  int          holds the value of the space needed for the task
**************************************************************************/
void biggest(PROCESS* mem[], int& index, int& indexSize, int sizeReq)
{
    index = -1, indexSize = 0;
    for(int i=0; i< MEM_FREE; i++)                                              // go through the memory
    {
        if (mem[i] == nullptr)                                                  // check if the size is in memory
        {
            for(int j=0; j + i < MEM_FREE; j++)
            {
                if(mem[i+j] != nullptr || i+j == MEM_FREE - 1)                  // check if memory spot is filled
                {
                    if ( j+1 > indexSize && j+1 >= sizeReq )                    // Check if counter is greater than the previous biggest
                    {
                        indexSize = j;                                          // Update previous biggest memory value
                        index = i;
                    }
                    break;
                }
            }
        }
    }
}
/***************************************************************************
 * void smallest(PROCESS* mem[], int& index, int& indexSize, int sizeReq)
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: finds the smallest memory block needed in the bestfit algorithm.
 * Parameters:
 * mem         I/P  PROCESS*[]   memory array
 * index       O/P  int&         will hold the value of the smallest memory block
 * indexSize   I/P  int&         holds the value of the smallest memory block
 * sizeReq     I/P  int          holds the value of the space needed for the task
**************************************************************************/
void smallest(PROCESS* mem[], int& index, int& indexSize, int sizeReq)
{
    indexSize = MEM_FREE + 1, index = -1;
    for(int i=0; i< MEM_FREE; i++)
    {
        if (mem[i] == nullptr)                                                   // check if the size is in memory
        {
            for(int j=0; j + i < MEM_FREE; j++)
            {
                if(mem[i+j] != nullptr || i+j == MEM_FREE - 1)                  // check if the memory spot if full
                {
                    if (j + 1< indexSize && j + 1 >= sizeReq )                  // check if value is smaller than previous
                    {
                        indexSize = j;                                          // Update previous smallest
                        index = i;
                    }
                    break;
                }
            }
        }
    }
}
/***************************************************************************
 * int bestFit (PROCESS* arr[])
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: bestfit algorithm that finds the block that is closest to size of the task.
 * Parameters:
 * arr   I/P   PROCESS*[]   info stream of tasks to be processed in memory
**************************************************************************/
int bestFit (PROCESS* arr[])
{
    PROCESS* mem[MEM_FREE];                                                         // Creates array of dynamic objects
    for(int i=0; i<MEM_FREE; i++)
    {
        mem[i] = nullptr;
    }
    int totalTime = 0;                                                    
    int current = 0;
    int bestIndex = -1, bestSize = MEM_FREE + 1;
    while ( !allProcessesQue(arr) )                                                 // While not all tasks have been placed
    {
        do
        {
            smallest(mem, bestIndex, bestSize, arr[current]->size);
            if (bestIndex >= 0)
            {

                addToMem(mem, arr[current], bestIndex);                         // Place item on taskStream at index

                current++;
            }
        }while(bestIndex >= 0 && current < NUM_PROCESSES);                         // Continue while we can place the item

        incrementTime(mem, arr, totalTime, current);
    }
    int biggestTime = 0;
    for(int i=0; i<MEM_FREE; i++)
    {
        if (mem[i] != nullptr)
        {
            if (mem[i]->time > biggestTime)
            {
                biggestTime = mem[i]->time;
            }
        }
    }
    totalTime+=biggestTime;
    rePROCESS(arr);
    return totalTime;
}
/***************************************************************************
 * int firstFit (PROCESS* arr[])
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: firstfit algorithm that finds the first closest block capable of processing the task in memory.
 * Parameters:
 * arr   I/P   PROCESS*[]   info stream of tasks to be processed in memory
**************************************************************************/
int firstFit(PROCESS* arr[])
{
    PROCESS* mem[MEM_FREE];                                                     // Creates array of dynamic objects
    for(int i=0; i<MEM_FREE; i++)
    {
        mem[i] = nullptr;
    }
    int totalTime = 0; 
    int current = 0;
    while ( !allProcessesQue(arr) )                                           // While not all tasks have been placed.
    {
        int put;
        do
        {
            put = searchArr(mem, 0, arr[current]->size);               	     // Check if item can be placed
            if (put >= 0)
            {
                addToMem(mem, arr[current], put);                           // Place item on taskStream at index
                current++;
            }
        }while(put >= 0 && current < NUM_PROCESSES);                      // Continue while we can place the item
        incrementTime(mem, arr, totalTime, current);
    }
    int biggestTime = 0;
    for(int i=0; i<MEM_FREE; i++)
    {
        if (mem[i] != nullptr)
        {
            if (mem[i]->time > biggestTime)
            {
                biggestTime = mem[i]->time;
            }
        }
    }
    totalTime+=biggestTime;
    rePROCESS(arr);
    return totalTime;
}
/***************************************************************************
 * int nextFit (PROCESS* arr[])
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: nextFit algorithm
 * Parameters:
 * arr   I/P   PROCESS*[]   info stream of tasks to be processed in memory
**************************************************************************/
int nextFit(PROCESS* arr[])
{
    PROCESS* mem[MEM_FREE];                             	// Creates array of dynamic objects
    for(int i=0; i<MEM_FREE; i++)
    {
        mem[i] = nullptr;
    }
    int totalTime = 0;  
    int current = 0;
    int last = 0;
    int put = 0;
    while ( !allProcessesQue(arr) )                     	// While not all tasks have been placed.
    {
        do
        {
            if (last == -1)
            {
                last = 0;
            }
            put = searchArr(mem, last , arr[current]->size);    // Check if item can be placed
            if (put >= 0)
            {
                last = put;
                addToMem(mem, arr[current], put);           	// Place item on taskStream at put
                current++;
            }
        }while(put >= 0 && current < NUM_PROCESSES);            // Continue while we can place the item
        incrementTime(mem, arr, totalTime, current);
    }
    int biggestTime = 0;
    for(int i=0; i<MEM_FREE; i++)
    {
        if (mem[i] != nullptr)
        {
            if (mem[i]->time > biggestTime)
            {
                biggestTime = mem[i]->time;
            }
        }
    }
    rePROCESS(arr);
    return totalTime;
}
/***************************************************************************
 * int worstFit (PROCESS* arr[])
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: worstFit algorithm that finds the block that is biggest capable of processing the task.
 * Parameters:
 * arr   I/P   PROCESS*[]   info stream of tasks to be processed in memory
**************************************************************************/
int worstFit(PROCESS* arr[])
{
    PROCESS* mem[MEM_FREE];                                             // Creates array of dynamic objects
    for(int i=0; i<MEM_FREE; i++)
    {
        mem[i] = nullptr;
    }
    int totalTime = 0, current = 0, worst = -1, worstSize = 0;
    while ( !allProcessesQue(arr) )                                     // While not all tasks have been placed.
    {
        do{
            biggest(mem, worst, worstSize, arr[current]->size);
            if (worst >= 0)
            {
                addToMem(mem, arr[current], worst);                  	// Place item on taskStream at put
                current++;
            }
        }while(worst >= 0 && current < NUM_PROCESSES);                  // Continue while we can place the item
        incrementTime(mem, arr, totalTime, current);
    }
    int biggestTime = 0;
    for(int i=0; i<MEM_FREE; i++)
    {
        if (mem[i] != nullptr)
        {
            if (mem[i]->time > biggestTime)
            {
                biggestTime = mem[i]->time;
            }
        }
    }
    totalTime+=biggestTime;
    rePROCESS(arr);
    return totalTime;
}
/***************************************************************************
 * int uniform( int lo, int hi )
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: generates a random uniform value
 * Parameters:
 * lo     I/P     int    minimum value
 * hi     I/P     int    maximum value
**************************************************************************/
int uniform( int lo, int hi )
{
    int x = 0;
    int y = hi - lo + 1;
    int z = RAND_MAX / y;
    while ( (x = rand()) > y * z );
    return x / z + lo;
}
/***************************************************************************
 * int normal( int lo, int hi )
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: generates a random normal value
 * Parameters:
 * loops     I/P     int    number of loops to run
 * ulimit    I/P     int
**************************************************************************/
int normal ( int loops, int ulimit )
{
    int x = 0;
    for( int i=0; i< loops; i++ )
    {
        x += uniform( 0, ulimit );
    }
    return x;
}
/***************************************************************************
 * int main( int argc, char *argv[] )
 * Author: Venkata Bapanapalli
 * Date: 19 April 2020
 * Description: populates the info stream with random values for both size and time of task.
 * Then runs all the algoritms 1000 times find the average and prints the results.
 * Parameters:
 * argc    I/P    int        The number of arguments on the command line
 * argv    I/P    char *[]   The arguments on the command line
 * main    O/P    int        Status code (not currently used)
**************************************************************************/
int main(int argc, char *argv[])
{
    int avg[4] = {0, 0, 0, 0};
    int numProcesses = 1000;
    for(int i=0; i<numProcesses; i++)                                   // Loop for 1000 experiments
    {
        PROCESS* stream[NUM_PROCESSES];
        for( int j=0; j < NUM_PROCESSES; j++)                           // Generate Task Info Stream
        {
            stream[j] = new PROCESS;
            stream[j]->size = normal(4,4);
            if( stream[j]->size < 1 ) stream[j]->size = 1;
            stream[j]->time = normal(4,4);
            if( stream[j]->time < 1 ) stream[j]->time = 1;
            stream[j]->oTime = stream[j]->time;
        }
        avg[0] += bestFit(stream);
        avg[1] += firstFit(stream);
        avg[2] += nextFit(stream);
        avg[3] += worstFit(stream);
    }
    cout << " BestFit: " << avg[0]/1000 << endl;
    cout <<  " FirstFit: " << avg[1]/1000<< endl;
    cout << " NextFit: " << avg[2]/1000 << endl;
    cout << " WorstFit: " << avg[3]/1000<< endl;
    return 0;
}

