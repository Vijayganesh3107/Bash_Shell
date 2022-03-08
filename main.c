




/*
Assignment_1
Name:Vijay Ganesh Panchapakesan
Student_Id:1001861777
*/







#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
//The dirent.h is used because in the code I have used opendir() which opens a particular directory
//or to check if a directory is present or not.
#include <dirent.h>
#define MAX_COMMAND_LEN 255
#define MAX_HISTORY_SIZE 15
int main()
{
    //The user_cmd stores the command that the user enters
    //The argv is used to store the tokenized value of the user_cmd with " " as the delimiter
    //argcounter is the count of number of values in argv which is declared inside the while
    char user_cmd[MAX_COMMAND_LEN];
    char* argv[10];
    
    //file_path variable is to store the final path 
    //to check if the commands are availabe in 
    //"/" or "/bin" or"/usr/bin" or "/usr/local/bin"
    char file_path1[50];
    char file_path2[50];
    char file_path3[50];
    char file_path4[50];
    
    //hisarr is used to store the top 15 commands typed by the user
    //hcount is used to count the number of elements in the hisarr
    //pidarr is used to store the max of 15 pids created for the processes
    int hcount=0;
    char* hisarr[MAX_HISTORY_SIZE];
    int pidcount=0;
    pid_t pidarr[MAX_HISTORY_SIZE];
    
    
    while(1){
        char *token;
        int argcounter=0;
        
    //program will print out a prompt of msh> when it is ready to accept input.
        printf("msh> ");
        
        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_LEN
        // This while command will wait here until the user
        // inputs something since fgets breaks when there
        // is no input
        if(!fgets(user_cmd,MAX_COMMAND_LEN, stdin))
        {
            break;
        }
        int length = strlen(user_cmd);
        
        // If the user types a blank line, your shell will, quietly
        //and with no other
        // output, print another prompt and accept a new line of input.
        if(length == 0)
        {
            continue;
        }
        
        //Making the last charecter of the user_cmd to a null character
        if (user_cmd[length - 1] == '\n'){
            user_cmd[length - 1] = '\0';
        }
        
        
       

        //if the hcount is less than 15 we jsut need 
        //to add the user_cmd to the array and increment hcount
        //This part is for adding the data for the first 15 commands 
        if(hcount<MAX_HISTORY_SIZE)
        {
        hisarr[hcount]=strdup(user_cmd);
        hcount++;
        }
        
        //if the hcount > 15 then remove the first element of hisarr
        //and move all the other element of hisarr above
        //i.e the 2nd element would become the 1st element and so on
        else
        {
        free(hisarr[0]);
        int i=1;
        while(i<MAX_HISTORY_SIZE)
        {
        hisarr[i-1]=hisarr[i];
        i++;
        }
        hisarr[MAX_HISTORY_SIZE-1]=strdup(user_cmd);
        }
        
         //If the user types !n then the below code would take 
        //the value of n from the !n and conver the same to an int
        //and getting the n-1th command from the hisarr
        //and making it as the user_cmd,so that this command would be executed
        if(strncmp(user_cmd,"!",1)==0)
        {
            char *cmd_int_part = user_cmd + 1;
            char *rest;
            char cmd[10];
            int no = strtol(cmd_int_part, &rest, 10);
            
            //If hcount<15 just we can append the data to the hisarr
            if(hcount<MAX_HISTORY_SIZE)
            {
                if((no<MAX_HISTORY_SIZE && no>0) || (no<hcount))
            {
                strcpy(cmd,hisarr[no]);
                strcpy(user_cmd,cmd);
            }
            else if(no>hcount){
                printf("No commands found in hisory\n");
            }
                
            }
            //This is when hisarr is full.In this case we need to move
            //every data one step above and remove the top element
            else if(hcount==MAX_HISTORY_SIZE)
            {
                if((no<MAX_HISTORY_SIZE && no>0) || (no<hcount))
            {
                strcpy(cmd,hisarr[no-1]);
                strcpy(user_cmd,cmd);
            }
            else if(no>hcount){
                printf("No commands found in hisory\n");
            }
            }
            
         }
        
        
        // Your shell will exit with status zero if the command is “quit” or “exit”
        if(strncmp(user_cmd,"exit",4)==0||(strncmp(user_cmd,"quit",4))==0)
        {
            strcpy(user_cmd,"exit");
        }
        
        
        //The token will have tokkenized value of the user_cmd
        //with delimiter as " "
        //while the token is not null then it will be added to
        //argv and argcounter will be incremented
        //the last value of arv is set to null.argv contains
        //all the commands that user entered
        //in a ptr array with " " as a delimiter
        token = strtok(user_cmd," ");
        if(token == NULL){
            continue;
        }
        while(token!=NULL){
            argv[argcounter]=token;
            token = strtok(NULL," ");
            argcounter++;
        }
        //Making the last token of the argv to a null character to denote the EOL.
        argv[argcounter]=NULL;
        
        //Your shell will exit with status zero if the command is “quit” or “exit”
        if((strncmp(*argv,"exit",4)==0))
        {
        exit(0);
        }
        
        //If user enters History then it would take all the elements from hisarr
        else if (strcmp(*argv,"history")==0)
        {
            int k;
        for(k=0;k<hcount;k++)
        {
        printf("%d %s\n",k,hisarr[k]);
        }
        
        }
        
        //Your shell shall support the showpids command to list the PIDs of
        // the last 15 processes spawned by your shell.
        //If user enters Showpids then it would take all the elements from hisarr
        else if(strcmp(*argv,"showpids")==0)
        {   int k;
            for( k=0;k<pidcount;k++)
            {
                printf("%d:%d\n",k+1,pidarr[k]);
            }
        }
        
        //Your shell shall support the cd command to change directories. Your
        // shell must handle cd .. .This functionality uses opendir which 
        //opens a directory given and if it is opening then chdir to that
        //directory else print no directory found
        else if (strcmp(*argv,"cd")==0)
        {
            char direct[10];
            strcpy(direct,(argv[argcounter-1]));
            //Opening the directory and checking if the directory is
            //present or not.
            DIR* directory = opendir(direct);
            if(directory)
            {
                chdir(direct);
            }
            else
            {
                printf("No such directory found \n");
            }
            
        }
        
        //For rest all commands we need to check if the command is available in  
        //path1,path2,path3,path4, if not display that the command is not found
        //This can be executed with the help of fork and excevp in my case.
        //In the fork in the child I am checking if the command
        //is present in any of the path
        //And if they are not available I am displaying the error message
        //In the parent I am just getting the pids and storing it in pidarr
        //I will be diplaying only the top 15 pids.
        else
        {
            char* path1= "/";
            char* path2= "/usr/local/bin/";
            char* path3= "/usr/bin/";
            char* path4= "/bin/";
            strcpy(file_path1, path1);
            strcat(file_path1, argv[0]);
            strcpy(file_path2, path2);
            strcat(file_path2, argv[0]);
            strcpy(file_path3, path3);
            strcat(file_path3, argv[0]);
            strcpy(file_path4, path4);
            strcat(file_path4, argv[0]);
            
            pid_t pid, wpid;
            int status;
            //Forking to get two process parent and a child.
            //If the os can access file_path1 or the other parts
            //then the exce of the command 
            //will take place with the help of excevp in my case.If
            //excevp in none of the path
            //is executed then it would throw an comment that the command
            //is not available
            pid = fork();
            if (pid == 0) {
                if ((execvp(file_path1,argv) == -1)&&(execvp(file_path2,argv) == -1)&&(execvp(file_path3,argv) == -1)&&(execvp(file_path4,argv) == -1)) {
                  printf("%s Command is not available\n",*argv);
                }
                exit(0);
                
            }
            //This the parent and here I am adding the pids to pid arr and
            //asking the parent to wait untill the child finishes the execution
            else if (pid > 0) {
                wait(&status);
                if(pidcount<MAX_HISTORY_SIZE)
                {
                pidarr[pidcount]=pid;
                pidcount++;
                }
                else
                {
                int i=1;
                while(i<MAX_HISTORY_SIZE)
                {
                pidarr[i-1]=pidarr[i];
                i++;
                }
                pidarr[MAX_HISTORY_SIZE-1]=pid;
                }
                
            }
            else {
                perror("Fork Error as returned -1");
            }
        }
    }
}









