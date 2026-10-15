#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void
usage(void)
{
    fprintf(2, "usage: xargs [-n N] cmd [args...]\n");
    exit(1);
}

int
run(char **args)
{
    int status, pid = fork();
    if (pid == 0) {
        exec(args[0], args);
        fprintf(2, "xargs: exec %s failed\n", args[0]);
        exit(1);
    } else if (pid > 0) {
        wait(&status);
        return status;
    } else {
        fprintf(2, "xargs: fork failed\n");
        exit(1);
    }
    return 1;
}

int
main(int argc, char *argv[])
{
    if (argc < 2) usage();

    char *args[MAXARG];
    int N = 0, cmd = 1;

    if (strcmp(argv[1], "-n") == 0) {
        cmd = 3;
        if (argc <= cmd) usage();
        if (argv[2][0] == '\0') usage();
        for (int i = 0; argv[2][i] != '\0'; ++i)
            if (argv[2][i] < '0' || argv[2][i] > '9') usage();
        N = atoi(argv[2]);
        if (N <= 0) usage();
    }

    int base = argc - cmd;
    if (base + 2 > MAXARG) usage();

    for (int i = 0; i < base; ++i)
        args[i] = argv[i + cmd];
    args[base] = 0;

    char buf[512], c;
    int new = 1, pos = 0, nwords = 0, success = 0;

    while (read(0, &c, 1) == 1) {
        if (c == ' ' || c == '\t' || c == '\n') {
            if (!new) {
                buf[pos++] = '\0';          // terminate BEFORE running
                ++nwords;
                new = 1;
                if (base + nwords >= MAXARG) {
                    fprintf(2, "xargs: too many arguments\n");
                    exit(1);
                }
                if (N > 0 && nwords == N) {
                    args[base + nwords] = 0;
                    if (run(args) != 0) success = 1;
                    nwords = 0; pos = 0;
                }
            }
            if (c == '\n' && N == 0 && nwords > 0) {
                args[base + nwords] = 0;
                if (run(args) != 0) success = 1;
                nwords = 0; pos = 0;
            }
        } else {
            if (pos >= (int)sizeof(buf) - 1) {   // leave room for '\0'
                fprintf(2, "xargs: input too long\n");
                exit(1);
            }
            if (new) {
                args[base + nwords] = &buf[pos];
                new = 0;
            }
            buf[pos++] = c;
        }
    }

    if (!new) {
        buf[pos++] = '\0';
        ++nwords;
        if (base + nwords >= MAXARG) {
            fprintf(2, "xargs: too many arguments\n");
            exit(1);
        }
    }
    if (nwords > 0) {
        args[base + nwords] = 0;
        if (run(args) != 0) success = 1;
    }
    exit(success);
}