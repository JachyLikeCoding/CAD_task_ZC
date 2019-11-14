#include "pch.h"
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include "solidModeling.h"
#include <iostream>
#include "ctime"

#define N  999 //��ɫ����ΪС�������3λ
void CALLBACK vertexCallback(GLvoid* vertex);
void CALLBACK beginCallback(GLenum type);
void CALLBACK endCallback();
void CALLBACK errorCallback(GLenum errorCode);
void displayCallback();
void mouseCallback(int button, int state, int x, int y);
void mouseMotionCallback(int x, int y);
void reshapeCallback(int w, int h);
void timerCallback(int id);
void InitGLUT(int argc, char ** argv);
void InitGL();
void drawGraphics();
void setRandomColor();
const int randomCount = 1000;
int eyePos[3], centerPos[3];
bool mouseLeftDown, mouseRightDown;
float mouseX, mouseY;
float cameraDistance;
float cameraAngleX, cameraAngleY;
float r, g, b;
float random[randomCount];
Solid *solid;
vector<Solid *> solidlist;


void InitGLUT(int argc, char ** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);// ��ʾģʽ��˫���塢RGBA
	// ��������
	glutInitWindowSize(1480, 1080);      // ���ڳߴ�
	glutInitWindowPosition(100, 100);  // ����λ��
	glutCreateWindow("CAD Task by ChiZhang");   // ���ڱ���

	//�ص�����
	glutDisplayFunc(displayCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(mouseMotionCallback);
	glutReshapeFunc(reshapeCallback);
	glutTimerFunc(33, timerCallback, 1);

}

void InitLights() {
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };

	GLfloat Light_ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //���������
	GLfloat Light_specular[] = { 0.5, 0.5, 0.5, 0.5 };  //���淴�����
	GLfloat Light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };	//���������

	GLfloat light0_position[] = { 0.0, 17.0, 100.0, 1.0 };//0��ƽ�й⣬1�����Դ
	GLfloat light1_position[] = { 50.0, 50.0, 100.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//�ƹ�����
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_diffuse);   //ɢ�������
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light_specular);  //���淴���
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_ambient);  //���������

	
	glEnable(GL_LIGHT0);     
	//glEnable(GL_LIGHT1); 
	glEnable(GL_LIGHTING);   
}


void InitGL() {
	glShadeModel(GL_SMOOTH); // ������Ӱƽ�� 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // ͸������

	glEnable(GL_DEPTH_TEST); // ������Ȳ��� 
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // ��ɫ���� 
	glClearStencil(0);
	glClearDepth(1.0f);          // ������Ȼ��� 
	glDepthFunc(GL_LEQUAL); // ������Ȳ��Ե�����
	
	//���ù���
	InitLights();

	//�������
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyePos[0], eyePos[1], eyePos[2], centerPos[0], centerPos[1], centerPos[2], -1, 0, 0);
}

void drawlineFrame() {
	glBegin(GL_LINES);

	glEnd();
}

void drawGraphics(){
	GLUtesselator * tessobj;
	tessobj = gluNewTess();
	//ע��ص�����  
	gluTessCallback(tessobj, GLU_TESS_VERTEX, (void (CALLBACK *)())vertexCallback);
	gluTessCallback(tessobj, GLU_TESS_BEGIN, (void (CALLBACK *)())beginCallback);
	gluTessCallback(tessobj, GLU_TESS_END, (void (CALLBACK *)())endCallback);
	gluTessCallback(tessobj, GLU_TESS_ERROR, (void (CALLBACK *)())errorCallback);

	for (int n = 0; n < solidlist.size(); n++) {
		solid = solidlist[n];
		int drawfacecount = 0;
		Face *face = solid->sfaces;
		//std::cout << "face number: " << solid->fList.size() << std::endl;
		//std::cout << "___________________face count: " << solid->faceCount << std::endl;
		srand(time(NULL));
		while (face != nullptr) {
			drawfacecount++;
			//��ɫ��̬�任
			/*r = random[(drawfacecount * 3 + rand() % 100) % randomCount];
			g = random[(drawfacecount * 3 + rand() % 100 + 1) % randomCount];
			b = random[(drawfacecount * 3 + rand() % 100 + 2) % randomCount];*/
			//�̶���ɫ
			r = random[(drawfacecount * 3 ) % randomCount];
			g = random[(drawfacecount * 3 + 1) % randomCount];
			b = random[(drawfacecount * 3 + 2) % randomCount];
			glColor3f(r, g, b); 
			
			gluTessBeginPolygon(tessobj, 0);
			Loop *loop = face->floops;
			HalfEdge *he;
			Vertex *startv;

			if (loop != nullptr) {
				gluTessBeginContour(tessobj);//���ö���εı���

				he = loop->ledge;
				startv = he->startv;

				do {
					gluTessVertex(tessobj, he->startv->vcoor, he->startv->vcoor);
					he = he->nxt;
				} while (he->startv != startv);

				gluTessEndContour(tessobj);
			}

			if (face->inloops.size() != 0) {
				for (int i = 0; i < face->inloops.size(); i++) {
					gluTessBeginContour(tessobj);//���ö���εı��� 	
					Loop *inloop = face->inloops[i];
					he = inloop->ledge;
					startv = he->startv;
					do {
						gluTessVertex(tessobj, he->startv->vcoor, he->startv->vcoor);
						he = he->nxt;
					} while (he->startv != startv);
					gluTessEndContour(tessobj);
				}
			}
			face = face->nextf;
			gluTessEndPolygon(tessobj);
		}
		//cout << "drawfacecount: " << drawfacecount << endl;
	}
	
	gluDeleteTess(tessobj);
}




