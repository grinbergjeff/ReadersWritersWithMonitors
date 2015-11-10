/*
Project 4 - Readers and Writers Using A Monitor
CS 311
Code by: Jeffrey Grinberg
*/

/*
Notes:
	Mode: B -- Writers have absolute priority
	
	Order of Coding Operation:
		-- Design Main
			-- Allocate space for threads
			-- Initialize Pthreads/Mutexes/Conditional Variables
			-- Create Pthreads
			-- Enforce that Writers are created first (added precaution).
			-- Join Pthreads
			-- Destroy Pthreads/Mutexes/Conditional Variables
		-- Implement Functions
			-- thread_reader
				- For every thread we read, we will run the respective monitor and read the DB.
			-- thread_writer
				- For every thread we write, we will run the respective monitor which tells us if we can write from the DB.
			-- read_monitor
				- The monitor will determine if we are allowed to read from the DB. It intakes if it should begin or end.
			-- write_monitor
				- The monitor will determine if we are allowed to write from the DB. It intakes if it should begin or end.
			-- writeToFile
				- Write the values of access timestamps to the output file.
				
	*** Consider implementing a debugger!
	
	*** INPUT AND OUTPUT FILES NEED TO BE HARDCODED IN [IN A DIRECTORY]!
	
	*** Instead of sleep, run a loop, but compile without optimization?
	
	*/

//Includes:
/*Configure for WIN32*/
#ifdef _WIN32
#include "pthread.h"
#else
#include <pthread.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "time_functions.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

// Variable Declarations:
int accessMax = 10; // How many thread accesses we will run through at most.

time_t seconds;
unsigned short milliseconds;

int * read_id; // Thread to ID
int * write_id;

int r; // number of readers
int w; // number of writers
int R; // delay for reader
int W; // delay for writer

//Declaration of ostream (write to files)
ifstream readthis;
ofstream writethis;
string file;

//Declaration of Mutexes
pthread_mutex_t out_lock;
pthread_mutex_t monitor_lock;
pthread_cond_t reader_condition;
pthread_cond_t writer_condition;

//Function Declarations
void * thread_reader(void *something);
void * thread_writer(void *something);
void read_monitor(int operation);
void write_monitor(int operation);
void writeToFile(string filename);

//Identify active and waiting readers/writers
		int activeReaders = 0;
		int activeWriters = 0;
		int delayedReaders = 0;
		int delayedWriters = 0;

int main() 
{
		//Declare the PThreads
		pthread_t * read_threads;
		pthread_t * write_threads;

		// Access input and output file
		//File Path:
		char *inFilePath = new char[50];
		char *outFilePath = new char[50];
		
		strcpy(inFilePath, filebase);
		strcat(inFilePath, "project4_in.txt");
		strcpy(outFilePath, filebase);
		strcat(outFilePath, "project4_out.txt");
		
		//Open the file we are writing the output to.
		//Open the input file:
		readthis.open(inFilePath);
		
		//Verify the file is not corrupt:
		if ( readthis.good() == false)
		{
			cout << "input file couldn't open properly.";
			exit(0); // Leave if file could not open properly.
		}
		
		//Grab from the input file
		readthis >> r >> w >> R >> W;
		cout << "r = " << r << "\n" << "w = " << w << "\n" << "R = "<< R << "\n" << "W = " << W << "\n";
		
	
		//Open the output file:
		writethis.open(outFilePath);
	
		get_wall_clock(&seconds, &milliseconds); // Implement time_function.h

		// Initialize the Pthreads
		pthread_mutex_init(&out_lock, NULL);
		pthread_mutex_init(&monitor_lock, NULL);
		pthread_cond_init(&reader_condition, NULL); // Conditional Variable for the Reader
		pthread_cond_init(&writer_condition, NULL); // Conditional Variable for the Writer
	
	
		//Make space for thread IDs
		read_id = new int[r]; // Make space for amount of readers we will have.
		write_id =  new int[w]; // Make space for amount of writers we will have.
	
		//Create the threads! WRITERS HAVE PRIORITY! Writers will be created first, and therefore run first.
		write_threads = new pthread_t[w];
		int i = 0;
		for ( i = 0; i < w; i++)
		{
			write_id[i] = i+1;
			pthread_create(&write_threads[i], NULL, thread_writer, &write_id[i]);
			cout << "Write Thread ID " << write_id[i] << " created" << " \n";
		}
		//Create the reader_threads
		read_threads = new pthread_t[r];
		for (i = 0; i < r; i++)
		{
			read_id[i] = i+1;
			pthread_create(&write_threads[i], NULL, thread_reader, &read_id[i]);
			cout << "Read Thread ID " << read_id[i] << " created" << " \n";
		}
		
		// JOIN THE THREADS!
		// As a precaution, JOIN the threads in the order in which they were created.
		for (i = 0; i < r; i++) // Join all Writer Threads
		{
			pthread_join(write_threads[i], NULL);
		}
		
		for (i = 0; i < r; i++) // Join all Reader Threads
		{
			pthread_join(read_threads[i], NULL);
		}
		// By now, all of the functions will have ran and the output will have been written
		// to the output file. We can close it.
		
		readthis.close();
		
		//WE ARE DONE HERE. Destroy everything! As Gary Oldman would SCREAM, 
		//EVERYONEEEEEEE! [Go watch Leon: The Professional]
		
		pthread_mutex_destroy(&out_lock);
		pthread_mutex_destroy(&monitor_lock);
		pthread_cond_destroy(&reader_condition);
		pthread_cond_destroy(&writer_condition);
		delete (write_threads);
		delete (read_threads);
		delete (write_id);
		delete (read_id);
		
	return 0;
}

