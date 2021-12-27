#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

const char *const file = "/test.txt";
//char *const file = "./tictactoe";
int size = 3;

struct {
    long mtype;
    char data[1];
} message;

void my_exit(int shmid, int msgid)
{
    perror("ERROR");
    if (shmid >= 0 && shmctl(shmid, IPC_RMID, NULL) >= 0)
        printf("<%d> CLEARED SHARED MEMORY ON ERROR!\n", getpid());
    if (msgid >= 0 && msgctl(msgid, IPC_RMID, NULL) >= 0)
        printf("<%d> CLEARED MESSAGE QUEUE ON ERROR!\n", getpid());
    _exit(-1);
}

void show_field(char *shared)
{
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++)
            printf(x < size - 1 ? "%c|" : "%c", shared[y * size + x]);
        printf("\n");
        if (y < size - 1) {
            for (int x = 0; x < size * 2 - 1; x++)
                printf("-");
            printf("\n");
        }
    }
}

int checkwin(char *f, char me)
{
    return f[0] == me && f[1] == me && f[2] == me
        || f[3] == me && f[4] == me && f[5] == me
        || f[6] == me && f[7] == me && f[8] == me
        || f[0] == me && f[3] == me && f[6] == me
        || f[1] == me && f[4] == me && f[7] == me
        || f[2] == me && f[5] == me && f[8] == me
        || f[0] == me && f[4] == me && f[8] == me
        || f[2] == me && f[4] == me && f[5] == me;
}

int checkdraw(char *f)
{
    for (int i = 0; i < size * size; i++)
        if (f[i] == ' ')
            return 0;
    return 1;
}

void tictactoe(int first)
{
    key_t key = ftok(file, 0);
    int shmid = -1;
    int msgid = -1;

    // shared create memory
    shmid = shmget(key, size * size, IPC_CREAT | 0644);
    // or open
    if (shmid < 0)
        my_exit(shmid, msgid);
    char *shared = shmat(shmid, NULL, 0);
    if (shared == (char *)-1)
        my_exit(shmid, msgid);
    if (first) {
        for (int i = 0; i < size * size; i++)
            shared[i] = ' ';
        show_field(shared);
    }

    // message queue
    msgid = msgget(key, IPC_CREAT | 0644); 
    if (msgid < 0)
        my_exit(shmid, msgid);

    // game
    int state = first;
    char me = first ? 'X' : 'O';
    char enemy = first ? 'O' : 'X';
    for (;;) {
        if (state == 1) { // reading player command
            int x, y;
            int count = 0;
            for (;;) {
                if (count > 10) {
                    printf("INFINITE SCANF\n");
                    _exit(-1);
                }
                if (scanf("%d", &x) <= 0 || scanf("%d", &y) <= 0 || x <= 0 || x > size || y <= 0 || y > size) {
                    count++;
                    getchar();
                    //while (getchar() != '\n')
                        //continue;
                    printf("Invalid input. Please type <x> and <y> coordinates.\n");
                    continue;
                }
                //while (getchar() != '\n')
                    //continue;
                x--;
                y--;
                if (shared[y * size + x] != ' ') {
                    printf("Invalid move. This position is already used.\n");
                    continue;
                }
                shared[y * size + x] = me;
                show_field(shared);
                message.mtype = 1;
                message.data[0] = '\0';
                msgsnd(msgid, (struct msgbuf *)(&message), 1, 0);
                state = 1 - state;
                if (checkwin(shared, me)) {
                    getchar();
                    if (first)
                        printf("FIRST PLAYER WON!\n");
                    else
                        printf("SECOND PLAYER WON!\n");
                    state = 2;
                } else if (checkdraw(shared)) {
                    printf("DRAW!\n");
                    state = 2;
                }
                break;
            }
        } else if (state == 0) { // waiting for other player
            msgrcv(msgid, (struct msgbuf *)(&message), 1, 1, 0);
            state = 1 - state;
            if (checkwin(shared, enemy) || checkdraw(shared))
                state = 2;
        } else
            break;
    }

    // free resources
    if (shmdt(shared) < 0)
        my_exit(shmid, msgid);
    if (shmctl(shmid, IPC_RMID, NULL) >= 0)
        printf("<%d> SHARED MEMORY DELETED!\n", getpid());
    if (msgctl(msgid, IPC_RMID, NULL) >= 0)
        printf("<%d> MESSAGE QUEUE DELETED!\n", getpid());
}

void clear()
{
    key_t key = ftok(file, 0);
    int shmid = shmget(key, size * size, 0);
    if (shmid >= 0 && shmctl(shmid, IPC_RMID, NULL) >= 0)
        printf("INITIAL SHARED MEMORY CLEAR!\n");
    int msgid = msgget(key, 0);
    if (msgid >= 0 && msgctl(msgid, IPC_RMID, NULL) >= 0)
        printf("INITIAL MESSAGE QUEUE CLEAR!\n");
}

int main()
{
    clear();
    pid_t pid = fork();
    if (pid < 0)
        my_exit(-1, -1);
    printf("Tic-tac-toe process %d\n", getpid());
    tictactoe(pid == 0);
    return 0;
}
