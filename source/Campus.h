#ifndef _CAMPUS_H_
#define _CAMPUS_H_

#include "../IEX/iextreme.h"
#include "Utility.h"


// �S�̊g�k�A�S�̈ړ��̃T�|�[�g
class Campus// ����
{
private:
	Campus() {}
	Campus(const Campus& r) = delete;
	Campus& operator=(const Campus& r) = delete;
public:
	POINT pos;
	float scale;
	POINT target;
	float tscale;
	bool zoomEnd;
	~Campus() {}
	static Campus* Inst()
	{
		static Campus inst;
		return &inst;
	}
	// �^�[�Q�b�g���W����ʒ����Ɍ����ē������A�g�k����
	void Zoom(int targetX, int targetY, float scale)
	{
		target.x = -targetX;
		target.y = -targetY;
		tscale = scale;
		zoomEnd = false;
	}
	// �Y�[���������I������@or�@�I����Ă���
	bool IsZoomEnd()
	{
		return zoomEnd;
	}
	void Update()
	{
		if (abs(target.x - pos.x) < 2 && abs(target.y - pos.y) < 2 && abs(tscale - scale) < 0.01f)
		{
			pos.x = target.x;
			pos.y = target.y;
			scale = tscale;
			zoomEnd = true;
		}
		else
		{
			// ���C�W��=0.9f
			int vx = (int)((target.x - pos.x)*(1.0f - 0.9f));
			int vy = (int)((target.y - pos.y)*(1.0f - 0.9f));
			float vs = (tscale - scale)*(1.0f - 0.9f);
			// ���x����
			vs = abs(vs) > 1.0f ? vs > 0 ? 1.0f : -1.0f : vs;
			pos.x += vx;
			pos.y += vy;
			scale += vs;
		}
	}
	// �X�P�[�������W�ɓK�p�A�L�����p�X���W==�^�[�Q�b�g���W�Ȃ��ʒ����ɕ`�悳���
	void Render(iex2DObj* image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		image->Render((int)(x*scale + (this->pos.x*scale) + iexSystem::ScreenWidth / 2),
					  (int)(y*scale + (this->pos.y*scale) + iexSystem::ScreenHeight / 2),
					  (int)(w*scale),
					  (int)(h*scale),
					  sx,
					  sy,
					  sw,
					  sh);
	}
};

#endif