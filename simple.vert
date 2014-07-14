#version 150 core
#extension GL_ARB_explicit_attrib_location : enable

// ���[�J�����W�n�̒��_�ʒu
layout (location = 0) in vec4 pv;

// ���[�J�����W�n����N���b�s���O���W�n�ւ̕ϊ��s��
uniform mat4 mc;

// ����
uniform float t;

// �R�c�e�N�X�`��
uniform sampler3D tex;

// �_�̑傫��
const float ps = 10.0;

// ��[�Ɖ��[�̈ʒu
const vec4 top = vec4(0.0, 1.0, 0.0, 0.0);
const vec4 bottom = vec4(0.0, -1.0, 0.0, 0.0);

void main()
{
  vec3 u = vec3(pv.x, fract(pv.y * 0.5 + 0.5 - t) * 2.0 - 1.0, pv.z);
  vec4 p = texture(tex, u * 0.5 + 0.5);
  gl_Position = mc * p;
  gl_PointSize = ps / gl_Position.w;
}