void GLGrid(Point& pt1, Point& pt2, int num){
	const float _xLen = (pt2.coord[0]- pt1.coord[0]) / num;
	const float _yLen = (pt2.coord[1] - pt1.coord[1]) / num;
	const float _zLen = (pt2.coord[2] - pt1.coord[2]) / num;
	glLineWidth(1.0f);
	glLineStipple(1, 0x0303);//������ʽ
	glBegin(GL_LINES);
	glEnable(GL_LINE_SMOOTH);
	//glColor3f(0.0f,0.0f, 1.0f); //��ɫ����
	int xi = 0;
	int yi = 0;
	int zi = 0;

	//����ƽ����X��ֱ��
	for(zi = 0; zi <= num; zi++){
		float z = _zLen * zi + pt1.coord[2];
		for (yi = 0; yi <= num; yi++){
			float y = _yLen * yi + pt1.coord[1];
			glVertex3f(pt1.coord[0], y, z);
			glVertex3f(pt2.coord[0], y, z);
		}
	}

	//����ƽ����Y��ֱ��

	for (zi = 0; zi <= num; zi++){
		float z = _zLen * zi + pt1.coord[2];
		for (xi = 0; xi <= num; xi++){
			float x = _xLen * xi + pt1.coord[0];
			glVertex3f(x, pt1.coord[1], z);
			glVertex3f(x, pt2.coord[1], z);
		}
	}
	//����ƽ����Z��ֱ��

	for (yi = 0; yi <= num; yi++){
		float y = _yLen * yi + pt1.coord[1];

		for (xi = 0; xi <= num; xi++){
			float x = _xLen * xi + pt1.coord[0];
			glVertex3f(x, y, pt1.coord[2]);
			glVertex3f(x, y, pt2.coord[2]);
		}
	}
	glEnd();

}

void GLDrawSpaceAxes(){
	GLUquadricObj *objCylinder = gluNewQuadric();
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.1, 6, 6);
	glColor3f(0.0f, 0.0f,  0.8f);
	gluCylinder(objCylinder, 0.1, 0.1, 50, 50, 5);//Z
	glTranslatef(0, 0, 50);
	gluCylinder(objCylinder, 0.3, 0.0, 0.6, 50, 5);//Z
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 0.5, 50);
	glRotatef(90, 0.0, 1.0, 0.0);
	//GLPrint("Z");// Print GL Text ToThe Screen
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.0f, 0.8f, 0.0f);
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(objCylinder, 0.1, 0.1, 50, 50, 5);//Y
	glTranslatef(0, 0, 50);

	gluCylinder(objCylinder, 0.3, 0.0, 0.6, 50, 5);//Y
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.5, 50, 0);
	//GLPrint("Y");// Print GL Text ToThe Screen
	glPopMatrix();

	glPushMatrix();
	glColor3f(0.8f, 0.0f, 0.0f);
	glRotatef(90, 0.0, 1.0, 0.0);
	gluCylinder(objCylinder, 0.1, 0.1, 50, 50, 5);//X
	glTranslatef(0, 0, 50);
	gluCylinder(objCylinder, 0.3, 0.0, 0.6, 50, 5);//X
	glPopMatrix();

	glPushMatrix();
	glTranslatef(50, 0.5, 0);
	//GLPrint("X");// Print GL Text ToThe Screen
	glPopMatrix();
}



void displayCallback() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //�����ɫ�����Լ���Ȼ���
	glPushMatrix();
	//�������
	glTranslatef(0, 0, cameraDistance);
	glRotatef(cameraAngleX, 1, 0, 0);
	glRotatef(cameraAngleY, 0, 1, 0);
	//����ͼ��
	drawGraphics();
	GLDrawSpaceAxes();
	glPopMatrix();
	glutSwapBuffers();
}

void setRandomColor() {
	srand(time(NULL));
	for (int i = 0; i < randomCount; i++)
		random[i] = rand() % (N + 1) / (float)(N + 1);
}

void mouseCallback(int button, int state, int x, int y){
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}
	else if (button == GLUT_RIGHT_BUTTON){
		if (state == GLUT_DOWN){
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}

void mouseMotionCallback(int x, int y){
	if (mouseLeftDown){
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown){
		cameraDistance += (y - mouseY) * 0.2f;
		mouseY = y;
	}
}

void timerCallback(int id) {
	glutTimerFunc(33, timerCallback, 1);//glutTimerFunc(������, �ص�����ָ��, ����ֵ);
	glutPostRedisplay();//�ػ�
}

void reshapeCallback(int w, int h){
	if (h == 0){
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)w/h, 1.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}


//����Ļص�����  
void CALLBACK vertexCallback(GLvoid* vertex){
	GLdouble* pt;
	pt = (GLdouble*)vertex;
	glVertex3dv(pt);
}

void CALLBACK beginCallback(GLenum type){
	glBegin(type);
}

void CALLBACK endCallback(){
	glEnd();
}

void CALLBACK errorCallback(GLenum errorCode){
	const GLubyte * estring;
	//��ӡ��������  
	estring = gluErrorString(errorCode);
	fprintf(stderr, "Tessellation Error: %s/n", estring);
	exit(0);
}

void display(int argc, char ** argv, vector<Solid *> _solidlist, int _eyePos[3], int _centerPos[3]) {
	//���Σ���ʼ������
	for (int i = 0; i < 3; i++) {
		eyePos[i] = _eyePos[i];
		centerPos[i] = _centerPos[i];
	}
	mouseLeftDown = mouseRightDown = false;
	solidlist = _solidlist;
	setRandomColor();

	// ��ʼ��
	InitGLUT(argc, argv);
	InitGL();

	// ֪ͨ��ʼGLUT���ڲ�ѭ��
	glutMainLoop();
}