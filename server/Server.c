#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 65536

/* ---------- GET PROCESS LIST ---------- */

void get_process_list(char *output)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if(!dir){
        sprintf(output,"{\"processes\":[]}");
        return;
    }

    strcpy(output,"{\"processes\":[");

    int first = 1;

    while((entry = readdir(dir)) != NULL){

        int pid = atoi(entry->d_name);

        if(pid <= 0)
            continue;

        char name[256] = "unknown";
        char path[256];

        snprintf(path,sizeof(path),"/proc/%d/exefile",pid);

        FILE *fp = fopen(path,"r");

        if(fp){
            fgets(name,sizeof(name),fp);
            fclose(fp);
        }

        name[strcspn(name,"\n")] = 0;

        /* fake cpu + memory example (can extend later) */

        int cpu = rand() % 20;
        int mem = rand() % 50000;
        int threads = rand() % 5 + 1;

        if(!first)
            strcat(output,",");

        first = 0;

        char entry_json[512];

        snprintf(entry_json,sizeof(entry_json),
        "{\"pid\":%d,\"name\":\"%s\",\"cpu\":%d,\"mem\":%d,\"threads\":%d}",
        pid,name,cpu,mem,threads);

        strcat(output,entry_json);
    }

    strcat(output,"]}");

    closedir(dir);
}

/* ---------- KILL PROCESS ---------- */

void kill_process(int pid)
{
    kill(pid,SIGTERM);
}

/* ---------- SEND RESPONSE ---------- */

void send_response(int client,const char *body)
{
    char response[BUFFER_SIZE];

    int len = snprintf(response,sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        body);

    write(client,response,len);
}

/* ---------- HANDLE REQUEST ---------- */

void handle_request(int client,char *request)
{
    char body[BUFFER_SIZE];

    memset(body,0,sizeof(body));

    if(strstr(request,"GET /processes")){

        get_process_list(body);

    }
    else if(strstr(request,"GET /kill/")){

        int pid;

        sscanf(request,"GET /kill/%d",&pid);

        kill_process(pid);

        sprintf(body,"{\"status\":\"killed\"}");
    }
    else{

        sprintf(body,"{\"message\":\"RPM Server Running\"}");
    }

    send_response(client,body);
}

/* ---------- MAIN SERVER ---------- */

int main()
{
    int server_fd,client_fd;

    struct sockaddr_in server_addr,client_addr;

    socklen_t addr_len = sizeof(client_addr);

    char buffer[BUFFER_SIZE];

    printf("Starting RPM Server...\n");

    server_fd = socket(AF_INET,SOCK_STREAM,0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));

    listen(server_fd,10);

    printf("RPM Server listening on port %d\n",PORT);

    while(1){

        client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_len);

        memset(buffer,0,sizeof(buffer));

        read(client_fd,buffer,sizeof(buffer));

        handle_request(client_fd,buffer);

        close(client_fd);
    }

    return 0;
}

