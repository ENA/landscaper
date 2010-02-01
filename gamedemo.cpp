#include "gamedemo.h"

namespace space{

void GameDemo::init(){
	//�w�b�_�`��
	//�o�[�W����(4) + �����_���V�[�h(4) + �v���C�l��(1) + ���_���(�v���C�l��) + ���O(�v���C�l��*(1+���O�̒���))

	//�v���C���[�ǂ�
	std::vector<GameMain::Player> pls;
	
	//�܂��o�[�W����
	unsigned int version;
	repfile.read((char*)&version,4);
	//�قȂ����玀�ʁB
	//if(version!=systemmain->version)endflg = true;

	//�����̎�
	unsigned int randomseed;
	repfile.read((char*)&randomseed,4);
	//���l��
	unsigned char playersize;
	repfile.read((char*)&playersize,1);
	//�l�����������_���
	for(unsigned int i=0;i<playersize;i++){
		GameMain::Player pl;
		unsigned int tob=0;
		repfile.read((char*)&tob,1);
		pl.typeofBase = tob;
		//���łɓ��͂�n���Ă����B
		systemmain->input[i+1].clear();
		pl.input = &systemmain->input[i+1];
		pls.push_back(pl);
	}
	//���O
	for(unsigned int i=0;i<playersize;i++){
		unsigned char size;
		char str[256];
		repfile.read((char*)&size,1);
		repfile.read(str,size);
		str[size]='\0';
		pls[i].name = str;
	}

	//�Q�[�����C���쐬
	gamemain = new GameMain(systemmain,pls,0,randomseed,false);
	systemmain->nowstates.push_back(gamemain);
	dispdemo = new StateDisplayDemo(systemmain);
	systemmain->nowstates.push_back(dispdemo);
}

GameDemo::GameDemo(ProgramSystemMain *_sys,std::string _rep)
	: SystemState(_sys,GAMEDEMO)
{
	repfile.open(_rep.c_str(),std::ios::in|std::ios::binary);
	//�ǂ��ł��悳�����ȃJ�E���^
	FinishBegin_cnt = UINT_MAX;
	//�t���O
	endflg = false;
}

GameDemo::~GameDemo(void)
{
	repfile.close();
}

void GameDemo::draw(){
	//�Ȃɂ������Ⴂ���܂���B
}

void GameDemo::update(){

	if(counter==0)init();

	if(!endflg){

		//===========
		//���͂̂���(���v���C�t�@�C������ǂݍ���)
		//==========
		//���v���C�o�C�i���`�� = �t���[����؂�(1) + ( �f�[�^�T�C�Y(1)+�}�E�X�n�_(2+2)+�ǉ��`�����N(�f�[�^�T�C�Y-4) ) (�l��)
		unsigned char pnum,dsize;
		//�l����
		for(unsigned i=0;i<gamemain->player.size();i++)
		{
			//playernum
			pnum = i;
			//�f�[�^�T�C�Y
			repfile.read((char*)&dsize,1);

			//�f�[�^��5�Ŋ���Ȃ��Ƃ�,�I���t���O�𗧂Ă�B
			if(dsize%5!=0){endflg=true;break;}
			//�f�[�^������ꍇ�͓ǂݍ���
			for(int i=0;i<dsize/5;i++){
				unsigned char button;
				Matrix21<int> bpos(0,0);
				//�{�^��������or�Ђ����f�[�^
				repfile.read((char*)&button,1);
				//���̈ʒu
				repfile.read((char*)&bpos.x,2);
				repfile.read((char*)&bpos.y,2);
				//�{�^���ɂ���ĕς���
				//0,1,2(���A�^�񒆁A�E)������
				//4,5,6(���A�^�񒆁A�E)������
				//3(�}�E�X�̈ʒu)
				switch(button){
					case 0:
						systemmain->input[pnum+1].mouse.setleft().push(bpos);
						break;
					case 1:
						systemmain->input[pnum+1].mouse.setmiddle().push(bpos);
						break;
					case 2:
						systemmain->input[pnum+1].mouse.setright().push(bpos);
						break;
					case 3:
						systemmain->input[pnum+1].mouse.setpos(bpos);
						break;
					case 4:
						systemmain->input[pnum+1].mouse.setleft().pull(bpos);
						break;
					case 5:
						systemmain->input[pnum+1].mouse.setmiddle().pull(bpos);
						break;
					case 6:
						systemmain->input[pnum+1].mouse.setright().pull(bpos);
						break;
				}
			}
		}

	}


	//====================
	//�Ƃ��ɂȂ����ǂ܂��B
	//====================
	//�J�E���^����蒙�܂�����ʂ̉�ʂɈڍs�����肷��B
	if(counter>FinishBegin_cnt){
		if(counter-FinishBegin_cnt > 30){
			//���ցB
			isNeed = false;
			gamemain->isNeed = false;
			dispdemo->isNeed = false;
			systemmain->setTitle();
			//�t�F�[�h�C��
			systemmain->setFadeIn(30);
		}
	}
	//����ȊO�͂����ʂ�B
	else{
		static bool push=false;
		//�Ȃ񂩉����ꂽ��,�܂��͏������������Ă���
		if( systemmain->input[0].mouse.getleft().isPush() || systemmain->input[0].keyboard.ispush(SDLK_RETURN) || (gamemain->deadcount+1>=gamemain->player.size()) || repfile.eof() || endflg){
			//�����
			FinishBegin_cnt = counter;
			//�t�F�[�h�A�E�g
			systemmain->setFadeOut(30);
			systemmain->sound_bgm.fadeout(480);
		}
		if( systemmain->input[0].mouse.getright().isPush()){
			if(!push){
				gamemain->player_ctrl++;
				if(gamemain->player_ctrl >= gamemain->player.size())gamemain->player_ctrl=0;
				gamemain->changePlayerCtrl(gamemain->player_ctrl);
				push = true;
			}
		}
		else{push = false;}
	}

	//�J�E���^
	counter++;

	return;
}

}//end of namespace space
