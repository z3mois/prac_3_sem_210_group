#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/shm.h>
#define SIZE 256

struct{
    long mtype;
    int Data;
} Message;

int main(int argc, char** argv){
    if (argc != 2){
        return 0;
    }
    key_t key1;
    int msgid;
    char *shmaddr;
    int shmid; 
    FILE *file;
    file = fopen(argv[1], "r");
    if (!file){
        return 0;
    }
    key1 = ftok("main", 'a');
    if (key1 == -1){
        return 0;
    }
    msgid = msgget(key1, 0666 | IPC_CREAT); // create queu
    shmid = shmget(key1, SIZE * sizeof(char), 0666 | IPC_CREAT);
    shmaddr = shmat(shmid, NULL, 0);
    while(!feof(file)){
        if(fgets(shmaddr, SIZE, file)){
            Message.mtype = 2;
            Message.Data = 0;
            msgsnd(msgid, (struct msgbuf*)(&Message), sizeof(int), 0);
            Message.mtype = 4;
            Message.Data = 0;
            msgsnd(msgid, (struct msgbuf*)(&Message), sizeof(int), 0);
            for (;;){
                if (msgrcv(msgid, (struct msgbuf*)(&Message), sizeof(int), 1, 0)){
                    printf("spaces in string:%d ", Message.Data);
                    break;
                }
            }
            for (;;){
                if (msgrcv(msgid, (struct msgbuf*)(&Message), sizeof(int), 3, 0)){
                    printf("numbers in string:%d\n", Message.Data);
                    break;
                }
            }
        }
    }
    printf("END\n");
    Message.mtype = 2;
    Message.Data = 1;
    msgsnd(msgid, (struct msgbuf*)(&Message), sizeof(int), 0);
    Message.mtype = 4;
    Message.Data = 1;
    msgsnd(msgid, (struct msgbuf*)(&Message), sizeof(int), 0);
    msgctl(msgid, IPC_RMID, NULL);
    shmdt(shmaddr);
    shmctl(shmid, IPC_RMID, NULL);
    fclose(file);
    return 0;
}