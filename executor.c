#include "executor.h"

/**
 * Execute a command using the fork-exec-wait pattern
 *
 * This function demonstrates the fundamental process management pattern
 * used by all Unix shells:
 * 1. Fork a child process
 * 2. Child calls exec to transform into the target command
 * 3. Parent waits for child to complete
 *
 * @param command The command to execute (e.g., "ls", "pwd", "echo")
 * @param args Array of arguments: [command, arg1, arg2, ..., NULL]
 *             Example: {"ls", "-l", NULL} or {"echo", "Hello", NULL}
 * @return Exit status of the command (0=success, non-zero=failure, -1=error)
 */
int execute_command(char *command, char **args) 
{
    pid_t pid;
    int status;

    //Fork 
    pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return -1;
    }

    //Child Process
    if (pid == 0)
    {
        if(execvp(command,args) == -1)
        {
            perror("execvp failed");
            exit(1);
        }
    }
    //Parent Process
    else
    {
        if(waitpid(pid, &status, 0) == -1)
        {
            perror("waitpid failed");
            return -1;
        }

        if (WIFEXITED(status)) 
        {
            return WEXITSTATUS(status);
        }
        else
        {
            return -1;
        }
    }

    return -1;  
}