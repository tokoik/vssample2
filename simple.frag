#version 330

// �t���[���o�b�t�@�ɏo�͂���f�[�^
layout (location = 0) out vec4 fc;                  // �t���O�����g�̐F

void main()
{
  fc = vec4(vec3(1.0 - gl_FragCoord.z), 1.0);
}
