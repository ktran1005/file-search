
/**
 * @mainpage CS361 - File search with multithreading
 * @section Description
 * 
 * <Include some description here>
 * 
 *  Make commands:
 * 
 *  make
 * 
 *  will build the program.
 * 
 *  make run
 * 
 *  will run the program with a target string to search.
 * 
 *  make clean
 * 
 * 	will clear out compiled code.
 * 
 *  make doc
 * 
 * 	will build the doxygen files.
 */

/**
 * @file
 * @author <Charles Tran>
 * @date 2022-2023
 * @section Description
  
*/


#include <iostream>
#include <thread>
#include <queue>
#include "search.h"
#include <condition_variable>
#include <filesystem>




/**
 The main program will take two command line input. The first command input is target (a string to search for).
 The second one is dir (a directory to search).  The second command line input is optional. The program will find 
 every line of every file in the directory (and all subdirectories) that include the target line.
 Note: The program cannot run if a target is not given. If a directory is not given, use the current directory
 as the strating point.
 
 * @param argc number of command line arguments, expected value is 3
 * @param argv has value argv[1] a string of text to search for, argv[2] (optional) a directory to search
 * @return 1 on failure and 0 on success
 */

int main(int argc, char* argv[]){

	//Sanity Check on inputs
	if (argc  < 2 ){
		std::cout << "The program needs at least 2 inputs to run the program."
			<< std::endl;
		return 1;
	}

	std::string myDirectory;
	if (argc == 2) {
		myDirectory = std::filesystem::current_path();
	}

	if (argc == 3) {
		myDirectory = std::filesystem::current_path();
		myDirectory += "/";
        // A directory to search
		myDirectory += argv[2];
	}


    // a string of text to search for
	std::string target = argv[1];
    // number of threads
	int numOfThreads= std::thread::hardware_concurrency();

	//Make threads
	std::thread* T = new std::thread[numOfThreads];
	// mutex lock
	std::mutex* sharedMutex= new std::mutex();
    // condition variable
	std::condition_variable* sharedCondVar = new std::condition_variable();

    // flags for condition variable
	bool done = false;
	bool canStart = false;
	
	//Make two queues. One is for the task, another one is for thread_id
	std::queue<std::string>* fileList = new std::queue<std::string>();
	std::queue<std::thread::id>* workers = new std::queue<std::thread::id>();
	
	std::cout << "!---- Search Started ----! "<< "\n";
	std::cout << "Target Folder: " << myDirectory << "\n";
	std::cout << "Target Text: " << target << "\n";
	std::cout << "Using a Pool of " << numOfThreads << " threads to search." << "\n";

	// Start the producer
	T[numOfThreads-1] = std::thread(producer,fileList, myDirectory, &done);
    // Start the workers
	for(int i=0; i < numOfThreads-1; i++){
		T[i] = std::thread(worker,sharedCondVar,sharedMutex,fileList,&done, target);
	}
	//Join Everything
	for(int i=numOfThreads-1; i >= 0 ; i--){
		T[i].join();
	}
    // Print out task is done
	std::cout << "!---- Search Complete ----!" << "\n";
	return 0;
}

// Citation
// https://en.cppreference.com/w/cpp/filesystem/recursive_directory_iterator
// https://en.cppreference.com/w/cpp/filesystem/current_path
// https://en.cppreference.com/w/cpp/filesystem/path/extension
// https://cplusplus.com/forum/windows/51591/