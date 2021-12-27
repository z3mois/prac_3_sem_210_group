#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

#include "linked_list.h"
#include "operator.h"
#include "term.h"

#define MAX_SIZE 256
#define print(s) write(2, s "\0", strlen(s) + 1);

int debug = 0;

void operator_show(operator *op, int tab)
{
    for (int i = 0; i < tab; i++)
        print("|   ");
    fprintf(stderr, "%s [%s]\n", operator_name[(int)op->type], op->text != NULL ? op->text : "");
    for (int i = 0; i < op->args.count; i++)
        operator_show((operator *)op->args.value[i], tab + 1);
}

void show_delimiter(char c)
{
    for (int i = 0; i < 80; i++)
        fprintf(stderr, "%c", c);
    print("\n");
}

linked_list_node *show(linked_list *l, linked_list_node *node)
{
    operator *op = node->value;
    operator_show(op, 0);
    return node->next;
}

linked_list_node *handle_command_brackets(linked_list *l, linked_list_node *node)
{
    operator *op = node->value;
    if (op->type == op_term && isbrackets(op->text))
        op->type = op_command;
    return node->next;
}

linked_list_node *handle_io(linked_list *l, linked_list_node *node)
{
    operator *op = node->value, *next_op;
    if (op->type == op_term && (isoutput(op->text) || isoutputadd(op->text) || isinput(op->text))) {
        op->type = op_io;
        linked_list_node *next_node = node->next;
        if (next_node == NULL || (next_op = (operator *)next_node->value)->type != op_term || !iscommand(next_op->text))
            return node;
        operator_add_arg(op, next_op);
        linked_list_remove(l, next_node);
    }
    return node->next;
}

linked_list_node *handle_command_args(linked_list *l, linked_list_node *node)
{
    operator *op = node->value, *next_op;
    if (op->type == op_term && iscommand(op->text)) {
        op->type = op_simple_command;
        linked_list_node *next_node = node->next, *tmp;
        while (next_node != NULL && (next_op = (operator *)next_node->value)->type == op_term && isarg(next_op->text)) {
            tmp = next_node->next;
            operator_add_arg(op, next_op);
            linked_list_remove(l, next_node);
            next_node = tmp;
        }
    }
    return node->next;
}

linked_list_node *handle_conveyor(linked_list *l, linked_list_node *node)
{
    operator *op = node->value, *next_op;
    if (op->type == op_simple_command) {
        operator *args[] = { op };
        operator *conveyor = operator_new(op_conveyor, NULL, args, 1);

        linked_list_node *next_node = node->next, *tmp;
        while (next_node != NULL && (next_op = (operator *)next_node->value)->type == op_term && isconveyor(next_op->text)) {
            next_node = next_node->next;
            if (next_node == NULL || (next_op = (operator *)next_node->value)->type != op_simple_command)
                return node;
            tmp = next_node->next;
            operator_add_arg(conveyor, next_op);
            operator_free((operator *)next_node->prev->value);
            free((operator *)next_node->prev->value);
            linked_list_remove(l, next_node->prev);
            linked_list_remove(l, next_node);
            next_node = tmp;
        }

        node->value = conveyor;
    }
    return node->next;
}

linked_list_node *handle_command(linked_list *l, linked_list_node *node)
{
    operator *op = node->value, *other_op;
    if (op->type == op_conveyor || op->type == op_command && op->text != NULL) {
        operator *args[] = { op };
        operator *command = op->type == op_command ? op : operator_new(op_command, NULL, args, 1);

        linked_list_node *other_node = node->prev, *tmp;
        while (other_node != NULL && (other_op = (operator *)other_node->value)->type == op_io) {
            tmp = other_node->prev;
            operator_add_arg(command, other_op);
            linked_list_remove(l, other_node);
            other_node = tmp;
        }
        other_node = node->next;
        while (other_node != NULL && (other_op = (operator *)other_node->value)->type == op_io) {
            tmp = other_node->next;
            operator_add_arg(command, other_op);
            linked_list_remove(l, other_node);
            other_node = tmp;
        }

        int input_was = 0;
        int output_was = 0;
        for (int i = 1; i < command->args.count; i++) {
            other_op = command->args.value[i];
            if (isinput(other_op->text)) {
                if (input_was)
                    return node;
                input_was = 1;
            }
            else if (isoutput(other_op->text) || isoutputadd(other_op->text)) {
                if (output_was)
                    return node;
                output_was = 1;
            }
        }

        node->value = command;
    }
    return node->next;
}

