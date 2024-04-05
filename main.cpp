#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>


#define local_ipv6 "fe80::bf77:c8ca:d474:d522"
#define server_ipv6 "fd15:4ba5:5a2b:1008:1bf0:f748:3884:6b52"
#define PORT 2000

int main() {
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    int sockfd;
    sockaddr_in6 server_sock;
    int addrlen = sizeof(server_sock);

    // 创建IPv6套接字
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        perror("socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, reinterpret_cast<const char *>(&opt), sizeof(opt));
    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, reinterpret_cast<const char *>(&opt), sizeof(opt));
    printf("create sock_fd\n");
    // 设置服务器端套接字地址
    memset(&server_sock, 0, sizeof(server_sock));
    server_sock.sin6_family = AF_INET6;
    inet_pton(AF_INET6, server_ipv6, &server_sock.sin6_addr);
    server_sock.sin6_port = htons(PORT);
    printf("try to connect server\n");
    // 连接到服务器
    if (connect(sockfd, (struct sockaddr *)&server_sock, addrlen) == SOCKET_ERROR) {
        perror("connect failed");
        closesocket(sockfd);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    printf("Connected to the server\n");

    char buff[1024];
    while(1)
    {
        memset(buff,1,1000);
        send(sockfd, buff, strlen(buff), 0);
        printf("send information\n");
        Sleep(1000); // 10000毫秒等于10秒
    }

    // 关闭套接字
    closesocket(sockfd);
    WSACleanup();

    return 0;
}
