// Charles Tran HW7
// Header files of producer thread and thread pool


#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <queue>
#include <thread>
#include <condition_variable>
#include <cstring>

//Simulate the Producer Thread
void producer(std::queue<std::string>* filesQueue, std::string dirToSearch,bool* donePushingIntoQueue);

//Simulate the Thread Pool
void worker(std::condition_variable *cv, std::mutex *mutex,std::queue<std::string>* files, bool* donePushingIntoQueue, std::string target);
	
//Print Info
void printInfo(std::string str, int i, std::string line);

#endif