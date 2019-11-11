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

    int cardEffectReturn; // integer to collect the return of card effect

    printf ("\nTESTING mine cardEffect() player discard copper and gain silver:\n"); 
    int choice1 = 1; // hand pos for treasure (copper) to trash
    int choice2 = silver; // gain a silver
    int handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper

    cardEffectReturn = mineCardEffect(&G, choice1, choice2, p, handPos);
    printf("Assert the function should return 0 since all choices are valid: ");
    assertEqual(cardEffectReturn, 0); // it should return 0 since choice is valid
    printf("Assert the player should gain a silver on their hand: ");

    assertEqual(G.hand[p][G.handCount[p]-1], silver); // player must gain a silver card

    printf ("\nTESTING mine cardEffect() player discard copper and gain gold:\n"); 
    choice1 = 1; // hand pos for treasure (copper) to trash
    choice2 = gold; // gain a silver
    handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper

    cardEffectReturn = mineCardEffect(&G, choice1, choice2, p, handPos);
    printf("Assert the function should return -1 since you cannot gain gold from discarding copper due to cost: ");
    assertEqual(cardEffectReturn, -1); // it should return 0 since choice is valid
    
    printf ("\nTESTING mine cardEffect() player discard copper and gain tribute:\n"); 
    choice1 = 1; // hand pos for treasure (copper) to trash
    choice2 = tribute; // gain a tribute
    handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper

    cardEffectReturn = mineCardEffect(&G, choice1, choice2, p, handPos);
    printf("Assert the function should return -1 since tribute should not be allowed to gain: ");
    assertEqual(cardEffectReturn, -1); // it should return 0 since choice is valid
    printf("Assert the player's last hand pos should still be copper (not tribute): ");
    assertEqual(G.hand[p][G.handCount[p]-1], copper); // player must gain a silver card

    printf ("\nTESTING mine cardEffect() player discard province and gain tribute:\n"); 
    choice1 = 1; // hand pos for province to trash
    choice2 = tribute; // gain a tribute
    handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper to start
    G.hand[p][1] = province; // set hand pos 1 to be province (to test discard)

    cardEffectReturn = mineCardEffect(&G, choice1, choice2, p, handPos);
    printf("Assert the function should return -1 since province should not be allowed to discard: ");
    assertEqual(cardEffectReturn, -1); // it should return -1 since choice to discard is invalid



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

