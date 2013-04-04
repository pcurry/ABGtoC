/* Filename: blackjack.c

This program plays a game of Blackjack with you.  The computer is the dealer
and you are the victim-er, I mean, player.  The dealer gets a card that the
player can see.  The dealer then asks the player if they want another card
by asking "Hit" or "Stand."  If the player chooses to hit, the dealer deals
the player another card. If the player chooses to stand, the dealer draws
or stands, and the game is played out according to the cards the player
and the dealer have.  As with casino rules Blackjack, the dealer stands on 17.
The winner is annoucnced only after both the player's and the dealer's
hands are finished.  */

/************************************************************
ANSI C standard header files appear next */
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>

/*************************************************************
Defined constants appear next */
#define BELL '\a'
#define DEALER 0
#define PLAYER 1
#define SCREENLINECOUNT 25

/* Must keep two sets of totals for dealer and for player.  The first set
counts Aces as 1 and the second counts Aces as 11.  

FIXME:  Unlike "real world" Blackjack, this program doesn't allow some 
Aces in a hand to be 1 while other Aces in that hand are counted as 11. */
#define ACELOW 0
#define ACEHIGH 1

/* Only one global variable is used in this entire program.  That variable 
holds 0, which means false, initially.  Once the user enters his or her name 
in initCardsScreen(), this variable is set to 1 (for true), so the name is 
never asked for again for the rest of the program. */
int askedForName = 0; /* False initially */

/************************************************************
This program's specific prototypes. */
void dispTitle(void);
void initCardsScreen(int cards[52], int playerPoints[2], int dealerPoints[2], 
		     int total[2], int * numbCards);
int dealCard(int * numCards, int cards[52]);
void dispCard(int cardDrawn, int points[2]);
void totalIt(int points[2], int total[2], int who);
void dealerGetsCard(int *numCards, int cards[52], int dealerPoints[2]);
void playerGetsCard(int *numCards, int cards[52], int playerPoints[2]);
char getAns(char mesg[]);
void findWinner(int total[2]);

/************************************************************
C's program execution always begins here, at main(). */
main() {
  int numCards; /* Equals 52 at the beginning of each game. */
  int cards[52], playerPoints[2], dealerPoints[2], total[2];
  char ans; /* for user's Hit/Stand or Yes/No response. */
  do {
    initCardsScreen(cards, playerPoints, dealerPoints, total, &numCards);
    dealerGetsCard(&numCards, cards, dealerPoints);
    printf("\n");  /*  Prints a blank line */
    playerGetsCard(&numCards, cards, playerPoints);
    playerGetsCard(&numCards, cards, playerPoints);
    do {
      ans = getAns("Hit or stand (H/S)? ");
      if (ans == 'H') {
	playerGetsCard(&numCards, cards, playerPoints);
      }
    } while (ans != 'S');
    /* Generate player's total score once the player stands. */
    totalIt(playerPoints, total, PLAYER);
    do {
      dealerGetsCard(&numCards, cards, dealerPoints);
    } while (dealerPoints[ACEHIGH] < 17);
    /* Dealer has to stand at 17. */
    totalIt(dealerPoints, total, DEALER);
    /* Calculate the dealer's hand total. */
    findWinner(total);
    ans = getAns("\nPlay again (Y/N)? ");
  } while (ans == 'Y');
  return;
}

/************************************************************
This function initializes the face values of the deck of cards by putting 
four sets of 1-13 in the 52-card array.  Also clears the screen and displays
a title.  */
void initCardsScreen(int cards[52], int playerPoints[2], int dealerPoints[2], 
		     int total[2], int *numCards) {
  int sub, val = 1; /* This function's Work variables */
  char firstName[15]; /* Holds the user's first name. */
  *numCards = 52;  /* Holds running total of number of cards */
  for (sub = 0; sub <= 51; sub++) {
    val = (val == 14) ? 1 : val; /* When val hits 14, reset to 1. */
    cards[sub] = val;
    val++;
  }
  for (sub = 0; sub <= 1; sub++) {
    /* Reset all the points counters. */
    playerPoints[sub]=dealerPoints[sub]=total[sub]=0;
  }
  dispTitle();
  /* Only ask for the player's name once per session. */
  if (askedForName == 0) { 
    printf("\nWhat is your first name? ");
    scanf(" %s", firstName);  /* Buffer overflow attack possible here. */
    askedForName = 1; /* Only ask once. */
    printf("Ok, %s, get ready for casino action!\n\n", firstName);
    getchar(); /* Discards a newline. Text claims one can ignore the */
               /* compiler warning generated.  We'll see. */
  }
  return;
}

