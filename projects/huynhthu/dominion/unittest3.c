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

    printf ("\nTESTING ambassador cardEffect() when player has 2 cards to reveal and return 3 copies (not enough card):\n"); 
    int choice1 = 2; // card to reveal and trash
    int choice2 = 3; // return 3 copy of choice1 card; testing the 1st if condition in line 1220
    int handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    // G.hand[0][2] = 5; // set hand for choice1 to be different than the choice1

    cardEffectReturn = playAmbassador(p, choice1, choice2, &G, handPos);
    printf("Assert that function return -1 as there is no bug: ");
    assertEqual(cardEffectReturn, -1); // it should return -1 if there is no bug


    printf ("\nTESTING ambassador cardEffect() when player has enough card to discard:\n"); 
    choice1 = 1; // card to reveal and trash
    choice2 = 2; // return 2 copy of choice1 card; testing the 1st if condition in line 1220
    handPos = 0;
    p = 0;
    handCount = maxHandCount;

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    cardEffectReturn = playAmbassador(p, choice1, choice2, &G, handPos);
    printf("Assert that function returns 0: ");
    assertEqual(cardEffectReturn, 0); // it should return 0 if there is no bug
    printf("Assert that handcount for player is 3 less then before playing the ambassador card: ");
    assertEqual(G.handCount[p], maxHandCount-3); // current player's handcount should now be 3 fewer than before 

    printf ("\nTESTING ambassador cardEffect() when player does not have enough card to discard:\n"); 
    choice1 = 1; // card to reveal and trash
    choice2 = 2; // return 2 copy of choice1 card; testing the 1st if condition in line 1220
    handPos = 0;
    p = 0;
    handCount = 1; // hand count only 1; not enough cards to discard

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    cardEffectReturn = playAmbassador(p, choice1, choice2, &G, handPos);
    printf("Assert that function returns -1 as not enough cards to discard: ");
    assertEqual(cardEffectReturn, -1); // it should return -1 to pass
    printf("Assert that the handcount of player should be same as before playing: ");
    assertEqual(G.handCount[p], handCount); // current player's handcount should not decrease as no play can be done

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

