#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

        
    // Ex 1: GET dummy from main server
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("3.8.116.10", "/api/v1/dummy", NULL, NULL, 0);
    send_to_server(sockfd, message);
    printf("%s", message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    // Ex 2: POST dummy and print response from main server
    char **body_data = calloc(2, sizeof(char*));
    body_data[0] = calloc(10, sizeof(char));
    body_data[1] = calloc(10, sizeof(char));
    body_data[0] = "test01234";
    body_data[1] = "test56789";
    message = compute_post_request("3.8.116.10", "/api/v1/dummy", "application/x-www-form-urlencoded", 
        body_data, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // Ex 3: Login into main server
   // memset(message, 0, BUFLEN);
   // memset(response, 0, BUFLEN);
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    char **body_data1 = calloc(2, sizeof(char*));
    body_data1[0] = calloc(17, sizeof(char));
    body_data1[1] = calloc(17, sizeof(char));
    body_data1[0] = "username=student";
    body_data1[1] = "password=student";
    message = compute_post_request("3.8.116.10", "/api/v1/auth/login", "application/x-www-form-urlencoded",
        body_data1, 2, NULL, 0);
    send_to_server(sockfd, message);
    printf("%s\n", message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // Ex 4: GET weather key from main server
    char *cookie1 = strstr(response,"connect");
    int count = 0;
    while(cookie1[count] != ';') {
        count ++;
    }
    char **cookies = calloc(3, sizeof(char*));
    cookies[0] = calloc(count, sizeof(char));
    for(int i = 0; i < count; i++){
        cookies[0][i] = cookie1[i];
    }
    cookies[1] = calloc(7, sizeof(char));
    cookies[1] = " Path=/";
    cookies[2] = calloc(9, sizeof(char));
    cookies[2] = " HttpOnly";
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("3.8.116.10", "/api/v1/weather/key", NULL, cookies, 3);
    send_to_server(sockfd, message);
    printf("%s\n", message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // Ex 5: Logout from main server
    char *cookie2 = strstr(response, "key");
    count = 0;
    while(cookie2[count] != '}') {
        count ++;
    }
    char **cookies1 = calloc(1, sizeof(char*));
    cookies1[0] = calloc(count - 3, sizeof(char));
    cookies1[0][0] = 'k';
    cookies1[0][1] = 'e';
    cookies1[0][2] = 'y';
    cookies1[0][3] = '=';
    for(int i = 4; i < count - 3; i++) {
        cookies1[0][i] = cookie2[i+2];
    }
    sockfd = open_connection("3.8.116.10", 8080, AF_INET, SOCK_STREAM, 0);
    message = compute_get_request("3.8.116.10", "/api/v1/auth/logout", NULL, cookies1, 1);
    send_to_server(sockfd, message);
    printf("%s\n", message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);
    close_connection(sockfd);

    // BONUS: make the main server return "Already logged in!"


    return 0;
}
