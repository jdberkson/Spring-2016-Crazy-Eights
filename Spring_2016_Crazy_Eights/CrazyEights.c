#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "function.h"
#include <Windows.h>
#define spade 06
#define club 05
#define diamond 04
#define heart 03


int main(void) {
	int i, j;
	int topcardface, playcardface;
	char topcardsuit, playcardsuit;
	char name[20];
	int cardnum;
	int counterplaycomp;	
	char play = 'y';
	int shufflelevel;

	printf("\n");
	printf("\n");
	//DIRECTIONS
	printf(" LET'S PLAY CRAZY EIGHTS\n");
	printf(" Rules\n");
	printf(" 1. Play a card that has either a face or suit that matches the top card.\t");
	printf(" 2. Playing an 8 allows you to change the suit.\n");
	printf(" 3. Run out of cards before the computer does!\n");
	printf("\n");
	printf("\n");
	printf("What's your name?");
	fgets(name, 20, stdin);

	while (play == 'y') {
		
		//CREATES DECK
		card *headp = NULL, *temp, *tail = NULL;
		int counteruser = 8;
		int countercomp = 8;
		temp = (card*)malloc(sizeof(card));
		for (i = 1; i < 14; ++i) {
			for (j = 0; j < 4; ++j) {
				temp = (card*)malloc(sizeof(card));
				temp->face = i;
				switch (j) {
				case 0:
					temp->suit = 'S';
					break;
				case 1:
					temp->suit = 'C';
					break;
				case 2:
					temp->suit = 'H';
					break;
				case 3:
					temp->suit = 'D';
					break;
				}
				if (headp == NULL) {
					headp = temp;
				}
				else {
					tail->next_card = temp;

				}
				tail = temp;
				tail->next_card = NULL;
			}
		}

		//DECK SHUFFLING
		printf("What level of shuffling would you like?(1-10)");
		scanf("%d", &shufflelevel);
		shuffle_deck(&headp, shufflelevel);

		//HAND DEALING
		card *userhead = NULL, *usertail = NULL;
		card *comphead = NULL, *comptail = NULL;
		card *current;

		for (i = 0; i < 16; ++i) {
			current = (card*)malloc(sizeof(card));
			current->face = headp->face;
			current->suit = headp->suit;
			if (i % 2 == 0) {

				if (userhead == NULL) {
					userhead = current;
				}
				else {
					usertail->next_card = current;
				}
				usertail = current;
				usertail->next_card = NULL;
			}
			else if (i % 2 == 1) {
				if (comphead == NULL) {
					comphead = current;
				}
				else {
					comptail->next_card = current;
				}
				comptail = current;
				comptail->next_card = NULL;
			}
			current = headp->next_card;
			free(headp);
			headp = current;
		}

		printf("\n\nGame is Beginning!!!\n\n\n");
		printf("Top card is:\n");
		assign_begin_card(headp, &topcardface, &topcardsuit);
		delete_card(&headp);

		//GAME BEGINS
		while (headp != NULL && check_win(comphead) != 1 && check_win(userhead) != 1) {
		
			//CHECKS IF USER HAND HAS A PLAYABLE CARD
			while (check_playable_card(counteruser, userhead, topcardface, topcardsuit) != 0 && headp != NULL ) {
				printf("Drawing card...\n");
				draw_card(&userhead, &headp);
				counteruser++;
			}
			//CHECKS IF DECK IS EMPTY = GAME OVER
			if (headp == NULL) {
				printf("Deck ran out of cards\n");
				calculate_winner(userhead, comphead);
			}
			else {
				printf("Your hand:\n");
				print_deck(userhead);
				//USER TURN
				do {
					printf("Which card would you like to play?(1-%d)", counteruser);
					scanf("%d", &cardnum);
					if (cardnum <= counteruser && counteruser > 0) {
						select_playcard(cardnum - 1, &playcardface, &playcardsuit, userhead);
					}
				} while ((playcardsuit != topcardsuit && playcardface != topcardface && playcardface != 8));
				--counteruser;
				topcardface = playcardface;
				topcardsuit = playcardsuit;
				delete_playcard(cardnum - 1, &userhead, &usertail);

				//CHECKS IF USER CAN PLAY MORE CARDS OF THE SAME FACE
				if (check_moreplays(userhead, topcardface) == 1) {
					do {
						print_deck(userhead);
						printf("You have more cards that can be played(1-%d)", counteruser);
						scanf("%d", &cardnum);
						if (cardnum <= counteruser && counteruser > 0) {
							select_playcard(cardnum - 1, &playcardface, &playcardsuit, userhead);
						}
					} while (( playcardface != topcardface ));
					delete_playcard(cardnum - 1, &userhead, &usertail);
					--counteruser;
				}

				if (check_moreplays(userhead, topcardface) == 1) {
					do {
						print_deck(userhead);
						printf("You have more cards that can be played(1-%d)", counteruser);
						scanf("%d", &cardnum);
						if (cardnum <= counteruser && counteruser > 0) {
							select_playcard(cardnum - 1, &playcardface, &playcardsuit, userhead);
						}
					} while ((playcardface != topcardface));
					delete_playcard(cardnum - 1, &userhead, &usertail);
					--counteruser;
				}

				if (check_moreplays(userhead, topcardface) == 1) {
					do {
						print_deck(userhead);
						printf("You have more cards that can be played(1-%d)", counteruser);
						scanf("%d", &cardnum);
						if (cardnum <= counteruser && counteruser > 0) {
							select_playcard(cardnum - 1, &playcardface, &playcardsuit, userhead);
						}
					} while ((playcardface != topcardface));
					delete_playcard(cardnum - 1, &userhead, &usertail);
					--counteruser;
				}

				//CHECKS IF USER PLAYS AN 8 AND ASKS FOR THE NEXT SUIT
				if (playcardface == 8) {
					printf("You played an 8, what would you like the suit to be?(S/C/H/D)");
					scanf(" %c", &topcardsuit);
					topcardface = playcardface;
					switch (topcardsuit) {
					case 'S':
						printf("Next suit is %c\n", spade);
						break;
					case 'C':
						printf("Next suit is %c\n", club);
						break;
					case 'H':
						printf("Next suit is %c\n", heart);
						break;
					case 'D':
						printf("Next suit is %c\n", diamond);
						break;
					}

				}
				else {
					topcardface = playcardface;
					topcardsuit = playcardsuit;
					printf("\nNew top card is:");
					print_card(topcardface, topcardsuit);
					printf("\n");
				}

				//CHECKS IF THE USER IS OUT OF CARDS = USER WIN
				if (check_win(userhead) == 1) {
					printf("%s wins!!!!\n", name);
				}

				//IF NOT CONTINUE
				else {

					//CHECKS FOR PLAYABLE CARD IN COMP HAND
					while (check_playable_card(countercomp, comphead, topcardface, topcardsuit) != 0 && headp != NULL) {
						printf("Drawing card...\n");
						draw_card(&comphead, &headp);
						++countercomp;
					}
					//CHECKS IF DECK IS OUT OF CARDS

					if (headp == NULL) {
						printf("Deck ran out of cards\n");
						calculate_winner(userhead, comphead);
					}

					//IF NOT, COMP MAKES MOVE
					else {
						//COMPUTER CHOOSES PLAYABLE CARD
						comp_move(comphead, &playcardface, &playcardsuit, topcardface, topcardsuit, &counterplaycomp);
						topcardface = playcardface;
						delete_playcard(counterplaycomp - 1, &comphead, &comptail);
						--countercomp;
						
						//IF COMPUTER PLAYS AN 8, A RANDOM SUIT IS SELECTED
						if (playcardface == 8) {
							printf("Computer played an 8.\n");
							printf("Computer has %d cards left.\n\n", countercomp);
							int randsuit = (rand() % 4) + 1;
							topcardface = playcardface;
							switch (randsuit) {
							case 1:
								printf("Computer chose %c\n", spade);
								topcardsuit = 'S';
								break;
							case 2:
								printf("Computer chose %c\n", club);
								topcardsuit = 'C';
								break;
							case 3:
								printf("Computer chose %c\n", heart);
								topcardsuit = 'H';
								break;
							case 4:
								printf("Computer chose %c\n", diamond);
								topcardsuit = 'D';
								break;
							}
						}

						else {
							topcardface = playcardface;
							topcardsuit = playcardsuit;

							printf("Computer played ");
							print_card(topcardface, topcardsuit);
							printf("Computer has %d cards left.\n\n", countercomp);
							printf("\nNew top card is:");
							print_card(topcardface, topcardsuit);
						}
					}
				}
			}
		}

		//ASKS USER TO PLAY AGAIN
		printf("Would you like to play again?(y/n)");
		scanf(" %c", &play);
	}
}


