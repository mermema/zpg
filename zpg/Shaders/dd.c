#define MAX_USERS 32

typedef struct {
    char name[50];
    int x, y;
    int active;
} User;

User usrarray[MAX_USERS];
int usr_count = 0;
 

if (fds[1].revents & POLLIN) {
    int len = read(sock, buf, sizeof(buf) - 1);
    if (len <= 0) break;
    buf[len] = '\0'; // ukonči string

    // 1️⃣ Hráč se pohnul
    char name[50];
    int x, y;
    if (sscanf(buf, "POSUSR %49s %d %d", name, &x, &y) == 3) {
        int found = 0;
        for (int i = 0; i < MAX_USERS; i++) {
            if (usrarray[i].active && strcmp(usrarray[i].name, name) == 0) {
                usrarray[i].x = x;
                usrarray[i].y = y;
                found = 1;
                printf("→ %s se přesunul na (%d,%d)\n", name, x, y);
                break;
            }
        }
        if (!found) {
            // nový uživatel (ještě ho neznáme)
            for (int i = 0; i < MAX_USERS; i++) {
                if (!usrarray[i].active) {
                    strcpy(usrarray[i].name, name);
                    usrarray[i].x = x;
                    usrarray[i].y = y;
                    usrarray[i].active = 1;
                    printf("+ Nový hráč: %s na (%d,%d)\n", name, x, y);
                    break;
                }
            }
        }
    }

    // 2️⃣ Hráč odešel
    else if (sscanf(buf, "QUITUSR %49s", name) == 1) {
        for (int i = 0; i < MAX_USERS; i++) {
            if (usrarray[i].active && strcmp(usrarray[i].name, name) == 0) {
                usrarray[i].active = 0;
                printf("- Hráč %s opustil hru.\n", name);
                break;
            }
        }
    }

    // 3️⃣ Ostatní zprávy vypíšeme standardně
    else {
        printf("%s", buf);
    }
}
