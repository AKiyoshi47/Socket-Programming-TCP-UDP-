#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <stdbool.h>

static void die(const char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
    // Set default port
    int port = 1212;
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }
    else if (argc > 2)
    {
        die("Usage: server <port>\n");
    }

    // Generate a random number between 1 and 100
    srand(time(NULL));
    int randNum = rand() % 100 + 1;
    //printf("Random Number: %d\n", randNum);

    // Create UDP socket
    int sock;
    struct sockaddr_in servaddr, cli_addr;
    socklen_t len = sizeof(cli_addr);

    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        die("Socket creation failed");
    }

    // Allow reusing the port
    int opt_val = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof(opt_val));

    // Setup server address structure
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Bind socket to address
    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        die("Bind failed");
    }



    // Variables for message, client name, and result
    char msg[1000];
    char name[1000];
    char result[1000];
    int count = 0; // Number of guesses
    bool first = true; // First guess flag

    // Loop to keep the server running
    while (1)
    {
        memset(msg, 0, sizeof(msg)); // Clear message buffer
        int guess;

        // Receive message from client
        int recv_len = recvfrom(sock, msg, sizeof(msg), 0, (struct sockaddr *)&cli_addr, &len);
        if (recv_len < 0)
        {
            die("recvfrom failed");
        }

        if (first)
        {
            sscanf(msg, "%s %d", name, &guess); // Extract name and guess
            first = false;
        }
        else
        {
            sscanf(msg, "%d", &guess); // Extract guess
        }

        count++; // Increment guess count
        char response[5000];

        // Check if the guess is correct
        if (guess == randNum)
        {
            strcpy(result, (count == 1) ? "Guess!" : "Guesses!");
            sprintf(response, "Correct %s, you guessed the right number in %d %s", name, count, result);
            sendto(sock, response, strlen(response), 0, (struct sockaddr *)&cli_addr, len);
            break; // End game after correct guess
        }
        else if (guess > randNum)
        {
            sendto(sock, "Server: High", strlen("Server: High"), 0, (struct sockaddr *)&cli_addr, len);
        }
        else
        {
            sendto(sock, "Server: Low", strlen("Server: Low"), 0, (struct sockaddr *)&cli_addr, len);
        }
    }

    close(sock);
    return 0;
}