/************************************************************
This function gets a card for the player and updates the player's points. */
void playerGetsCard(int *numCards, int cards[52], int playerPoints[2]) {
  int newCard;
  newCard = dealCard(numCards, cards);
  printf("You draw: ");
  dispCard(newCard, playerPoints);
}

/************************************************************
This function gets a card for the dealer and updates the dealer's points. */
void dealerGetsCard(int *numCards, int cards[52], int dealerPoints[2]) {
  int newCard;
  newCard = dealCard(numCards, cards);
  printf("The dealer draws: ");
  dispCard(newCard, dealerPoints);
}

/* FIXME: break common code into a helper function which is called with the 
 * FIXME: string as a parameter. char[] I guess. */

/************************************************************
This function gets a card from the deck and stores it in either the 
dealer's or the player's hold of cards. */
int dealCard(int * numCards, int cards[52]) {
  int cardDrawn, subDraw;
  time_t t; /* Gets time for a random value */
  srand(time(&t)); /* Seeds the random-number generator. */
  subDraw = (rand() % (*numCards));  /* From 0 to numCards */
  cardDrawn = cards[subDraw];
  cards[subDraw] = cards[*numCards -1];
  (*numCards)--;
  return cardDrawn;
}

/************************************************************
Displays the last-drawn card and updates points with it. */
void dispCard(int cardDrawn, int points[2]) {
  switch (cardDrawn) {
    /* FIXME: Helper function for the face cards. */
  case(11) : printf("%s\n", "Jack");
    points[ACELOW] += 10;
    points[ACEHIGH] += 10;
    break;
  case(12) : printf("%s\n", "Queen");
    points[ACELOW] += 10;
    points[ACEHIGH] += 10;
    break;
  case(13) : printf("%s\n", "King");
    points[ACELOW] += 10;
    points[ACEHIGH] += 10;
    break;
  default: points[ACELOW] += cardDrawn;
    if (cardDrawn == 1) {
      printf("%s\n", "Ace");
      points[ACEHIGH] += 11;
    }
    else {
      points[ACEHIGH] += cardDrawn;
      printf("%d\n", cardDrawn); 
    }
  }
  return;
}

/************************************************************
Figures the total for player or dealer to see who won.  This function
takes into account the facte that Ace is either 1 or 11. */
void totalIt(int points[2], int total[2], int who) {
  /* The following routine first looks to see if the total points counting
     Aces as 1 is equal to the total points counting Aces as 11.  If so, 
     or if the totaly points counting Aces as 11 is more than 21, the 
     program uses the total with Aces as 1 only. */
  if ((points[ACELOW] == points[ACEHIGH]) || (points[ACEHIGH] > 21)) {
    total[who] = points[ACELOW]; /* Treat all Aces as 1. */
  }
  else {
    total[who] = points[ACEHIGH]; /* Keeps all Aces as 11. */
  }
  
  if (who == PLAYER) { /* Determines the message printed. */
    printf("You have a total of %d\n\n", total[PLAYER]);
  }
  else {
    printf("The house stands with a total of %d\n\n", total[DEALER]);
  }
  return;
}

/************************************************************
Prints the winning player.  */
void findWinner(int total[2]) {
  if(total[DEALER] == 21) {
    printf("The house wins.\n");
    return;
  }
  if((total[DEALER] > 21) && (total[PLAYER] > 21)) {
    printf("Nobody wins.\n");
    return;
  }
  if((total[DEALER] >= total[PLAYER]) && (total[DEALER] < 21)) {
    printf("The house wins.\n");
    return;
  } 
  if((total[PLAYER] > 21) && (total[DEALER] < 21)) {
    printf("The house wins.\n");
    return;
  } 
  printf("%s%c", "You win!\n", BELL);
  return;
}

/************************************************************
Gets the user's uppercase, single-character response. */
char getAns(char mesg[]) {
  char ans;
  printf("%s", mesg); /* Prints the prompt message passed. */
  ans = getchar();
  getchar(); /* Discards newline. Claims we can ignore compiler warning here. */
  return toupper(ans);
}

/************************************************************
Clears everything off the screen. */
void dispTitle(void) {
  int i = 0;
  while (i < SCREENLINECOUNT) {
    printf("\n"); /* Clears screen by printing a number of blank lines. */
    i++;
  }
  printf("\n\nStep right up to the Blackjack tables*\n\n");
  return;
}
