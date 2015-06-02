
#include	"iextreme.h"
#include	"Control.h"
#include	"Game.h"

#include	"Clock.h"
#include	<random>
//****************************************************************************************
//
//	Clock�N���X
//
//****************************************************************************************

//----------------------------------------------------------------------------
//	�萔�錾
//----------------------------------------------------------------------------
const float Clock::ANGLE_1MINUTE = 2 * PI / MPH;
const float Clock::ANGLE_1HOUR = 2 * PI / HOUR_CYCLE;

//----------------------------------------------------------------------------
//	�������E���
//----------------------------------------------------------------------------

Clock::Clock()
{}

void Clock::Init(int id, const Vector2& centerPos, int colW, int colH, float scale, float speed, Behavior behavior)
{
	TimeObj::Init(id, centerPos, colW, colH, scale, speed, behavior);
	timeCount = 0;
	frameCount = 0;
	hourAngle = 0;
	minuteAngle = 0;
}

void Clock::AppendImage(int idx, iex2DObj* image, const ImageParam& param)
{
	TimeObj::AppendImage(idx, image, param);
	dst[idx].w = IMAGE_WIDTH;
	dst[idx].h = IMAGE_HEIGHT;
}

void Clock::Update()
{
	switch(state)
	{
		case TimeObj::MOVE:
			switch(behavior)
			{
				case Behavior::SMOOTH:
					Update_Smooth();
					break;
				case Behavior::STEPING:
					Update_Steping();
					break;
				default:
					break;
			}
			break;
		case TimeObj::STOP:
			timeCount = frameCount = 0;
			// ���P�ʂ̊p�x+�b�P�ʂ̊p�x
			minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
			// ���ԒP��+���P��+�b�P��
			hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
			break;
		case TimeObj::CHECK:
			Update_Check();
			break;
		default:
			break;
	}
}

void Clock::Render()
{
	POINT p;
	p.x = pos.x;
	p.y = pos.y;

	Campus::GetInst()->Add(imageList[BACK], pos.x - dst[BACK].w*scale / 2 + dst[BACK].x*scale,
						   pos.y - dst[BACK].h*scale / 2 + dst[BACK].y*scale,
						   dst[BACK].w*scale, dst[BACK].h*scale,
						   src[BACK].x, src[BACK].y, src[BACK].w, src[BACK].h);

	Campus::GetInst()->Add(imageList[HOUR], pos.x - dst[HOUR].w*scale / 2 + dst[HOUR].x*scale,
						   pos.y - dst[HOUR].h*scale / 2 + dst[HOUR].y*scale,
						   dst[HOUR].w*scale, dst[HOUR].h*scale,
						   src[HOUR].x, src[HOUR].y, src[HOUR].w, src[HOUR].h,
						   p, hourAngle);
	Campus::GetInst()->Add(imageList[MINUTE], pos.x - dst[MINUTE].w*scale / 2 + dst[MINUTE].x*scale,
						   pos.y - dst[MINUTE].h*scale / 2 + dst[MINUTE].y*scale,
						   dst[MINUTE].w*scale, dst[MINUTE].h*scale,
						   src[MINUTE].x, src[MINUTE].y, src[MINUTE].w, src[MINUTE].h,
						   p, minuteAngle);
}

inline void Clock::Update_Time(float speed)
{
	//���Ԍo�߂̃x�N�g���ɉ����ċt�]
	int v = this->speed > 0 ? speed : -speed;

	frameCount += v;
	// ����
	if(frameCount >= FPS / abs(this->speed))
	{
		frameCount -= FPS / abs(this->speed);
		timeCount += v;
		if(timeCount >= MPH*HOUR_CYCLE)
			timeCount -= timeCount / (MPH*HOUR_CYCLE)*(MPH*HOUR_CYCLE);
	}
	// �t��
	else if(frameCount < 0)
	{
		frameCount += FPS / abs(this->speed);
		timeCount += v;
		if(timeCount < 0)
			timeCount += timeCount / (MPH*HOUR_CYCLE)*(MPH*HOUR_CYCLE);
	}
}

inline void Clock::Update_Check()
{
	Update_Time(50);
	// ���P�ʂ̊p�x+�b�P�ʂ̊p�x
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
	// ���ԒP��+���P��+�b�P��
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
}

