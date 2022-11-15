#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define chunk 1048576

void prints(char progress_string[])
{
    write(STDOUT_FILENO, progress_string, strlen(progress_string));
}

off_t string2num(char arg[])
{
    off_t num = 0;
    for (int i = 0; i < strlen(arg); i++)
    {
        if ((arg[i] > 47) && arg[i] < 58)
            num = 10 * num + (arg[i] - '0');
        else
        {
            perror("Invalid argument");
            exit(1);
        }
    }
    return num;
}

int main(int argc, char *argv[])
{
    off_t str, stp;
    char progress_string[15];
    float progress = 0.00;
    if (argc != 4)
    {
        perror("Invalid Argument");
        exit(1);
    }
    str = string2num(argv[2]);
    stp = string2num(argv[3]);
    int rfile_d = open(argv[1], O_RDONLY);
    if (rfile_d == -1)
    {
        perror("Unable to open file");
        exit(1);
    }
    off_t filesize = lseek(rfile_d, 0, SEEK_END), bytes_read = 0;
    char filename[20], loc[200];
    int i = 0, j = strlen(argv[1]) - 1, flag = 0;
    strcpy(loc, argv[1]);
    ;
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
    char wdir[100] = "./Assignment/2_";
    strcat(wdir, filename);
    mkdir("./Assignment", 0700);
    int wfile_d = open(wdir, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (wfile_d == -1)
    {
        perror("Not able to create file");
        exit(1);
    }
    lseek(wfile_d, 0, SEEK_SET);
    char *readchunk = (char *)malloc(chunk * sizeof(char));
    char *writechunk = (char *)malloc(chunk * sizeof(char));
    off_t prev_pos;
    lseek(rfile_d, str - 1, SEEK_SET);

    while (lseek(rfile_d, 0, SEEK_CUR) > 0)
    {

        if (chunk > lseek(rfile_d, 0, SEEK_CUR))
        {
            prev_pos = lseek(rfile_d, 0, SEEK_CUR);
            bytes_read += prev_pos;
            lseek(rfile_d, 0, SEEK_SET);
            read(rfile_d, readchunk, prev_pos);
            for (int i = 0; i < prev_pos; i++)
            {
                writechunk[i] = readchunk[prev_pos - i - 1];
            }
            write(wfile_d, writechunk, prev_pos);
            progress = (float)(bytes_read * 100) / filesize;
            sprintf(progress_string, "\r%f", progress);
            prints(progress_string);
            break;
        }

        prev_pos = lseek(rfile_d, -chunk, SEEK_CUR);
        read(rfile_d, readchunk, chunk);
        for (int i = 0; i < chunk; i++)
        {
            writechunk[i] = readchunk[chunk - i - 1];
        }
        bytes_read += chunk;
        write(wfile_d, writechunk, chunk);
        progress = (float)bytes_read * 100 / filesize;
        lseek(rfile_d, prev_pos, SEEK_SET);
        sprintf(progress_string, "\r%f", progress);
        prints(progress_string);
    }
    lseek(rfile_d, str-1, SEEK_SET);
    while (lseek(rfile_d, 0, SEEK_CUR) <= stp)
    {
        prev_pos = lseek(rfile_d, 0, SEEK_CUR);
        if ((stp-lseek(rfile_d,0,SEEK_CUR)<chunk))
        {
            read(rfile_d, readchunk, stp - prev_pos);
            bytes_read += stp - prev_pos;
            for (int i = 0; i < stp - prev_pos; i++)
            {
                writechunk[i] = readchunk[i];
            }
            write(wfile_d, writechunk, stp - prev_pos);
            progress = (float)bytes_read * 100 / filesize;
            sprintf(progress_string, "\r%f", progress);
            prints(progress_string);
            break;
        }
        read(rfile_d, readchunk, chunk);
        bytes_read += chunk;
        for (int i = 0; i < chunk; i++)
        {
            writechunk[i] = readchunk[i];
        }
        write(wfile_d, writechunk, chunk);
        progress = (float)bytes_read * 100 / filesize;
        sprintf(progress_string, "\r%f", progress);
        prints(progress_string);
    }
    lseek(rfile_d, 0, SEEK_END);
    while (lseek(rfile_d, 0, SEEK_CUR) > stp)
    {
        if (chunk > ((lseek(rfile_d, 0, SEEK_CUR) - stp)))
        {
             prev_pos = lseek(rfile_d, 0, SEEK_CUR);
            // printf("%ld",stp);
            lseek(rfile_d, stp-prev_pos, SEEK_CUR);
            read(rfile_d, readchunk, prev_pos-stp);
            for (int i = 0; i < prev_pos-stp; i++)
            {
                writechunk[i] = readchunk[prev_pos -stp - i - 1];
            }
            write(wfile_d, writechunk, prev_pos-stp);
            bytes_read+=prev_pos-stp;            
            progress =(float)(bytes_read) * 100 / filesize;
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
        bytes_read+=chunk;
        progress = (float)(bytes_read) * 100 / filesize;
        lseek(rfile_d, prev_pos, SEEK_SET);
        sprintf(progress_string, "\r%f", progress);
        prints(progress_string);
    }
    close(rfile_d);
    close(wfile_d);
    fflush(readchunk);
    fflush(writechunk);
}