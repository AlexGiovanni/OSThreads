//Alexis Santiago Project 1 02/14/2019
/*
* This is a very minimal shell. It finds an executable in the
* PATH, then loads it and executes it (using execv). 
*/
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 128
struct command_t {
	char *name;
	int argc;
	char *argv[MAX];
};
/*
*This function reads the PATH variable for this
*environment, then builds an array, dirs[], of the
*directories in PATH
*/
void parse_path(char *dirs[]){ 
  char *pathEnvVar;
  char *thePath;
  for(int i=0; i<64; i++)
    dirs[i] = NULL;
  pathEnvVar = (char *) getenv("PATH");
  thePath = (char *) malloc(strlen(pathEnvVar)+1);
  strcpy(thePath, pathEnvVar);

    int counter = 0;//used to keep track of array position
   //tokenize the string using : as the delimeter1	
    for (char *token = strtok(thePath,":"); token != NULL; token = strtok(NULL, ":")){
 	  char * temp = (char *) malloc(strlen(token)+1);
      strcpy(temp, token);
      dirs[counter]=strcat(temp, "/");//adds / to directories
      counter++;
	}
  
   	
}//end of parse path

/*
*This function builds argv[] and sets argc number
*/
 void parse_line(char *line, struct command_t *command){ 
	//tokenize the line
    char *token;
    token = strtok(line, " ");
    int counter = 0;
    command->argv[counter]=token;//strcat(token, "\0"); 
    command->argc=1;
    while(token != NULL){
    	token = strtok(NULL, " ");
    	counter++;
    	strtok(token,"\n");//remove new line char
		command->argv[counter] = token;//strcat(token, "\0");
		command->argc++;
    }

}//end of parse line

/*
*This function searches the directories identified by the dir
argument to see if argv[0] (the file name)appears there
*returns string containing the path to the executable file if it exists
*/
char *lookupPath(char **argv, char**dirs){
   
  if(*argv[0] == '/'){
  	return argv[0];
  }
 
//look in PATH directories
//use access() to see if the file is in a dir
for(int i=0; i<sizeof(*dirs)/sizeof(*dirs[0]); i++){
    char * cmd = (char *) malloc(strlen(dirs[i])+strlen(argv[0]));//allocates new string
   	strcpy(cmd, dirs[i]);
    strcat(cmd, argv[0]);
    strtok(cmd,"\n");//remove new line char

    if( access(cmd, F_OK | X_OK) == 0){
      return cmd;
    }
      memset(cmd, 0, strlen(cmd));//used to clear cmd and search another directory
}



//file name not fount
  fprintf(stderr, "%s: command not found\n", argv[0]);
  return NULL;

}//end lookup path

int main(){
  char line[MAX];//stores the line before tokenizing it
  struct command_t command;
  char *dirs[64];
  parse_path(dirs);

  while(1){

 
    printf("\nsh> ");
    fgets(line, MAX, stdin);  
    if(strcmp(line, "exit\n")== 0){
    	exit(0);
    }
   

    parse_line(line, &command);
    command.name = lookupPath(command.argv, dirs);
    if (command.name==NULL){//no command
    	continue;
    }


    int pid = fork();
    if(pid == 0){//child
      strtok(command.name,"\n");//remove new line char
	  execv( command.name , command.argv);
      exit(0);
	}
	else{
      wait(NULL);
    }
   

  }//end of while 

  return 0;
}


