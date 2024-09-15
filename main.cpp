//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "glApplication.h"
#include "glutWindow.h"
#include <iostream>
#include "glsl.h"
#include <time.h>
#include "glm.h"
#include <FreeImage.h> //*** Para Textura: Incluir librería

using namespace std;
//-----------------------------------------------------------------------------


class myWindow : public cwc::glutWindow
{
protected:
   cwc::glShaderManager SM;
   //cwc::glShader* arregloTexturas[10];
   cwc::glShader *shader;
   cwc::glShader* shader1; //Para Textura: variable para abrir los shader de textura
   GLuint ProgramObject;
   clock_t time0,time1;
   float timer010;  // timer counting 0->1->0
   bool bUp;        // flag if counting up or down.
   GLMmodel* arregloModel[10];
   //GLMmodel* objmodel_ptr;
   //GLMmodel* objmodel_ptr1; //*** Para Textura: variable para objeto texturizado
   GLuint arregloTexturas[10];
   GLuint texid; //*** Para Textura: variable que almacena el identificador de textura


public:
	myWindow(){}

	// Método para cargar modelos 3D
	void cargarObjetos(std::string objeto, int posicion) {
		arregloModel[posicion] = NULL;

		if (!arregloModel[posicion])
		{
			std::string cadena = "./Mallas/" + objeto;
			char* cadena2 = new char[cadena.length() + 1];
			arregloModel[posicion] = glmReadOBJ(strcpy(cadena2, cadena.c_str()));
			if (!arregloModel[posicion])
				exit(0);

			glmUnitize(arregloModel[posicion]);
			glmFacetNormals(arregloModel[posicion]);
			glmVertexNormals(arregloModel[posicion], 90.0);
		}
	}

	// Método que abre la textura en JPG
	void initialize_textures(std::string textura, int posicion)
	{
		int w, h;
		GLubyte* data = 0;
		//data = glmReadPPM("soccer_ball_diffuse.ppm", &w, &h);
		//std::cout << "Read soccer_ball_diffuse.ppm, width = " << w << ", height = " << h << std::endl;

		//dib1 = loadImage("soccer_ball_diffuse.jpg"); //FreeImage


		glGenTextures(1, &arregloTexturas[posicion]);
		glBindTexture(GL_TEXTURE_2D, arregloTexturas[posicion]);
		glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		std::string cadena = "./Mallas/" + textura;
		char* cadena2 = new char[cadena.length() + 1];
		// Loading JPG file
		FIBITMAP* bitmap = FreeImage_Load(
			FreeImage_GetFileType(strcpy(cadena2, cadena.c_str()), 0),
			strcpy(cadena2, cadena.c_str()));  //*** Para Textura: esta es la ruta en donde se encuentra la textura

		FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
		int nWidth = FreeImage_GetWidth(pImage);
		int nHeight = FreeImage_GetHeight(pImage);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
			0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

		FreeImage_Unload(pImage);
		//
		glEnable(GL_TEXTURE_2D);
	}

