// Copyright 2022 Raducanu George-Cristian 311CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct for card
typedef struct card{
    int val;
    struct card *next;
    struct card *prev;
}card;

// struct for deck
typedef struct deck{
    struct card *head;
    struct card *tail;
    int num;
    struct deck *next;
    struct deck *prev;
}deck;

// structure for dealer
typedef struct dealer{
    int num;
    struct deck *head, *tail;
}dealer;

// function for creating dealer
dealer *create_dealer()
{
	dealer *new;
	new = malloc(sizeof(dealer));
	new->num = 0;
	new->head = NULL;
	new->tail = NULL;
	return new;
}

// function for creating deck
deck *create_deck()
{
	deck *new;
	new = malloc(sizeof(deck));
	new->num = 0;
	new->head = NULL;
	new->tail = NULL;
	new->prev = NULL;
	new->next = NULL;
	return new;
}

// function for creating a card
card *create_card(int value)
{
	card *new;
	new = malloc(sizeof(card));
	new->val = value;
	new->prev = NULL;
	new->next = NULL;
	return new;
}

// function for adding last card of value value to deck q
void add_last_card(deck *q, int value)
{
    card *new = create_card(value);
    if (!q->num) {
        q->tail = new;
        q->head = new;
    } else {
        new->prev = q->tail;
		q->tail->next = new;
        q->tail = new;
    }
	q->num++;
}

// function for adding last deck to dealer
void add_last_deck(dealer *s, deck *d)
{
    if (!s->num) {
        s->tail = d;
        s->head = d;
    } else {
        d->prev = s->tail;
		s->tail->next = d;
        s->tail = d;
    }
	s->num++;
}

// function for counting number of words in a line
int num_words(char s[])
{
	int words = 0, i;
	for (i = 0; s[i]; ++i) {
		if (s[i] == 32)
			words++;
	}
	if (i > 0)
		words++;
	return words;
}

// function for verifying the correctitude of the card
int verify_card(char s[], int *val, int *symbol)
{
	int i, value, ok = -1;
	char type_cards[4][8] = {"HEART", "SPADE", "DIAMOND", "CLUB"};
	for(i = 0; i < 4; i++)
		if(!strcmp(strrchr(s, ' ') + 1, type_cards[i])) {
			ok = i;
		}
	if(num_words(s) != 2)
		return -1;
	if(ok < 0)
		return -1;

	strcpy(strchr(s, ' '), "");
	value = atoi(s);
	if(value < 1 || value > 14)
		return -1;
	*val = value;
	*symbol = ok;
	return 0;
}

// function for verifing that command is correct
int verif_comm(char s[])
{
	char comm[14][14];
	int pos = -1;
	strcpy(comm[0], "ADD_DECK");
	strcpy(comm[1], "DEL_DECK");
	strcpy(comm[2], "DEL_CARD");
	strcpy(comm[3], "ADD_CARDS");
	strcpy(comm[4], "DECK_NUMBER");
	strcpy(comm[5], "DECK_LEN");
	strcpy(comm[6], "SHUFFLE_DECK");
	strcpy(comm[7], "MERGE_DECKS");
	strcpy(comm[8], "SPLIT_DECK");
	strcpy(comm[9], "REVERSE_DECK");
	strcpy(comm[10], "SHOW_DECK");
	strcpy(comm[11], "SHOW_ALL");
	strcpy(comm[12], "SORT_DECK");
	strcpy(comm[13], "EXIT");
	for (int i = 0; i < 14; i++)
		if (!(strstr(s, comm[i]) - s))
			pos = i;
	if ((pos == 4 || pos == 11 || pos == 13) && num_words(s) != 1)
			pos = -1;
	if ((!pos || pos == 1 || pos == 5 || pos == 6
		|| pos == 9 || pos == 10 || pos == 12) && num_words(s) != 2)
		pos = -1;
	if ((pos == 2 || pos == 3 || pos == 7 || pos == 8) &&num_words(s) != 3)
		pos = -1;
	return pos;
}

