#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#define SERVER_PORT 21
char* address;

int getIp(char* name) {
    struct hostent *h;

/**
 * The struct hostent (host entry) with its terms documented

    struct hostent {
        char *h_name;    // Official name of the host.
        char **h_aliases;    // A NULL-terminated array of alternate names for the host.
        int h_addrtype;    // The type of address being returned; usually AF_INET.
        int h_length;    // The length of the address in bytes.
        char **h_addr_list;    // A zero-terminated array of network addresses for the host.
        // Host addresses are in Network Byte Order.
    };

    #define h_addr h_addr_list[0]	The first address in h_addr_list.
*/
    if ((h = gethostbyname(name)) == NULL) {
        herror("gethostbyname()");
        return 1;
    }

    printf("Host name  : %s\n", h->h_name);
    printf("IP Address : %s\n", inet_ntoa(*((struct in_addr *) h->h_addr)));

    address = h->h_addr;

    return 0;
}

int main(int argc, char **argv) {

    if (getIP(argv[1]) == 1) {
        printf("Error in getIP\n");
        return 1;
    }

    char message1[100] = "user ";
    char message2[100] = "pass ";
    char message3[100] = "pasv";
    char message4[100] = "retr "


    strcat(message1, agrv[2]);
    strcat(message2, agrv[3]);
    strcat(message4, argv[4]);

    
    size_t bytes;

    /*server address handling*/
    int sockfd;
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);    /*32 bit Internet address network byte ordered*/
    server_addr.sin_port = htons(SERVER_PORT);        /*server TCP port must be network byte ordered */

    /*open a TCP socket*/
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    /*User Message*/
    bytes = write(sockfd, message1, strlen(message1));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }

    /*Passive Mode Message*/
    bytes = write(sockfd, message3, strlen(message3));
    if (bytes > 0)
        printf("Bytes escritos %ld\n", bytes);
    else {
        perror("write()");
        exit(-1);
    }


    char conf[100];
    bytes = read(sockfd, conf, 100);

    printf("Resultado: %s\n", conf);


    if (close(sockfd)<0) {
        perror("close()");
        exit(-1);
    }
    return 0;
}




