
/******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include "file.h"
#include "score.h"
#include "title.h"
#include "sound.h"
#include "showinput.h"
/*******************************************************************************
* �}�N����`
*******************************************************************************/



/*******************************************************************************
* �\���̒�`
*******************************************************************************/



/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/



/*******************************************************************************
�}�N����`
*******************************************************************************/



/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
SAVEDATA	save;					// �Z�[�u�f�[�^�쐬�ꏊ
static BOOL g_Loadcheck = FALSE;	//�ǂݍ��ݐ����H

/*******************************************************************************
�֐���:	void SaveData( void )
����:	void
�߂�l:	void
����:	�Z�[�u�f�[�^���쐬���A�t�@�C���֏o�͂���
*******************************************************************************/
void SaveData(void)
{

	//�N���A����stage���L�^
	if (*GetStageClear() > *GetStage())
	{
		save.stage = *GetStageClear();
	}
	else
	{
		save.stage = *GetStage();
	}
	if (save.stage)
	{
		save.cursor = 1;
	}
	else {
		save.cursor = 0;
	}
	{
		save.vm = GetVolume()->master;
		save.vb = GetVolume()->BGM;
		save.vs = GetVolume()->SE;
	}
	{
		save.SI = *GetShowSI();
	}
	// �Z�[�u�f�[�^�̃`�F�b�N�T�������߂�
	{
		char* adr = (char*)&save;	// �f�[�u�f�[�^�̐擪�A�h���X��adr�ɓ����
		int  sum = 0;				// �v�Z����`�F�b�N�T��

		save.sum = 0;				// �Z�[�u�f�[�^���̃`�F�b�N�T�����O�ŃN���A���Ă���

		for (int i = 0; i < sizeof(SAVEDATA); i++)
		{
			sum += adr[i];
		}

		save.sum = sum;
	}



	// SAVEDATA�\���̂��ƑS�����t�@�C���ɏo�͂���
	FILE* fp;

	printf("\n�Z�[�u�J�n�E�E�E");
	fp = fopen("savedata.bin", "wb");			// �t�@�C�����o�C�i���������݃��[�h��Open����

	if (fp != NULL)								// �t�@�C��������Ώ������݁A������Ζ���
	{
		fwrite(&save, 1, sizeof(SAVEDATA), fp);	// �w�肵���A�h���X����w�肵���o�C�g�����t�@�C���֏�������
		fclose(fp);								// Open���Ă����t�@�C�������
		printf("�I���I\n");
	}
	else
	{
		printf("�t�@�C���G���[�I\n");
	}

}


/*******************************************************************************
�֐���:	void LoadData( void )
����:	void
�߂�l:	void
����:	�Z�[�u�f�[�^���t�@�C������ǂݍ���
*******************************************************************************/
void LoadData(void)
{
	//PLAYER *player = GetPlayer();	// �v���C���[�̃A�h���X���擾����
	//ENEMY  *enemy  = GetEnemy();	// �G�l�~�[�̃A�h���X���擾����

	// �t�@�C������Z�[�u�f�[�^��ǂݍ���
	FILE* fp;

	printf("\n���[�h�J�n�E�E�E");
	fp = fopen("savedata.bin", "rb");	// �t�@�C�����o�C�i���ǂݍ��݃��[�h��Open����

	if (fp != NULL)						// �t�@�C��������Ώ������݁A������Ζ���
	{
		fread(&save, 1, sizeof(SAVEDATA), fp);	// �w�肵���A�h���X�֎w�肵���o�C�g�����t�@�C������ǂݍ���
		fclose(fp);								// Open���Ă����t�@�C�������
		printf("�I���I\n");
		g_Loadcheck = TRUE;

	}
	else
	{
		//*GetStage() = -1;
		printf("�t�@�C���G���[�I\n");
	}

	if (g_Loadcheck)
	{
		// �Z�[�u�f�[�^�̃`�F�b�N�T���������Ă��邩���ׂ�
		{
			char* adr = (char*)&save;	// �f�[�u�f�[�^�̐擪�A�h���X��adr�ɓ����
			int  sum = 0;				// �v�Z����`�F�b�N�T��
			int  org = save.sum;		// �Z�[�u�f�[�^���̃`�F�b�N�T��

			save.sum = 0;				// �Z�[�u�f�[�^���̃`�F�b�N�T�����O�ŃN���A���Ă���

			for (int i = 0; i < sizeof(SAVEDATA); i++)
			{
				sum += adr[i];
			}

			// ���X�̃`�F�b�N�T���ƍČv�Z�����`�F�b�N�T�������������ׂĂ���
			if (sum != org)
			{
				// �f�[�^�������񂳂�Ă���I
				return;
			}
		}

		{
			*GetStageClear() = save.stage;
		}

		{
			*Get_g_Cursor() = save.cursor;

		}

		{
			GetVolume()->master = save.vm;
			GetVolume()->BGM = save.vb;
			GetVolume()->SE = save.vs;
		}

		{
			*GetShowSI() = save.SI;
		}
	}
	g_Loadcheck = FALSE;
}

BOOL GetLoadCheck(void)
{
	return g_Loadcheck;
}


