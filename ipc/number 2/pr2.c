#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <string.h>
#define SIZE 256

struct{
    long mtype;
    char Data;
} Message;

int main(int argc, char** argv){
    key_t key1;
    int msgid;
    char *shmaddr = NULL;
    int shmid; 
    key1 = ftok("main", 'a');
    if (key1 == -1){
        return 0;
    }
    msgid = msgget(key1, 0666); // create queu 2
    shmid = shmget(key1, SIZE, 0666);
    shmaddr = shmat(shmid, NULL, 0);
    int i = 0;
    int cnt = 0;
    for (;;){
        cnt = 0;
        i = 0;
        for (;;){
            if (msgrcv(msgid, (struct msgbuf*)(&Message), sizeof(int), 4, 0))
                break;
        }
        if (Message.Data == 1){
            break;
        }
        while (shmaddr[i] != '\0'){
            if ((shmaddr[i] >= '0') && (shmaddr[i] <= '9'))
                cnt ++;
            i++;
        }
        Message.mtype = 3;
        Message.Data = cnt;
        msgsnd(msgid, (struct msgbuf*)(&Message), sizeof(int), 0);
    }
    shmdt(shmaddr);
    return 0;
}