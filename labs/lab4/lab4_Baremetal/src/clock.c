
#include "tm4c1294ncpdt.h"
#include "clock.h"

int PLL_Init(enum frequency freq) {
    SYSCTL->MOSCCTL &= ~(0x4);                      // Power up MOSC
    SYSCTL->MOSCCTL &= ~(0x8);                      // Enable MOSC
    while ((SYSCTL->RIS & 0x100) == 0) {};          // Wait for MOSC to be ready
    SYSCTL->RSCLKCFG |= (0x3 << 20);                // Select MOSC as system clock source
    SYSCTL->RSCLKCFG |= (0x3 << 24);                // Select MOSC as PLL clock source

    SYSCTL->PLLFREQ0 |= 0x60;                       // Set MINT field
    SYSCTL->PLLFREQ1 |= 0x4;                        // Set N field
    
    SYSCTL->MEMTIM0 &= ~((0xF << 22) | (0xF << 6));     // Reset FBCHT and EBCHT
    SYSCTL->MEMTIM0 &= ~((0xF << 16) | (0xF << 0));     // Reset EWS and FWS
    SYSCTL->MEMTIM0 &= ~((0x1 << 21) | (0x1 << 5));     // Reset FBCE and EBCE
    
    SYSCTL->RSCLKCFG &= ~(0x1 << 28);                   // Temporarilly bypass PLL

    switch (freq) {
        case 120:
            SYSCTL->MEMTIM0 |= (0x6 << 22) | (0x6 << 6);    // Set FBCHT and EBCHT
            SYSCTL->MEMTIM0 |= (0x5 << 16) | (0x5 << 0);    // Set EWS and FWS
            SYSCTL->RSCLKCFG |= 0x3;                        // Set PSYSDIV to use 120 MHZ clock
            SYSCTL->RSCLKCFG &= ~0x3FC;                     // Update PSYSDIV field
            break;
        case 60:
            SYSCTL->MEMTIM0 |= (0x3 << 22) | (0x3 << 6);    // Set FBCHT and EBCHT
            SYSCTL->MEMTIM0 |= (0x2 << 16) | (0x2 << 0);    // Set EWS and FWS
            SYSCTL->RSCLKCFG |= 0x7;                        // Set PSYSDIV to use 60 MHZ clock
            SYSCTL->RSCLKCFG &= ~0x3F8;                     // Update PSYSDIV field
            break;
        case 12:
            SYSCTL->MEMTIM0 |= (0x1 << 21) | (0x1 << 5);    // Set FBCE and EBCE
            SYSCTL->RSCLKCFG |= 0x27;                       // Set PSYSDIV to use 12 MHZ clock
            SYSCTL->RSCLKCFG &= ~0x3D8;                     // Update PSYSDIV field
            break;
        default:
            return -1;
    }

    SYSCTL->RSCLKCFG |= (0x1 << 30);                // Enable new PLL settings
    SYSCTL->PLLFREQ0 |= (0x1 << 23);                // Power up PLL
    while ((SYSCTL->PLLSTAT & 0x1) == 0) {};        // Wait for PLL to lock and stabilize

    SYSCTL->RSCLKCFG |= (0x1u << 31) | (0x1 << 28);  // Use PLL and update Memory Timing Register
    return 1;
}
