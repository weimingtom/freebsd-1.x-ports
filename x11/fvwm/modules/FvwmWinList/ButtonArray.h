/* FvwmWinList Module for Fvwm. 
 *
 *  Copyright 1994,  Mike Finger (mfinger@mermaid.micro.umn.edu or
 *                               Mike_Finger@atk.com)
 *
 * The functions in this header file that are the original work of Mike Finger.
 * 
 * No guarantees or warantees or anything are provided or implied in any way
 * whatsoever. Use this program at your own risk. Permission to use this
 * program for any purpose is given, as long as the copyright is kept intact.
 *
 *  Things to do:  Convert to C++  (In Progress)
 */

/* Struct definitions */
typedef struct button {
  char *title;
  int up,needsupdate,tw;
  struct button *next;
} Button;

typedef struct {
  int count;
  Button *head,*tail;
  int x,y,w,h;
} ButtonArray;

/* Function Prototypes */
int InitArray(ButtonArray *array,int x,int y,int w,int h);
void UpdateArray(ButtonArray *array,int x,int y,int w, int h);
int AddButton(ButtonArray *array, char *title, int up);
int UpdateButton(ButtonArray *array, int butnum, char *title, int up);
void RemoveButton(ButtonArray *array, int butnum);
Button *find_n(ButtonArray *array, int n);
void FreeButton(Button *ptr);
void FreeAllButtons(ButtonArray *array);
void DoButton(Button *ptr, int x, int y, int w, int h);
void DrawButtonArray(ButtonArray *array, int all);
void SwitchButton(ButtonArray *array,int butnum);