// This function will attempt to access the database. When it does, it will activate the read_monitor
// which performs the identification of active and delayed readers/writers and the implementation of 
// the mutexes and locks. The thread_reader will record the current time and output it to the output file
void * thread_reader(void *something)
{
	int ID = *((int *)something);
	string writeOut;
	
	cout << "Starting to run Reader_ID " << " " << ID << "\n";
	
	int i;
	for (i = 0; i < accessMax; i++)
	{
		// Activate the monitor!
		read_monitor(1);
		
		//Time Math
		get_wall_clock(&seconds, &milliseconds);
		milliseconds = milliseconds;
		seconds = seconds / 10000000;
		
		
		//Write this to the Output File
		writeOut = ">>> DB value read =: %hu:%hu by reader number: %d\n", (unsigned short)seconds, milliseconds, ID;
		writeToFile(writeOut);
		//Write this to the Terminal Window
		cout << writeOut;
		
		//Stop the monitor;
		read_monitor(0);
		
		//Delay the Reader!
		millisleep(R); // Foreman recommends the possibility of a Loop for better results?
	}
	pthread_exit(0);
	return 0; //This is a void.
}



void * thread_writer(void *something)
{

}

//When the monitor is on (operation is true or '1'), the read monitor will send out a lock for mutex, giving it 
// undivided and protected access to the critical section, preventing the other writers/readers from accessing.
// However, if there are delayed or active writers, the monitor will tell the current reader to WAIT and increment
// the amount of delayed readers we have.
// If the monitor is off, we decrement the amount of readers we have since we are not using it. If we do have writers waiting,
// we need to send the signal to the monitor to active the monitor for the writer and let the delayed writer become active.
void read_monitor(int operation)
{
	if (operation) //If read_monitor is turned on
	{
		//While Monitor is on, incorporate Mutual Exclusivity
		pthread_mutex_lock(&monitor_lock);
		
		//Now, here is the important part! WRITERS HAVE PRIORITY! IF A READER IS LOOKING TO RUN
		// AND THERE ARE EITHER ACTIVE OR WAITING WRITERS, IT ***MUST*** WAIT!!!
		
		while((delayedWriters + activeWriters) > 0) // If there are delayed or active writers
		{
			delayedReaders++; // Readers must WAIT.
			pthread_cond_wait(&reader_condition, &monitor_lock); // If there are no writers waiting, decrement
			delayedReaders--;
		}
		//If there are no delayed or active writers, then we can increment the active Readers
		activeReaders++;
		pthread_mutex_lock(&monitor_lock);
	}
	else // If read_monitor is turned off
	{
		pthread_mutex_lock(&monitor_lock); // Implement Mutex.
		activeReaders--; // Not using Read_monitor, therefore, activeReaders decrements.
		if (delayedWriters > 0 && activeReaders == 0)
		{
			pthread_cond_signal(&writer_condition); // If we have Writers waiting to work, SIGNAL the monitor to make it run!
		}
		pthread_mutex_unlock(&monitor_lock);
	}
}


void write_monitor(int operation)
{
	
}

// Writes the status of the threads that access the DB to the output file
void writeToFile(string writeme)
{
	pthread_mutex_lock(&out_lock);
	writethis << writeme;
	pthread_mutex_unlock(&out_lock);
	
	writethis.close();
}