linked_list_node *handle_and_or(linked_list *l, linked_list_node *node)
{
    operator *op = node->value, *other_op;
    if (op->type == op_term && (isand(op->text) || isor(op->text))) {
        op->type = isand(op->text) ? op_and : op_or;

        linked_list_node *other_node = node->prev;
        if (other_node == NULL || (other_op = (operator *)other_node->value)->type != op_command && other_op->type != op_and && other_op->type != op_or)
            return node;
        operator_add_arg(op, other_op);
        linked_list_remove(l, other_node);

        other_node = node->next;
        if (other_node == NULL || (other_op = (operator *)other_node->value)->type != op_command && other_op->type != op_and && other_op->type != op_or)
            return node;
        operator_add_arg(op, other_op);
        linked_list_remove(l, other_node);
    }
    return node->next;
}

linked_list_node *handle_background(linked_list *l, linked_list_node *node)
{
    operator *op = node->value, *prev_op;
    if (op->type == op_term && (isbackground(op->text))) {
        op->type = op_background;
        linked_list_node *prev_node = node->prev;
        if (prev_node == NULL || (prev_op = (operator *)prev_node->value)->type != op_command && prev_op->type != op_and && prev_op->type != op_or)
            return node;
        operator_add_arg(op, prev_op);
        linked_list_remove(l, prev_node);
    }
    return node->next;
}

linked_list_node *handle_semicolon(linked_list *l, linked_list_node *node)
{
    linked_list_node *next = node->next;
    operator *op = node->value, *prev_op;
    if (op->type == op_term && (issemicolon(op->text))) {
        linked_list_node *prev_node = node->prev;
        if (prev_node == NULL || (prev_op = (operator *)prev_node->value)->type != op_command && prev_op->type != op_and && prev_op->type != op_or)
            return node;
        operator_free(op);
        free(op);
        linked_list_remove(l, node);
    }
    return next;
}

linked_list_node *handle_root(linked_list *l, linked_list_node *node)
{
    linked_list_node *root = l->head, *next = node->next;
    operator *op = node->value, *root_op;
    if (root == NULL || (*(operator *)root->value).type != op_root)
        root = linked_list_add_before(l, l->head, operator_new(op_root, NULL, NULL, 0));
    root_op = root->value;

    if (op->type == op_command || op->type == op_and || op->type == op_or || op->type == op_background) { 
        operator_add_arg(root_op, op);
        linked_list_remove(l, node);
    }
    return next;
}

linked_list_node *free_op(linked_list *l, linked_list_node *node)
{
    operator *op = node->value;
    operator_free(op);
    free(op);
    return node->next;
}

int check_redirect_io(operator *op)
{
    char *input = NULL;
    char *output = NULL;
    int outputadd = 0;
    for (int i = 0; i < op->args.count; i++) {
        operator *arg = op->args.value[i];
        if (arg->type == op_io) {
            if (isinput(arg->text))
                input = ((operator *)arg->args.value[0])->text;
            else if (isoutput(arg->text))
                output = ((operator *)arg->args.value[0])->text;
            else if (isoutputadd(arg->text)) {
                output = ((operator *)arg->args.value[0])->text;
                outputadd = 1;
            }
        }
    }
    //fprintf(stderr, "REDIRECTING INPUT: %s, OUTPUT: %s, ADD: %d\n", input, output, outputadd);
    if (input != NULL) {
        int fd = open(input, O_RDONLY);
        if (fd < 0) {
            print("Error with open\n");
            return -1;
        }
        if (dup2(fd, 0) < 0) {
            print("Error with dup\n");
            return -1;
        }
        close(fd);
    }
    if (output != NULL) {
        int fd = open(output, outputadd ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd < 0) {
            print("Error with open\n");
            return -1;
        }
        if (dup2(fd, 1) < 0) {
            print("Error with dup\n");
            return -1;
        }
        close(fd);
    }
    return 0;
}

