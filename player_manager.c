#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"core_types.h"

Player* player_table[MAX_PLAYERS_HASH];

unsigned long hash_func(const char *str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash % MAX_PLAYERS_HASH;
}

void init_players(){
    memset(player_table, 0, sizeof(player_table));
    printf("[SYS] Player Database Initialized (Capacity: %d)\n", MAX_PLAYERS_HASH);
}

Player* find_player(const char* rfid){
    unsigned long index = hash_func(rfid);
    Player *current = player_table[index];

    while (current != NULL){
        if (strcmp(current->rfid_tag, rfid) == 0){
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Player* create_player(const char* rfid, const char* name){
    unsigned long index = hash_func(rfid);
    
    Player *new_p = (Player*)malloc(sizeof(Player));
    if (!new_p) {
        printf("[CRITICAL] Memory Allocation Failed!\n");
        return NULL;
    }

    strncpy(new_p->rfid_tag, rfid, ID_LEN);
    strncpy(new_p->name, name, 31);
    new_p->credits = 0;
    new_p->tickets = 0;
    new_p->last_seen = time(NULL);
    
    new_p->next = player_table[index];
    player_table[index] = new_p;
    
    return new_p;
}

void add_credits(Player* p, int amount){
    if(p) p->credits += amount;
}

/**
 * Peer-to-Peer Transfer
 */
int transfer_credits(Player* sender, Player* receiver, int amount){
    if (!sender || !receiver) return 0;
    
    if (sender->credits >= amount){
        sender->credits -= amount;
        receiver->credits += amount;
        return 1; 
    }
    return 0; 
}

/**
 * Transactional Redemption
 */
int redeem_tickets(Player* p, int cost){
    if (!p) return 0;
    
    if (p->tickets >= cost){
        p->tickets -= cost;
        printf("[TXN] Redemption Success! Items dispensed. Balance: %d\n", p->tickets);
        return 1;
    } else {
        printf("[TXN] Insufficient Tickets. Need: %d, Have: %d\n", cost, p->tickets);
        return 0;
    }
}

void free_players(){
    for (int i = 0; i < MAX_PLAYERS_HASH; i++){
        Player *current = player_table[i];
        while (current != NULL) {
            Player *temp = current;
            current = current->next;
            free(temp);
        }
    }
}