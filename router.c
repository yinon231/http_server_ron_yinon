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

char *users[30] = {
    "name=ron&psw=1234\0",
    "name=yinon&psw=12345\0"};
int user_index = 2;

void route()
{
    ROUTE_START()
    fprintf(stderr, "1\n");
    ROUTE_GET("/eran")
    {
    }

    /*   ROUTE_GET("/")
       {
           printf("HTTP/1.1 200 OK\r\n\r\n");
           printf("Hello! You are using %s", request_header("User-Agent"));
       }*/

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
        printf("HTTP/1.1 200 OK\r\n\r\n");
        printf("Wow, seems that you POSTed %d bytes. \r\n", payload_size);
        printf("Fetch the data using `payload` variable.");
    }

    ROUTE_POST("/pass")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        char name[8] = "";
        char password[8] = "";
        char supplied_name[8] = "";
        char supplied_password[8] = "";
        int auth = 0;
        // parse the payload
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

        for (int i = 0; i < user_index; i++)
        {

            char user_copy[128];
            strncpy(user_copy, users[i], sizeof(user_copy) - 1);
            user_copy[sizeof(user_copy) - 1] = '\0';

            // parse the username and password
            char *token2 = strtok(user_copy, "&");
            while (token2 != NULL)
            {
                if (strncmp(token2, "name=", 5) == 0)
                {
                    strcpy(supplied_name, token2 + 5);
                }
                else if (strncmp(token2, "psw=", 4) == 0)
                {
                    strcpy(supplied_password, token2 + 4);
                }
                token2 = strtok(NULL, "&");
            }
            // check if the username and password match
            if (strcmp(supplied_password, password) == 0 && strcmp(supplied_name, name) == 0)
            {
                auth = 1;
            }
        }

        if (auth)
        {
            printf("authorized access");
        }
        else
        {
            printf("Unauthorized access.\r\n");
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

    ROUTE_POST("/register")
    {
        printf("HTTP/1.1 200 OK\r\n\r\n");
        char name[8] = "";
        char password[8] = "";
        // parse the payload
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

        // add the new user to the array
        snprintf(users[user_index], sizeof(users[user_index]), "name=%s&psw=%s", name, password);
        user_index++;

        printf("User %s registered successfully.\r\n", name);
    }

    ROUTE_END()
}
