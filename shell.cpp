#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include<stdio.h> 
#include<vector>
#include<chrono>
#include<ctime>
#include <iomanip>
#include <unistd.h>

using namespace std;

const auto start= std::chrono::system_clock::now();

int transfer(string input,vector<string> &history);
void showhistory(vector<string> &history);
int main(void) {
    // forever (until "exit" is entered by the user
    vector<string> history;
    int result=1;
    pid_t PID=getpid();
    while (result) {
        // print a prompt
        cout << "input command:";
        string input;

        // read a line of input from the user
        getline(cin, input); // at this point, I need to 'tokenize' cmd
	
	if(input=="exit")
		break;
	history.push_back(input);
	result=transfer(input,history);

    }

    return 0;
}
int transfer(string input,vector<string> &history)
{
 	auto start1 = std::chrono::system_clock::now();
	string temp,empty;
	vector<string> sv;
	for(int i=0;i<input.size();i++)
	{
		if(input[i]!=' ')
			temp+=input[i];
		else
		{
			sv.push_back(temp);
			temp=empty;
			
		}
	}
	sv.push_back(temp);
	string cmd;		
	cmd=sv[0];
	char** args=new char*[sv.size()+1];
	for(int i=0;i<sv.size();i++)
	{
		args[i]=(char*)sv[i].c_str();

	}
	args[sv.size()]=(char*)NULL;
	cout <<"command is:" << cmd << endl;
        // we might decide if the input is a builtin command
	if(cmd=="exit")
	{
		delete[] args;
		return 0;
	}
	else if(cmd=="history")
	{
		showhistory(history);
		delete[] args;
		return 1;
	}
	else if(cmd=="running_time")
	{
			
		auto end = std::chrono::system_clock::now();
	//	std::chrono::duration<double> elapsed_seconds = end-start;
		std::time_t diff=std::chrono::system_clock::to_time_t(end)-std::chrono::system_clock::to_time_t(start);
		std::time_t hh=diff/3600;
		std::time_t mm=(diff%3600)/60;
		std::time_t ss=diff%60;
		cout<< "running_time is  "<<hh<<":"<<mm << ":"<<ss<<endl;	
		delete[] args;
		return 1;
	}
	else if(cmd=="ptime")
	{
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start1;
		std::time_t end_time = std::chrono::system_clock::to_time_t(end);
		cout << "finished computation at " << std::ctime(&end_time) <<endl;
		
		cout << "Time spent executing last child processes: " << std::fixed <<  std::showpoint << std::setprecision(4) << elapsed_seconds.count() << " seconds" << endl;
        	delete[] args;
		return 1;
	}
	else if(args[0][0]=='^')
	{
		string newS=cmd.substr(1);
		
		int index= stoi(newS);
		
		if(index>=1&&index<=history.size())
		{
			history.push_back(history[index-1]);
			transfer(history[index-1],history);
				
		}
		else{
		cout << "invalid input number" << endl;
			
		}
		delete[] args;
		return 1;
	}
	
	else{
            // fork
	   try{
 	    int status=0;
	    pid_t PID=fork();
	    cout << "pid is :" <<PID << endl;
            if (PID==0)
	    {
   		
		int value=execvp((char*)sv[0].c_str(), args);
		if(value==-1)
		{
			cout << "the command is not correct!" << endl;
			exit(-1);
		}
            }
	    else
 	    {
                // parent waits for the child to finish
               
                 wait(&status);
		
             }
	  }
	catch(...){
               // command not found, or similar error
             cerr << args[0] << " did something wrong\n";
             
	}
	delete[] args;
        return 1;  
      }
	

}
void showhistory(vector<string> &history)
{
	cout << "---- Command History ---" << endl;
	for(int i=0;i<history.size();i++)
	{
		cout << i+1<< " :" <<std::fixed << history[i] << endl; 
	}

}