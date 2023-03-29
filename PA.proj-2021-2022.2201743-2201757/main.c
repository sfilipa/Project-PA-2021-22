/**
 * @file main.c
 * @brief Description
 * @date 2018-1-1
 * @author name of author
 */

/*
* Micael Ferreira Marques - nº2201743
* Sara Filipa dos Santos Martins - nº2201757
*/

/* Include of libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>
#include <signal.h>

#include "debug.h"
#include "memory.h"
#include "args.h"

/* Defining global variables */
#define MAX_STRING 100

char *filename = NULL;
char *fichfilename = NULL;
char *directory = NULL;
char *file = NULL; //to store the name of the file to make the print on signal SIGUSR1

/* Defining counters */
int countOk = 0;
int countMis = 0;
int countError = 0;
int countFiles = 0;
int countFileNumber = 0; //to make the print of number of file on signal SIGUSR1

/* Declaration of functions */
int extensionfile (void);
int checkIfFileExists(void);
void treat_sign(int signal);
void help();
void optionFile();
void optionBatch();
void optionDirectory();
int checkIfFileEmpty();

char timeSIGUSR1[MAX_STRING];

struct sigaction act;

/* Main code */
int main(int argc, char *argv[]) {
	
	/* gengetopt arguments to be able to use gengetopt */
	struct gengetopt_args_info args;

	/* Declare and format time */
	time_t execTime;
	struct tm *dataInfo;
	
	/* gengetopt arguments to be able to use gengetopt */
	if(cmdline_parser(argc, argv, &args) != 0){
		return 1;
	}

	/* Save day, month, year, hours, minutes and seconds in variable dataInfo */
	time(&execTime);
	dataInfo = localtime(&execTime);
	strftime(timeSIGUSR1, MAX_STRING, "%Y/%m/%d_%Hh%M:%S", dataInfo);

    /* Routine of treat signals */
    act.sa_handler = treat_sign;
    act.sa_flags = 0;
    
	/* Captures SIGQUIT and SIGUSR1 signals */
    if (sigaction(SIGQUIT, &act, NULL) < 0) {
		ERROR(2, "sigaction - SIGQUIT");
    }
    
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
			ERROR(1, "sigaction - SIGUSR1");
	}
	
    /* Arg --help */
    if(args.no_help_given){
        help();
        return 0;
    }

    /* Informs user about PID */
    printf("CheckFile's PID: %d\n\n", getpid());

	/*Arg -f --file */
	for (int i = 0; i < (int)args.file_given; ++i){
		filename = args.file_arg[i];
		optionFile();
	}
	
	/*Arg -b --batch */
	if (args.batch_given){
		fichfilename = args.batch_arg;
		optionBatch();
	}

	/*Arg -d --dir */
	if (args.dir_given){
		directory = args.dir_arg;
		optionDirectory();
	}

	/* gengetopt arguments to be able to use gengetopt */
    cmdline_parser_free(&args);
    
    return 0;
}

