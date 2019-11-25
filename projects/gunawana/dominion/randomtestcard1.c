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

int assertNotEqual(int lhs, int rhs); 

int main() {
    int i, j, choice1;
    srand(time(NULL));
    int seed = 1000;
    int numPlayer = 2;
    int p, handCount, index;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 5;
    int iterCount = 100;

    // arrays of all coppers, silvers, and golds
    int hands[MAX_HAND];


    for (i = 0; i < MAX_HAND; i++)
    {
        hands[i] = rand() % 4 + 1; // range 1 - 4 for estate, duchy, province, or copper

    }

    int handPos;
    p = 0;
    // handCount = maxHandCount;
    int tempCoins;
    index = 0;
    for (p = 0; p < numPlayer; p++){
        for (handCount = 0; handCount <= maxHandCount; handCount++){
            for (j = 0; j < iterCount; j++){ // loop for estate number in supply
                choice1 = rand() % 2; // choice 1 is either 0 or 1
                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                handPos = 0;
                G.supplyCount[estate] = rand() % 9; // estate supply # 0 - 8
                printf("choice1 = %d, estate supply count = %d\n", choice1, G.supplyCount[estate]);           
                initializeGame(numPlayer, k, seed, &G); // initialize a new game           
                G.handCount[p] = handCount; // set the number of cards on hand
                memcpy(G.hand[p], hands, sizeof(int) * handCount); // set all the cards to estate
                G.whoseTurn = p;

                // choice1 = 1; // gain estate
                G.coins = 0; 
                tempCoins = G.coins;
                playBaron(p, choice1, &G);
                printf("Assert that the number of coins should increase to 4: ");
                assertEqual(G.coins, tempCoins + 4); // the coins should increase to 4 

                // choice1 = 0; // discard estate
                // baronCardEffect(&G, choice1, p, handPos);
                printf("Assert that estate is obtained on player's hand: ");
                assertEqual(G.discard[p][G.discardCount[p]-1], estate); // player must gain an estate card on hand
                index++;
            }
        }
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

int assertNotEqual(int lhs, int rhs){
    if (lhs != rhs){
        printf("PASS\n");
        return 1;
    }
    printf("FAIL\n");
    return 0;
}

