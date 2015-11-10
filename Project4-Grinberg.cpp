/*
Project 4 - Readers and Writers Using A Monitor
CS 311
Code by: Jeffrey Grinberg
*/

/*
Notes:
	Mode: B -- Writers have absolute priority
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
void read_mon(int mode);
void write_mon(int mode);
void writeToFile(string filename);

int main(int argc, char *argv[]) 
{
	// Make sure we have received the filename:
	if (argc != 2)
	{
		cout << "Please input your filename for the output with the extension." << "\n";
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
	cout << "How many readers will the program use?" << "\n"; // Ask user for input
	cin >> r;
	cout << "How many writer will the program use?" << "\n"; // Ask user for input
	cin >> w;
	cout << "What delay will the reader have?" << "\n"; // Ask user for input
	cin >> R;
	cout << "What delay will the writer have?" << "\n"; // Ask user for input
	cin >> W;
	cout << "The program will write to: "; // Ask user for input
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
	//read_id = new(r); // Make space for amount of readers we will have.
	//write_id =  new(w); // Make space for amount of writers we will have.
	
	//Create the threads! WRITERS HAVE PRIORITY!
	
	}
	return 0;
}