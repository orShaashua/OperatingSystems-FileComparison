/*
 * Name:Or Sha'ashua
 */


#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>

#define IDENTICAL 3
#define SIMILAR  2
#define DIFFERENT 1
#define LINE_BREAK '\n'
#define SPACE ' '
#define FAILURE (-1)
#define ERROR "Error in system call"
#define FILE_DESCRIPTORS_STDERR 2

typedef enum filesRelationship{identicalFiles, sameTextFiles, differentFiles }filesRelationship;
filesRelationship checkFilesRelationship(int file1, int file2);
void closeFiles(int file1, int file2);



int main(int argc, char *argv[]){

    if (argc!= 3) {
        write(FILE_DESCRIPTORS_STDERR,ERROR,strlen(ERROR));
        exit(FAILURE);
    }

    int f1 = open(argv[1], O_RDONLY);//open file in the first path
    int f2 = open(argv[2], O_RDONLY);//open file in the second path
    //error in open file
    if (f1 < 0 || f2 < 0){
        write(FILE_DESCRIPTORS_STDERR,ERROR,strlen(ERROR));
        exit(FAILURE);
    }

    filesRelationship type = checkFilesRelationship(f1,f2);
    closeFiles(f1,f2); //close the files
    if (type == identicalFiles){
        return IDENTICAL;
    }else if (type == sameTextFiles){
        return SIMILAR;
    }else{
        return DIFFERENT;
    }
}


/**
 * check if file1 and file2 are identical or have the same text or different.
 */
filesRelationship checkFilesRelationship(int file1, int file2){
    typedef enum stepForward {stepForwardFile1,stepForwardFile2,stepForwardBothFiles }stepForward;
    char c1[1], c2[1];
    int returnValOfFile1,returnValOfFile2;
    filesRelationship type = identicalFiles;
    stepForward step = stepForwardBothFiles;
    while (true) {

        if (step == stepForwardBothFiles ||step == stepForwardFile1) {
            returnValOfFile1 = read(file1, c1, 1);
        }
        if(step == stepForwardBothFiles ||step == stepForwardFile2) {
            returnValOfFile2 = read(file2, c2, 1);
        }

        if (returnValOfFile1 == 0 && returnValOfFile2 == 0){         //end of two files
            return  type;
        }else if (returnValOfFile1 < 0 || returnValOfFile2 < 0){     //error in read one of the files
            write(FILE_DESCRIPTORS_STDERR,ERROR,strlen(ERROR));
            exit(FAILURE);
        }else if (returnValOfFile1 != 0 && returnValOfFile2 != 0) {  //if both files have char
            if ((c1[0] == LINE_BREAK && c2[0] == LINE_BREAK)||(c1[0] == SPACE && c2[0] == SPACE)){
                step = stepForwardBothFiles; //if for both file the char is space or line break
                continue;                    // continue to the next char in both
            }else if (c1[0] == LINE_BREAK ||c1[0] == SPACE){
                step = stepForwardFile1; //if only in the first file the char is space or break line
                type = sameTextFiles;    //step forward in file1 only and for now type similar.
                continue;
            }else if (c2[0] == LINE_BREAK ||c2[0] == SPACE) {
                step = stepForwardFile2;//if only in the second file the char is space or break line
                type = sameTextFiles;   //step forward in file2 only and for now type similar.
                continue;
            } else if (c1[0] != c2[0]) {//if the chars are not space or break line check if the chars not equals
                if (tolower(c1[0]) == tolower(c2[0])) { //check if  the chars similar in lower case
                    type = sameTextFiles; //if so for now type similar and continue the next char
                    continue;
                }
                return differentFiles; //character is not equal return type different
            }
            step = stepForwardBothFiles;
        }else { //one file end before another

            //if the first file don't end and the next character is '\n' or space.
            if (returnValOfFile1 && (c1[0] == LINE_BREAK || c1[0] == SPACE)) {
                step = stepForwardFile1;
                type = sameTextFiles;
                continue;
                //if the second file don't end and the next character is '\n' or space.
            } else if (returnValOfFile2 && (c2[0] == LINE_BREAK || c2[0] == SPACE)) {
                step = stepForwardFile2;
                type = sameTextFiles;
                continue;
            } else {
                return differentFiles;
            }
        }
    }
}

/*
 * close the files
 * */
void closeFiles(int file1, int file2){
    close(file1);
    close(file2);
}
