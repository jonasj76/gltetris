/* Copyright (c) 2010, Jonas Johansson <jonasj76@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

/* Game parameters */
#define BOARD_COLS 10
#define BOARD_ROWS 20

/* Init parameters */
#define INIT_WIN_WIDTH  200
#define INIT_WIN_HEIGHT 400

/* Window parameters */
#define WIN_DEPTH_BITS 8

int block_size_x;
int block_size_y;

typedef struct color_t
{
   float r;
   float g;
   float b;
} color_t;

typedef int shape_t[4];

/* A shape consists of a block at the center and 3 other
 * blocks at the coodinates specified in the space_t type.
 * First parameter specifies the array position for the
 * shape rotated.
 */
shape_t shapes[] =
{
   /* basic shapes */
   /*  0 */  {  0, -BOARD_COLS - 1, -BOARD_COLS,     -1             }, /* square */
   /*  1 */  {  7, -1,              +1,              +2             }, /* horizintal line */
   /*  2 */  {  8, -BOARD_COLS,      BOARD_COLS,      BOARD_COLS + 1}, /* L */
   /*  3 */  { 11, -BOARD_COLS,      BOARD_COLS,      BOARD_COLS - 1}, /* flipped L */
   /*  4 */  { 14, -BOARD_COLS,     -1,               +1            }, /* hat */
   /*  5 */  { 17, -BOARD_COLS,     +1,               BOARD_COLS + 1}, /* flash going right */
   /*  6 */  { 18, -BOARD_COLS,     -1,               BOARD_COLS - 1}, /* flash going left */

   /* rotated shapes */
   /*  7 */  {  1, -BOARD_COLS,      BOARD_COLS,      BOARD_COLS * 2}, /* verical line */
   /*  8 */  {  9, -1,              +1,               BOARD_COLS - 1}, /* resting L pointing down at left */
   /*  9 */  { 10, -BOARD_COLS - 1, -BOARD_COLS,      BOARD_COLS    }, /* up-side-down flipped L */
   /* 10 */  {  2, -1,              +1,              -BOARD_COLS + 1}, /* resting L pointing up at right */
   /* 11 */  { 12, -BOARD_COLS - 1, -1,              +1             }, /* resting L pointing up at left */
   /* 12 */  { 13, -BOARD_COLS,     -BOARD_COLS + 1,  BOARD_COLS    }, /* up-side-down L */
   /* 13 */  {  3, -1,              +1,               BOARD_COLS + 1}, /* resting L pointing down at right */
   /* 14 */  { 15, -BOARD_COLS,     +1,               BOARD_COLS    }, /* tilted hat pointing right */
   /* 15 */  { 16, -1,              +1,               BOARD_COLS    }, /* up-side-down hat */
   /* 16 */  {  4, -BOARD_COLS,     -1,               BOARD_COLS    }, /* tilted hat pointing left */
   /* 17 */  { 5, -BOARD_COLS,     -BOARD_COLS + 1,  -1            }, /* tilted flash going left */
   /* 18 */  { 6, -BOARD_COLS - 1, -BOARD_COLS,      +1            }  /* tilted flash going right */
};

int num_shapes = 7;
int sel_shape  = 0;
int cur_shape  = 0;

void draw_block (int pos, struct color_t color)
{
   float x, y;
   int row,col;

   /* Calculate row and columnl */
   row = pos % BOARD_COLS;
   col = pos / BOARD_COLS;
   /* Set starting coordinates */
   x = block_size_x * row;
   y = block_size_y * col;

   glColor3f (color.r, color.g, color.b);
   glBegin (GL_QUADS);
   glVertex2f (x,                y);
   glVertex2f (x + block_size_x, y);
   glVertex2f (x + block_size_x, y + block_size_y);
   glVertex2f (x,                y + block_size_y);
   glEnd ();
}