/*Function extensionfile used to verify the type and extension of a filename*/
int extensionfile (void){
	int link[2]; //File descriptors for pipe
	char storeOutputFile[4096]; //variable to save the file output
	
	pid_t pid;
	 /*Creating pipe to make the communication between parent process and child process*/
	if (pipe(link)==-1){
		ERROR(1, "Error executing pipe.\n");
	}
	
	/*Use of fork() to create a new process*/
	switch (pid = fork()) { 
    case -1:
        ERROR(1, "Error executing fork().\n");
		break;
    case 0:
		/*we will write, we can close link[0]*/
		close(link[0]);
		/*the link[1] is the file descriptor of stdout*/
        dup2(link[1], STDOUT_FILENO);
        
		execlp("file", "file", "-i", filename, NULL);
        ERROR(1, "Error executing execlp.\n");
        break;
    default:
        /*we will read, we can close link[1]*/
		close(link[1]);
		/*reading what was written on the pipe and store it on the variable storeOutputFile*/
        read(link[0], storeOutputFile, sizeof(storeOutputFile));

		char copy[4096]; 
		
		strcpy(copy, storeOutputFile); //to copy the output of "file", to not loose

		char *type = strtok(copy, "/"); 
		type = strtok(NULL, ";"); //to separate the file type from the rest of the string that is not necessary
		
		int pos = strlen(filename)-1; //get the length of filename
		
		while(filename[pos] != '.'){ //to get the extension of the file
			pos=pos-1;
					
			if (pos == 0){ //when the file has no extension in the name
				fprintf(stderr, "[ERROR] file has no extension, <%s>.\n\n", filename);
				return 0;
			}
		}
		pos=pos+1;
				
		/* To take the information for the case that is not supported by checkfile "[INFO]"*/
		char *info = strtok(storeOutputFile, " ");
		info = strtok(NULL, " ");//to separate the extension from the rest of the string that is not necessary
		
		if(checkIfFileEmpty()){
		}else{
			/* compares file extension to the supported extensions from checkFile, and does the same thing for the type*/
			if(strcmp(type,"pdf")==0 || strcmp(type,"gif")==0 || strcmp(type, "jpg")==0 || strcmp(type, "png")==0 || strcmp(type, "mp4")==0 || strcmp(type, "zip")==0 || strcmp(type, "html")==0){
				countFiles = countFiles + 1;
				if(strcmp(&filename[pos],"pdf")==0 || strcmp(&filename[pos],"gif")==0 || strcmp(&filename[pos], "jpg")==0 || strcmp(&filename[pos], "png")==0 || strcmp(&filename[pos], "mp4")==0 || strcmp(&filename[pos], "zip")==0 || strcmp(&filename[pos], "html")==0){
					if(strcmp(type, &filename[pos]) == 0){
						printf("[OK] '%s': extension '%s' matches file type '%s'.\n\n", filename, &filename[pos], type);
						countOk = countOk + 1;
					}else{
						printf("[MISMATCH] '%s': extension is '%s', file type is '%s'.\n\n", filename, &filename[pos], type);
						countMis = countMis + 1;
					}
				}else{
					printf("[MISMATCH] '%s': extension is '%s', file type is '%s'.\n\n", filename, &filename[pos], type);
					countMis = countMis + 1;
				}
			}else{
				printf("[INFO] '%s': type '%s' is not supported by checkfile.\n\n", filename, info);

			}
		}
        wait(NULL);    
        break;
    }
    return 0;
}

/* Function to check if a file exits */
int checkIfFileExists(){
    FILE *file;
    if ((file = fopen(filename, "r"))){
        fclose(file);
        return 1;
	}
    return 0;
}
/*Function to check if file is empty*/
int checkIfFileEmpty(){
    FILE *file;
	file = fopen(filename, "r");
	if (NULL != file) {
		fseek (file, 0, SEEK_END);
		size_t size = ftell(file);

		if (0 == size) {
			fprintf(stderr,"[ERROR] file is empty <%s>.\n\n",filename);
			return 1;
		}
	}
    return 0;
}

/* Function to treat signals*/
void treat_sign(int signal) {
	/* Copy of errno */
    int aux = errno;

	/* captures the signals and then print the information*/
    if(signal == SIGUSR1){
		printf("\n[Signal *SIGUSR1* captured]\nProcessing initialized at %s. \nFile - number: %d / name: %s.\n\n", timeSIGUSR1, countFileNumber, file);
    }
    if(signal == SIGQUIT) {
		printf("\n[Signal *SIGQUIT* captured]\nSent by PID: %d\nUse SIGINT to terminate application.\n\n", getpid());
	}
    /* Recovers the value of errno */
    errno = aux;
}

/* Arg --help */
void help(){
    printf("\n\t*******************************\n\n");
    printf("\t  Micael Marques - Nº 2201743\n");
    printf("\t  Sara Martins   - Nº 2201757\n");
    printf("\n\t*******************************\n");
    printf("Arg -f, --file  <fich>: \tAnalises the file 'fich',indicating if the extension of the file  matches the contect the file actually is. checkFile supports several occurrences of -f / --fich at the same time.\n\n");
    printf("Arg -b, --batch <fich_with_filenames>: \tReads the file 'fich_with_filenames', reads every line and gives the path/name for every line of the file.Also lets the user know if the extension matches the type of file or not. \n");
    printf("Arg -d, --dir <directory>: \tAnalises each of the files in a directory '<directory>', analising every extension. Only reads the actual directory and not subdirectories.\n\n");
    printf("Arg -h, --no-help: \tLists a help list.Supported type of files are: PDF, GIF, JPG, PNG, MP4, ZIP, HTML.\n\n");
}

