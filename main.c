#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>


//1. compile and run C program

void compile_and_run(const char *src)
{
    char out[256];
    snprintf(out, sizeof(out),"a.out");

    int id =fork();
    if(id == 0)
    {
        //child, compile with gcc
        execlp("gcc","gcc","-o",out,src,NULL);
        perror("execlp");
        exit(1);

    }

    wait(NULL); //wait for compilation

    id = fork();
    if(id == 0)
    {
        //child, run the program
        execl(out,out,NULL);
        perror("execl");
        exit(1);
    }
    wait(NULL);
}


//2 mini shell 

void mini_shell()
{
    char line[256];
    while(1)
    {
        printf("mini-shell>");
        fgets(line, sizeof(line), stdin);
        line[strcspn(line,"\n")]=0;
        fflush(stdout);
        if(strcmp(line,"exit")==0) break;

        int id =fork();
        if(id==0)
        {
            char *argv[10];
            int i=0;
            char *t= strtok(line," ");

            while(t && i<9)
            {
                argv[i++]=t;
                t=strtok(NULL," ");
            }

            argv[i]=NULL;
            execvp(argv[0],argv);
            perror ("execvp");
            exit(1);

        }
        wait(NULL);
    }
}


//3 communicate through file

void file_comm(const char *file, const char *message)
{
    //parent will write message
    int fd = open(file,O_CREAT|O_TRUNC|O_WRONLY,0644);
    if(fd<0)
    {
        perror("open");
        return;
    }
    write(fd,message,strlen(message));
    write(fd,"\n",1);
    close(fd);


    int id = fork();
    if(id == 0)
    {
        //child reads and prints from file

        execlp("cat","cat",file,NULL);
        perror("execlp");
        exit(1);
    }

    wait(NULL);

}

int main(int argc, char**argv)
{
    if (argc < 2) {
        printf("Usage:\n  %s compile-run <program.c>\n  %s mini-shell\n  %s comm <file> <message>\n",
               argv[0], argv[0], argv[0]);
        return 1;
    }
    if(strcmp(argv[1],"compile-run") == 0 && argc==3)
       compile_and_run(argv[2]);
    else if(strcmp(argv[1],"mini-shell")==0)
      mini_shell();
    else if(strcmp(argv[1],"comm")==0 && argc == 4)
       file_comm(argv[2],argv[3]);\
    else 
    {
             printf("Invalid usage! :( \n");
             printf("Usage:\n  %s compile-run <program.c>\n  %s mini-shell\n  %s comm <file> <message>\n",
               argv[0], argv[0], argv[0]);

    }
    
  return 0;
}