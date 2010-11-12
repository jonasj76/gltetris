#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

/* Game parameters */
#define BLOCK_SIZE       20
#define WIN_BLOCK_WIDTH  10
#define WIN_BLOCK_HEIGHT 20

/* Window parameters */
#define WIN_WIDTH      WIN_BLOCK_WIDTH  * BLOCK_SIZE
#define WIN_HEIGHT     WIN_BLOCK_HEIGHT * BLOCK_SIZE
#define WIN_DEPTH_BITS 8

void draw_block (int x, int y)
{
   x = BLOCK_SIZE * x;
   y = BLOCK_SIZE * y;

   glBegin (GL_QUADS);
   glVertex2f (x,              y);
   glVertex2f (x + BLOCK_SIZE, y);
   glVertex2f (x + BLOCK_SIZE, y + BLOCK_SIZE);
   glVertex2f (x,              y + BLOCK_SIZE);
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

void init_scene (void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (0.0, WIN_WIDTH, 0.0, WIN_HEIGHT, -1.0, 1.0);
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
   if (!glfwOpenWindow (WIN_WIDTH, WIN_HEIGHT, 0, 0, 0, 0, WIN_DEPTH_BITS, 0, GLFW_WINDOW))
   {
      printf ("error: glfwOpenWindow() failed.\n");
      exit (EXIT_FAILURE);
   }

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
