#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"core_types.h"

Machine machines[MAX_MACHINES];
int machine_count =0;

/**
 * Ghost Maintenance Logger.
 */
void log_machine_action(Machine *m, char type, int val){
    int idx = m->log_head;
    m->logs[idx].timestamp = time(NULL);
    m->logs[idx].action_type = type;
    m->logs[idx].value = val;
    m->log_head = ((m->log_head + 1) % GHOST_LOG_SIZE);
}

void init_machines(){
    for(int i=0; i<5; i++){
        machines[i].id = i;
        sprintf(machines[i].name, "Cabinet-%02d", i+1);
        machines[i].flags = STATUS_IDLE;
        machines[i].base_cost = 1.0;
        machines[i].cost_per_play = 1.0;
        machines[i].total_plays = 0;
        machines[i].session_plays = 0;
        machines[i].log_head = 0;
        machines[i].last_heartbeat = time(NULL);
        
        // Init Leaderboard
        machines[i].high_score = 500; 
        strcpy(machines[i].champion_name, "CPU");
        
        machine_count++;
    }
    printf("[SYS] Hardware Bus: %d units online.\n", machine_count);
}

/**
 * Dynamic Surge Pricing Algorithm.
 */
void update_surge_pricing(){
    for(int i=0; i<machine_count; i++){
        if(machines[i].session_plays > 10){
            machines[i].flags |= STATUS_HIGH_LOAD;
            machines[i].cost_per_play = machines[i].base_cost * 1.5; 
        } 
        else{
            machines[i].flags &= ~STATUS_HIGH_LOAD;
            machines[i].cost_per_play = machines[i].base_cost;
        }
    }
}

/**
 * Watchdog Timer
 * Scans for 'zombie' processes.
 */
void run_watchdog_scan(){
    time_t now = time(NULL);
    int fixed_count = 0;
    
    printf("[WDT] Scanning Hardware Bus...\n");
    
    for(int i=0; i<machine_count; i++){
        Machine *m = &machines[i];
        
        if (m->flags & STATUS_PLAYING){
            double elapsed = difftime(now, m->last_heartbeat);
            
            // Threshold: 120 seconds
            if(elapsed > 120.0){
                printf("[CRITICAL] Machine %d Watchdog Timeout! (Stuck for %.0fs)\n", m->id, elapsed);
                
                m->flags = STATUS_IDLE; 
                m->flags |= STATUS_JAMMED; 
                
                log_machine_action(m, 'E', 999); 
                fixed_count++;
            }
        }
    }
    
    if(fixed_count > 0){
        printf("[WDT] Watchdog recovered %d stuck units.\n", fixed_count);
    } else {
        printf("[WDT] System Nominal.\n");
    }
}

/**
 * Play Trigger with Scoring Logic
 */
int trigger_play_with_score(int machine_index, const char* player_name){
    if(machine_index < 0 || machine_index >= machine_count) return 0;
    
    Machine *m = &machines[machine_index];
    
    if(m->flags & STATUS_JAMMED){
        printf("[ERR] Machine %d is JAMMED. Technician required.\n", m->id);
        return 0;
    }

    m->flags |= STATUS_PLAYING;
    m->total_plays++;
    m->session_plays++;
    m->last_heartbeat = time(NULL);
    
    log_machine_action(m, 'P', 1);

    // Gameplay Simulation
    // Generate a random score between 100 and 1500
    int score = (rand() % 1400) + 100;
    
    if (score > m->high_score){
        printf("\n*** NEW HIGH SCORE! ***\n");
        printf("%s beat %s with %d pts!\n", player_name, m->champion_name, score);
        m->high_score = score;
        strncpy(m->champion_name, player_name, 31);
    } else {
        printf("\nGame Over. Score: %d (High: %d by %s)\n", score, m->high_score, m->champion_name);
    }
    
    return score;
}

void print_ghost_logs(int machine_index){
    if(machine_index < 0 || machine_index >= machine_count) return;
    Machine *m = &machines[machine_index];
    
    printf("\n--- GHOST LOGS: %s ---\n", m->name);
    for(int i=0; i<GHOST_LOG_SIZE; i++){
        int idx = (m->log_head + i) % GHOST_LOG_SIZE;
        if(m->logs[idx].timestamp == 0) continue; 
        
        printf("[%ld] Type: %c | Val: %d\n", 
               m->logs[idx].timestamp, m->logs[idx].action_type, m->logs[idx].value);
    }
    printf("---------------------------\n");
}