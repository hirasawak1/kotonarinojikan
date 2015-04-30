#pragma once
#include	<Windows.h>

class TextLoader
{
public:
	//�������E���
	TextLoader();
	~TextLoader();
	//����
	bool Load(char* fileName);		//	�t�@�C���̓ǂݍ���
	bool EndCheck();				//	�I�_�m�F�itrue�ŏI�_
	float LoadFloat();				//	�����擾�ifloat�l
	int LoadInt();					//	�����擾�iint�l
	void LoadStr(LPSTR str);		//	������擾
	bool Search(LPSTR str);			//	�����񌟍�
	bool SearchLine(LPSTR str);		//	�s�������񌟍�
	void nextLine();				//	���̍s�ֈړ�
	void Push();					//	���݂̈ʒu��ۑ�
	void Pop();						//	�ۑ����ꂽ�ʒu�Ɉړ�

private:
	char* fileBuf;		//�ǂݍ��񂾃t�@�C���̓��e
	long fileSize;		//�t�@�C���̃T�C�Y
	long nowPoint;		//�Ǎ��ʒu
	long keepPoint;		//�Ǎ��ʒu�̈ꎞ�ۑ��ϐ�

	int	SearchTop();	//���̕�����̐擪�ֈړ�


};