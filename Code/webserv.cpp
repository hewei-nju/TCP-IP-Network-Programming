/** @author heweibright@gmail.com 
 *  @date 2021/9/6 16:53
 *  Copyright (c) All rights reserved.
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/sendfile.h>

const int BUF_SIZE = 1024;
const int SMALL_BUF = 100;

void *request_handler(void *arg);

void send_data(FILE *fp, const char *ct, const char *file_name);

const char *content_type(const char *file);

void send_error(FILE *fp);

void error_handling(const char *msg);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage : " << argv[0] << " <port>\n";
        std::terminate();
    }

    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr;

    if ((serv_sock = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, reinterpret_cast<sockaddr *>(&serv_adr), sizeof(serv_adr)) == -1)
        error_handling("bind() error");
    if (listen(serv_sock, 20) == -1)
        error_handling("listen() error");

    pthread_t tid;
    while (true) {
        struct sockaddr_in clnt_adr;
        socklen_t clnt_adr_size = sizeof(clnt_adr);
        if ((clnt_sock = accept(serv_sock, reinterpret_cast<sockaddr *>(&clnt_adr), &clnt_adr_size)) == -1)
            error_handling("accept() error");
        std::cout << "Connection Request : " << inet_ntoa(clnt_adr.sin_addr) << ":" << ntohs(clnt_adr.sin_port) << '\n';
        pthread_create(&tid, nullptr, request_handler, &clnt_sock);
        pthread_join(tid, nullptr);////
    }
    close(serv_sock);
    return 0;
}

void *request_handler(void *arg) {
    int clnt_sock = *((int *) arg);
    char req_line[SMALL_BUF];
    FILE *clnt_read;
    FILE *clnt_write;

    char method[10];
    char ct[15];
    char file_name[30];

    clnt_read = fdopen(clnt_sock, "r");
    clnt_write = fdopen(dup(clnt_sock), "w");
    fgets(req_line, SMALL_BUF, clnt_read);
    if (strstr(req_line, "HTTP/") == nullptr) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return nullptr;
    }

    // GET /filename.html HTTP/1.1/r/n
    strcpy(method, strtok(req_line, " /"));
    // GET\0/filename.html HTTP/1.1/r/n
    strcpy(file_name, strtok(req_line + strlen(method) + 2, " "));
    strcpy(ct, content_type(file_name));
    if (strcmp(method, "GET") != 0) {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return nullptr;
    }

    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
}

void send_data(FILE *fp, const char *ct, const char *file_name) {
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE *send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    // 由于html文件的编码问题，fopen可能会出现问题？
    // 我使用浏览器去按照相应的方式去浏览该html文件却不能正常进行
    // 然后在调试时，一直不能正确的按照预期执行，因为fopen一直报错
    // 我尝试了fstream的函数，也尝试了通过sendfile直接发送的方式，都没有效果
    // 然后我运行该程序，使用wget http://127.0.0.1:port/index.html能够正常的获取该资源
    // 所以这是为什么呢？为什么浏览器不能正常浏览该文件呢？
    send_file = fopen(file_name, "r");
    if (send_file == nullptr) {
        send_error(fp);
        return;
    }

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    while (fgets(buf, BUF_SIZE, send_file) != nullptr) {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

const char *content_type(const char *file) {
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    // file = filename.extension
    strcpy(file_name, file);
    strtok(file_name, ".");
    // filename/0extension
    strcpy(extension, file_name + strlen(file_name) + 1);
    if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/plain";
}

void send_error(FILE *fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>发生错误！查看请求文件名和请求方式！"
                     "</font></body></html>";

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content, fp);
    fflush(fp);
}

void error_handling(const char *msg) {
    std::cerr << msg << '\n';
    std::terminate();
}

