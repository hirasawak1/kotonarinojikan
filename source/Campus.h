#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include "../IEX/iextreme.h"
#include "Utility.h"


// �S�̊g�k�A�S�̈ړ��̃T�|�[�g
class Campus
{
private:
	Campus();
	Campus(const Campus& r) = delete;
	Campus& operator=(const Campus& r) = delete;
public:
	const int NEARLY_POS;
	const float NEARLY_SCALE;
	float posx,posy;
	float scale;
	POINT target;
	float tscale;
	float friction;
	bool zoomEnd;
	static Campus* Inst()
	{
		static Campus inst;
		return &inst;
	}
	// �^�[�Q�b�g���W����ʒ����Ɍ����ē������A�g�k����
	void Zoom(int targetX, int targetY, float scale,float friction=0.9f);
	// �Y�[���������I������@or�@�I����Ă���
	bool IsZoomEnd();
	void Update();
	// �X�P�[�������W�ɓK�p�A�L�����p�X���W==�^�[�Q�b�g���W�Ȃ��ʒ����ɕ`�悳���
	void Render(iex2DObj* image, int x, int y, int w, int h, int sx, int sy, int sw, int sh);
};

#endif