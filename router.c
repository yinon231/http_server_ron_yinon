#include "httpd.h"
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define BUF_SIZE 1024

#define MAX_LINE_LENGTH 256

struct user
{
    char name[8];
    char password[8];
    int auth;
};


void show_all_password()
{
    printf("HTTP/1.1 200 OK\r\n\r\n");
    printf("Content-Type: text/plain\r\n");
    printf("\r\n");
    
    FILE *file = fopen("password.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening password file: %s\n", strerror(errno));
    }

    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), file))
    {
        // Remove newline if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        fprintf(stderr,"'%s',", line);
    }
  
}
// Function to add a new user to the password file
void add_user(const char *name, const char *password)
{
    FILE *file = fopen("password.txt", "a");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening password file: %s\n", strerror(errno));
        return;
    }

    fprintf(file, "name=%s&psw=%s\n", name, password);
    fclose(file);
}

// Function to verify user credentials
int verify_user(const char *name, const char *password)
{
    FILE *file = fopen("password.txt", "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error opening password file: %s\n", strerror(errno));
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int auth = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Remove newline if present
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
        }

        char user_copy[MAX_LINE_LENGTH];
        strncpy(user_copy, line, sizeof(user_copy) - 1);
        user_copy[sizeof(user_copy) - 1] = '\0';

        char supplied_name[64] = "";
        char supplied_password[64] = "";

        char *token = strtok(user_copy, "&");
    
        while (token != NULL)
        {
            if (strncmp(token, "name=", 5) == 0)
            {
               
                strcpy(supplied_name, token + 5);
            }
            else if (strncmp(token, "psw=", 4) == 0)
            {
               
                strcpy(supplied_password, token + 4);
            }
            token = strtok(NULL, "&");
        }

        if (strcmp(supplied_name, name) == 0 &&
            strcmp(supplied_password, password) == 0)
        {
            auth = 1;
            break;
        }
    }

    fclose(file);
    return auth;
}
int pass()
{
    struct user userData;
    userData.auth = 0;
    strcpy(userData.name, "");
    strcpy(userData.password, "");
    // parse the payload
    char *token = strtok(payload, "&");
    while (token != NULL)
    {
        if (strncmp(token, "name=", 5) == 0)
        {
            strcpy(userData.name, token + 5);
        }
        else if (strncmp(token, "psw=", 4) == 0)
        {
            strcpy(userData.password, token + 4);
        }
        token = strtok(NULL, "&");
    }
    fprintf(stderr,"auth: %d\n", userData.auth);
    fprintf(stderr,"auth address: %p\n", &userData.auth);
    fprintf(stderr,"name address: %p\n", userData.name);
    fprintf(stderr,"password address: %p\n", userData.password);

    // Verify user credentials
    if (verify_user(userData.name, userData.password))
    {
        userData.auth = 1;
    }
    return userData.auth;

}


void route()
{
    ROUTE_START()
    fprintf(stderr, "1\n");
    ROUTE_GET("/eran")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        int fd = open("page3.html", O_RDONLY);

        int n;
        char buffer[BUF_SIZE];

        while ((n = read(fd, buffer, BUF_SIZE)) > 0)
            if (write(STDOUT_FILENO, buffer, n) != n)
                fprintf(stderr, "Wow write error\n");

        close(fd);
    }

    ROUTE_GET("/")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        // printf("Hello! you got here %s , %s %s",method,uri,qs);
        int fd = open("page1.html", O_RDONLY);

        int n;
        char buffer[BUF_SIZE];

        while ((n = read(fd, buffer, BUF_SIZE)) > 0)
            if (write(STDOUT_FILENO, buffer, n) != n)
                fprintf(stderr, "Wow write error\n");
    }

    ROUTE_POST("/")
    {
        printf("HTTP/1.1 303 See Other\r\n");
        printf("Location: /\r\n");
        printf("\r\n");

        char name[8] = "";
        char password[8] = "";

        char *token = strtok(payload, "&");
        while (token != NULL)
        {
            if (strncmp(token, "name=", 5) == 0)
            {
                strcpy(name, token + 5);
            }
            else if (strncmp(token, "psw=", 4) == 0)
            {
                strcpy(password, token + 4);
            }
            token = strtok(NULL, "&");
        }

        // Add user to the password file
        add_user(name, password);
    }

    ROUTE_POST("/pass")
    {
      
        if (pass())
        {
            printf("HTTP/1.1 303 See Other\r\n");
            printf("Location: /eran\r\n");
            return;
        }
        else
        {
            printf("HTTP/1.1 403 \r\n\r\n");
            printf("Wrong password page.\r\n");
        }
    }

    ROUTE_GET("/register")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        // printf("Hello! you got here %s , %s %s",method,uri,qs);
        int fd = open("page2.html", O_RDONLY);

        int n;
        char buffer[BUF_SIZE];

        while ((n = read(fd, buffer, BUF_SIZE)) > 0)
            if (write(STDOUT_FILENO, buffer, n) != n)
                fprintf(stderr, "Wow write error\n");
    }

    ROUTE_END()
}

