#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#define spade 06
#define club 05
#define diamond 04
#define heart 03

typedef struct card_s {
	char suit;
	int face;
	struct card_s *next_card;
} card;

void print_deck(card *deck) {
	int i = 1;
	while (deck != NULL) {
		switch (deck->face) {
		case 1:
			printf("A");
			break;
		case 11:
			printf("J");
			break;
		case 12:
			printf("Q");
			break;
		case 13:
			printf("K");
			break;
		default:
			printf("%d", deck->face);
			break;
		}
		switch (deck->suit) {
		case 'S':
			printf("%c\t", spade);
			break;
		case 'C':
			printf("%c\t", club);
			break;
		case 'H':
			printf("%c\t", heart);
			break;
		case 'D':
			printf("%c\t", diamond);
			break;
		}

		if (i % 4 == 0) {
			printf("\n");
		}
		++i;
		deck = deck->next_card;
	}
	printf("\n");
}
void print_card(int face, char suit) {
	switch (face) {
	case 1:
		printf("A");
		break;
	case 11:
		printf("J");
		break;
	case 12:
		printf("Q");
		break;
	case 13:
		printf("K");
		break;
	default:
		printf("%d", face);
		break;
	}
	switch (suit) {
	case 'S':
		printf("%c\t", spade);
		break;
	case 'C':
		printf("%c\t", club);
		break;
	case 'H':
		printf("%c\t", heart);
		break;
	case 'D':
		printf("%c\t", diamond);
		break;
	}
	printf("\n");
}

//Begins game by assigning the first card in the deck to the top card
void assign_begin_card(card *headp, int *face, char *suit) {
	*face = headp->face;
	*suit = headp->suit;
	switch (headp->face) {
	case 1:
		printf("A");
		break;
	case 11:
		printf("J");
		break;
	case 12:
		printf("Q");
		break;
	case 13:
		printf("K");
		break;
	default:
		printf("%d", headp->face);
		break;
	}
	switch (headp->suit) {
	case 'S':
		printf("%c\t", spade);
		break;
	case 'C':
		printf("%c\t", club);
		break;
	case 'H':
		printf("%c\t", heart);
		break;
	case 'D':
		printf("%c\t", diamond);
		break;
	}
	printf("\n");
}

//Shuffles deck
void shuffle_deck(card **h, int shufflelevel) {
	shufflelevel = shufflelevel * 10;
	srand((int)time(0));
	card *card1, *card2, *tempCard;
	int i, j, k;
	int num;
	for (i = 0; i < shufflelevel; ++i) {
		num = (rand() % 52);

		card1 = *h;
		for (k = 0; k < i % 51; ++k) {
			card1 = card1->next_card;
		}

		card2 = *h;
		for (j = 0; j < num; ++j) {
			card2 = card2->next_card;
		}

		tempCard = (card*)malloc(sizeof(card));
		tempCard->face = card1->face;
		tempCard->suit = card1->suit;
		card1->face = card2->face;
		card1->suit = card2->suit;
		card2->face = tempCard->face;
		card2->suit = tempCard->suit;
	}
}

//Deletes only the first card in the deck
void delete_card(card **headdeck) {
	card *temp;
	temp = (card*)malloc(sizeof(card));
	*temp = **headdeck;
	*headdeck = (*headdeck)->next_card;
	free(temp);
}

//Deletes first card in the deck, and adds it to the input hand.
void draw_card(card **headhand, card **headdeck) {
	int face = (*headdeck)->face;
	char suit = (*headdeck)->suit;
	card *new_card = (card*)malloc(sizeof(card));
	new_card->face = face;
	new_card->suit = suit;
	new_card->next_card = *headhand;
	*headhand = new_card;
	card *temp;
	temp = (card*)malloc(sizeof(card));
	*temp = **headdeck;
	*headdeck = (*headdeck)->next_card;
	free(temp);
}

	

//Assings card in hand to card user wants to play
void select_playcard(int num, int *face, char *suit, card *headhand) {
	int i;
	for (i = 0; i < num; ++i) {
		headhand = headhand->next_card;
	}
	*face = headhand->face;
	*suit = headhand->suit;
}

//Deletes card from hand that comp/user plays
void delete_playcard(int cardnum, card **headhand, card **tailhand) {
	int i;
	card *targetp = *headhand;
	card **temp = headhand;
	for (i = 0; i < cardnum; ++i) {
		targetp = targetp->next_card;
	}
	while ((*temp) != targetp) {
		temp = &(*temp)->next_card;
	}
	if (targetp == *headhand) {
		*headhand = targetp->next_card;
	}
	if (targetp == *tailhand) {
		*tailhand = *temp;
	}
	*temp = targetp->next_card;
	free(targetp);
}

//Chooses a card from the computer hand that can be played
void comp_move(card *headhand, int *playcardface, char *playcardsuit, int topcardface, char topcardsuit, int *cardnum) {
	*cardnum = 0;	
	do{
		*playcardface = headhand->face;
		*playcardsuit = headhand->suit;
		headhand = headhand->next_card;
		*cardnum = *cardnum + 1;
	}while (*playcardsuit != topcardsuit && *playcardface != topcardface && *playcardface != 8);
}
//Checks if the input hand is empty, if so then the hand wins
int check_win(card *headhand) {
	if (headhand == NULL) {
		return 1;
	}
	return 0;
}

//Checks to see if hand has a card that can be played. If yes, then returns 0. If no, then returns 1
int check_playable_card(int counter, card *handhead, int topcardface, char topcardsuit) {
	card *pt = handhead;
	int i;

		for (i = 0; i < counter, pt != NULL; ++i) {
			if (pt->face == topcardface || pt->suit == topcardsuit || pt->face == 8) {
				return 0;
			}
			pt = pt->next_card;
		}
	return 1;
}

//If the deck runs out of cards, this function calculates the comp and user score and determines the winner
void calculate_winner(card *userhead, card *comphead) {
	int userscore = 0, compscore = 0;
	while (userhead != NULL) {
		userscore = userscore + userhead->face;
		userhead = userhead->next_card;
	}
	while (comphead != NULL) {
		compscore = compscore + comphead->face;
		comphead = comphead->next_card;
	}
	if (userscore > compscore) {
		printf("Computer wins :(\n");
	}
	else if (userscore == compscore) {
		printf("It's a tie!!!\n");
	}
	else {
		printf("You win!!! :)\n");
	}
	printf("User score: %d \nComputer score: %d\n", userscore, compscore);
}

//Checks to see if there are more cards that the user can play in the same turn
int check_moreplays(card *headhand, int topcardface) {
	int num = 0;
	while (headhand != NULL) {
		if (headhand->face == topcardface) {
			return 1;
		}
		headhand = headhand->next_card;
	}
	return 0;
}