void free_all(linked_list *list)
{
    linked_list_iterate(list, free_op);
    linked_list_free(list);
}


void my_exit(linked_list *list, int result)
{
    free_all(list);
    if (debug)
        fprintf(stderr, "PROCESS %d (%d) EXITED\n", getpid(), getppid());
    _exit(result);
}

int execute_op(linked_list *list, operator *op)
{
    switch (op->type) {
        case op_simple_command: {
            char *args[op->args.count + 2];
            args[0] = op->text;
            for (int i = 0; i < op->args.count; i++)
                args[i + 1] = ((operator *)op->args.value[i])->text;
            args[op->args.count + 1] = NULL;
            execvp(op->text, args);
            print("Error with execvp\n");
            my_exit(list, -1);
        } case op_conveyor: {
            // trivial conveyor
            if (op->args.count == 1) {
                operator *simple_cmd = op->args.value[0];
                return execute_op(list, simple_cmd);
            }
            // opening buffers for interprocess communication of the conveyor
            int buffer[op->args.count - 1][2];
            for (int i = 0; i < op->args.count - 1; i++)
                pipe(buffer[i]);
            pid_t child[op->args.count];
            for (int i = 0; i < op->args.count; i++) {
                operator *cmd = op->args.value[i];
                if ((child[i] = fork()) < 0) {
                    print("Error with fork()\n");
                    // close all buffers
                    for (int j = 0; j < op->args.count - 1; j++) {
                        close(buffer[j][0]);
                        close(buffer[j][1]);
                    }
                    // kill all child processes
                    for (int j = 0; j < i; j++) {
                        print("KILL 1\n");
                        kill(child[j], SIGKILL);
                        wait(NULL);
                    }
                    return -1;
                } else if (child[i] == 0) { // child
                    // input redirect
                    if (i > 0 && dup2(buffer[i - 1][0], 0) < 0)
                        my_exit(list, -1);
                    // output redirect
                    if (i < op->args.count - 1 && dup2(buffer[i][1], 1) < 0)
                        my_exit(list, -1);
                    // close all buffers
                    for (int j = 0; j < op->args.count - 1; j++) {
                        close(buffer[j][0]);
                        close(buffer[j][1]);
                    }
                    my_exit(list, execute_op(list, cmd));
                }
            }
            // close all buffers
            for (int i = 0; i < op->args.count - 1; i++) {
                close(buffer[i][0]);
                close(buffer[i][1]);
            }

            // wait for last child process
            int status;
            waitpid(child[op->args.count - 1], &status, 0);
            if (!WIFEXITED(status)) {
                // kill all child processes
                for (int i = 0; i < op->args.count - 1; i++) {
                    print("KILL 2\n");
                    kill(child[i], SIGKILL);
                    wait(NULL);
                }
                return -1;
            }
            int result = WEXITSTATUS(status);
            // wait for all child processes except the last one
            for (int i = 0; i < op->args.count - 1; i++) {
                wait(&status);
                if (!WIFEXITED(status)) {
                    // kill all the rest of child processes
                    for (int j = i + 1; j < op->args.count - 1; j++) {
                        print("KILL 3\n");
                        kill(child[j], SIGKILL);
                        wait(NULL);
                    }
                    return -1;
                }
            }
            return result;
        } case op_command: {
            // brackets command in a separate shell instance
            if (op->text != NULL) {
                pid_t child;
                if ((child = fork()) < 0)
                    return -1;
                else if (child == 0) { // child
                    if (check_redirect_io(op) != 0)
                        my_exit(list, -1);
                    execl("./myshell", "myshell", op->text, NULL);
                    print("Error with execvp\n");
                    my_exit(list, -1);
                }
                int status;
                wait(&status);
                if (!WIFEXITED(status))
                    return -1;
                return WEXITSTATUS(status);
            }

            // redirecting input and output
            pid_t child;
            if ((child = fork()) < 0)
                return -1;
            else if (child == 0) { // child
                operator *conveyor = op->args.value[0];
                if (check_redirect_io(op))
                    my_exit(list, -1);
                my_exit(list, execute_op(list, conveyor));
            }

            int status;
            wait(&status);
            if (!WIFEXITED(status))
                return -1;
            return WEXITSTATUS(status);
        } case op_and: {
            operator *first = op->args.value[0];
            operator *second = op->args.value[1];
            if (execute_op(list, first) == 0)
                return execute_op(list, second);
            return -1;
        } case op_or: {
            operator *first = op->args.value[0];
            operator *second = op->args.value[1];
            if (execute_op(list, first) != 0)
                return execute_op(list, second);
            return 0;
        } case op_background: {
            pid_t child;
            if ((child = fork()) < 0)
                return -1;
            else if (child == 0) { // child
                if ((child = fork()) < 0)
                    my_exit(list, -1);
                else if (child == 0) { // child
                    int fd = open("/dev/null", O_RDONLY);
                    if (fd < 0 || dup2(fd, 0) < 0)
                        my_exit(list, -1);
                    close(fd);
                    signal(SIGINT, SIG_IGN);
    
                    operator *child = op->args.value[0];
                    my_exit(list, execute_op(list, child));
                }
                my_exit(list, 0);
            }
            int status;
            wait(&status);
            if (!WIFEXITED(status))
                return -1;
            return WEXITSTATUS(status);
        } case op_root: {
            int result = 0;
            for (int i = 0; i < op->args.count; i++) {
                operator *command = op->args.value[i];
                result = execute_op(list, command);
            }
            return result;
        }
    }
    print("Invalid operration\n");
    return -1;
}

