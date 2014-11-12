#ifdef _WIN32
#include <windows.h>
#endif

#define GLEW_STATIC 1
#include "GPGPU.h"
#include "Fluid.h"

static int winWidth = 512;
static int winHeight = 512;
static int winId = -1;
static GLUI *glui;


GPGPU  *gpgpu;

static Slab Velocity, Density, Pressure, Temperature;
static Surface Divergence, Obstacles, HiresObstacles;

/*****************************************************************************
*****************************************************************************/
static void
leftButtonDownCB(void)
{
   gpgpu->restart();
}

/*****************************************************************************
*****************************************************************************/
static void
leftButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
middleButtonDownCB(void)
{
}


/*****************************************************************************
*****************************************************************************/
static void
middleButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
rightButtonDownCB(void)
{
}


/*****************************************************************************
*****************************************************************************/
static void
rightButtonUpCB(void)
{
}

/*****************************************************************************
*****************************************************************************/
static void
mouseCB(int button, int state, int x, int y)
{
   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
      leftButtonDownCB();
   else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
      leftButtonUpCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
      middleButtonDownCB();
   else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
      middleButtonUpCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
      rightButtonDownCB();
   else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
      rightButtonUpCB();
}


/*****************************************************************************
*****************************************************************************/
static void
motionCB(int x, int y)
{
}


/*****************************************************************************
*****************************************************************************/
void
reshapeCB(int width, int height)
{
    if (height == 0) height = 1;
    
    glViewport(0, 0, width, height);
    
	// Use orthographic projection
    glMatrixMode(GL_PROJECTION);    
    glLoadIdentity();               
    gluOrtho2D(-1, 1, -1, 1);       
    glMatrixMode(GL_MODELVIEW);     
    glLoadIdentity();   
}

/*****************************************************************************
*****************************************************************************/
void
keyboardCB(unsigned char key, int x, int y)
{
}

/*****************************************************************************
*****************************************************************************/
void
idleFunc()
{		
	glutPostRedisplay();
}

/*****************************************************************************
*****************************************************************************/
void
refreshCB()
{
	//for (int i = 0; i < 10; ++i) {
		// Update the cells' states
		gpgpu->update();  

		// Display the results
		gpgpu->display();
	//}
    glutSwapBuffers();

	glutPostRedisplay();
}

/*****************************************************************************
*****************************************************************************/
void initialize()
{
	// Initialize glew library
    glewInit();

    // Create the gpgpu object
    gpgpu = new GPGPU(512, 512);
}

/*****************************************************************************
*****************************************************************************/
void MakeGUI()
{
	glui = GLUI_Master.create_glui("GUI", 0, 0, 0);
	glui->add_statictext("GPGPU example");

	glui->set_main_gfx_window(winId);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idleFunc);
}

/*****************************************************************************
*****************************************************************************/
int
main(int argc, char *argv[])
{
	// init OpenGL/GLUT
	glutInit(&argc, argv);
	
	// create main window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(winWidth, winHeight);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	winId = glutCreateWindow("MyWindow");

	initialize();

	// setup callbacks
	glutDisplayFunc(refreshCB);
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(motionCB);

	// force initial matrix setup
	reshapeCB(winWidth, winHeight);

	// set modelview matrix stack to identity
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// make GLUI GUI
	MakeGUI();


	glutMainLoop();
	
	return (TRUE);
}
