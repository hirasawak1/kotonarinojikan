#ifndef _STUB_H_
#define _STUB_H_

#include "../IEX/iextreme.h"
#include "Utility.h"
#include <map>

// �S�̊g�k�A�S�̈ړ��̃T�|�[�g
class Campus
{
private:
	Campus() {}
	Campus(const Campus& r) = delete;
	Campus& operator=(const Campus& r) = delete;
public:
	float x, y;
	float scale;
	float tx, ty;
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
		tx = -targetX;
		ty = -targetY;
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
		if (abs(tx - x) < 2 && abs(ty - y) < 2 && abs(tscale - scale) < 0.01f)
		{
			x = tx;
			y = ty;
			scale = tscale;
			zoomEnd = true;
		}
		else
		{
			// ���C�W��=0.9f
			float vx = (tx - x)*(1.0f - 0.9f);
			float vy = (ty - y)*(1.0f - 0.9f);
			float vs = (tscale - scale)*(1.0f - 0.9f);
			// ���x����
			vs = abs(vs) > 1.0f ? vs > 0 ? 1.0f : -1.0f : vs;
			x += vx;
			y += vy;
			scale += vs;
		}
	}
	// �X�P�[�������W�ɓK�p�A�L�����p�X���W==�^�[�Q�b�g���W�Ȃ��ʒ����ɕ`�悳���
	void Render(iex2DObj* image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		image->Render(x*scale + this->x*scale + 1280 / 2,
					  y*scale + this->y*scale + 720 / 2,
					  w*scale,
					  h*scale,
					  sx,
					  sy,
					  sw,
					  sh);
	}
};

bool Wait()// �^������
{
	static int count = 0;
	if (count > 60)
	{
		count = 0;
		return true;
	}
	else
	{
		count++;
		return false;
	}
}

bool IsFirstboot()
{
	return true;
}

bool GameMain()// �Q�[���I���Ȃ�true
{
	return !(KEY_Get(KEY_ENTER) == 3);
}

bool IsGameClear()
{
	return true;
}

bool SelectStage()
{
	if (KEY_Get(KEY_ENTER) == 3)
	{
		// �I�����ꂽ�X�e�[�W��ێ�
		return true;
	}
	else
	{
		return false;
	}
}

bool AddStage()
{
	return Wait();
}

bool FadeIn()
{
	return Wait();
}

bool FadeOut()
{
	return Wait();
}

//�@�摜�p���ۃN���X
class ImageHoge
{
public:
	iex2DObj*  image;

	ImageHoge() { image = nullptr; }
	virtual ~ImageHoge() { SafeDelete(image); }
	virtual void Render() = 0;
};

//�@�w�i
class BackHoge :public ImageHoge
{
public:
	BackHoge()
	{
		image = new iex2DObj("DATA/title.png");
	}

	void Render()override
	{
		Campus::Inst()->Render(image, 0, 0, 1280, 720, 0, 0, 1280, 720);
	}
};

class BigClockHoge :public ImageHoge
{
public:
	BigClockHoge()
	{
		image = new iex2DObj("DATA/bigclock.png");
	}
	void Render()override
	{
		Campus::Inst()->Render(image, 850, 90, 256, 256, 0, 0, 512, 512);
	}
};

class ClockHoge :ImageHoge
{
public:
	int count;
	ClockHoge()
	{
		count = 0;
		image = new iex2DObj("DATA/clock.png");
	}
	void Render()override
	{
			Campus::Inst()->Render(image, 855 + 128 - 25 + sinf((count-1) * ((2 * PI) / 12)) * 100,
								   90 + 128 - 25 - cosf((count-1)*((2 * PI) / 12)) * 100,
								   50, 50, 0, 0, 128, 128);
		
	}
	void RenderSelect()
	{
		for (int i = 0; i < count; i++)
		{
			// �厞�v�ɉ����悤�ɔz�u
			Campus::Inst()->Render(image, 855 + 128 - 25 + sinf(i * ((2 * PI) / 12)) * 100,
								   90 + 128 - 25 - cosf(i*((2 * PI) / 12)) * 100,
								   50, 50, 0, 0, 128, 128);
		}
	}
	void Append()
	{
		count = count < 12 ? count + 1 : count;
	}
	int GetCount()
	{
		return count;
	}
	Vector3 GetPos(int id)
	{
		// �e���v�̒��S�_
		return Vector3(855 + 128  + sinf(id * ((2 * PI) / 12)) * 100,
					   90 + 128 - cosf(id*((2 * PI) / 12)) * 100, 0);
	}
};

class StageHoge :ImageHoge
{
public:
	StageHoge()
	{
		image = new iex2DObj("DATA/main.png");
	}
	void Render()override
	{
		image->Render(0, 0, 1280, 720, 0, 0, 1280, 720);
	}
};

#endif //#ifndef _STUB_H_
