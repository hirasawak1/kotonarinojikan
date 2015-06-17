
#include	"iextreme.h"
#include	"Control.h"
#include	"Game.h"

#include	"Metronom.h"

//****************************************************************************************
//
//	Metronom�N���X
//
//****************************************************************************************

//--------------------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------------------

//	�R���X�g���N�^
Metronom::Metronom(void)
{}

//	������
void Metronom::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior)
{
	TimeObj::Init(id, centerPos, colW, colH, scale, speed, behavior);
	timeCount = 0.0f;
	frameCount = 0;
	melt = 0.0f;
	alpha = 0.0f;
	animationCounter = 0;
	org_speed = 0.01f;
}

//	�C���[�W�ǉ�
void Metronom::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	TimeObj::AppendImage(idx, image, param);
	dst[idx].w = IMAGE_WIDTH;
	dst[idx].h = IMAGE_HEIGHT;
}

//	�X�V
void Metronom::Update(void)
{
	switch (state)
	{
	case TimeObj::SUCCESS:
		SuccessCnt--;


	case TimeObj::MOVE:
		switch (behavior)
		{
		case ORIGIN:
			Move();
			break;

		case DOUBLE:
			Move(2.0f);
			break;
		}
		break;
	case TimeObj::STOP:
		timeCount = frameCount = 0;
		break;
	case TimeObj::CHECK:
		Update_Check();
		break;
	default:
		break;
	}
}

//	�`��
void Metronom::Render(void)
{
	POINT p;
	p.x = 0;
	p.y = 0;
	float x, y, w, h, sx, sy, sw, sh;

	//	���g���m�[��
	x = pos.x - dst[METRONOM].w*scale / 2 + dst[METRONOM].x*scale;
	y = pos.y - dst[METRONOM].h*scale / 2 + dst[METRONOM].y*scale;
	w = dst[METRONOM].w*scale;
	h = dst[METRONOM].h*scale;
	sx = src[METRONOM].x;
	sy = src[METRONOM].y;
	sw = src[METRONOM].w;
	sh = src[METRONOM].h;
	Campus::GetInst()->Add(imageList[METRONOM], x, y, w, h, sx, sy, sw, sh);

	//	�j�`��
	x = pos.x - dst[METRONOM].w*scale / 2 + dst[METRONOM].x*scale;
	y = pos.y - dst[METRONOM].h*scale / 2 + dst[METRONOM].y*scale + 40;
	w = dst[METRONOM].w*scale;
	h = dst[METRONOM].h*scale;
	sx = src[METRONOM].x;
	sy = src[METRONOM].y;
	sw = src[METRONOM].w;
	sh = src[METRONOM].h;
	Campus::GetInst()->Add(imageList[NEEDLE], x, y, w, h, sx, sy, sw, sh, p, angle);

	if (behavior == ORIGIN)
	{
		//	��1�`��
		x = pos.x - dst[METRONOM].w*scale / 2 + dst[METRONOM].x*scale;
		y = pos.y - dst[METRONOM].h*scale / 2 + dst[METRONOM].y*scale + 40;
		w = dst[METRONOM].w*scale;
		h = dst[METRONOM].h*scale;
		sx = src[METRONOM].x;
		sy = src[METRONOM].y;
		sw = src[METRONOM].w;
		sh = src[METRONOM].h;
		Campus::GetInst()->Add(imageList[SPINDLE1], x, y, w, h, sx, sy, sw, sh, p, angle);
	}

	if (behavior == DOUBLE)
	{
		//	��2�`��
		x = pos.x - dst[METRONOM].w*scale / 2 + dst[METRONOM].x*scale;
		y = pos.y - dst[METRONOM].h*scale / 2 + dst[METRONOM].y*scale + 40;
		w = dst[METRONOM].w*scale;
		h = dst[METRONOM].h*scale;
		sx = src[METRONOM].x;
		sy = src[METRONOM].y;
		sw = src[METRONOM].w;
		sh = src[METRONOM].h;
		Campus::GetInst()->Add(imageList[SPINDLE2], x, y, w, h, sx, sy, sw, sh, p, angle);
	}

	//	���g���m�[��
	x = pos.x - dst[FACE].w*scale / 2 + dst[FACE].x*scale;
	y = pos.y - dst[FACE].h*scale / 2 + dst[FACE].y*scale;
	w = dst[FACE].w*scale;
	h = dst[FACE].h*scale;
	sx = src[FACE].x;
	sy = src[FACE].y;
	sw = src[FACE].w;
	sh = src[FACE].h;
	Campus::GetInst()->Add(imageList[FACE], x, y, w, h, sx, sy, sw, sh);
}

//	���ԍX�V
inline void Metronom::Update_Time(float speed)
{
	//���Ԍo�߂̃x�N�g���ɉ����ċt�]
	float v = this->speed > 0 ? speed : -speed;
	timeCount = org_speed * v;
}

//	�`�F�b�N���̓���
inline void Metronom::Update_Check(void)
{

}

//	���삩������
void	Metronom::Move(float speed)
{
	Update_Time();
	param += timeCount * speed;
	angle = (D3DX_PI / 180 * 45) * sinf(param);
}
