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
    int i, j, index;
    int seed = 1000;
    int numPlayer = 2;
    int p, deckCount, discardCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int iterCount = 100;

    // arrays of all coppers, silvers, and golds
    int decks[MAX_HAND], discards[MAX_HAND];

    for (i = 0; i < MAX_HAND; i++)
    {
        decks[i] = rand() % 27;
        discards[i] = rand() % 27;
    }

    int handPos;
    int tributeRevealedCards[2];
    index = 0;
    for (p = 0; p < numPlayer; p++){
        // for (handCount = 0; handCount <= maxHandCount; handCount++){
            for (j = 0; j < iterCount; j++){ // loop for estate number in supply

                memset(&G, 23, sizeof(struct gameState));   // clear the game state
                handPos = 0;
                deckCount = rand() % 3; // handcount 0 - 2
                discardCount = rand() % 3; //  0 - 2
 
                printf("p = %d, deckCount = %d, discardCount = %d\n", p, deckCount, discardCount );           
                initializeGame(numPlayer, k, seed, &G); // initialize a new game    

                G.whoseTurn = p;
                G.deckCount[p] = deckCount; // set the number of cards on hand
                G.discardCount[p] = discardCount;
                memcpy(G.deck[p], decks, sizeof(int) * deckCount); // set all the cards to estate
                memcpy(G.discard[p], discards, sizeof(int) * discardCount); // set all the cards to estate
                if (p == 0){
                    G.discardCount[p+1] = discardCount;
                    G.deckCount[p+1] = deckCount;
                    memcpy(G.deck[p+1], decks, sizeof(int) * deckCount); // set all the cards to estate
                    memcpy(G.discard[p+1], discards, sizeof(int) * discardCount); // set all the cards to estate
                }
                else {
                    G.discardCount[p-1] = discardCount;
                    G.deckCount[p-1] = deckCount;
                    memcpy(G.deck[p-1], decks, sizeof(int) * deckCount); // set all the cards to estate
                    memcpy(G.discard[p-1], discards, sizeof(int) * discardCount); // set all the cards to estate
                }
                tributeRevealedCards[0] = -1;
                tributeRevealedCards[1] = -1;
                // choice1 = 1; // gain estate
                playTribute(&G, tributeRevealedCards, p, handPos);
                printf("Assert that the revealed card should be copper: ");
                assertEqual(tributeRevealedCards[0], copper);

                printf("Assert that only one card is revealed: ");
                assertEqual(tributeRevealedCards[1], -1);

                printf("Assert that the revealed card should be copper: ");
                assertEqual(tributeRevealedCards[0], copper);

                printf("Assert that the 2nd revealed card should be copper: ");
                assertEqual(tributeRevealedCards[1], copper);
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