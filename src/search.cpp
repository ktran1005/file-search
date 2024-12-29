// Charles Tran HW7

// libraries needed in this problem
#include "search.h"
#include <thread>
#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <iomanip>

// a flag for condition variable
bool canStartApp = true;

//Simulate Producer thread implementation
// The main task of producer thread is to find files that under the extension that we want
// add it to the queue and signal to thread pool so they can do their task
void producer(std::queue<std::string>* filesQueue, std::string dirToSearch,bool* donePushingIntoQueue)
{


	for (const auto &p : std::filesystem::recursive_directory_iterator(dirToSearch))
    {

        std::filesystem::path file = p.path();
        std::string extension = file.extension();
        if (extension == ".cc" || extension == ".c" || extension == ".cpp" || extension == ".h" || extension == ".hpp" || extension == ".pl" || extension == ".sh" || extension == ".py" || extension == ".txt")
        {
            std::string element = file;
            filesQueue->push(element);
        }
    }
	// Set this flag to true to know the producer thread is done.
	*donePushingIntoQueue = true;

}
	
// Simulate Thread Pool implementation
// The main task of thread pool is to take one file from the queue, open it and search for a text string.
// if a thread finishes early, they will check the queue if there is any files available in the queue that need to open.
void worker(std::condition_variable *cv, std::mutex *mutex,std::queue<std::string>* files, bool* donePushingIntoQueue, std::string target)
{
	std::ifstream myFile;
	while(*donePushingIntoQueue != true || files->size()!=0){
		std::unique_lock<std::mutex> lk(*mutex);
		// condition variable
		cv->wait(lk,[]{
			return canStartApp;}); 
		std::string str;
		// check if the queue is not empty
		if (!files->empty()){
			str = files->front();
			files->pop();
		}

		// release the lock
		lk.unlock();
		// signal all other threads
		cv->notify_all();

		// each thread will open their file and search for the text
		myFile.open(str);
		// check if we open the file successfully
		if (myFile.is_open()) {
			std::string line;
			int i=0;
			// read line by line from the file
			while (getline(myFile, line)) {
				i++;
				// convert it to array of character to find a match
				if (strstr(line.c_str(), target.c_str())) {
					printInfo(str, i, line);
				}
			}
			// close the file
			myFile.close();
		}

	}
}


//Print Info
void printInfo(std::string str, int i, std::string line){
	static std::mutex ioLock;
	std::lock_guard<std::mutex> lk(ioLock);
	std::cout << "----------" << "\n";
	std::cout << "Thread 0x"<< std::hex <<std::this_thread::get_id() << " found a match." << std::endl;
	std::cout << "File: " << str << "\n";
	std::cout << "Line " << std::dec <<i << ": " << line << "\n";
	std::cout << "----------" << "\n";
	
}