int shell(char *cmd)
{
    int result;
    int len = strlen(cmd);
    int index = 0;
    char *term;
    linked_list ops;
    int count = 0, countsv;
    int size;
    int term_len;

    // reading terms into linked list
    linked_list_init(&ops);
    while ((term_len = read_term(cmd, len, &index, &term)) > 0) {
        operator *op = operator_new(op_term, term, NULL, 0);
        linked_list_add(&ops, op);
    }

    // building an expression tree
    if (term_len < 0
            || linked_list_iterate(&ops, handle_command_brackets) != 0
            || linked_list_iterate(&ops, handle_io) != 0
            || linked_list_iterate(&ops, handle_command_args) != 0
            || linked_list_iterate(&ops, handle_conveyor) != 0
            || linked_list_iterate(&ops, handle_command) != 0
            || linked_list_iterate(&ops, handle_and_or) != 0
            || linked_list_iterate(&ops, handle_background) != 0
            || linked_list_iterate(&ops, handle_semicolon) != 0
            || linked_list_iterate(&ops, handle_root) != 0
            || ops.count > 1) {
        if (debug)
            linked_list_iterate(&ops, show);
        print("Invalid command\n");
        if (debug)
            show_delimiter('=');

        free_all(&ops);
        return -1;
    }
    //выполненеие дерева выражений
    if (ops.count > 0) {
        operator *root = ops.head->value;
        result = execute_op(&ops, root);
    }
    // освобождаем память
    free_all(&ops);

    return result;
}

int main(int argc, char *argv[])
{
    if (argc == 2) {
        char *brackets = argv[1];
        int size = strlen(brackets) + 1;
        char cmd[size - 2];
        for (int i = 1; i < size - 1; i++)
            cmd[i - 1] = brackets[i];
        cmd[size - 3] = '\0';
        fprintf(stderr, "(%s)\n", cmd);
        return shell(cmd);
    }
    char cmd[MAX_SIZE];
    int result = 0;
    fprintf(stderr, "Shell pid = %d. Shell started.\n", getpid());
    print("Your command: ");
    while (strcmp(fgets(cmd, MAX_SIZE, stdin), "end\n") != 0) {
        result = shell(cmd);
        print("Your command: ");
    }
    return result;
}
