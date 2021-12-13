#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <GLUT/glut.h>
#include <cmath>
#include <iostream>

#include "./obj_loader.h"

float angle = 0.f;

obj modelToRender;

void changeSize(int w, int h) {
  if (h == 0) {
    h = 1;
  }

  float ratio = w * 1.0f / h;

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glViewport(0, 0, w, h);
  gluPerspective(45, ratio, 1, 100);

  glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  gluLookAt(0.f, 0.f, 8.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);

  glRotatef(angle, 0.1f, 0.2f, 0.3f);

  glColor3f(1.f, 1.f, 1.f);

  glBegin(GL_TRIANGLES);
  for (size_t i = 0; i < modelToRender.faces.size(); ++i) {
    auto face = modelToRender.faces.at(i);
    auto v1 = modelToRender.vertices.at(face.vi1 - 1);
    auto v2 = modelToRender.vertices.at(face.vi2 - 1);
    auto v3 = modelToRender.vertices.at(face.vi3 - 1);
    auto n = modelToRender.normals.at(face.ni1 - 1);

    glNormal3f(n.x, n.y, n.z);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
  }
  glEnd();

  angle += 0.01f;

  glutSwapBuffers();
}

bool init() {
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(320, 240);
  glutCreateWindow("Primitive OBJ Loader");

  GLfloat light_position[] = { 5.0, 5.0, 5.0, 1.0 };
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);

  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutIdleFunc(renderScene);

  std::cout << "\nGL_VERSION " << glGetString(GL_VERSION) << "\nGL_VENDOR "
            << glGetString(GL_VENDOR) << "\nGL_RENDERER "
            << glGetString(GL_RENDERER)
            // << "\nGL_EXTENSIONS " << glGetString(GL_EXTENSIONS)
            << std::endl;

  // load model here
  auto loadedModel = load_obj("assets/bunny.obj.model");

  if (!loadedModel) {
    return false;
  }

  modelToRender = loadedModel.value();

  return true;
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  if (init()) {
    glutMainLoop();
  }

  std::cout << "If we are here, something went wrong..." << std::endl;
  return 1;
}
