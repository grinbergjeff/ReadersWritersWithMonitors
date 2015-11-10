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

int main(int argc, char *argv[]) 
{
	// Make sure we have received the filename:
	if (argc != 6)
	{
		cout << "Please input your filename for the output with the extension and the amount/delays for the reader/writers." << "\n";
		cout << "Your run command with arguments needs to look like:" << "\n";
		cout << "./a.out 'filename' #ofReaders #ofWriters AmtReaderDelay AmtWriterDelay " << "\n";
		cout << "Program will exit. Please run the program again!" << "\n";
		exit(0);
	}
	else
	{
		//Declare the PThreads
		pthread_t * read_threads;
		pthread_t * write_threads;

		// Request User to input information
		cout << "The program will run Mode B (2):" << "\n";
		cout << "Writers have absolute priority over the readers." << "\n";
		cout << "The program will use " << argv[2] << " readers." "\n"; 
		r = atoi(argv[2]);
		cout << "The program will use " << argv[3] << " writers." "\n"; 
		w = atoi(argv[3]);
		cout << "The readers will have a delay of " << argv[4] << " ms." "\n"; 
		R = atoi(argv[4]);
		cout << "The writers will have a delay of " << argv[5] << " ms." "\n"; 
		W = atoi(argv[5]);
		cout << "The program will write to: ";
		cout << argv[1] <<  "\n";
	
		get_wall_clock(&seconds, &milliseconds); // Implement time_function.h
	
		//Open the file we are writing the output to.
		//writethis.open(argv[1]);

		// Initialize the Pthreads
		pthread_mutex_init(&out_lock, NULL);
		pthread_mutex_init(&monitor_lock, NULL);
		pthread_cond_init(&reader_condition, NULL); // Conditional Variable for the Reader
		pthread_cond_init(&writer_condition, NULL); // Conditional Variable for the Writer
	
		//Identify active and waiting readers/writers
		int activeReaders = 0;
		int activeWriters = 0;
		int delayedReaders = 0;
		int delayedWriters = 0;
	
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
		
		//writethis.close(argv[1]);
		
		//WE ARE DONE HERE. Destroy everything! As Gary Oldman would SCREAM, 
		//EEEEVVVEEERRRRRYYTTTHHHIING! [Go watch: Leon: The Professional]
		
		pthread_mutex_destroy(&out_lock);
		pthread_mutex_destroy(&monitor_lock);
		pthread_cond_destroy(&reader_condition);
		pthread_cond_destroy(&writer_condition);
		delete (write_threads);
		delete (read_threads);
		delete (write_id);
		delete (read_id);
	}
	return 0;
}

void * thread_reader(void *something)
{
	
}
void * thread_writer(void *something)
{

}
void read_monitor(int operation)
{

}
void write_monitor(int operation)
{

}
void writeToFile(string filename)
{

}