// function for adding to deck of num_cards
void add_deck(dealer *list, int num_cards)
{
    deck *new = create_deck();
	char comm[30];
    int value, symbol = -1;
    int i = 0;
    while (i < num_cards) {
		char *r = fgets(comm, 30, stdin);
		r = r;  // for error supresion
		comm[strlen(comm) - 1] = '\0';
		if(!verify_card(comm, &value, &symbol))
		{
			value = value + 20 * symbol;
            add_last_card(new, value);
            ++i;
		} else {
			printf("The provided card is not a valid one.\n");
		}
    }
	add_last_deck(list, new);
	int wasa = list->tail->num;
    printf("The deck was successfully created with %d cards.\n", wasa);
}

// function for showing deck of index index
void show_deck(dealer *list, int index)
{
    deck *t = list->head;
    deck *q = list->head;
    int count = 0;
    while (q != NULL) {
        count++;
        q = q->next;
    }
    list->num = count;
    if (index >= list->num || index < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    for (int i = 0; i < index; ++i)
        t = t->next;
    printf("Deck %d:\n", index);
    card *w = t->head;
    while (w != NULL) {
        printf("\t%d ", (w->val)%20);
        int d = (w->val)/20;
        if (d == 0)
            printf("HEART\n");
        if (d == 1)
            printf("SPADE\n");
        if (d == 2)
            printf("DIAMOND\n");
        if (d == 3)
            printf("CLUB\n");
        w = w->next;
    }
}

// function that shows all cards and decks
void show_all(dealer *list)
{
    deck *t = list->head;
    int count = 1;
	if (list->head == NULL) {
		return;
	}
    while (t->next != NULL) {
        count++;
        t = t->next;
    }
    list->num = count;
	deck *o = list->head;
    for (int index = 0; index < list->num; ++index) {
        printf("Deck %d:\n", index);
    	card *w = o->head;
    	while (w != NULL) {
        printf("\t%d ", (w->val)%20);
        int d = (w->val)/20;
        if (d == 0)
            printf("HEART\n");
        if (d == 1)
            printf("SPADE\n");
        if (d == 2)
            printf("DIAMOND\n");
        if (d == 3)
            printf("CLUB\n");
        w = w->next;
    }
		o = o->next;
    }
}

// function for freeing a deck
void free_deck(deck *t)
{
    card *aux, *p;
    if (t != NULL) {
        if (t->num) {
            aux = t->head;
            while (aux->next != NULL) {
                p = aux->next;
                free(aux);
                aux = p;
            }
            free(aux);
        }
		free(t);
    }
}

// function for freeing the dealer
void free_dealer(dealer *list)
{
	deck *aux, *p;
	if (list != NULL) {
        if (list->num) {
            aux = list->head;
            while (aux->next != NULL) {
                p = aux->next;
                free_deck(aux);
                aux = p;
            }
            free_deck(aux);
        }
		free(list);
    }
}

// function for adding cards to pack
void add_cards(dealer *list, int pack, int cards) {
    deck *t = list->head;
    if (pack >= list->num || pack < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    for (int i = 0; i < pack; ++i) {
        t = t->next;
    }
	char comm[30];
    int value, symbol = -1;
    int i = 0;
    while (i < cards) {
		char *r = fgets(comm, 30, stdin);
		r = r;  // for error supresion
		comm[strlen(comm) - 1] = '\0';
		if (!verify_card(comm, &value, &symbol))
		{
			value = value + 20 * symbol;
            add_last_card(t, value);
            ++i;
		} else {
			printf("The provided card is not a valid one.\n");
		}
    }
    printf("The cards were successfully added to deck %d.\n", pack);
}

// function for deck_number
void deck_number(dealer *list)
{
	deck *t = list->head;
    int count = 1;
	if (list->head == NULL) {
		list->num = 0;
		printf("The number of decks is %d.\n", list->num);
		return;
	}
    while (t->next != NULL) {
        count++;
        t = t->next;
    }
    list->num = count;
    printf("The number of decks is %d.\n", list->num);
}

// function for length of deck index
void deck_len(dealer *list, int index)
{
    deck *t = list->head;
    if (index >= list->num || index < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    for (int i = 0; i < index; ++i)
        t = t->next;
    printf("The length of deck %d is %d.\n", index, t->num);
}

// function for deletion of a deck
void del_deck(dealer *list, int pack)
{
    deck *del;
	if (list == NULL || pack >= list->num || pack < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
	if (list->num == 1 && pack == 0) {
		del = list->head;
		free_deck(del);
		list->num = 0;
		list->head = NULL;
		list->tail = NULL;
		printf("The deck %d was successfully deleted.\n", pack);
		return;
	}
	if(list->num == 1)
	{
		del = list->head;
		free_deck(del);
	}
	else
    if (!pack)
	{
		del = list->head;
		list->head = list->head->next;
		list->head->prev = NULL;
		del->next = NULL;
		free_deck(del);
	}
	else
	if(pack == (list->num - 1))
	{
		del = list->tail;
		list->tail = list->tail->prev;
		list->tail->next = NULL;
		del->prev = NULL;
		free_deck(del);
	}
	else
	{
		del = list->head;
		for (int i = 0; i < pack; ++i)
        	del = del->next;
		del->next->prev = del->prev;
		del->prev->next = del->next;
		del->next = NULL;
		del->prev = NULL;
		free_deck(del);
	}
    list->num--;
	printf("The deck %d was successfully deleted.\n", pack);
}

// function for deletion of deck for merge without messages
void del_deck_merge(dealer *list, int pack)
{
    deck *del;
	if (list == NULL || pack >= list->num || pack < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
	if(list->num == 1)
	{
		del = list->head;
		free_deck(del);
	}
	else
    if (!pack)
	{
		del = list->head;
		list->head = list->head->next;
		list->head->prev = NULL;
		del->next = NULL;
		free_deck(del);
	}
	else
	if(pack == (list->num - 1))
	{
		del = list->tail;
		list->tail = list->tail->prev;
		list->tail->next = NULL;
		del->prev = NULL;
		free_deck(del);
	}
	else
	{
		del = list->head;
		for (int i = 0; i < pack; ++i)
        	del = del->next;
		del->next->prev = del->prev;
		del->prev->next = del->next;
		del->next = NULL;
		del->prev = NULL;
		free_deck(del);
	}
    list->num--;
}

// function for deleting a card
void del_card(dealer *list, int pack, int i_card)
{
    if (list == NULL || pack >= list->num || pack < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    deck *a = list->head;
    for (int i = 0; i < pack; ++i)
        a = a->next;
    if (i_card  >= a->num || i_card < 0)  {
        printf("The provided index is out of bounds for deck %d.\n", pack);
		return;
    }
	if(a->num <= 1)
		del_deck_merge(list, pack);
	else
	{
		card *del;
		if(!i_card)
		{
			del = a->head;
			a->head = a->head->next;
			a->head->prev = NULL;
			del->next = NULL;
			free(del);
		}
		else
		if(i_card == (a->num - 1))
		{
			del = a->tail;
			a->tail = a->tail->prev;
			a->tail->next = NULL;
			del->prev = NULL;
			free(del);
		}
		else
		{
			del = a->head;
			for (int i = 0; i < i_card; ++i) {
        	 	del = del->next;
			}
			del->next->prev = del->prev;
			del->prev->next = del->next;
			del->next = NULL;
			del->prev = NULL;
			free(del);
		}
		a->num--;
	}
    printf("The card was successfully deleted from deck %d.\n", pack);
}

// function for reversing a deck of index index
void reverse_deck(dealer *list, int index)
{
    if (index >= list->num || index < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    deck *t = list->head;
    for (int i = 0; i < index; ++i)
        t = t->next;
    card *temp = NULL;
    card *curr = t->head;
    while (curr != NULL) {
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev;
    }
    if (temp != NULL)
        t->head = temp->prev;
    card *l = t->head;
    while (l->next != NULL)
        l = l->next;
    t->tail = l;
    printf("The deck %d was successfully reversed.\n", index);
}

// function for merging two decks i1 and i2
void merge_decks(dealer *list, int i1, int i2)
{
    if (i1 >= list->num || i1 < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    if (i2 >= list->num || i2 < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    deck *t1 = list->head;
    for (int i = 0; i < i1; ++i)
        t1 = t1->next;
    deck *t2 = list->head;
    for (int i = 0; i < i2; ++i)
        t2 = t2->next;
    deck *w = create_deck();
    add_last_deck(list, w);
    card *a1 = t1->head;
    card *a2 = t2->head;
    while (a1 != NULL && a2 != NULL) {
        if (a1 != NULL) {
            add_last_card(list->tail, a1->val);
            a1 = a1->next;
        }
        if (a2 != NULL) {
            add_last_card(list->tail, a2->val);
            a2 = a2->next;
        }
    }
	while (a1 != NULL) {
            add_last_card(list->tail, a1->val);
            a1 = a1->next;
    }
	while (a2 != NULL) {
            add_last_card(list->tail, a2->val);
            a2 = a2->next;
    }
    if (i1 < i2) {
    del_deck_merge(list, i1);
    del_deck_merge(list, i2 - 1);
    } else {
        del_deck_merge(list, i2);
        del_deck_merge(list, i1 - 1);
    }
    printf("The deck %d and the deck %d were successfully merged.\n", i1, i2);
}

// function for shuffling a deck of index index
void shuffle(deck *t, int index)
{
	if (t->head->next == NULL) {
		printf("The deck %d was successfully shuffled.\n", index);
		return;
	}
    card *a1 = t->head;
    int len = t->num;
    card *a3 = t->head;
    for (int i = 0; i < len/2; ++i)
        a3 = a3->next;
    card *a2 = a3->prev;
    card *a4 = t->tail;
    t->head = a3;
    a1->prev = a4;
    a4->next = a1;
    a3->prev = NULL;
    a2->next = NULL;
    t->tail = a2;
    printf("The deck %d was successfully shuffled.\n", index);
}

// the effective function for shuffle that is being called
void shuffle_deck(dealer *list, int i1)
{
    if (i1 >= list->num || i1 < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    deck *t = list->head;
    for (int i = 0; i < i1; ++i)
        t = t->next;
    shuffle(t, i1);
}

// function for spliting deck i_pack at i_split card
void split_deck(dealer *list, int i_pack, int i_split)
{
    if (i_pack >= list->num || i_pack < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    deck *t = list->head;
    for (int i = 0; i < i_pack; ++i)
        t = t->next;
    if (i_split >= t->num || i_split < 0) {
        printf("The provided index is out of bounds for deck %d.\n", i_pack);
        return;
    }
    if (i_split == 0) {
		printf("The deck %d was successfully split by index %d.\n", i_pack, i_split);
        return;
    }
    card *w = t->head;
    for (int i = 0; i < i_split; ++i)
        w = w->next;
    card *o = w->prev;
    o->next = NULL;
    w->prev = NULL;
    t->tail = o;
    deck *new = create_deck();
    if (t->next != NULL) {
        new->next = t->next;
        t->next->prev = new;
        t->next = new;
        new->prev = t;
    } else {
        add_last_deck(list, new);
    }
    new->head = w;
    card *curr = new->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    new->num = t->num - i_split;
    new->tail = curr;
    t->num = i_split;
    list->num++;
    printf("The deck %d was successfully ", i_pack);
	printf("split by index %d.\n", i_split);
}

// function for sort deck of index index
void sort_deck(dealer *list, int index)
{
    if (index >= list->num || index < 0) {
        printf("The provided index is out of bounds for the deck list.\n");
        return;
    }
    deck *t = list->head;
    for (int i = 0; i < index; ++i) {
        t = t->next;
    }
    int swaped;
    card *ptr;
    card *lptr = NULL;
    if (t->head == NULL)
        return;
    do {
        swaped = 0;
        ptr = t->head;
        while (ptr->next != lptr) {
            int v1 = ptr->val % 20;
            int v2 = ptr->next->val % 20;
            int sy1 = ptr->val / 20;
            int sy2 = ptr->next->val / 20;
            if (v1 > v2) {
                int aux = ptr->val;
                ptr->val = ptr->next->val;
                ptr->next->val = aux;
                swaped = 1;
            } else if (v1 == v2 && sy1 > sy2) {
                int aux = ptr->val;
                ptr->val = ptr->next->val;
                ptr->next->val = aux;
                swaped = 1;
            }
			ptr = ptr->next;
        }
		lptr = ptr;
    } while (swaped);
    printf("The deck %d was successfully sorted.\n", index);
    return;
}

// function that updates the count and tail and list at each turn
void get_count(dealer *list)
{
	int count = 0;
	deck *t = list->head;
	if (list == NULL)
		return;
	if (t == NULL)
		return;
	list->tail = t;
	while (t != NULL) {
		count++;
		list->tail = t;
		t = t->next;
	}
	list->tail->next = NULL;
	list->head->prev = NULL;
	list->num = count;
	deck *a = list->head;
	for (int i = 0; i < list->num; ++i) {
		card *x = a->head;
		if (x == NULL)
			return;
		int n = 0;
		a->tail = x;
		while (x != NULL) {
			n++;
			a->tail = x;
			x = x->next;
		}
		a->num = n;
		a->head->prev = NULL;
		a->tail->next = NULL;
		a = a->next;
	}
}

int main(void) {
    dealer *A = create_dealer();
    char comm[30];
    int ok = 0, number, pos, index, p1, p2;
    while (ok == 0) {
		char *r = fgets(comm, 30, stdin);
		r = r;  // for error supresion
		int c = verif_comm(comm);
		if (c < 0) {
			printf("Invalid command. Please try again.\n");
		} else {
			if(c == 0)
			{
				number = atoi(strrchr(comm, ' ') + 1);
				add_deck(A, number);
			}
			if(c == 1)
			{
				pos = atoi(strrchr(comm, ' ') + 1);
				del_deck(A, pos);
			}
			if(c == 2)
			{
				index = atoi(strrchr(comm, ' ') + 1);
				strcpy(strrchr(comm, ' '), "");
				pos = atoi(strrchr(comm, ' ') + 1);
				del_card(A, pos, index);
			}
			if(c == 3)
			{
				number = atoi(strrchr(comm, ' ') + 1);
				strcpy(strrchr(comm, ' '), "");
				pos = atoi(strrchr(comm, ' ') + 1);
				add_cards(A, pos, number);
			}
			if(c == 4)
				deck_number(A);
			if(c == 5)
			{
				pos = atoi(strrchr(comm, ' ') + 1);
				deck_len(A, pos);
			}
			if(c == 6)
			{
				pos = atoi(strrchr(comm, ' ') + 1);
				shuffle_deck(A, pos);
			}
			if(c == 7)
			{
				p2 = atoi(strrchr(comm, ' ') + 1);
				strcpy(strrchr(comm, ' '), "");
				p1 = atoi(strrchr(comm, ' ') + 1);
				merge_decks(A, p1, p2);
			}
			if(c == 8)
			{
				index = atoi(strrchr(comm, ' ') + 1);
				strcpy(strrchr(comm, ' '), "");
				pos = atoi(strrchr(comm, ' ') + 1);
				split_deck(A, pos, index);
			}
			if(c == 9)
			{
				pos = atoi(strrchr(comm, ' ') + 1);
				reverse_deck(A, pos);
			}
			if(c == 10)
			{
				pos = atoi(strrchr(comm, ' ') + 1);
				show_deck(A, pos);
			}
			if(c == 11)
				show_all(A);
			if(c == 12)
			{
				pos = atoi(strrchr(comm, ' ') + 1);
				sort_deck(A, pos);
			}
			if(c == 13)
				ok = 1;
			get_count(A);
		}
		fflush(stdout);
		fflush(stdin);
    }
	free_dealer(A);
    return 0;
}
