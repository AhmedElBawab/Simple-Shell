#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include <string.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <signal.h>
#include <sys/types.h>
#include <fstream>
#include <stdlib.h>

using namespace std;

string command;

void sigChld(int arg){
    //ofstream myfile;
    //myfile.open ("/media/ahmed/My Passport/Mine/Faculty/Operating Systems/Simple Shell/Interrupts");
    //myfile << "Child process was terminated\n";
    //myfile.close();

    FILE *file;
    file = fopen("/media/ahmed/My Passport/Mine/Faculty/Operating Systems/Simple Shell/Interrupts" , "a+");

    if(file == NULL){
        cout << "Error in file!" << endl;
    }

    fprintf(file , "Child process was terminated\n");

    fclose(file);

	signal(SIGCHLD, sigChld) ;
}

void get_command(){
    cout << "simple-shell:$ ";

    std::getline(std::cin, command);

    if(command == "exit"){
        exit(0);
    }

    if(command == ""){
        while(command == ""){
            cout << "simple-shell:$ ";
            std::getline(std::cin, command);
        }
    }

}

int main()
{
    signal(SIGCHLD, sigChld) ;

    get_command();

    while(command != "exit"){

        // Splite the string with spaces
        std::istringstream buf(command);
        std::istream_iterator<std::string> beg(buf), end;

        std::vector<std::string> tokens(beg, end); // done!

        if(command.find('&') != std::string::npos){
                tokens.pop_back();
        }

        std::vector<char *> argv(tokens.size() + 1);    // one extra for the null

        for (std::size_t i = 0; i != tokens.size(); ++i)
        {
            argv[i] = &tokens[i][0];
        }

        // Create a child process for the new command
        pid_t pid = fork();

        if(pid == 0){
            // This is a child command
            // Execute the command
            execvp(argv[0], argv.data());

            // Appeare when error
            cout << "No such command!" << endl;

        }else if(pid > 0){
            // This is parent process
            // Check the last char to decide to wait for child or not
            if(command.find('&') == std::string::npos){
                waitpid(-1, NULL, 0);
            }
        }else{
            // fork failed
            cout << "Fork failed! can't make a child process" << endl;
            return 1;
        }

        get_command();
    }

    return 0;
}
