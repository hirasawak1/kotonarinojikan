#include "../IEX/iextreme.h"
#include "EDXLIB.h"

struct EDX::EDX_TextureMgr::TexSave EDX::EDX_TextureMgr::TexSave[MAX_TEXTURE];

BOOL EDX::EDX_TextureMgr::Init(){
	for (int i = 0; i<MAX_TEXTURE; i++){
		if (TexSave[i].lpTexture) TexSave[i].lpTexture->Release();
		TexSave[i].lpTexture = NULL;
		TexSave[i].UseObjCount = 0;
	}
	return TRUE;
}

LPDIRECT3DTEXTURE9* EDX::EDX_TextureMgr::LoadTexture(char* filename, D3DXIMAGE_INFO* lpinfo){
	int SaveTexNo = -1;
	HRESULT hr;
	//���ɓǂ݂���ł���e�N�X�`�����𒲂ׂ�
	for (int i = 0; i<MAX_TEXTURE; i++){
		//�e�N�X�`���̒��ɉ��������Ă��Ȃ��Ȃ�continue�Ŗ߂�
		if (!TexSave[i].lpTexture) continue;

		//�t�@�C�����Ŕ�r���Ĉ�v���ĂȂ��Ȃ�߂�
		if (lstrcmpi(TexSave[i].FilePass, filename)) continue;
		//��v�������̂��������Ȃ炻�̗v�f�ԍ����L�^���ă��[�v����o��
		SaveTexNo = i;
		break;
	}

	//SaveTexNo��-1�Ƃ������Ƃ́A������Ȃ������Ƃ�������
	if (SaveTexNo == -1){
		//�V�����e�N�X�`���Ȃ̂ŐV�K�o�^
		//�󂫂�T��
		for (SaveTexNo = 0; SaveTexNo < MAX_TEXTURE; SaveTexNo++){
			if (!TexSave[SaveTexNo].UseObjCount) break;
		}
		//���������ς���������I���
		if (SaveTexNo > MAX_TEXTURE)return NULL;
		//�����̃t�@�C���l�[�����t�@�C���p�X�ɕۑ����Ă���
		CopyMemory(TexSave[SaveTexNo].FilePass, filename, strlen(filename) + 1);
		//��������V�K�o�^
		//�摜�̏������炤
		D3DXGetImageInfoFromFile(filename, &TexSave[SaveTexNo].info);

		hr =  D3DXCreateTextureFromFileEx(//�摜����e�N�X�`���[�����
			iexSystem::GetDevice(),//�r�f�I�J�[�h�f�o�C�X
			(LPCSTR)filename,//�t�@�C���̈ʒu
			TexSave[SaveTexNo].info.Width, TexSave[SaveTexNo].info.Height,//��Ŏ擾���Ă���T�C�Y���g�p����
			5,//�~�b�v�}�b�v���x���B
			0,//�e�N�X�`���̐�����m�ۂ��郁�����̏ꏊ���w�肷��B�~�b�v�}�b�v���������ɂ��Ă���
			TexSave[SaveTexNo].info.Format,//�摜�̏����B��Ŏ擾���Ă��镨���g��
			D3DPOOL_DEFAULT,//�e�N�X�`�����i�[���郁�����̈ʒu�ƃt���O�ɂ��āB
			D3DX_FILTER_LINEAR,//���`���
			D3DX_FILTER_LINEAR,//�~�b�v�}�b�v�ɑ債�Ă����`���
			NULL,//�����ɂ���F���w��ł���B�������Ȃ��ꍇ��0�Ƃ�NULL
			NULL,//���̉摜�̏����i�[����D3DXIMAGE_INFO�\���̂ւ̃|�C���^��n���B��ŕۑ����Ă�̂ł���Ȃ�
			NULL,
			&TexSave[SaveTexNo].lpTexture//�e�N�X�`���I�u�W�F�N�g�ւ̃|�C���^
			);
	}
	if (lpinfo){
		*lpinfo = TexSave[SaveTexNo].info;
	}
	TexSave[SaveTexNo].UseObjCount++;
	return &TexSave[SaveTexNo].lpTexture;
}

void EDX::EDX_TextureMgr::Release(LPDIRECT3DTEXTURE9 lpTexture){
	if (!lpTexture)return;
	int SaveTexNo = -1;
	//�����Ă����e�N�X�`���Ɠ�������T��
	for (int i = 0; i<MAX_TEXTURE; i++){
		//�e�N�X�`���̒��ɉ��������Ă��Ȃ��Ȃ�߂�
		if (!TexSave[i].lpTexture) continue;
		//��v���ĂȂ��Ȃ�߂�
		if (TexSave[i].lpTexture != lpTexture) continue;
		//�ʒu���Ă��炻�̗v�f�ԍ����L�^���Ė߂�
		SaveTexNo = i;
		break;
	}

	//no��-1�ȊO�Ƃ������Ƃ͌������Ă���
	if (SaveTexNo != -1){
		//���̃e�N�X�`���𗘗p���Ă���2DObj�̃J�E���^�����炷
		TexSave[SaveTexNo].UseObjCount--;
		//�܂��g���Ă���2DObj���c���Ă���Ȃ炱���ŏI���
		if (TexSave[SaveTexNo].UseObjCount > 0) return;
		//�����łȂ��Ȃ�e�N�X�`��������
		//edxSystem::GetDevice()->SetTexture(0, NULL);
		//	�e�N�X�`��������ď�����
		if (TexSave[SaveTexNo].lpTexture){
			TexSave[SaveTexNo].lpTexture->Release();
			TexSave[SaveTexNo].lpTexture = NULL;
		}
		TexSave[SaveTexNo].UseObjCount = 0;
	}else {//LoadTexture�֐��Ń��[�h����Ă��Ȃ��ꍇ�͌�����Ȃ��̂ŁA�����[�X���邾��
		lpTexture->Release();
	}
}