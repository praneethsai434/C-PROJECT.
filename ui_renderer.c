#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "core_types.h"

extern Machine machines[];
extern int machine_count;

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define RESET "\x1B[0m"
#define CLR   "\033[2J\033[1;1H"

void draw_dashboard(){
    printf("%s", CLR); 
    printf("========================================================================\n");
    printf(" ARCADE KERNEL v4.2 [COMPETITIVE] | Time: %ld\n", time(NULL));
    printf("========================================================================\n");
    printf("| %-10s | %-8s | %-5s | %-5s | %-5s | %-12s |\n", "ID", "STATUS", "COST", "PLAYS", "BEST", "CHAMPION");
    printf("|------------|----------|-------|-------|-------|--------------|\n");

    for (int i = 0; i < machine_count; i++){
        Machine *m = &machines[i];
        
        char status_str[16];
        char *color = RESET;

        if (m->flags & STATUS_JAMMED){
            strcpy(status_str, "JAMMED");
            color = RED;
        } 
        else if (m->flags & STATUS_PLAYING){
            strcpy(status_str, "PLAYING");
            color = GRN;
        } 
        else{
            strcpy(status_str, "IDLE");
        }

        printf("| %-10s | %s%-8s%s | %5.2f | %-5d | %-5d | %-12s |\n", 
               m->name, color, status_str, RESET, 
               m->cost_per_play, m->session_plays, 
               m->high_score, m->champion_name);
    }
    printf("========================================================================\n");
    printf("USER: [n] Register [p] Play [r] Redeem [t] Transfer\n"); 
    printf("SYS:  [l] Logs     [e] Export [w] Watchdog [q] Quit\n");
    printf("> ");
}