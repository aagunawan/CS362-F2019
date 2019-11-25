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

    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
    }

    printf ("\nTESTING minion cardEffect() choice 1 :\n"); 
    int choice1 = 1; // player chooses +2 coins
    int choice2 = 0; 
    int handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    int currentCoins = G.coins; // store current number of coins
    
    playMinion(p, choice1, choice2, &G, handPos);

    printf("Assert that the number of coins should increase by 2: ");
    assertEqual(G.coins, currentCoins + 2); // the coins should increase by 2

    printf ("\nTESTING minion cardEffect() choice 2 :\n"); 
    choice1 = 0; // player chooses +2 coins
    choice2 = 1; 
    handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    G.handCount[p+1] = handCount;   
    memcpy(G.hand[p+1], coppers, sizeof(int) * 2); // set all the cards to copper

    playMinion(p, choice1, choice2, &G, handPos);
    printf("Assert that the hand of current player should increase to 4: ");
    assertEqual(G.handCount[p], 4); // the coins should increase by 2
     
    printf("Assert that the hand of the other player should not change: ");   
    assertEqual(G.handCount[p+1], handCount); // the coins should increase by 2

    printf ("\nTESTING minion cardEffect() choice 2 with another player also has at least 5 cards:\n"); 
    choice1 = 0; // player chooses +2 coins
    choice2 = 1; 
    handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    G.handCount[p+1] = handCount; 
    memcpy(G.hand[p+1], coppers, sizeof(int) * handCount); // set all the cards to 
    
    playMinion(p, choice1, choice2, &G, handPos);
    printf("Assert that the hand of current player should increase to 4: ");    
    assertEqual(G.handCount[p], 4); // the coins should increase by 2

    printf("Assert that the hand of the other player should increase to 4: ");    
    assertEqual(G.handCount[p+1], 4); // the coins should increase by 2

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

