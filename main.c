#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<time.h> 
#include"core_types.h"
#include "persistence.h"

// Forward Declarations
void init_players();
void init_machines();
Player* create_player(const char* rfid, const char* name);
Player* find_player(const char* rfid);
void add_credits(Player* p, int amount);
int redeem_tickets(Player* p, int cost);
int transfer_credits(Player* sender, Player* receiver, int amount);
void free_players();
void update_surge_pricing();
int trigger_play_with_score(int machine_index, const char* player_name);
void run_watchdog_scan();                
void print_ghost_logs(int machine_index);
void draw_dashboard();

extern Machine machines[];
extern int machine_count;

void flush_input(){
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(){
    srand(time(NULL)); 
    
    printf("[BOOT] Initializing Kernel...\n");
    init_players();
    init_machines();
    load_system_data();

    if (find_player("TAG_ADMIN") == NULL){
        create_player("TAG_ADMIN", "SysAdmin");
    }
    
    int running = 1;
    char input_buffer[64];

    while (running){
        draw_dashboard();
        
        if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) break;
        char command = input_buffer[0];

        switch (command){
            case 'q': 
                running = 0; 
                break;
            
            case 'n': // Register
                printf("Scan RFID Tag: ");
                char rfid[16];
                scanf("%15s", rfid);
                flush_input();
                
                Player *p =find_player(rfid);
                if (p) {
                    printf("Welcome back, %s! Credits: %d\n", p->name, p->credits);
                } else {
                    printf("New Tag! Enter Name: ");
                    char name[32];
                    scanf("%31s", name);
                    flush_input();
                    p = create_player(rfid, name);
                    add_credits(p, 20); 
                    printf("Registered!\n");
                }
                sleep(2); 
                break;

            case 'p': // Play with High Scores
                printf("Scan RFID to Play: ");
                char p_tag[16];
                scanf("%15s", p_tag);
                flush_input();
                
                Player *gamer = find_player(p_tag);
                if(gamer){
                    printf("Identity: %s | Credits: %d\nEnter Machine ID: ", gamer->name, gamer->credits);
                    int mid;
                    scanf("%d", &mid);
                    flush_input();
                    
                    if(gamer->credits > 0){
                        gamer->credits--;
                        trigger_play_with_score(mid, gamer->name); 
                        update_surge_pricing();
                    } else {
                        printf("Insufficient Credits!\n");
                    }
                } else {
                    printf("Unknown User.\n");
                }
                printf("Press Enter...");
                getchar();
                break;

            case 't': // P2P Transfer
                printf("--- P2P TRANSFER ---\n");
                printf("Sender RFID: ");
                char s_tag[16];
                scanf("%15s", s_tag);
                flush_input();
                
                Player *sender = find_player(s_tag);
                if(!sender) { printf("Sender not found.\n"); sleep(1); break; }
                
                printf("Receiver RFID: ");
                char r_tag[16];
                scanf("%15s", r_tag);
                flush_input();
                
                Player *receiver = find_player(r_tag);
                if(!receiver) { printf("Receiver not found.\n"); sleep(1); break; }
                
                printf("Amount to transfer: ");
                int amt;
                scanf("%d", &amt);
                flush_input();
                
                if(transfer_credits(sender, receiver, amt)){
                    printf("SUCCESS! %s sent %d credits to %s.\n", sender->name, amt, receiver->name);
                } else {
                    printf("FAILED. Check balance.\n");
                }
                sleep(2);
                break;

            case 'w': // Watchdog
                run_watchdog_scan();
                printf("Press Enter...");
                getchar();
                break;

            case 'r': // Redeem
                printf("--- PRIZE STORE ---\n1. Eraser (10)\n2. Plushie (1000)\nScan RFID: ");
                char shop_tag[16];
                scanf("%15s", shop_tag);
                flush_input();
                Player *shopper = find_player(shop_tag);
                if(shopper){
                     printf("User: %s | Tix: %d\nItem ID: ", shopper->name, shopper->tickets);
                     int item_id, cost=0;
                     scanf("%d", &item_id);
                     flush_input();
                     if(item_id == 1) cost = 10;
                     else if(item_id == 2) cost = 1000;
                     if(cost > 0) redeem_tickets(shopper, cost);
                }
                sleep(2);
                break;

            case 'l': // Logs
                printf("Machine ID: ");
                int lid;
                scanf("%d", &lid);
                flush_input();
                print_ghost_logs(lid);
                printf("Press Enter...");
                getchar();
                break;

            case 'e': // Export
                export_daily_report(machines, machine_count);
                printf("Press Enter...");
                getchar();
                break;
        }
    }

    printf("[SHUTDOWN] Saving system state...\n");
    save_system_data(); 
    free_players();     
    return 0;
}