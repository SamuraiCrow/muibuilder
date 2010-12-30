#include <exec/types.h>

typedef struct chainon1 
	{
	APTR element;
	int ident;
	struct chainon1 * next;
	} chainon;
typedef struct queue1 {
	chainon * head;
	chainon * tail;
	int nb_elements;
	} queue;

extern void add( queue*, APTR );
extern void delete_nth( queue*, int);
extern void delete_all( queue* );
extern APTR nth( queue*, int);
extern queue * create( void );
extern void exchange( queue *, int, int);
extern void up( queue*, int );
extern void down( queue* , int );
