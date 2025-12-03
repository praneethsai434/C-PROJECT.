#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include"core_types.h"

/**
 * Saves the current state of the Player Hash Table to a binary file.
 */
void save_system_data();

/**
 * Loads player data from the binary file and reconstructs the Hash Table.
 */
void load_system_data();

/**
 * Generates a Management Report in CSV format.
 */
void export_daily_report(Machine machines[], int count);

#endif