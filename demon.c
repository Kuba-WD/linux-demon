#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>

#include <stdbool.h>

#define DEFAULT_SLEEP_TIME 20
   
static void skeleton_daemon()
{
    pid_t pid;
    
    /* Fork off the parent process */
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
     /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* On success: The child process becomes session leader */
    if (setsid() < 0)
        exit(EXIT_FAILURE);
    
    /* Catch, ignore and handle signals */
    /*TODO: Implement a working signal handler */
    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);
    
    /* Fork off for the second time*/
    pid = fork();
    
    /* An error occurred */
    if (pid < 0)
        exit(EXIT_FAILURE);
    
    /* Success: Let the parent terminate */
    if (pid > 0)
        exit(EXIT_SUCCESS);
    
    /* Set new file permissions */
    umask(0);
    
    /* Change the working directory to the root directory */
    /* or another appropriated directory */
    chdir("/");
    
    /* Close all open file descriptors */
    int x;
    for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close (x);
    }
    
    /* Open the log file */
    openlog ("firstdaemon", LOG_PID, LOG_DAEMON);
}

void sigusr1_handler()
{
    syslog(LOG_NOTICE, "odebrano sygna³ SIGUSR1");
    // do implementacji
}

int main(int argc, char* argv[])
{
    skeleton_daemon();
    int sleep_time = DEFAULT_SLEEP_TIME;

    bool arg_v = 0; // Dodatkowa opcja -v

    for (int i = 0; i < argc; i++) // Sprawdzamy argumenty wywo³ania demona
    {
        // strcmp zwraca 0, jeœli porównywane stringi s¹ takie same
        
        if (strcmp(argv[i], "-t") == 0)
        {
            sleep_time = atoi(argv[i + 1]); // Po -t musi byæ podana iloœæ sekund, jakiœ wyj¹tek by siê przyda³
            syslog(LOG_NOTICE, "czas spania ustawiony na %d", sleep_time);
            continue;
        }
        
        if (strcmp(argv[i], "-v") == 0)
        {
            arg_v = 1;
            syslog(LOG_NOTICE, "opcja -v");
            continue;
        }
    }

    while (1)
    {
        signal(SIGUSR1, sigusr1_handler);


        //TODO: Insert daemon code here.
        syslog (LOG_NOTICE, "First daemon started.");
        sleep (sleep_time);
        break;
    }
   
    syslog (LOG_NOTICE, "First daemon terminated.");
    closelog();
    
    return EXIT_SUCCESS;
}
