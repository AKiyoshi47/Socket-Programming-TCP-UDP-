#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pwd.h>
#include <netinet/tcp.h>
#include <time.h>
#include <stdbool.h>

static void die(const char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{

    //declares the default port
    int port = 1212;
    //updates the port if the user provides it
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }
    else if (argc > 2)
    {
        die("Usage: server <port>\n");
    }
    //generates a random number between 1 and 100
    srand(time(NULL));
    int randNum = rand() % 100 +1;
    //printf("randNum: %d\n", randNum);

    // initializes variables for server and client file descriptors
    int server_fd, client_fd;
    // initializes server and client address structures
    struct sockaddr_in servaddr, cli_addr;
    socklen_t len = sizeof(cli_addr);

    // creates a socket for TCP connection
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        die("socket failed");
    }

    //setup server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // allow port number to be re-used
    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);
    
    //binds socket to address
    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        die("bind failed");
    }

    //listens for connections
    if (listen(sock, 10) < 0)
    {
        die("listen failed");
    }

    //outer loop used to keep server running in search of other clients
    while(1){
    //accepts connection from client
    if ((client_fd = accept(sock, (struct sockaddr *)&cli_addr, &len)) < 0)
    {
        die("accept failed");
    }

    // initializes variables for message, name, and result
    char msg[1000];
    char name[1000];
    char result[1000];
    int count = 0; //initializes count to 0
    bool first = true; //initializes first to true to determine if it is the first guess

    //inner loop used to keep server running for current client
    while (1)
    {
        memset(msg, 0, sizeof(msg)); //clears message buffer
        int guess;
        recv(client_fd, msg, sizeof(msg), 0); //receives message from client
        if(first){
            sscanf(msg, "%s %d", name, &guess);//scans in received message and separates name from number
            first = false;
            
        }else{
            sscanf(msg, "%d", &guess);//after 1st guess, only the number is scanned
            
        }
        count++; //increments count for each guess

        //checks if guess is correct
        if (guess == randNum)
         {
            //checks if count is 1 or more than 1 to determine if guess or guesses should be printed
            if (count == 1)
            {
                strcpy(result, "Guess!");
            }
            else if (count > 1)
            {
                strcpy(result, "Guesses!");
            }
            char response[5000];
            //formats response to be sent to client
            sprintf(response, "Correct %s, you guessed the right number in %d %s", name, count, result);
            send(client_fd, response, strlen(response), 0);//send response to client
            close(client_fd);//close client connection
            break;
            //if guess is not correct, checks if guess is higher or lower than random number
            //sends "Server: High" or "Server: Low" to client
        }else if(guess>randNum){
                send(client_fd, "Server: High", strlen("Server: High"), 0);
            }else if(guess<randNum){
                send(client_fd, "Server: Low", strlen("Server: Low"), 0);
            }
    }
}
    close(sock);
    return 0;
}
   