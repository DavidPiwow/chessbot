#ifndef MOVETREE
#define MOVETREE

#include "helper_functions.h"

struct _node {
	Coordinates* location;
	MoveChoices* choices;
	int rating;
	struct _node* next;
	struct _node* previous;
};

typedef struct _node MoveNode;


#endif