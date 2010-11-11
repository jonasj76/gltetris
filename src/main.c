#include <stdio.h>
#include <stdlib.h>
#include <GL/glfw.h>

#define WIDTH  640
#define HEIGHT 480

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
   if (!glfwOpenWindow (WIDTH, HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW))
   {
      printf ("error: glfwOpenWindow() failed.\n");
      exit (EXIT_FAILURE);
   }

   /* Main loop. Wait until ESC key was pressed or window was closed */
   while (GLFW_PRESS != glfwGetKey (GLFW_KEY_ESC) && glfwGetWindowParam (GLFW_OPENED))
   {
      glfwSwapBuffers();
   }

   /* Close window and and terminate */
   glfwTerminate();
   exit (EXIT_SUCCESS);
}

/**
* Local Variables:
* version-control: t
* c-file-style: "ellemtel"
* End:
*/
