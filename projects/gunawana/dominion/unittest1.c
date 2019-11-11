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

// set NOISY_TEST to 0 to remove printfs from output
#define NOISY_TEST 1

int assertEqual(int lhs, int rhs); 

int assertNotEqual(int lhs, int rhs); 

int main() {
    int i;
    int seed = 1000;
    int numPlayer = 2;
    int p, handCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine
               , remodel, smithy, village, baron, great_hall};
    struct gameState G;
    int maxHandCount = 5;

    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    int estates[MAX_HAND];

    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
        estates[i] = estate;
    }

    printf ("\nTESTING baron cardEffect() player chooses to discard an estate :\n"); 
    int choice1 = 1; // card to reveal and trash
    int handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], estates, sizeof(int) * handCount); // set all the cards to estate
    G.coins = 0; 
    int tempCoins = G.coins;
    baronCardEffect(&G, choice1, p, handPos);
    printf("Assert that the number of coins should increase to 4: ");
    assertEqual(G.coins, tempCoins + 4); // the coins should increase to 4 

    printf ("\nTESTING baron cardEffect() player chooses to gain an estate :\n"); 
    choice1 = 0; // card to reveal and trash
    handPos = 0;
    p = 0;
    handCount = maxHandCount;
    G.supplyCount[estate] = 8; // initialize supply count for estate

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to estate
    G.supplyCount[estate] = 5; // setup a non-empty supply of estate

    baronCardEffect(&G, choice1, p, handPos);
    printf("Assert that estate is obtained on player's hand: ");
    assertEqual(G.discard[p][G.discardCount[p]-1], estate); // player must gain an estate card on hand

    printf ("\nTESTING baron cardEffect() player chooses to gain an estate but the estate supply count is 0 :\n"); 
    choice1 = 0; // card to reveal and trash
    handPos = 0;
    p = 0;
    handCount = maxHandCount;
    G.supplyCount[estate] = 0; // initialize supply count for estate

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to estate
    G.supplyCount[estate] = 5; // setup a non-empty supply of estate

    baronCardEffect(&G, choice1, p, handPos);
    printf("Assert that estate is NOT obtained on player's hand: ");
    assertNotEqual(G.discard[p][G.discardCount[p]-1], estate); // player must gain an estate card on hand


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

