#include	"TextLoader.h"
#include	<stdio.h>

//*****************************************
//	�������E���
//*****************************************
TextLoader::TextLoader()	: fileBuf(NULL), fileSize(0), nowPoint(0), keepPoint(0)
{
}

TextLoader::~TextLoader()
{
	if( fileBuf ) delete[] fileBuf;
	fileBuf = NULL;
}


//*****************************************
//	�������E���
//*****************************************

//�����@�F[in]�t�@�C����
//�߂�l�F����ɊJ�������ǂ���
//�����@�F�w�肳�ꂽ�t�@�C�����e�L�X�g�`���œǂݍ��݁A���e���o�b�t�@�Ɋm�ۂ���B
bool TextLoader::Load(char* fileName)
{
	FILE* fp;
	fopen_s(&fp, fileName, "r");
	if( fp == NULL ) return false;

	fseek( fp, 0, SEEK_END );
	fileSize = ftell( fp );

	if( fileBuf ){
		delete[] fileBuf;
		fileBuf = NULL;
	}
	fileBuf = new char[fileSize+1];

	fseek( fp, 0, SEEK_SET);
	fread(fileBuf, sizeof(char), fileSize, fp);
	fileBuf[fileSize] = '\n';

	fclose(fp);

	return true;
}

//�����@�F�Ȃ�
//�߂�l�F�I�_���ǂ���
//�����@�F�ǂݍ��݈ʒu���I�_���ǂ�����Ԃ��B
bool	TextLoader::EndCheck()
{
	if( nowPoint > fileSize ) return true;
	return false;
}

//�����@�F�Ȃ�
//�߂�l�Ffloat�^�̒l
//�����@�F�ǂݍ��񂾒l��float�^�Ƃ��ĕԂ��B
float TextLoader::LoadFloat()
{
	char	temp[20];
	LoadStr(temp);

	return (float)atof(temp);
}

//�����@�F�Ȃ�
//�߂�l�Fint�^�̒l
//�����@�F�ǂݍ��񂾒l��int�^�Ƃ��ĕԂ��B
int TextLoader::LoadInt()
{
	char	temp[20];
	LoadStr(temp);

	return atoi(temp);
}

//�����@�F[out]������
//�߂�l�F�Ȃ�
//�����@�F��؂�܂ŕ�����𔲂��o���Ĉ����̕ϐ��Ɋi�[����B
void	TextLoader::LoadStr(LPSTR str)
{
	int		len = 0;
	
	int mode = SearchTop();
	if( nowPoint >= fileSize ) return;
	
	//	���O�ǂݍ��݃��[�h
	if( mode == 1 ){
//		str[len] = '"';
//		len   ++;
		nowPoint ++;
		while(true){
			if( IsDBCSLeadByte(fileBuf[nowPoint]) ){ 
				str[len] = fileBuf[nowPoint];
				len   ++;
				nowPoint ++;
			} else {
				if( fileBuf[nowPoint] == '"' ) break;
			}
			if( nowPoint >= fileSize ) break;

			str[len] = fileBuf[nowPoint];
			len   ++;
			nowPoint ++;
		}
//		str[len] = '"';
//		len   ++;
		str[len] = '\0';
		nowPoint ++;
		return;
	}


	while(true){
		if( nowPoint>0 && !IsDBCSLeadByte(fileBuf[nowPoint-1]) ){ 
			if( fileBuf[nowPoint] == ';' ) break;
			if( fileBuf[nowPoint] == ')' ) break;
			if( fileBuf[nowPoint] == '(' ) break;
			if( fileBuf[nowPoint] == ',' ) break;
			if( fileBuf[nowPoint] == ' ' ) break;
			if( fileBuf[nowPoint] == '\t' ) break;
			if( fileBuf[nowPoint] == 0x0d ) break;
			if( fileBuf[nowPoint] == 0x0a ) break;
			if( nowPoint >= fileSize ) break;
		}
		str[len] = fileBuf[nowPoint];
		len   ++;
		nowPoint ++;
	}
	str[len] = '\0';
	nowPoint ++;
}

//�����@�F�Ȃ�
//�߂�l�F����ɏ������s�����ꍇ��0�A�I�_�̏ꍇ��-1�A"�̏ꍇ��1��Ԃ��B
//�����@�F���̕�����̐擪�ֈړ�
int TextLoader::SearchTop()
{
	while(true){
		if( nowPoint >= fileSize ) return -1;

		if( IsCharAlphaNumeric(fileBuf[nowPoint]) ) break;
		if( IsDBCSLeadByte(fileBuf[nowPoint]) ) break;

		if( fileBuf[nowPoint] == '{' ) break;
		if( fileBuf[nowPoint] == '}' ) break;
		if( fileBuf[nowPoint] == '"' ) return 1;
		if( fileBuf[nowPoint] == '.' ) break;
		if( fileBuf[nowPoint] == '-' ) break;
		if( fileBuf[nowPoint] == '=' ) break;
		nowPoint ++;
		if( nowPoint >= fileSize ) return -1;
	}
	return	0;
}

//�����@�F[in]�������镶����
//�߂�l�F�����񂪑��݂������ǂ���
//�����@�F�^����ꂽ����������݂̒n�_����I�_�܂Ō�������B
bool	TextLoader::Search( LPSTR str )
{
	char	temp[128];

	while(!EndCheck()){
		LoadStr(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;
	}
	return false;
}

//�����@�F[in]�������镶����
//�߂�l�F�����񂪑��݂������ǂ���
//�����@�F�^����ꂽ����������݂̒n�_����s�̒��Ō�������B�Ǎ��ʒu�͍X�V���Ȃ��B
bool	TextLoader::SearchLine( LPSTR str )
{
	char	temp[128];
	DWORD	oldIndex = nowPoint;
	
	//	�s���ݒ�
	while(true){
		nowPoint --;
		if( fileBuf[nowPoint] == '\n' ) break;
		if( fileBuf[nowPoint] == '\r' ) break;
	}
	nowPoint ++;
	//	�s������
	while(true){
		if( fileBuf[nowPoint] == '\n' ) break;
		if( fileBuf[nowPoint] == '\r' ) break;
		if( !IsCharAlphaNumeric(fileBuf[nowPoint]) ){
			nowPoint ++;
			continue;
		}
		LoadStr(temp);
		if( lstrcmp( temp, str ) == 0 ) return true;
	}
	nowPoint = oldIndex;
	return false;
}

//�����@�F�Ȃ�
//�߂�l�F�Ȃ�
//�����@�F���̍s�ֈړ�����B
void	TextLoader::nextLine()
{
	while(true){
		if( nowPoint >= fileSize ) return;
		if( fileBuf[nowPoint] == '\n' ) break;
		if( fileBuf[nowPoint] == '\r' ) break;
		nowPoint++;
	}
}

//�����@�F�Ȃ�
//�߂�l�F�Ȃ�
//�����@�F���݂̓Ǎ��ʒu���m�ۂ���B
void TextLoader::Push()
{
	keepPoint = nowPoint;
}

//�����@�F�Ȃ�
//�߂�l�F�Ȃ�
//�����@�F�m�ۂ��Ă���ꏊ�ɓǍ��ʒu���ړ�����B
void TextLoader::Pop()
{
	nowPoint = keepPoint;
}