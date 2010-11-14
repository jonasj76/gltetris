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

typedef int shape_t[3][3];

shape_t shape1 =
{
   {0, 1, 0},
   {1, 1, 1},
   {0, 0, 0}
};
shape_t shape2 =
{
   {1, 1, 0},
   {0, 1, 1},
   {0, 0, 0}
};


void draw_block (int row, int col, struct color_t color)
{
   float x, y;

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

void draw_shape (shape_t shape, int row, int col, struct color_t color)
{
   int x, y;

   for (x=0; x<3; x++)
      for (y=0; y<3; y++)
      {
	 if (shape[y][x])
	 {
	    draw_block (row+x, col+y, color);
	 }
      }
}

void render_scene (void)
{
   int x, y;
   color_t red   = {1.0, 0.0, 0.0};
   color_t green = {0.0, 1.0, 0.0};
   color_t blue  = {0.0, 0.0, 1.0};

   glClear (GL_COLOR_BUFFER_BIT);
   for (x=0; x<BOARD_COLS; x++)
      for (y=0; y<BOARD_ROWS; y++)
      {
	 if ((x+y)%2)
	    draw_block (x, y, red);
      }
   draw_shape (shape1, 5,  5, green);
   draw_shape (shape2, 1, 10, blue);
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

   /* Main loop. Wait until ESC key was pressed or window was closed */
   while (GLFW_PRESS != glfwGetKey (GLFW_KEY_ESC) && glfwGetWindowParam (GLFW_OPENED))
   {
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
