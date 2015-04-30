#include	"Stage.h"
#include	"iextreme.h"
#include	"TextLoader.h"
#include	<string>
#include	<sstream>
#include	<assert.h>

void Object::Setting(int x, int y, float scale, float speed)
{
	this->x = x;	this->y = y;
	this->scale = scale; this->speed = speed;
}

//*********************************************************************

StageMNG::StageMNG()
{
}

StageMNG::~StageMNG()
{
	delete tex;
	tex = nullptr;
	delete[] obj;
	obj = nullptr;
}

void StageMNG::LoadStage(const int stageNum)
{
	//���̔j��
	objMax = 0;
	delete[] obj;
	obj = nullptr;

	//stageNum����t�@�C�����̐ݒ�
	std::stringstream buf;
	buf << stageNum;
	//�t�@�C�������ʕ����̐ݒ�
	std::string fileName = "DATA/BG/stage"+ buf.str() +".sdt";
	TextLoader loader;
	if( !loader.Load((char*)fileName.c_str()) ){
		assert(!"�X�e�[�W�t�@�C�����J���܂���i���݂��܂���j");
		return;
	}

	//��
	objMax = loader.LoadInt();
	obj = new Object[objMax];
	//�ݒ�p�ϐ�
	int x, y;
	float scale, speed;
	//�ݒ�
	for(int i=0; i<objMax; i++){
		x = loader.LoadInt(); y = loader.LoadInt();
		scale = loader.LoadFloat(); speed = loader.LoadFloat();

		obj[i].Setting(x, y, scale, speed);
	}
}

void StageMNG::Update()
{

}

void StageMNG::Render()
{

}