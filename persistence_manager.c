#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include"core_types.h"

extern Player* create_player(const char* rfid, const char* name);
extern Player* player_table[MAX_PLAYERS_HASH]; 

typedef struct{
    char rfid_tag[ID_LEN];
    char name[32];
    int credits;
    int tickets;
    time_t last_seen;
}PlayerFileRecord;

void save_system_data(){
    FILE *fp = fopen("arcade_data.bin", "wb"); 
    if (!fp){
        printf("[ERR] Disk Write Failure! Check permissions.\n");
        return;
    }

    int record_count = 0;
    for (int i = 0; i < MAX_PLAYERS_HASH; i++){
        Player *current = player_table[i];
        while (current != NULL){
            PlayerFileRecord record;
            memset(&record, 0, sizeof(PlayerFileRecord)); 
            
            strncpy(record.rfid_tag, current->rfid_tag, ID_LEN);
            strncpy(record.name, current->name, 32);
            record.credits = current->credits;
            record.tickets = current->tickets;
            record.last_seen = current->last_seen;

            fwrite(&record, sizeof(PlayerFileRecord), 1, fp);
            
            current = current->next;
            record_count++;
        }
    }
    fclose(fp);
    printf("[SYS] Persistence: %d records secured to 'arcade_data.bin'.\n", record_count);
}

void load_system_data(){
    FILE *fp = fopen("arcade_data.bin", "rb"); 
    if (!fp){
        printf("[SYS] No backup found. Initializing fresh database.\n");
        return;
    }

    PlayerFileRecord record;
    int count = 0;

    while (fread(&record, sizeof(PlayerFileRecord), 1, fp) == 1) {
        Player *p = create_player(record.rfid_tag, record.name);
        if (p){
            p->credits = record.credits;
            p->tickets = record.tickets;
            p->last_seen = record.last_seen;
            count++;
        }
    }
    fclose(fp);
    printf("[SYS] Persistence: Restored %d players from disk.\n", count);
}

void export_daily_report(Machine machines[], int count){
    FILE *fp = fopen("daily_report.csv", "w");
    if (!fp){
        printf("[ERR] Could not write CSV report.\n");
        return;
    }

    fprintf(fp, "Machine ID,Name,Plays,Revenue,Status\n");
    for(int i=0; i<count; i++){
        float revenue = machines[i].session_plays * machines[i].base_cost;
        char *status_txt = (machines[i].flags & STATUS_JAMMED) ? "JAMMED" : "OK";
        
        fprintf(fp, "%d,%s,%d,%.2f,%s\n", 
            machines[i].id, machines[i].name, machines[i].session_plays, revenue, status_txt
        );
    }
    fclose(fp);
    printf("[SYS] Analytics: Data exported to 'daily_report.csv'.\n");
}