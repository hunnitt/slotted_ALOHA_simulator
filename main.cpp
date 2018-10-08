#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define T 100000 
// time slots
#define STATE_FIRST -1
// waiting for first transmission
#define STATE_WAIT 0
// waiting for second and later transmission
#define STATE_TRANSMITTED 1
// transmitted
#define STATE_BACKLOGGED 2
// backlogged == waiting for retransmission

void usage(void) {
    printf("Usage : ./slotted_ALOHA_simul < window size > < probability for 1st transmission(option) >\n");
    printf("( if you don't write the option, probability for 1st transmission will be 1/e. )\n");
    exit(0);
}

int uniform_distribution(int wnd_size) {
    int num = (int)rand(); 
    int res = num % wnd_size;
    return res;
}

int main(int argc, char * argv[]) {
    static int senders = 1;
    int wnd_size;
    double P;
    int successful_slot_cnt;
    double slot_efficiency;

    if (argc != 2 && argc != 3) 
        usage();
    if (argc == 2) { wnd_size = atoi(argv[1]); P = exp(-1); } 
    // nodes transmit pkt in first time with 1/e
    if (argc == 3) { wnd_size = atoi(argv[1]); P = atof(argv[2]); }

    printf("slot efficiency if W = %02d\n", wnd_size);

    for (; senders <= 32; senders++) {
        slot_efficiency = 0.0;
        successful_slot_cnt = 0;
        char remaining_time[senders];
        // remaining time to retransmit
        char state[senders];
        // state of senders : WAIT, TRANSMITTED, BACKLOGGED
        memset(remaining_time, STATE_FIRST, senders);
        memset(state, STATE_FIRST, senders);

        for (int t=1; t<=T; t++) {
            for (int i=0; i<senders; i++) {
                if (state[i] == STATE_FIRST) {
                    if ( ((double)rand() / (double)RAND_MAX) <= P) {
                        // i th packet's transmission
                        state[i] = STATE_TRANSMITTED;
                    }
                }
                else if (state[i] == STATE_BACKLOGGED || 
                         state[i] == STATE_WAIT) {
                    if (remaining_time[i] == 0) {
                        state[i] = STATE_TRANSMITTED;
                        remaining_time[i] = uniform_distribution(wnd_size);
                    }
                    else if (remaining_time[i] > 0) {
                        remaining_time[i]--;
                    }
                }
            }
            
            int transmit_cnt = 0;
            // transmit_cnt == 1 -> successful slot
            // transmit_cnt >= 2 -> collision
            // transmit_cnt == 0 -> idle
            int last_transmit_user = -1;
            // to find successful node

            for (int j=0; j<senders; j++) {
                if (state[j] == STATE_TRANSMITTED) {
                    transmit_cnt++;
                    last_transmit_user = j;
                }
            }

            if (transmit_cnt == 1) {
                state[last_transmit_user] = STATE_WAIT;
                remaining_time[last_transmit_user] = uniform_distribution(wnd_size);
                successful_slot_cnt++;
            }

            else if (transmit_cnt > 1) {
                for (int j=0; j<senders; j++) {
                    if (state[j] == STATE_TRANSMITTED) {
                        state[j] = STATE_BACKLOGGED;
                        remaining_time[j] = uniform_distribution(wnd_size);
                    }
                }
            }

            else {
                    
            }

        }
        slot_efficiency = (double)successful_slot_cnt / (double)T;
        printf("%2d ==> %.3lf\n", senders, slot_efficiency);
    }
	return 0;
}
