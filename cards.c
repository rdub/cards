#include <stdlib.h>	// strtol
#include <stdio.h>	// printf
#include "cards.h"

/*
 * init_card() sets a given card to the given value, and clears the @next pointer.
 */
static void init_card(card_t card, card_value_t value) {	
	card->value = value;
	card->next = NULL;
}

/*
 * is_in_order() returns 1 if the given cards are back in their original order.
 */
static int is_in_order(card_t cards, card_value_t count) {
	card_value_t ct = 0U;
	card_t head = cards;
	
	/*
	 * In our data structure, the card having the same value as its index constitutes it being in order.
	 */
	for(ct = 0U; head && ct < count; ct++) {
		if(head->value != ct) {
			return 0;
		}
		// We can't simply index, because we'd be smashing memory. Iterate the linked list properly.
		head = head->next;
	}
	
	return 1;
}

/*
 * enqueue_tail() takes a list and element as arguments.  It places the element in the tail of the list.
 * the @list param is unmodified, and is only passed in value-return form for consistency with the rest of the
 * queue API.
 * @param list is assumed to be non-NULL.
 */
static void enqueue_tail(card_t *list, card_t element) {
	card_t head = (*list);
	
	while(head->next) {
		head = head->next;
	}
	
	head->next = element;
}

/*
 * enqueue_head() takes a list and element as arguments. It places the element in the head of the list.
 * the @list param is then adjusted to reflect the new head of the list.
 * @param list is assumed to be non-NULL
 */
static void enqueue_head(card_t *list, card_t element) {
	element->next = (*list);
	(*list) = element;
}

/*
 * dequeue_head() takes a list and value-return element pointer as arguments. It removes the head of the list,
 * adjusts the @list parameter to point to the next item, and returns the 'pop'ed item in the @popped_element param.
 * @param list Pointer to the list head - will be altered by this method (value-return).
 * @param popped_element storage for the pointer to the popped element.
 */
static void dequeue_head(card_t *list, card_t *popped_element) {
	// take head of the queue
	card_t tmp = (*list);
	
	// move the queue past this head element (from the caller's perspective)
	*list = (*list)->next;
	
	// unlink the returned element
	tmp->next = NULL;
	
	// and return-by-value
	*popped_element = tmp;
}

/*
 * do_round() performs one complete round of the algorithm specified.  
 * @param hand_ptr Value-return pointer to the head of the list.
 * @param table_ptr Storage for a temporary pointer (representing the table's stack).
 */
static void do_round(card_t *hand_ptr, card_t *table_ptr) {
	card_t hand = (*hand_ptr), table = (*table_ptr);
	
	while(hand) {
		card_t tmp = NULL;
		
		// move one card from hand to table
		dequeue_head(&hand, &tmp);
		// push card onto table's stack
		enqueue_head(&table, tmp);
		
		if(!hand) break;	// ran out of cards?
		
		// move second card from top of hand queue to bottom of hand queue
		dequeue_head(&hand, &tmp);
		
		if(!hand) {
			// this is the last card, place it on top of the table stack this time
			enqueue_head(&table, tmp);
		} else {
			// we have other cards, still.  tack this card onto the tail of our hand queue
			enqueue_tail(&hand, tmp);
		}
	}
	
	// switch the hand and the table pointers for our caller.
	*hand_ptr = table;
	*table_ptr = NULL;
}

/*
 * rounds_to_loop() performs the algorithm start-to-finish. It allocates memory, runs the iterations, and returns the results.
 * @param cardCount Count of cards in initial deck.
 */
uint32_t rounds_to_loop(card_value_t cardCount) {
	uint32_t ct = 0UL;
	
	// Allocate some memory for our cards
	card_t cards = (card_t)malloc(sizeof(struct card) * cardCount);
	if(!cards) {
		// return our error code if it didn't work.
		return -1UL;
	}
	
	// initialize the cards
	for(ct = 0UL; ct < cardCount; ct++) {
		init_card(&(cards[ct]), ct);
		
		if(ct) {
			// link them up in correct order
			cards[ct-1].next = &(cards[ct]);
		}
	}
	
	// @hand points to the top of the "in-hand" deck
	card_t hand = cards;
	// @desk points to the top of the "on-the-table" deck (empty at first)
	card_t desk = NULL;
	
	// Iterate over the algorithm
	for(ct = 0UL; ; ct++) {
		// Perform a round of the task
		do_round(&hand, &desk);
		
		// Check if the result is in order.
		if(is_in_order(hand, cardCount)) { 
			break;
		}
	}
	
	// Returning count would return one too few (due to the break statement in the loop), so increment by 1.
	return ct + 1;
}

/*
 * main() - takes the card count to use as it's sole argument.
 * ex: './cards 3' would print out how many rounds it takes for a 3 card deck.
 */
int main(int argc, char *argv[]) {
	// sanity check
	if(argc > 1) {
		// convert the passed string to a value, squash it to card_value_t size
		unsigned long ct = (unsigned long)strtol(argv[1], NULL, 10);
		if(ct > MAX_CARD_COUNT) {
			printf("Too many cards. Max is %lu.\n", (long)MAX_CARD_COUNT);
			return -1;
		}
		
		card_value_t count = (card_value_t)ct;
		
		if(count <= 1) {
			printf("%u cards takes %u rounds to get back in order.\n", count, count);
			return count;	// defined by edge cases in documentation (see cards.h)
		}
		
		int rounds = (int)rounds_to_loop(count);
		
		if(-1UL != rounds) {
			printf("%u cards takes %u rounds to get back in order.\n", count, rounds);
		} else {
			printf("error! Probably out of memory.\n");
		}
		
		return rounds;
		
	}
	
	// Indicate error condition, for no arguments
	return -1;
}