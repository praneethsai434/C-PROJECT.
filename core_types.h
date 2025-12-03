#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include<time.h>
#include<stdint.h>
#include<stdbool.h>

// ---SYSTEM CONSTANTS---
#define MAX_PLAYERS_HASH 1024  // Size of Hash Table
#define MAX_MACHINES 50        // Max physical cabinets supported
#define GHOST_LOG_SIZE 20      // Size of the circular buffer for debugging
#define ID_LEN 16              // Length of RFID Tag Strings

// ---BITMASKS FOR MACHINE STATUS---
#define STATUS_OFFLINE    0x00 
#define STATUS_IDLE       0x01 
#define STATUS_PLAYING    0x02 
#define STATUS_JAMMED     0x04 
#define STATUS_HIGH_LOAD  0x08 

// ---DATA STRUCTURES---

// 1. Ghost Maintenance Log
typedef struct{
    time_t timestamp;
    char action_type; // 'P'lay, 'E'rror, 'M'aintenance
    int value;
}ActionLog;

// 2. Machine Logic Unit
typedef struct{
    int id;
    char name[32];
    uint8_t flags;        
    float cost_per_play;  
    float base_cost;      
    int total_plays;      
    int session_plays;    
    
    time_t last_heartbeat; 
    
    // Competitive Data
    int high_score;         
    char champion_name[32]; 
    
    // The "Black Box" circular buffer
    ActionLog logs[GHOST_LOG_SIZE];
    int log_head;         
}Machine;

// 3. Player Account (Hash Table Node)
typedef struct Player{
    char rfid_tag[ID_LEN]; 
    char name[32];
    int credits;
    int tickets;
    time_t last_seen;
    struct Player *next;   
}Player;

#endif