/* Main function for -f / --file */
void optionFile(){
	
	/*Ignores the signal SIGURS1*/
	act.sa_handler = SIG_IGN;
	sigaction(SIGUSR1,&act,NULL);
	
	if(checkIfFileExists()){
		extensionfile();
	}else{
		fprintf(stderr, "[ERROR] cannot open file <%s> - %s\n\n",filename, strerror(errno)); // mudar se der tempo para cada tipo de erro
	}
}

/* Main function for -b / --batch */
void optionBatch(){
	FILE *fptr;
	char *lineptr = NULL;
	size_t n = 0;
	ssize_t res;
		
	/* Open file given by user*/
	fptr = fopen(fichfilename, "r");
	if (fptr == NULL){
		/* Case fails */
		fprintf(stderr, "[ERROR] cannot open file <%s> - %s\n\n",fichfilename, strerror(errno));
	}else{
		/* Case success */
		printf("[INFO] analyzing files listed in '%s'\n", fichfilename);
		while ((res = getline(&lineptr, &n, fptr)) != -1) {			
			/*To count the files for SIGUSR1*/
			countFileNumber = countFileNumber + 1;
			
			/*stores the name of file on filename, and part the string on "\n"*/
			size_t size = strlen(lineptr) + 1;
			filename = malloc(size);
			filename = strtok(lineptr, "\n"); 
			strcpy(filename, lineptr);
			
			/*To store the filename for SIGUSR1*/
			file = filename;
				
			if(checkIfFileExists()){
				extensionfile();
			}else{
				fprintf(stderr, "[ERROR] cannot open file  <%s> - %s\n\n",filename, strerror(errno));
				countError = countError + 1;
				countFiles = countFiles + 1;
			}
		}
		printf("[SUMMARY] files analyzed: '%d'; files OK: %d; Mismatch: %d; errors: %d\n\n", countFiles, countOk, countMis, countError);
		free(lineptr);
		fclose(fptr);
	}
}

/* Main function for -d / --dir */
void optionDirectory(){
	
	/*Ignores the signal SIGURS1*/
	act.sa_handler = SIG_IGN;
	sigaction(SIGUSR1,&act,NULL);

	/* Opens the directory */
	DIR *d;
	struct dirent *dir;
	FILE *fptr1;
	d = opendir(directory);
		
	if (d){
		/* Sucess case, opens the directory and reads the files on it */
		printf("[INFO] analyzing files of directory '%s'\n\n", directory);
		while ((dir = readdir(d)) != NULL){
			/*Ignores the subdirectories*/
			if (dir->d_type == 4) {
			}else{
				/*If is a file, opens it*/
				fptr1 = fopen(dir->d_name, "r");
				/* Reads file by file letting the user know what are the files that failed to open */
				if (fptr1 == NULL){  
					fprintf(stderr, "[ERROR] cannot open file <%s> - %s\n",dir->d_name, strerror(errno));
				}else{
					/*stores the name of file on filename, and part them string on "\n"*/
					size_t size = strlen(dir->d_name) + 1;
					filename = malloc(size);
					filename = strtok(dir->d_name, "\n"); 
					strcpy(filename, dir->d_name);
						
					if(checkIfFileExists()){
						extensionfile();
					}else{
						fprintf(stderr, "[ERROR] cannot open file  <%s> - %s\n\n", filename, strerror(errno));
						countFiles = countFiles + 1;
						countError = countError + 1;	
					} 			
					fclose(fptr1);
				}
			}
		}
		closedir(d);
	}else{
		/* Fail case, can't open the directory */
		fprintf(stderr, "[ERROR] cannot open dir <%s> - %s\n", directory, strerror(errno));
	}
	/* prints all the counters so the user has a short summary of everything */
	printf("[SUMMARY] files analyzed: '%d'; files OK: %d; Mismatch: %d; errors: %d\n\n", countFiles, countOk, countMis, countError);
}