void draw_shape (shape_t shape, int col, int row, struct color_t color)
{
   int pos;

   pos = col + row * BOARD_COLS;
   draw_block (pos,            color);
   draw_block (pos + shape[1], color);
   draw_block (pos + shape[2], color);
   draw_block (pos + shape[3], color);
}

void render_scene (void)
{
   int x, y;
   color_t red   = {1.0, 0.0, 0.0};
   color_t green = {0.0, 1.0, 0.0};

   glClear (GL_COLOR_BUFFER_BIT);
   for (x=0; x<BOARD_COLS; x++)
      for (y=0; y<BOARD_ROWS; y++)
      {
	 if ((x+y)%2)
	    draw_block (x + y*BOARD_COLS, red);
      }

   draw_shape (shapes[cur_shape], 5, 5, green);
}

void GLFWCALL resize_window (int width, int height)
{
   /* Calculate block x- and y-size */
   block_size_x = width  / BOARD_COLS;
   block_size_y = height / BOARD_ROWS;

   /* Recalculate window size to match block size multiple (fix rounding problem) */
   width  = block_size_x * BOARD_COLS;
   height = block_size_y * BOARD_ROWS;

   /* Set new window size */
   glfwSetWindowSize (width, height);

   /* Setup new viewport */
   glViewport (0, 0, width, height);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   /* Origin at top-left */
   glOrtho (0.0, width, height, 0.0, -1.0, 1.0);
}

void init (void)
{
   int width  = INIT_WIN_WIDTH;
   int height = INIT_WIN_HEIGHT;

   /* Open an OpenGL window */
   if (!glfwOpenWindow (width, height, 0, 0, 0, 0, WIN_DEPTH_BITS, 0, GLFW_WINDOW))
   {
      printf ("error: glfwOpenWindow() failed.\n");
      exit (EXIT_FAILURE);
   }

   /* Set window title */
   glfwSetWindowTitle ("glTetris");

   /* Select clearing (background) color */
   glClearColor (0.0, 0.0, 0.0, 0.0);

   /* Setup callback for window resizing */
   glfwSetWindowSizeCallback (resize_window);

   /* Setup window with init values */
   resize_window (width, height);
}

int main (void)
{
   int pressed = 0;

   /* Initalize GLFW */
   if (!glfwInit())
   {
      printf ("error: glfwInit() failed.\n");
      exit (EXIT_FAILURE);
   }

   /* Register exit function */
   if (atexit (glfwTerminate))
   {
      printf ("error: atexit() failed.\n");
      exit (EXIT_FAILURE);
   }

   /* Initialize */
   init ();

   printf ("Press SPACE to cycle through shapes\n");
   printf ("Press 'r' to rotate shape\n");
   render_scene ();
   /* Main loop. Wait until ESC key was pressed or window was closed */
   while (GLFW_PRESS != glfwGetKey (GLFW_KEY_ESC) && glfwGetWindowParam (GLFW_OPENED))
   {
      if (GLFW_PRESS == glfwGetKey (GLFW_KEY_SPACE) && !pressed)
      {
	 pressed = 1;
	 if (++sel_shape >= num_shapes)
	    sel_shape = 0;
	 cur_shape = sel_shape;
	 printf("Selecting Next Shape (%d)\n", cur_shape);

      }
      if (GLFW_PRESS == glfwGetKey ('R') && !pressed)
      {
	 pressed = 1;
	 cur_shape = shapes[cur_shape][0];
	 printf("Rotating Shape\n");
      }
      if (GLFW_RELEASE == glfwGetKey (GLFW_KEY_SPACE) &&
	  GLFW_RELEASE == glfwGetKey ('R'))
      {
	 pressed = 0;
      }

      render_scene ();
      glfwSwapBuffers ();
   }

   /* Close window and and terminate */
   glfwTerminate ();
   exit (EXIT_SUCCESS);
}

/**
* Local Variables:
* version-control: t
* c-file-style: "ellemtel"
* End:
*/
