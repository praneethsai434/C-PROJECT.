Arcade Management Kernel (v4.2)

A bare-metal compatible, low-latency management kernel for arcade cabinets featuring real-time state management, dynamic economics, and binary persistence.

📖 Overview

The Arcade Management Kernel is a high-performance control system designed to run on resource-constrained embedded hardware. Unlike heavy general-purpose operating systems, this kernel operates with a minimal footprint, utilizing direct memory management and bitwise logic to handle hardware states.

It powers a network of arcade cabinets, managing user authentication via RFID, processing financial transactions (Credits/Tickets), and enforcing dynamic surge pricing based on real-time machine load.
Key Features

⚡ Zero-Latency Auth: Instant player lookup using the DJB2 Hashing Algorithm (O(1) time complexity).

💾 Binary Persistence: Custom serialization layer that saves state to arcade_data.bin while stripping pointer addresses to ensure data integrity across reboots.

📈 Dynamic Surge Pricing: Deterministic algorithm that increases game costs by 50% during high-traffic sessions (>10 plays/hr).

🛡️ Watchdog Timer: Automated system integrity scanner that detects and resets "zombie" processes (machines stuck in PLAYING state > 
120s).

🏆 Social Leaderboards: Decentralized high-score tracking per machine, storing the "Champion" name in memory.

💸 P2P Economy: Secure peer-to-peer credit transfer system allowing users to share funds via RFID.

🎁 Redemption Store: Transactional ticket exchange logic for prize dispensing.

🛠️ Tech Stack

Language: C99 (Strict ANSI Compliance)

Build System: GNU Make

Data Structures:

Hash Map: For User Database (Collision handling via Chained Linked Lists).

Ring Buffer: For "Ghost" Maintenance Logs (Fixed memory footprint).

Bitmasks: For Machine Status (IDLE, PLAYING, JAMMED, HIGH_LOAD).

🚀 Getting Started

Prerequisites

GCC Compiler (gcc)

GNU Make (Optional, but recommended)

Compilation

Clone the repository and compile using the provided Makefile:

# Standard Build (Optimized -O2)
make

# Debug Build (Symbols enabled, no optimization)
make debug

# Clean Artifacts
make clean


If you do not have Make installed, you can compile manually:
gcc main.c player_manager.c machine_manager.c ui_renderer.c persistence_manager.c -o arcade_os


Running the Kernel
Execute the binary to start the main event loop:
./arcade_os


🎮 User Manual

The system interface is a VT100-compatible terminal dashboard.
Core Commands
Key
Function
Description
n
Register / Login
Scan RFID. If new, registers user + 20 promo credits.
p
Play Game
Select a machine to play. Triggers gameplay simulation & scoring.
t
P2P Transfer
Transfer credits from one RFID tag to another.
r
Redeem Prize
Exchange tickets for items (Eraser, Plushie, etc.).
w
Watchdog Scan
Manually trigger the system integrity scan to fix stuck units.
l
View Logs
Inspect the Ring Buffer logs for a specific machine.
e
Export Data
Dump current analytics to daily_report.csv.
q
Shutdown
Save state to disk and halt the kernel.

📂 File Structure
main.c: The central event loop and system bootstrap logic.

core_types.h: Definitions of structs (Player, Machine), bitmasks, and constants.

machine_manager.c: Hardware logic, Surge Pricing, Watchdog, and High Score systems.

player_manager.c: Hash Map implementation, P2P transfers, and user creation.

persistence_manager.c: Binary I/O layer handling Data Transfer Objects (DTOs).

ui_renderer.c: Terminal rendering engine using ANSI escape codes.

👥 Authors
Praneeth Sai Varada
Raghav Sharma 
