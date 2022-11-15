#include <sys/types.h>  
#include <sys/stat.h>   
#include <fcntl.h>      
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define chunk 1048576 //We have defined a chunk of size 1MB to do the reversal chunk by chunk

void prints(char progress_string[])     //this is the same as printf() which uses STDOUT system call
{
    write(STDOUT_FILENO,progress_string,strlen(progress_string));
}
int main(int argc, char *argv[])
{
    char progress_string[15];
    float progress = 0.00;
    if (argc != 2)              //Here we're checking the argument if wheather there is a file passed to our program
    {
        perror("Invalid Argument");
        exit(1);
    }
    int rfile_d = open(argv[1], O_RDONLY);      //file descriptor for our open file
    if (rfile_d == -1)
    {
        perror("Unable to open file");
        exit(1);
    }
    off_t filesize = lseek(rfile_d, 0, SEEK_END);       
    char filename[20], loc[200];
    int i = 0, j = strlen(argv[1]) - 1, flag = 0;
    strcpy(loc, argv[1]);
   //Here we're typing the extract the input file name to create the output file name
    while (j < strlen(loc) && (j >= 0))
    {
        if (flag == 0)
        {
            if (loc[j] == '/')
            {
                flag = 1;
                j++;
            }
            else
                j--;
        }
        if ((flag == 1))
        {
            filename[i] = loc[j];
            j++;
            i++;
        }
        if (j == 0)
            strcpy(filename, loc);
    }
    char wdir[100] = "./Assignment/1_";
    strcat(wdir, filename);
    mkdir("./Assignment", 0700);
    int wfile_d = open(wdir, O_RDWR | O_CREAT | O_TRUNC, 0600);        //Here we're opening output file into another file descriptor
    if (wfile_d == -1)
    {
        perror("Not able to create file");
        exit(1);
    }
    lseek(wfile_d, 0, SEEK_SET);
    char *readchunk = (char *)malloc(chunk * sizeof(char));
    char *writechunk = (char *)malloc(chunk * sizeof(char));
    off_t prev_pos;
    //file reversal and writing into output file with progress.
    while (lseek(rfile_d, 0, SEEK_CUR) > 0)
    {

        if (chunk > lseek(rfile_d, 0, SEEK_CUR))        //if the data to be read has size less than our chunk size
        {
            prev_pos = lseek(rfile_d, 0, SEEK_CUR);
            lseek(rfile_d, 0, SEEK_SET);
            read(rfile_d, readchunk, prev_pos);
            for (int i = 0; i < prev_pos; i++)
            {
                writechunk[i] = readchunk[prev_pos - i - 1];
            }
            write(wfile_d, writechunk, prev_pos);
            progress = 100.00;
            sprintf(progress_string,"\r%f",progress);
            prints(progress_string);
            break;
        }

        prev_pos = lseek(rfile_d, -chunk, SEEK_CUR);
        read(rfile_d, readchunk, chunk);
        for (int i = 0; i < chunk; i++)
        {
            writechunk[i] = readchunk[chunk - i - 1];
        }
        write(wfile_d, writechunk, chunk);
        progress = (float)(filesize - prev_pos) * 100 / filesize;
        lseek(rfile_d, prev_pos, SEEK_SET);
        sprintf(progress_string,"\r%f",progress);
        prints(progress_string);

    }
    close(rfile_d);
    close(wfile_d);
     fflush(readchunk);
    fflush(writechunk);
}
