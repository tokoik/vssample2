#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>

// �⏕�v���O����
#include "gg.h"
using namespace gg;

// �A�j���[�V�����̎����i�b�j
const double cycle(5.0);

// �e�N�X�`���̃T�C�Y
const GLsizei texsize = 128;

//
// �E�B���h�E�֘A�̏���
//
class Window
{
  // �E�B���h�E�̎��ʎq
  GLFWwindow *const window;
  
  // �������e�ϊ��s��
  GgMatrix mp;
  
  // �g���b�N�{�[��
  GgTrackball tb;
  
public:
  
  // �R���X�g���N�^
  Window(const char *title = "Game Graphics", int width = 640, int height = 480)
  : window(glfwCreateWindow(width, height, title, NULL, NULL))
  {
    if (window == NULL)
    {
      // �E�B���h�E���쐬�ł��Ȃ�����
      std::cerr << "Can't create GLFW window." << std::endl;
      exit(1);
    }
    
    // ���݂̃E�B���h�E�������Ώۂɂ���
    glfwMakeContextCurrent(window);
    
    // �쐬�����E�B���h�E�ɑ΂���ݒ�
    glfwSwapInterval(1);
    
    // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
    ggInit();
    
    // ���̃C���X�^���X�� this �|�C���^���L�^���Ă���
    glfwSetWindowUserPointer(window, this);
    
    // �}�E�X�{�^���𑀍삵���Ƃ��̏���
    glfwSetMouseButtonCallback(window, mouse);
    
    // �E�B���h�E�̃T�C�Y�ύX���ɌĂяo��������o�^����
    glfwSetFramebufferSizeCallback(window, resize);
    
    // �E�B���h�E�̐ݒ������������
    resize(window, width, height);
  }
  
  // �f�X�g���N�^
  virtual ~Window()
  {
    glfwDestroyWindow(window);
  }
  
  // �E�B���h�E�����ׂ����𔻒肷��
  int shouldClose() const
  {
    return glfwWindowShouldClose(window) | glfwGetKey(window, GLFW_KEY_ESCAPE);
  }
  
  // �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
  void swapBuffers()
  {
    // �J���[�o�b�t�@�����ւ���
    glfwSwapBuffers(window);
    
    // OpenGL �̃G���[���`�F�b�N����
    ggError("SwapBuffers");
    
    // �C�x���g�����o��
    glfwPollEvents();
    
    // ���{�^���h���b�O
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
    {
      // �}�E�X�̌��݈ʒu���擾����
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      
      // �g���b�N�{�[������
      tb.motion(static_cast<float>(x), static_cast<float>(y));
    }
  }
  
  // �}�E�X�{�^���𑀍삵���Ƃ��̏���
  static void mouse(GLFWwindow *window, int button, int action, int mods)
  {
    // ���̃C���X�^���X�� this �|�C���^�𓾂�
    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
    
    if (instance)
    {
      // �}�E�X�J�[�\���̌��݈ʒu���擾����
      double x, y;
      glfwGetCursorPos(window, &x, &y);
      
      switch (button)
      {
        case GLFW_MOUSE_BUTTON_1:
          // �g���b�N�{�[������
          if (action)
          {
            // �g���b�N�{�[�������J�n
            instance->tb.start(static_cast<float>(x), static_cast<float>(y));
          }
          else
          {
            // �g���b�N�{�[�������I��
            instance->tb.stop(static_cast<float>(x), static_cast<float>(y));
          }
          break;
        case GLFW_MOUSE_BUTTON_2:
          break;
        case GLFW_MOUSE_BUTTON_3:
          break;
        default:
          break;
      }
    }
  }
  
  // �E�B���h�E�̃T�C�Y�ύX���̏���
  static void resize(GLFWwindow *window, int width, int height)
  {
    // �E�B���h�E�S�̂��r���[�|�[�g�ɂ���
    glViewport(0, 0, width, height);
    
    // ���̃C���X�^���X�� this �|�C���^�𓾂�
    Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
    
    if (instance != NULL)
    {
      // �������e�ϊ��s������߂�i�A�X�y�N�g�� w / h�j
      instance->mp.loadPerspective(0.8f, (float)width / (float)height, 2.0f, 4.5f);
      
      // �g���b�N�{�[�������͈̔͂�ݒ肷��
      instance->tb.region(width, height);
    }
  }
  
  // ���e�ϊ��s������o��
  const GgMatrix &getMp() const
  {
    return mp;
  }
  