inline void Clock::Update_Smooth()
{
	Update_Time();
	// ���P�ʂ̊p�x+�b�P�ʂ̊p�x
	minuteAngle = (timeCount % MPH * ANGLE_1MINUTE) + (frameCount*(ANGLE_1MINUTE / (FPS / abs(speed))));
	// ���ԒP��+���P��+�b�P��
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount %MPH * (ANGLE_1HOUR / MPH)) + (frameCount*(ANGLE_1HOUR / MPH / (FPS / abs(speed))));
}

inline void Clock::Update_Steping()
{
	Update_Time();
	minuteAngle = timeCount % MPH * ANGLE_1MINUTE;
	hourAngle = (timeCount / MPH * ANGLE_1HOUR) + (timeCount % MPH * (ANGLE_1HOUR / MPH));
}


//	�R���X�g���N�^
//Clock::Clock(void) :TimeObject(256, 256)
//{
//
//}
//
////	�f�X�g���N�^
//Clock::~Clock(void)
//{
//	delete	m_Clock;	m_Clock = NULL;
//}
//
////	������
//bool		Clock::Initialize(void)
//{
//	//	���v�摜
//	m_Clock = new iex2DObj("DATA/timer.png");
//
//	//	�ϐ�������
//	org_speed = PI / 180;
//	selectNum = 0;
//	cul_speed = 0.0f;
//	clear = false;
//	t = 1.0f;
//	step = 0;
//
//	//	�I�u�W�F�N�g��񏉊���
//	InitObj();
//
//	return true;
//}
//
////	�I�u�W�F�N�g������
//void	Clock::InitObj(void)
//{
//	//	���̃I�u�W�F�N�g
//	SetObj(0, 640, 360, 30.0f, 0.0f, 1.0f);
//
//	//	���̃I�u�W�F�N�g
//	SetObj(1, 940, -40, 120.0f, 0.0f, 2.0f);
//	SetObj(2, 340, 60, 300.0f, 0.0f, 0.5f);
//	SetObj(3, 540, -340, 240.0f, 0.0f, 1.0f);
//}
//
////	�I�u�W�F�N�g���ݒ�
//void	Clock::SetObj(int n, int x, int y, float hAngle, float mAngle, float s)
//{
//	obj[n].pos.x = x;
//	obj[n].pos.y = y;
//	obj[n].hourAngle = PI / 180.0f * hAngle;
//	obj[n].minuteAngle = PI / 180.0f * mAngle;
//	obj[n].speed = s;
//
//	//	�����ʒu�ۑ�
//	obj[n].start = obj[n].pos;
//	obj[n].end = obj[n].pos;
//	obj[n].shift.x = 0;
//	obj[n].shift.y = 0;
//}
//
////----------------------------------------------------------------------------
////	�X�V
////----------------------------------------------------------------------------
//
////	�X�V
//void	Clock::Update(void)
//{
//	//	����
//	Move();
//
//	switch (step)
//	{
//		case 0:		//	�Y�[���A�E�g
//			m_Camera.param += PI / 180 * CameraInfo::ZOOM_SPEED;
//			m_Camera.scale = (int)(CameraInfo::ZOOM_MAX * sinf(m_Camera.param));
//			CalcPos();	//	����v�Z
//			if (m_Camera.scale <= 0)
//			{
//				m_Camera.param = 0.0f;
//				m_Camera.scale = 0;
//				step++;
//			}
//			break;
//
//		case 1:		//	�Q�[���J�n
//			Control();
//			break;
//	}
//
//	//	�p�����[�^���Z
//	t += 0.01f;
//	if (t >= 1.0f)	t = 1.0f;
//}
//
////----------------------------------------------------------------------------
////	�`��
////----------------------------------------------------------------------------
//
////	�`��
//void	Clock::Render(void)
//{
//	//	�ϐ�����
//	DWORD	color[4] = {0xFFFF7777, 0xFF77FF77, 0xFF7777FF, 0xFFFF77FF};
//	int	x, y, w, h, sx, sy, sw, sh;
//	POINT	p;
//
//	for (int i = 0; i < CLOCK_MAX; i++)
//	{
//		//	���v�`��
//		x = obj[i].pos.x - 150 - m_Camera.scale / 2 + obj[i].shift.x / 2;
//		y = obj[i].pos.y - 150 - m_Camera.scale / 2 + obj[i].shift.y / 2;
//		p.x = obj[i].pos.x + obj[i].shift.x;
//		p.y = obj[i].pos.y + obj[i].shift.y;
//		w = h = 300 + m_Camera.scale;
//		sx = sy = 0;
//		sw = sh = 256;
//		m_Clock->Render(x, y, w, h, sx, sy, sw, sh, RS_COPY, color[i]);
//
//		//	���j�`��
//		sx = 256;
//		m_Clock->Render(x, y, w, h, sx, sy, sw, sh, p, obj[i].minuteAngle);
//
//		//	���j�`��
//		sx = 0;
//		sy = 256;
//		m_Clock->Render(x, y, w, h, sx, sy, sw, sh, p, obj[i].hourAngle);
//
//		//	�X�s�[�h�\��
//		x = obj[i].pos.x + 100;
//		y = obj[i].pos.y + 100;
//	}
//}
//
////----------------------------------------------------------------------------
////	����֐�
////----------------------------------------------------------------------------
//
////�@�͈͓��ɂ��邩���ׂ�
//bool	Clock::CheckWithin(const POINT& p, const int& startX, const int& startY, const int size)
//{
//	POINT	cursorPos = p;
//
//	//	�J�[�\�����W�ݒ�
//	GetCursorPos(&cursorPos);
//	ScreenToClient(iexSystem::Window, &cursorPos);
//
//	if ((startX < cursorPos.x) && (cursorPos.x < startX + size))
//	{
//		if ((startY < cursorPos.y) && (cursorPos.y < startY + size))
//			return true;
//	}
//	return false;
//}
//
////	POINT���
//void	Clock::Lerp(POINT& out, const POINT p1, const POINT p2, float t)
//{
//	float rate = t * t * (3.0f - 2.0f * t);   // 3���֐���Ԓl�ɕϊ�
//
//	out.x = (long)(p1.x * (1.0f - rate) + p2.x * rate);
//	out.y = (long)(p1.y * (1.0f - rate) + p2.y * rate);
//}
//
////	����
//void	Clock::Move(void)
//{
//	//	���v�̐j�̉�]
//	for (int i = 0; i < CLOCK_MAX; i++)
//	{
//		//	�I�𒆂̃I�u�W�F�N�g
//		if (i == selectNum)
//		{
//			//	���S����̋��������߂�
//			length.x = 640 - obj[i].start.x;
//			length.y = 360 - obj[i].start.y;
//
//			//	�ŏI�̈ʒu�����߂�
//			obj[i].end.x = obj[i].start.x + length.x;
//			obj[i].end.y = obj[i].start.y + length.y;
//
//			//	���S�Ɉړ�( �ړ���� )
//			Lerp(obj[i].pos, obj[i].start, obj[i].end, t);
//
//			//	�I�𒆂̃I�u�W�F�N�g�̃X�s�[�h��ۑ�
//			cul_speed = obj[i].speed;
//
//			//	���̃X�s�[�h�ŉ�]
//			obj[i].minuteAngle += org_speed;
//			obj[i].hourAngle += org_speed / 12.0f;
//		}
//		else
//		{
//			//	�ŏI�̈ʒu�����߂�
//			obj[i].end.x = obj[i].start.x + length.x;
//			obj[i].end.y = obj[i].start.y + length.y;
//
//			//	�ړ�
//			Lerp(obj[i].pos, obj[i].start, obj[i].end, t);
//
//			//	��]
//			obj[i].minuteAngle += org_speed * (obj[i].speed / cul_speed);
//			obj[i].hourAngle += (org_speed / 12.0f) * (obj[i].speed / cul_speed);
//		}
//	}
//}
//
////	�R���g���[��
//void	Clock::Control(void)
//{
//	//	�G���^�[�őI��
//	if (KEY(KEY_ENTER) == 3)
//	if (obj[selectNum].speed == obj[0].speed && selectNum != 0)
//		clear = true;
//
//	//	�}�E�X�N���b�N�`�F�b�N
//	for (int n = 0; n < 4; n++)
//	{
//		//	�}�E�X�J�[�\�����͈͓���������
//		if (CheckWithin(Mouse::cursor, obj[n].pos.x - 150, obj[n].pos.y - 150, GameInfo::WNDSIZE))
//		{
//			//	�N���b�N����
//			if (Mouse::Click())
//			{
//				//	�ړ����I�����Ă��邩�`�F�b�N
//				if (t < 1.0f)		return;
//
//				//	�I�𒆂̃I�u�W�F�N�g����Ȃ�������
//				if (selectNum != n)
//				{
//					//	�I�������I�u�W�F�N�g�̔ԍ�
//					selectNum = n;
//
//					//	���̈ʒu�ۑ�
//					for (int i = 0; i < CLOCK_MAX; i++)
//						obj[i].start = obj[i].pos;
//
//					//	�p�����[�^�ݒ�
//					t = 0.0f;
//				}
//			}
//		}
//	}
//}
//
////	�ʒu�֌W�v�Z
//void	Clock::CalcPos(void)
//{
//	float cx, cy, vx, vy;
//	cx = 640;
//	cy = 360;
//	float	l;
//
//	for (int i = 0; i < CLOCK_MAX; i++)
//	{
//		vx = obj[i].pos.x - cx;
//		vy = obj[i].pos.y - cy;
//
//		l = sqrt(vx * vx + vy *vy);
//
//		vx /= l;
//		vy /= l;
//
//		if (selectNum != i)
//		{
//			obj[i].shift.x = (int)(vx * m_Camera.scale * 2);
//			obj[i].shift.y = (int)(vy * m_Camera.scale * 2);
//		}
//	}
//}
//
//void Clock::Check()
//{
//
//	//�ړ��I��
//	if (t >= 1.0f)
//	{
//		t = 0.0f;
//
//		//����
//		switch (obj[selectNum].flag)
//		{
//			case FLAG_RED:
//				if (obj[selectNum].speed == 1.0f)
//				{
//					obj[selectNum].state = STATE_SUCCESS;
//					obj[selectNum].SuccessFlg = true;
//				}
//				else
//				{
//					obj[selectNum].state = STATE_BERAK;
//					ShakeFlg = true;
//				}
//				break;
//			case FLAG_GREEN:
//				if (obj[selectNum].speed == 2.0f)
//				{
//					obj[selectNum].state = STATE_SUCCESS;
//					obj[selectNum].SuccessFlg = true;
//				}
//				else
//				{
//					obj[selectNum].state = STATE_BERAK;
//					ShakeFlg = true;
//				}
//				break;
//			case FLAG_BLUE:
//				if (obj[selectNum].speed == 0.5f)
//				{
//					obj[selectNum].state = STATE_SUCCESS;
//					obj[selectNum].SuccessFlg = true;
//				}
//				else
//				{
//					obj[selectNum].state = STATE_BERAK;
//					ShakeFlg = true;
//				}
//				break;
//		}
//		//�ʒu�ۑ�
//		for (int i = 0; i < 4; i++)
//		{
//			obj[i].start = obj[i].pos;
//		}
//		//���̎��v��
//		for (int i = selectNum + 1; i < 4; i++)
//		{
//			//�t���O���܂������Ă��Ȃ��āA�������Ă��Ȃ�����
//			if (obj[i].flag&&obj[i].state != STATE_SUCCESS)
//			{
//				selectNum = i;
//				goto Check_found;
//			}
//		}
//
//		//������Ȃ���ΏI��
//
//		CheckFlg = false;
//		return;
//
//		//�������Ă���Ύ��̈ړ�
//	Check_found:;
//	}
//}
//
//void Clock::ClearCheck()
//{
//
//	for (int i = 0; i < 4; i++)
//	{
//		if (obj[i].state != STATE_SUCCESS)return;
//	}
//	clear = true;
//}
//
//void Clock::Shake()
//{
//	std::uniform_real_distribution<float> rad(0.0f, 1.0f);
//
//	std::mt19937 mtRand{std::random_device()()};
//
//	ShakeVar.x = rad(mtRand)*SHAKE_POWER - SHAKE_POWER / 2;
//	ShakeVar.y = rad(mtRand)*SHAKE_POWER - SHAKE_POWER / 2;
//}
//
//void Clock::FlagUpdate()
//{
//	HaveFlag[FLAG_NOT] = DefaultHaveFlag[FLAG_NOT];
//	HaveFlag[FLAG_RED] = DefaultHaveFlag[FLAG_RED];
//	HaveFlag[FLAG_GREEN] = DefaultHaveFlag[FLAG_GREEN];
//	HaveFlag[FLAG_BLUE] = DefaultHaveFlag[FLAG_BLUE];
//	for (int i = 0; i < 4; i++)
//	{
//		switch (obj[i].flag)
//		{
//			case FLAG_NOT:
//				HaveFlag[FLAG_NOT]--;
//				break;
//			case FLAG_RED:
//				HaveFlag[FLAG_RED]--;
//				break;
//			case FLAG_GREEN:
//				HaveFlag[FLAG_GREEN]--;
//				break;
//			case FLAG_BLUE:
//				HaveFlag[FLAG_BLUE]--;
//				break;
//		}
//	}
//}