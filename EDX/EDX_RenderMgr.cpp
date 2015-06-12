#include "../IEX/iextreme.h"
#include "EDXLIB.h"

BOOL EDX::EDX_RenderMgr::ScreenUpdate(){
	BOOL success;
	success = EndSceneModule();
	success = BeginSceneModule();
	return success;
}

BOOL EDX::EDX_RenderMgr::BeginSceneModule(){
	iexSystem::GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET| D3DCLEAR_ZBUFFER,_ARGB(0,0, 0, 0), 1.0f, 0);
	iexSystem::GetDevice()->BeginScene();
	return TRUE;
}

BOOL EDX::EDX_RenderMgr::EndSceneModule(){
	iexSystem::GetDevice()->EndScene();
	if (FAILED(iexSystem::GetDevice()->Present(NULL, NULL, NULL, NULL))){
		//iexSystem::ResetDevice();
		//// ���A�\�H
		//		if (iexSystem::GetDevice()->TestCooperativeLevel() == D3DERR_DEVICENOTRESET) {
		//		// �����Ńf�o�C�X�̍Đ������摜�Ƃ��f�ނ̍ă��[�h
		//		}

	//	return FALSE;
	}
	return TRUE;
}

void EDX::EDX_RenderMgr::SetRenderType(RT RenderType){
	switch (RenderType){
	case TYPE_DEFAULT:
		//�A���t�@�u�����h����			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Z�o�b�t�@�Q�ƗL��
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Z�o�b�t�@�������ݗL��
		RM::SetRS(RM::DEPTHWRITE_ENABLE);
		//�A���t�@�e�X�g���Ȃ�
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//�f�B�U�L��
		RM::SetRS(RM::DITHER_ENABLE);
		//�J�����O����
		RM::SetRS(RM::CULL_DISABLE);
		//�@�����K������
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//���`�⊮
		RM::SetRS(RM::LINEAR_ENABLE);
		//���Z���Ȃ�
		RM::SetRS(RM::ADD_DISABLE);
		break;
	case TYPE_ADD:
		//�A���t�@�u�����h����			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Z�o�b�t�@�Q�ƗL��
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Z�o�b�t�@�������ݗL��
		RM::SetRS(RM::DEPTHWRITE_ENABLE);
		//�A���t�@�e�X�g���Ȃ�
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//�f�B�U�L��
		RM::SetRS(RM::DITHER_ENABLE);
		//�J�����O����
		RM::SetRS(RM::CULL_DISABLE);
		//�@�����K������
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//���`�⊮
		RM::SetRS(RM::LINEAR_ENABLE);
		//���Z����
		RM::SetRS(RM::ADD_ENABLE);
		break;
	case TYPE_2DEFFECT:
		//�A���t�@�u�����h����			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Z�o�b�t�@�Q�ƗL��
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Z�o�b�t�@�������ݖ���
		RM::SetRS(RM::DEPTHWRITE_DISABLE);
		//�A���t�@�e�X�g���Ȃ�
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//�f�B�U�L��
		RM::SetRS(RM::DITHER_ENABLE);
		//�J�����O����
		RM::SetRS(RM::CULL_DISABLE);
		//�@�����K������
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//���`�⊮
		RM::SetRS(RM::LINEAR_ENABLE);
		//���Z���Ȃ�
		RM::SetRS(RM::ADD_DISABLE);
		break;
	case TYPE_2DADDEFFECT:
		//�A���t�@�u�����h����			
		RM::SetRS(RM::ALPHA_ENABLE);
		//Z�o�b�t�@�Q�ƗL��
		RM::SetRS(RM::DEPTHREAD_ENABLE);
		//Z�o�b�t�@�������ݖ���
		RM::SetRS(RM::DEPTHWRITE_DISABLE);
		//�A���t�@�e�X�g���Ȃ�
		RM::SetRS(RM::ALPHATEST_DISABLE);
		//�f�B�U�L��
		RM::SetRS(RM::DITHER_ENABLE);
		//�J�����O����
		RM::SetRS(RM::CULL_DISABLE);
		//�@�����K������
		RM::SetRS(RM::NORMALIZE_DISABLE);
		//���`�⊮
		RM::SetRS(RM::LINEAR_ENABLE);
		//���Z���Ȃ�
		RM::SetRS(RM::ADD_ENABLE);
		break;
	}
}

void EDX::EDX_RenderMgr::SetRS(RS State){
	switch (State){
	case ADD_DISABLE:
		iexSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		iexSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		break;
	case ADD_ENABLE:
		iexSystem::GetDevice()->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		iexSystem::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		break;
	case ALPHA_ENABLE://�A���t�@�u�����h
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		iexSystem::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		break;
	case ALPHA_DISABLE://�A���t�@�u�����h
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		break;
	case DEPTHREAD_ENABLE://Z�o�b�t�@�ɏ]���ĕ`��
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, TRUE);
		break;
	case DEPTHREAD_DISABLE://Z�o�b�t�@�𖳎����ĕ`��
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);
		break;
	case DEPTHWRITE_ENABLE://Z�o�b�t�@�ɏ�������
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		break;
	case DEPTHWRITE_DISABLE://Z�o�b�t�@�ɏ������܂Ȃ�
		iexSystem::GetDevice()->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		break;
	case ALPHATEST_ENABLE://�A���t�@�e�X�g
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0xfe);
		break;
	case ALPHATEST_DISABLE://�A���t�@�e�X�g�Ȃ�
		iexSystem::GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		break;
	case DITHER_ENABLE://�f�B�U
		iexSystem::GetDevice()->SetRenderState(D3DRS_DITHERENABLE, TRUE);
		break;
	case DITHER_DISABLE://�f�B�U�Ȃ�
		iexSystem::GetDevice()->SetRenderState(D3DRS_DITHERENABLE, FALSE);
		break;
	case CULL_ENABLE://�J�����O
		iexSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		break;
	case CULL_DISABLE://�J�����O�Ȃ�
		iexSystem::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		break;
	case NORMALIZE_ENABLE://�@�����K��
		iexSystem::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
		break;
	case NORMALIZE_DISABLE://�@�����K���Ȃ�
		iexSystem::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
		break;
	case LINEAR_ENABLE://���`�⊮
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		break;
	case POINT_ENABLE://�_�⊮
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		iexSystem::GetDevice()->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		break;
	}
}