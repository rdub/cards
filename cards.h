/*
 * cards.h - Header file for desk of cards simulation
 *
 * Ryan Du Bois - rdub@apple.com
 * 
 */

#ifndef _CARDS_H_
#define _CARDS_H_

#include <stdint.h>

/*
 Problem: You are given a deck containing N cards.  While holding the deck:

1. Take the top card off the deck and set it on the table
2. Take the next card off the top and put it on the bottom of the deck in your hand.
3. Continue steps 1 and 2 until all cards are on the table.  This is a round.
4. Pick up the deck from the table and repeat steps 1-3 until the deck is in the original order.

Part A
Write a program to determine how many rounds it will take to put a deck back into the original order.  This will involve creating a data structure to represent the order of the cards. Do not use an array. This program should be written in C only. It should take a number of cards in the deck as a command line argument and write the result to stdout.  Please ensure the program compiles and runs correctly (no pseudo-code).  This isn't a trick question; it should be fairly straightforward.

Part B
Please describe (in English, not code), how you would modify the code if you were writing this for a firmware project (i.e. implementing on a microcontroller).
*/

/* 
 * values limited to 256 max cards in deck
 * To adjust this limitation, simply change this type from uint8_t to uint16_t, for example.
 */
typedef uint8_t card_value_t;
#define MAX_CARD_COUNT ((1 << (sizeof(card_value_t)*8)) - 1)	

/*!struct card
 * @abstract Describes each card as an element of a linked list, with an inherent value.
 * @element value Value of the card
 * @element next Pointer to next card (in top-down order).
 */
typedef struct card {
	card_value_t value;		
	struct card *next;	// using a linked list to represent my deck.
} * card_t;

/*
 * @fn rounds_to_loop(uint8_t cardCount)
 * @abstract Calculate the number of rounds (described above) needed to loop a deck of cards back to original order.
 * @discussion Problem space is limited by the max value of the card_value_t type.
 * Edge cases: if @cardCount is given as 0, function will return 0 as the number of rounds. 
 * 		@cardCount 1 returns 1 round 
 * 		(we count placing this one card on the table and picking back up as 1 round).
 * @param cardCount Number of cards in the deck
 * @return Number of rounds needed to loop back to original deck order. -1UL is returned if the method
 * could not allocate enough memory to solve the given number of cards
 */
uint32_t rounds_to_loop(card_value_t cardCount);

#endif /* end of include guard: _CARDS_H_ */