  // �g���b�N�{�[���̕ϊ��s������o��
  const GLfloat *getTb() const
  {
    return tb.get();
  }
};

//
// �v���O�����I�����̏���
//
static void cleanup()
{
  // GLFW �̏I������
  glfwTerminate();
}

//
// ���C���v���O����
//
int main(int argc, const char * argv[])
{
  // GLFW ������������
  if (glfwInit() == GL_FALSE)
  {
    // �������Ɏ��s����
    std::cerr << "Can't initialize GLFW" << std::endl;
    return 1;
  }
  
  // �v���O�����I�����̏�����o�^����
  atexit(cleanup);
  
  // OpenGL Version 3.3 Core Profile ��I������
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // �E�B���h�E���쐬����
  Window window("Vertex Shader Sample 2");
  
  // �w�i�F���w�肷��
  glClearColor(0.2f, 0.3f, 0.6f, 0.0f);
  
  // �B�ʏ�����L���ɂ���
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

  // �v���O�����I�u�W�F�N�g�̍쐬
  const GLuint program(ggLoadShader("simple.vert", "simple.frag"));
  
  // uniform �ϐ��̃C���f�b�N�X�̌����i������Ȃ���� -1�j
  const GLint mwLoc(glGetUniformLocation(program, "mw"));
  const GLint mcLoc(glGetUniformLocation(program, "mc"));
  const GLint texLoc(glGetUniformLocation(program, "tex"));
  const GLint tLoc(glGetUniformLocation(program, "t"));
  
  // �r���[�ϊ��s��� mv �ɋ��߂�
  const GgMatrix mv(ggLookat(2.0f, 1.5f, 2.5f, 0.0f, -0.2f, 0.0f, 0.0f, 1.0f, 0.0f));
  
  // �}�`�f�[�^�̍쐬
  const std::unique_ptr<const GgPoints> cube(ggPointsCube(10000, 2.0f));

  // �R�c�e�N�X�`���̍쐬
  GLfloat(*v)[texsize][texsize][3] = new GLfloat[texsize][texsize][texsize][3];
  for (int k = 0; k < texsize; ++k)
  {
    GLfloat z = GLfloat(k) * 2.0f / GLfloat(texsize - 1) - 1.0f;
    for (int j = 0; j < texsize; ++j)
    {
      GLfloat y = GLfloat(j) * 2.0f / GLfloat(texsize - 1) - 1.0f;
      for (int i = 0; i < texsize; ++i)
      {
        GLfloat x = GLfloat(i) * 2.0f / GLfloat(texsize - 1) - 1.0f;
        GLfloat l = sqrt(x * x + y * y + z * z);
        GLfloat s = fabs(x) / l;
        GLfloat t = fabs(y) / l;
        GLfloat u = fabs(z) / l;
        if (s > t) s = t;
        if (s > u) s = u;

        v[k][j][i][0] = x * s;
        v[k][j][i][1] = y * s;
        v[k][j][i][2] = z * s;
      }
    }
  }

  GLuint tex;
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_3D, tex);
  glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, texsize, texsize, texsize, 0, GL_RGB, GL_FLOAT, v);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  delete[] v;
  
  // �o�ߎ��Ԃ̃��Z�b�g
  glfwSetTime(0.0);
  
  // �E�B���h�E���J���Ă���Ԃ���Ԃ��`�悷��
  while (window.shouldClose() == GL_FALSE)
  {
    // �E�B���h�E����������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // �V�F�[�_�v���O�����̎g�p�J�n
    glUseProgram(program);
    
    // ���f���r���[�ϊ�
    const GgMatrix mw(mv * window.getTb());

    // ���f���r���[�E���e�ϊ�
    const GgMatrix mc(window.getMp() * mw);
    
    // uniform �ϐ���ݒ肷��
    glUniformMatrix4fv(mwLoc, 1, GL_FALSE, mw.get());
    glUniformMatrix4fv(mcLoc, 1, GL_FALSE, mc.get());
    glUniform1i(texLoc, 0);

    // �����̌v��
    glUniform1f(tLoc, static_cast<float>(fmod(glfwGetTime(), cycle) / cycle));
    
    // �e�N�X�`��
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D, tex);

    // �}�`�̕`��
    cube->draw();
    
    // �V�F�[�_�v���O�����̎g�p�I��
    glUseProgram(0);
    
    // �J���[�o�b�t�@�����ւ��ăC�x���g�����o��
    window.swapBuffers();
  }
  
  return 0;
}
