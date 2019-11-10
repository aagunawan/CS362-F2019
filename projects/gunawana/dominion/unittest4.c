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

    // choice for tribute test
    int handPos = 0;

    // arrays of all coppers, silvers, and golds
    int coppers[MAX_HAND];
    for (i = 0; i < MAX_HAND; i++)
    {
        coppers[i] = copper;
    }

    printf ("\nTESTING tribute cardEffect() when next player only has 1 card on deck:\n"); 

    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    handCount = maxHandCount;
    p = 0;
    G.handCount[p] = handCount;                 // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper
    G.discardCount[1] = 0; //discard Count of next player is set to 0
    G.deckCount[1] = 1; //deck count of next player is set to 1
    G.deck[1][G.deckCount[1]-1] = copper;
    int tributeRevealedCards[2] = {-1, -1};
    
    tributeCardEffect(&G, tributeRevealedCards, p, handPos);

    printf("Assert that the revealed card should be copper: ");
    assertEqual(tributeRevealedCards[0], copper);


    printf("Assert that only one card is revealed: ");
    assertEqual(tributeRevealedCards[1], -1);

    printf ("\nTESTING tribute cardEffect() when next player only has 1 card on discard:\n"); 
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    p = 0;
    handCount = maxHandCount;
    G.handCount[p] = handCount;                 // set the number of cards on hand
    G.discardCount[1] = 1; //discard Count of next player is set to 0
    G.deckCount[1] = 0; //deck count of next player is set to 1
    G.discard[1][G.discardCount[1]-1] = copper;
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -1;

    
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper

    tributeCardEffect(&G, tributeRevealedCards, p, handPos);

    printf("Assert that the revealed card should be copper: ");
    assertEqual(tributeRevealedCards[0], copper);

    printf("Assert that only one card is revealed: ");
    assertEqual(tributeRevealedCards[1], -1);

    printf ("\nTESTING tribute cardEffect() when next player has 2 cards on deck:\n"); 
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;                 // set the number of cards on hand
    G.discardCount[1] = 0; //discard Count of next player is set to 0
    G.deckCount[1] = 2; //deck count of next player is set to 1
    G.deck[1][0] = copper;
    G.deck[1][G.deckCount[1]-1] = copper;
    G.discard[1][G.discardCount[1]-1] = 0;
    p = 0;
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -1;

    
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper

    tributeCardEffect(&G, tributeRevealedCards, p, handPos);

    printf("Assert that the 1st revealed card should be copper: ");
    assertEqual(tributeRevealedCards[0], copper);

    printf("Assert that the 2nd revealed card should be copper: ");
    assertEqual(tributeRevealedCards[1], copper);

    printf ("\nTESTING tribute cardEffect() when next player has 2 cards on discard:\n"); 
    memset(&G, 23, sizeof(struct gameState));   // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game
    G.handCount[p] = handCount;  
    G.discardCount[1] = 2; //discard Count of next player is set to 0
    G.deckCount[1] = 0; //deck count of next player is set to 1
    G.deck[1][0] = copper;
    G.deck[1][G.deckCount[1]-1] = copper;
    G.discard[1][G.discardCount[1]-1] = copper;
    G.discard[1][0] = copper;
    p = 0;
    tributeRevealedCards[0] = -1;
    tributeRevealedCards[1] = -1;

                   // set the number of cards on hand
    memcpy(G.hand[p], coppers, sizeof(int) * handCount); // set all the cards to copper

    tributeCardEffect(&G, tributeRevealedCards, p, handPos);

    printf("Assert that the revealed card should be copper: ");
    assertEqual(tributeRevealedCards[0], copper);

    printf("Assert that the 2nd revealed card should be copper: ");
    assertEqual(tributeRevealedCards[1], copper);
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