#ifndef _STUB_H_
#define _STUB_H_

#include "../IEX/iextreme.h"
#include "Campus.h"
#include "Utility.h"
#include <map>


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
