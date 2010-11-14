#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

/* Game parameters */
#define INIT_BLOCK_SIZE  20
#define WIN_BLOCK_WIDTH  10
#define WIN_BLOCK_HEIGHT 20

/* Window parameters */
#define INIT_WIN_WIDTH   WIN_BLOCK_WIDTH  * INIT_BLOCK_SIZE
#define INIT_WIN_HEIGHT  WIN_BLOCK_HEIGHT * INIT_BLOCK_SIZE
#define WIN_DEPTH_BITS 8

int block_size_x = INIT_BLOCK_SIZE;
int block_size_y = INIT_BLOCK_SIZE;

void draw_block (int x, int y)
{
   x = block_size_x * x;
   y = block_size_y * y;

   glBegin (GL_QUADS);
   glVertex2f (x,                y);
   glVertex2f (x + block_size_x, y);
   glVertex2f (x + block_size_x, y + block_size_y);
   glVertex2f (x,                y + block_size_y);
   glEnd ();
}

void render_scene (void)
{
   int x, y;

   glClear (GL_COLOR_BUFFER_BIT);
   for (x=0; x<WIN_BLOCK_WIDTH; x++)
      for (y=0; y<WIN_BLOCK_HEIGHT; y++)
      {
	 if ((x+y)%2)
	    draw_block (x, y);
      }
}

void GLFWCALL resize_window( int width, int height )
{
   block_size_x = width  / WIN_BLOCK_WIDTH;
   block_size_y = height / WIN_BLOCK_HEIGHT;

   glViewport (0, 0, width, height);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0.0, width, 0.0, height, -1.0, 1.0);
}

void init_scene (void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glfwSetWindowSizeCallback (resize_window);

   resize_window (INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
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

   /* Open an OpenGL window */
   if (!glfwOpenWindow (INIT_WIN_WIDTH, INIT_WIN_HEIGHT, 0, 0, 0, 0, WIN_DEPTH_BITS, 0, GLFW_WINDOW))
   {
      printf ("error: glfwOpenWindow() failed.\n");
      exit (EXIT_FAILURE);
   }

   /* Initialize render scene */
   init_scene ();

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
