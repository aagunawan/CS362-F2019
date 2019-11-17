/* -----------------------------------------------------------------------
 * Demonstration of how to write unit tests for dominion-base
 * Include the following lines in your makefile:
 *
 * testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
 *      gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)
 * -----------------------------------------------------------------------
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int assertEqual(int lhs, int rhs); 

int main() {
    int i, j, index, choice1, choice2;
    int seed = 1000;
    int numPlayer = 2;
    int p, handCount, handCountOther;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int iterCount = 100;

    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];

    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
    }

    int handPos;
    // handCount = maxHandCount;
    int tempCoins;
    index = 0;
    for (p = 0; p < numPlayer; p++){
        // for (handCount = 0; handCount <= maxHandCount; handCount++){
            for (j = 0; j < iterCount; j++){ // loop for estate number in supply
                choice1 = rand() % 2; // choice 1 is either 0 or 1
                choice2 = 1 - choice1; // ensure that only either choice 1 or 2 is set to 1. 
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                handPos = 0;
                handCount = rand() % 5 + 1; // handcount 0 - 5
                handCountOther = rand() % 5 + 1; 
 
                printf("p = %d, choice1 = %d, choice2 = %d, handCount = %d, handCountOther = %d\n", p, choice1, choice2, handCount, handCountOther );           
                initializeGame(numPlayer, k, seed, &G); // initialize a new game    

                G.whoseTurn = p;
                G.handCount[p] = handCount; // set the number of cards on hand
                memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to estate

                if (p == 0){
                    G.handCount[p+1] = handCountOther; // set the number of cards on hand
                    memcpy(G.hand[p+1], coppers, sizeof(int) * handCountOther); // set all the cards to estate  
                }
                else {
                    G.handCount[p-1] = handCountOther; // set the number of cards on hand
                    memcpy(G.hand[p-1], coppers, sizeof(int) * handCountOther); // set all the cards to estate  
                }
                

                // choice1 = 1; // gain estate
                G.coins = 0; 
                tempCoins = G.coins;
                minionCardEffect(&G, choice1, choice2, p, handPos);

                printf("Assert that the number of coins should increase to 2: ");
                assertEqual(G.coins, tempCoins + 2); // the coins should increase to 4 

                printf("Assert that the hand of current player should increase to 4: ");
                assertEqual(G.handCount[p], 4); // the coins should increase by 2
                 
                printf("Assert that the hand of the other player should not change: ");   
                assertEqual(G.handCount[p+1], handCount); // the coins should increase by 2

                printf("Assert that the hand of current player should increase to 4: ");    
                assertEqual(G.handCount[p], 4); // the coins should increase by 2

                printf("Assert that the hand of the other player should increase to 4: ");    
                assertEqual(G.handCount[p+1], 4); // the coins should increase by 2

                index++;
            }
        // }
    }
    return 0;
}

int assertEqual(int lhs, int rhs){
    if (lhs == rhs){
        printf("PASS\n");
        return 1;
    }
    printf("FAIL\n");
    return 0;
}

