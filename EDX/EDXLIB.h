#pragma once

///////////////////////////////////////////
//										 //
//				EDX_Input				 //
//										 //
///////////////////////////////////////////
int MouseGet(int butontype);
enum{
	EDX_CLICK_L, EDX_CLICK_R
};

class EDX_Input{
	//�}�E�X���͏�Ԃ̕ۑ�
	static int Click[2];
public:
	static BOOL GetState();
	static unsigned int GetClickModule(int BUTTON_TYPE);
};