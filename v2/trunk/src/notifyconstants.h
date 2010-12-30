#define TY_NOTHING      0 /* No extra argument to consider */
#define TY_WINOBJ       1 /* notification on an element of a window (so window is needed) */
			  /* used only with "Activate" because we need to know the window */
			  /* in which is the object                                       */
#define TY_FUNCTION     2 /* argument is a call hook */
#define TY_ID           3 /* argument is a constant identifier */
#define TY_VARIABLE     4 /* argument is a variable name */
#define TY_CONS_INT     5 /* argument is a constant integer value */
#define TY_CONS_STRING  6 /* argument is a constant string */
#define TY_CONS_CHAR    7 /* argument is a constant character */
#define TY_CONS_BOOL    8 /* argument is a constant Boolean */

#define Current_Max	24