	// Método para renderizar la escena
	virtual void OnRender(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
      timer010 = 0.005; //for screenshot!
	  glRotatef(timer010*360, 0, 0.01, 0);
	  glTranslatef(timer010 * 100, 0, 0);
      glPushMatrix();

	  // VISTAS 
	  // Vista de frente
	  glTranslatef(0, -5, -16);

	  // Vista desde arriba 
	  //glRotatef(90, 1, 0, 0);
	  //glTranslatef(0, -25, 0);

	  //Vista desde el lado izquierdo
	  //glRotatef(90, 0, 1, 0);
	  //glTranslatef(15, 0, 0);

	  // Vista desde el lado derecho
	  //glRotatef(90, 0, -1, 0);
	  //glTranslatef(-15, 0, 0);

	  if (shader) shader->begin();

		  // Niña
		  glPushMatrix();
			  glTranslatef(0, 0, 8);
			  glRotatef(180, 0, 1, 0);
			  glmDraw(arregloModel[2], GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();

		  // Árbol
		  glPushMatrix();
			  glTranslatef(0, 0, 3);
			  glScalef(5, 5, 5);
			  glmDraw(arregloModel[4], GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();

		  // Bus
		  glPushMatrix();
			  glTranslatef(10, 0, 3);
			  glScalef(5, 5, 5);
			  glmDraw(arregloModel[5], GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();

		  // Coliseo
		  glPushMatrix();
			  glTranslatef(17, 0, 3);
			  glScalef(3, 3, 3);
			  glmDraw(arregloModel[6], GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();

		  // Bandera
		  glPushMatrix();
			  glTranslatef(-17, 0, 7);
			  glScalef(3, 3, 3);
			  glmDraw(arregloModel[8], GLM_SMOOTH | GLM_MATERIAL);
		  glPopMatrix();


	  if (shader) shader->end();

	  //*** Para Textura: llamado al shader para objetos texturizados
	  if (shader1) shader1->begin();

	      // Suelo
		  glPushMatrix();
			  glTranslatef(0, -0.8, 8);
			  glScalef(25, -0.4, 30);
			  glBindTexture(GL_TEXTURE_2D, arregloTexturas[0]);
			  glmDraw(arregloModel[0], GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();

		  // Fuente
		  glPushMatrix();
			  glTranslatef(0.5, 2, 6.2);
			  glScalef(4, 4, 4);
			  glRotatef(0, 1, 0, 0);
			  glBindTexture(GL_TEXTURE_2D, arregloTexturas[1]);
			  glmDraw(arregloModel[1], GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();

		  // Trofeo
		  glPushMatrix();
			  glTranslatef(0, 0, 16);
			  glRotatef(0, 0, 0, 0);
			  glScalef(4, 4, 4);
			  glBindTexture(GL_TEXTURE_2D, arregloTexturas[2]);
			  glmDraw(arregloModel[3], GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();

		  // Balon
		  glPushMatrix();
			  glTranslatef(-12, 2, 6.2);
			  glScalef(2, 2, 2);
			  glRotatef(0, 1, 0, 0);
			  glBindTexture(GL_TEXTURE_2D, arregloTexturas[3]);
			  glmDraw(arregloModel[7], GLM_SMOOTH | GLM_MATERIAL | GLM_TEXTURE);
		  glPopMatrix();


	  if (shader1) shader1->end();


      glutSwapBuffers();
      glPopMatrix();

      UpdateTimer();

		Repaint();
	}

	virtual void OnIdle() {}

	// When OnInit is called, a render context (in this case GLUT-Window) 
	// is already available!
	virtual void OnInit()
	{
		glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);

		shader = SM.loadfromFile("vertexshader.txt","fragmentshader.txt"); // load (and compile, link) from file
		if (shader==0) 
         std::cout << "Error Loading, compiling or linking shader\n";
      else
      {
         ProgramObject = shader->GetProgramObject();
      }

	 //*** Para Textura: abre los shaders para texturas
		shader1 = SM.loadfromFile("vertexshaderT.txt", "fragmentshaderT.txt"); // load (and compile, link) from file
		if (shader1 == 0)
			std::cout << "Error Loading, compiling or linking shader\n";
		else
		{
			ProgramObject = shader1->GetProgramObject();
		}

      time0 = clock();
      timer010 = 0.0f;
      bUp = true;


	  //Abrir mallas

	  cargarObjetos("suelo.obj", 0);
	  cargarObjetos("fuente.obj", 1);
	  cargarObjetos("nina.obj", 2);
	  cargarObjetos("champions.obj", 3);
	  cargarObjetos("arbol.obj", 4);
	  cargarObjetos("bus.obj", 5);
	  cargarObjetos("coliseo.obj", 6);
	  cargarObjetos("bola.obj", 7);
	  cargarObjetos("objFlag.obj", 8);
	
	  //*** Para Textura: abrir archivo de textura
	  initialize_textures("suelo.jpg", 0);
	  initialize_textures("fuente.jpg", 1);
	  initialize_textures("metal.jpg", 2);
	  initialize_textures("bola.jpg", 3);
      DemoLight();

	}

	virtual void OnResize(int w, int h)
   {
      if(h == 0) h = 1;
	   float ratio = 1.0f * (float)w / (float)h;

      glMatrixMode(GL_PROJECTION);
	   glLoadIdentity();
	
	   glViewport(0, 0, w, h);

      gluPerspective(45,ratio,1,100);
	   glMatrixMode(GL_MODELVIEW);
	   glLoadIdentity();
	   gluLookAt(0.0f,0.0f,4.0f, 
		          0.0,0.0,-1.0,
			       0.0f,1.0f,0.0f);
   }
	virtual void OnClose(void){}
	virtual void OnMouseDown(int button, int x, int y) {}    
	virtual void OnMouseUp(int button, int x, int y) {}
	virtual void OnMouseWheel(int nWheelNumber, int nDirection, int x, int y){}

	virtual void OnKeyDown(int nKey, char cAscii)
	{       
		if (cAscii == 27) // 0x1b = ESC
		{
			this->Close(); // Close Window!
		} 
	};

	virtual void OnKeyUp(int nKey, char cAscii)
	{
      if (cAscii == 's')      // s: Shader
         shader->enable();
      else if (cAscii == 'f') // f: Fixed Function
         shader->disable();
	}

   void UpdateTimer()
   {
      time1 = clock();
      float delta = static_cast<float>(static_cast<double>(time1-time0)/static_cast<double>(CLOCKS_PER_SEC));
      delta = delta / 4;
      if (delta > 0.00005f)
      {
         time0 = clock();
         if (bUp)
         {
            timer010 += delta;
            if (timer010>=1.0f) { timer010 = 1.0f; bUp = false;}
         }
         else
         {
            timer010 -= delta;
            if (timer010<=0.0f) { timer010 = 0.0f; bUp = true;}
         }
      }
   }

   void DemoLight(void)
   {
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_NORMALIZE);
     /*
     // Light model parameters:
     // -------------------------------------------
     
     GLfloat lmKa[] = {0.0, 0.0, 0.0, 0.0 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);
     
     glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
     glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);
     
     // -------------------------------------------
     // Spotlight Attenuation
     
     GLfloat spot_direction[] = {1.0, -1.0, -1.0 };
     GLint spot_exponent = 30;
     GLint spot_cutoff = 180;
     
     glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
     glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
     glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);
    
     GLfloat Kc = 1.0;
     GLfloat Kl = 0.0;
     GLfloat Kq = 0.0;
     
     glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION,Kc);
     glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
     glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);
     
     
     // ------------------------------------------- 
     // Lighting parameters:

     GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
     GLfloat light_Ka[]  = {1.0f, 0.5f, 0.5f, 1.0f};
     GLfloat light_Kd[]  = {1.0f, 0.1f, 0.1f, 1.0f};
     GLfloat light_Ks[]  = {1.0f, 1.0f, 1.0f, 1.0f};

     glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
     glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
     glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
     glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

     // -------------------------------------------
     // Material parameters:

     GLfloat material_Ka[] = {0.5f, 0.0f, 0.0f, 1.0f};
     GLfloat material_Kd[] = {0.4f, 0.4f, 0.5f, 1.0f};
     GLfloat material_Ks[] = {0.8f, 0.8f, 0.0f, 1.0f};
     GLfloat material_Ke[] = {0.1f, 0.0f, 0.0f, 0.0f};
     GLfloat material_Se = 20.0f;

     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
     glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
	 */
   }
};

//-----------------------------------------------------------------------------

class myApplication : public cwc::glApplication
{
public:
	virtual void OnInit() {std::cout << "Hello World!\n"; }
};

//-----------------------------------------------------------------------------

int main(void)
{
	myApplication*  pApp = new myApplication;
	myWindow* myWin = new myWindow();

	pApp->run();
	delete pApp;
	return 0;
}

//-----------------------------------------------------------------------------

