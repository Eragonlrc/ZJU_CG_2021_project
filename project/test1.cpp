#include<gl/glut.h>
#include<gl/glu.h>
#include<gl/gl.h>

#include "Camera.h"
#include "SkyBox.h"

Camera m_Camera;
SkyBox m_SkyBox;


void init(void)
{
    /** �û��Զ���ĳ�ʼ������ */
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    /** �������� */
    glEnable(GL_TEXTURE_2D);

    /** ��ʼ����� */
    if (!m_SkyBox.Init())
    {
        MessageBox(NULL, (LPCWSTR)"��ʼ�����ʧ��!", (LPCWSTR)"����", MB_OK);
        exit(0);
    }

    /** ��������� */
    m_Camera.setCamera(0, 0, 1, 0, 0, 0, 0, 1, 0);
}

void display(void)
{
    /** �û��Զ���Ļ��ƹ��� */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    /** ��������� */
    m_Camera.setLook();

    /** ������� */
    m_SkyBox.CreateSkyBox(0, 0, 1, 1.0, 0.5, 1.0);

    glutSwapBuffers();                   /**< ǿ��ִ�����е�OpenGL���� */
}

void ChangeSize(int width, int height)
{
    glViewport(0, 0, width, height);                                    /**< ���������ӿ� */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 4000.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void motion(int x, int y)
{
    m_Camera.setViewByMouse();

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case '1':
        m_Camera.setSpeed(0.2f);
        break;
    case '2':
        m_Camera.setSpeed(1.0f);
        break;
    case 'w':
        m_Camera.moveCamera(m_Camera.getSpeed());
        break;
    case 's':
        m_Camera.moveCamera(-m_Camera.getSpeed());
        break;
    case 'a':
        m_Camera.yawCamera(-m_Camera.getSpeed());
        break;
    case 'd':
        m_Camera.yawCamera(m_Camera.getSpeed());
        break;
    }

    glutPostRedisplay();
    printf("key::%d", key);
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition((GetSystemMetrics(SM_CXSCREEN) >> 1) - 400, (GetSystemMetrics(SM_CYSCREEN) >> 1) - 300);
    glutCreateWindow("��պ�");
    init();
    glutReshapeFunc(ChangeSize);
    glutDisplayFunc(display);
    glutMotionFunc(motion);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}