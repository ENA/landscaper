#include "systemstate.h"
#include "gamemain.h"
#include "network.h"

namespace space
{

	PawnPeople* GameMain::setPawn(Object::PLAYERID _pid, double _x, double _y){
		objects.push_back(new PawnPeople(this,_pid,_x,_y) );
		return pointer_cast<PawnPeople*>(objects.back());
	}
	NormalBullet* GameMain::setNormalBullet(Object::PLAYERID _pid, double _x, double _y, double vx, double vy){
		objects.push_back(new NormalBullet(this,_pid,_x,_y,vx,vy) );
		return pointer_cast<NormalBullet*>(objects.back());
	}
	NormalBuilding* GameMain::setNormalBuilding(Object::PLAYERID _pid, double _x,double _y){
		objects.push_back(new NormalBuilding(this,_pid,_x,_y) );
		return pointer_cast<NormalBuilding*>(objects.back());
	}
	Fortress* GameMain::setFortress(Object::PLAYERID _pid, double _x,double _y){
		objects.push_back(new Fortress(this,_pid,_x,_y) );
		return pointer_cast<Fortress*>(objects.back());
	}
	Bomb* GameMain::setBomb(Object::PLAYERID _pid, double _x,double _y){
		objects.push_back(new Bomb(this,_pid,_x,_y) );
		return pointer_cast<Bomb*>(objects.back());
	}
	GroundBomb* GameMain::setGroundBomb(Object::PLAYERID _pid, double _x,double _y){
		objects.push_back(new GroundBomb(this,_pid,_x,_y) );
		return pointer_cast<GroundBomb*>(objects.back());
	}
	Shop* GameMain::setShop(Object::PLAYERID _pid, double _x,double _y){
		objects.push_back(new Shop(this,_pid,_x,_y) );
		return pointer_cast<Shop*>(objects.back());
	}
	Smith* GameMain::setSmith(Object::PLAYERID _pid,double _x,double _y){
		objects.push_back(new Smith(this,_pid,_x,_y));
		return pointer_cast<Smith*>(objects.back());
	}

	void GameMain::changePlayerCtrl(unsigned int ch){
		player_ctrl = ch;
		item[1].r.x=0; item[1].r.y=32*player_ctrl; item[1].r.w=32; item[1].r.h=32;
		item[2].r.x=0; item[2].r.y=32*player_ctrl; item[2].r.w=32; item[2].r.h=32;
		item[3].r.x=0; item[3].r.y=32*player_ctrl; item[3].r.w=32; item[3].r.h=32;
		item[4].r.x=0; item[4].r.y=32*player_ctrl; item[4].r.w=32; item[4].r.h=32;
		item[17].r.x=0; item[17].r.y=0; item[17].r.w=32; item[17].r.h=32;
		item[18].r.x=0; item[18].r.y=32*player_ctrl; item[18].r.w=32; item[18].r.h=32;
	}

	GameMain::GameMain(ProgramSystemMain* _sys, std::vector<Player> _pl,unsigned int _ctrlpl, unsigned int _rands,bool _rep)
		: SystemState(_sys,GAMEMAIN)
	{
		setrep = _rep;

		//�����p�̃t�H���g���[�h
		moneyfont.open("image/number_large.gif");
		moneyfont.fontsetting(18,20,-'0');

		//�X�e�[�^�X�p�̃t�H���g
		statusfont.open("image/status_num.gif");
		statusfont.fontsetting(9,10,-'0');
		
		//�v���C���[�̐ݒ�Ƃ��B
		player = _pl;	//�v���C���[
		player_ctrl = _ctrlpl;	//�����̔ԍ�
		randamseed = _rands;	//�����_���V�[�h
		srand(randamseed);	//����܂�
		objects.clear();

		//�t�B�[���h�T�C�Y�A�y�т��̑��̉�ʃT�C�Y������
		//�t�B�[���h�T�C�Y�B�����ڂƕ������ĂȂ��B����͎�����ς��Ȃ��Ă͂Ȃ�Ȃ��B
		//�I�t�Z�b�g�������������߁A�኱�}�V�ɂȂ�܂����B
		field_rect.w=440; field_rect.h=420;
		field_rect.x=10;  field_rect.y=systemmain->screen->h-(field_rect.h+10);
		//�e�v���C���[�̃X�e�[�^�X�̈�
		states_rect.x=10; states_rect.y=10;
		states_rect.w=field_rect.w;
		states_rect.h=field_rect.y-20;
		//�����\���̈�
		money_rect.x = states_rect.x + states_rect.w + 10 + 4;
		money_rect.y = 10;
		money_rect.w = 162;
		money_rect.h = 37;
		//�R���p�l�̈�
		control_rect.x = money_rect.x;
		control_rect.y = money_rect.y + money_rect.h + 5;
		control_rect.w=162;
		control_rect.h=155;
		//���̑��f�[�^�\���̈�
		data_rect.x = control_rect.x;
		data_rect.y = control_rect.y + control_rect.h + 20;
		data_rect.w=162;
		data_rect.h=213;
		//�A�C�e���I������Ƃ���̍��W�w��
		for(unsigned int i=0;i<12;i++){
			items_rect[i].x =  control_rect.x + control_rect.w/2 - 74 + (i%4)*38;
			items_rect[i].y =  control_rect.y + control_rect.h/2 - 56 + (i/4)*38;
			items_rect[i].w =  36;
			items_rect[i].h =  36;
		}

		//�܂��ςȏ������i�A�C�e���̎�ޕ\���j
		{
			//��ޕύX�̓����蔻��̂��
			flippos = WinRect(control_rect.x,control_rect.y+control_rect.h-14,control_rect.w,28);

			//�A�C�e���̎�ޕ\������Ƃ��̉摜�̔z��
			std::vector<const SDLSURFACE*> &sv = sortofitems_drawpict;

			//����
			sv.push_back(&SDLFILESRFS::inst().get("image/building.gif"));
			//���̑�
			sv.push_back(&SDLFILESRFS::inst().get("image/supply.gif"));
		}


		//�����I�u�W�F�N�g�ݒu
		//�e�v���C���[�Ɋւ���
		for(unsigned int i=0;i<player.size();i++)
		{
			player[i].bmousepos.x = player[i].input->mouse.getpos().x+500;
			player[i].bmousepos.y = player[i].input->mouse.getpos().y+500;
			for(int j=0;j<12;j++)player[i].shownitems[j] = j+1; //�����A�C�e��
			player[i].sortofitems = 0; //�A�C�e���̎��
			player[i].score = 0; //�X�R�A������
			player[i].bekilled_cnt = 0; //�������ꂽ��������
			player[i].killed_cnt = 0; //����������������
			player[i].spendmoney = 0; //�g�p���z������
			player[i].money = 300; //������������
			player[i].selected_item = 0; //�I���A�C�e��������
			for(unsigned int j=0;j<64;j++){player[i].items[j]=0;} //�擾�A�C�e��������
			player[i].selected.clear(); //�I��̈��������
			player[i].drawoffset=Matrix21<double>(0.0,0.0); //�t�B�[���h�̕\���ʒu
			player[i].drawoffset_b = player[i].drawoffset;
			player[i].pushtime_left = 0;
			player[i].pushtime_middle = 0;
			player[i].pushtime_right = 0;
			player[i].pushtime_key.clear();
			player[i].ispull_left = false;
			player[i].ispull_middle = false;
			player[i].ispull_right = false;
			player[i].ispull_key.clear();
			//���_�ݒu
			player[i].base = new PlayerBase(this,i+1,
				field_rect.w/2 + cos(2.0*(double)i*M_PI/(double)player.size())*(double)(field_rect.w/16*7) ,
				field_rect.h/2 + sin(2.0*(double)i*M_PI/(double)player.size())*(double)(field_rect.h/16*7) );
			objects.push_back( player[i].base );
		}

		//�t�B�[���h�̕��̐ݒu
		//�ΎR
		objects.push_back( new FieldVolcano(this,field_rect.w/2,field_rect.h/2) );
		//��
		for(int i=0;i<10;i++){
			double r = (double)(rand()%220+20);
			double theta = (double)(rand()%36000)/18000.0*M_PI;
			objects.push_back( new FieldRock(this,r*cos(theta)+field_rect.w/2,r*sin(theta)+field_rect.h/2-20) );
		}
		//��
		for(int i=0;i<10;i++){
			double r = (double)(rand()%220+20);
			double theta = (double)(rand()%36000)/18000.0*M_PI;
			objects.push_back( new FieldTree(this,r*cos(theta)+field_rect.w/2,r*sin(theta)+field_rect.h/2-20) );
		}
		//�T�{�e��
		for(int i=0;i<10;i++){
			double r = (double)(rand()%220+20);
			double theta = (double)(rand()%36000)/18000.0*M_PI;
			objects.push_back( new FieldSaboten(this,r*cos(theta)+field_rect.w/2,r*sin(theta)+field_rect.h/2-20) );
		}

		//BGM
		systemmain->sound_bgm.load("sound/bgm.wav");
		systemmain->sound_bgm.play(-1);
		
		//�A�C�e���ݒ�
		for(int i=0;i<64;i++){
			item.push_back(Item());
		}
			
		//����
		item[1].obj = new space::NormalBuilding();
		item[1].showpic = &(SDLFILESRFS::inst().get("image/ie_all.gif"));
		item[2].obj = new space::Fortress();
		item[2].showpic = &SDLFILESRFS::inst().get("image/yousai_all.gif");
		item[3].obj = new space::Shop();
		item[3].showpic = &SDLFILESRFS::inst().get("image/shop_all.gif");
		item[4].obj = new space::Smith();
		item[4].showpic = &SDLFILESRFS::inst().get("image/smith_all.gif");
		//�A�C�e��
		item[17].obj = new space::Bomb();
		item[17].showpic = &SDLFILESRFS::inst().get("image/bomb.gif");
		item[18].obj = new space::GroundBomb();
		item[18].showpic = &SDLFILESRFS::inst().get("image/groundbomb.gif");

		//�摜�ݒ�͂��̒�
		changePlayerCtrl(player_ctrl);

		//�I���悤�J�E���g
		forEnd_cnt = 240;

		//�����A�����蔻��̎��Ԃ�
		mortontree.Init(8, -250.0f, -250.0f, 750.0f, 750.0f);

		if(setrep){
			//���v���C�t�@�C���p��
			std::stringstream ss;
			ss << "replay/" << player_ctrl << time(NULL) << ".rep";
			replayfile.open(ss.str().c_str(),std::ios::out|std::ios::binary);
			//�w�b�_�`��
			//�o�[�W����(4) + �����_���V�[�h(4) + �v���C�l��(1) + ���_���(�v���C�l��) + ���O(�v���C�l��*(1+���O�̒���))
			replayfile.write((char*)&systemmain->version,4);
			replayfile.write((char*)&randamseed,4);
			int t = player.size();
			replayfile.write((char*)&t,1);
			for(unsigned int i=0;i<player.size();i++){replayfile.write((char*)&player[i].typeofBase,1);}
			for(unsigned int i=0;i<player.size();i++){
				int t = player[i].name.size();
				replayfile.write((char*)&t,1);
				replayfile.write(player[i].name.c_str(),player[i].name.size());
			}
		}

		deadcount = 0;

	}

	GameMain::~GameMain()
	{
		while(!objects.empty())
		{
			delete objects.back();
			objects.pop_back();
		}
		systemmain->sound_bgm.stop();
		//�����Ń��v���C�t�@�C����������ꍇ�́A�܂萳��ɃQ�[�����I�����Ă��Ȃ��B
		if(replayfile.is_open()) replayfile.close();
	}

	void GameMain::DisplayReload()
	{
	}

	void GameMain::setting3D()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(15.0, 1.0, 0.1, 500.0);
	
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glViewport(410,250,200,200);
	}

	void GameMain::update()
	{
		//���S�m�F�J�E���g
		deadcount = 0;
		//�悭�g������
		std::list<Object*>::iterator _it,_itj;

		//====================
		//�S�v���C���[�ɑ΂���
		//====================
		//������̂��ꂱ��
		//���łɃ��v���C�ۑ�
		static int pss = 2;
		for(unsigned char i=0;i<player.size();i++)
		{
			
			//���͂̂���
			Input ctrinp = *player[i].input;
			RawData rd,rlast;
			rd.clear(),rlast.clear();
			unsigned char inp;

			//�}�E�X�ʒu
			if( player[i].bmousepos!=ctrinp.mouse.getpos()){
				inp = 3;
				rd.write(&inp,1);
				rd.write(&ctrinp.mouse.getpos().x,pss);
				rd.write(&ctrinp.mouse.getpos().y,pss);
				player[i].bmousepos = ctrinp.mouse.getpos();
			}

			//mouse
			//LEFT
			if( ctrinp.mouse.getleft().isPush() ){
				player[i].pushtime_left++;
				if(player[i].pushtime_left == 1){
					inp = 0;
					rd.write(&inp,1);
					rd.write(&ctrinp.mouse.getleft().getPushpos().x,pss);
					rd.write(&ctrinp.mouse.getleft().getPushpos().y,pss);
				}
			}
			else{
				if(player[i].pushtime_left!=0){
					player[i].ispull_left=true;
					inp = 4;
					rd.write(&inp,1);
					rd.write(&ctrinp.mouse.getleft().getPullpos().x,pss);
					rd.write(&ctrinp.mouse.getleft().getPullpos().y,pss);
				}
				else player[i].ispull_left=false;
				player[i].pushtime_left = 0;
			}
			//MIDDLE
			if( ctrinp.mouse.getmiddle().isPush() ){
				player[i].pushtime_middle++;
				if(player[i].pushtime_middle == 1){
					inp = 1;
					rd.write(&inp,1);
					rd.write(&ctrinp.mouse.getmiddle().getPushpos().x,pss);
					rd.write(&ctrinp.mouse.getmiddle().getPushpos().y,pss);
				}
			}
			else{
				if(player[i].pushtime_middle!=0){
					player[i].ispull_middle=true;
					inp = 5;
					rd.write(&inp,1);
					rd.write(&ctrinp.mouse.getmiddle().getPullpos().x,pss);
					rd.write(&ctrinp.mouse.getmiddle().getPullpos().y,pss);
				}
				else player[i].ispull_middle=false;
				player[i].pushtime_middle = 0;
			}
			//RIGHT
			if( ctrinp.mouse.getright().isPush() ){
				player[i].pushtime_right++;
				if(player[i].pushtime_right == 1){
					inp = 2;
					rd.write(&inp,1);
					rd.write(&ctrinp.mouse.getright().getPushpos().x,pss);
					rd.write(&ctrinp.mouse.getright().getPushpos().y,pss);
				}
			}
			else{
				if(player[i].pushtime_right!=0){
					player[i].ispull_right=true;
					inp = 6;
					rd.write(&inp,1);
					rd.write(&ctrinp.mouse.getright().getPullpos().x,pss);
					rd.write(&ctrinp.mouse.getright().getPullpos().y,pss);
				}
				else player[i].ispull_right=false;
				player[i].pushtime_right = 0;
			}

			//���v���C�o�C�i���`�� = ( �f�[�^�T�C�Y(1)+�`�����N(�f�[�^�T�C�Y) ) (�l��)
			unsigned char appendsize = (unsigned char)rd.size();
			rlast.write(&appendsize,1);
			if(appendsize)rlast.write(&rd[0],rd.size());
			//��������
			if(setrep) replayfile.write((char*)&rlast[0],rlast.size());

			//key
			const Keyboard::KeyMap &km = ctrinp.keyboard.getkeys();
			std::map<unsigned short,unsigned short> &now = player[i].pushtime_key;
			//�؂�
			std::map<unsigned short,unsigned short>::iterator nowit = now.begin();
			for(;nowit!=now.end();nowit++)
			{
				if( km.find(nowit->first) == km.end() ){
				  std::map<unsigned short,unsigned short>::iterator tit = nowit;
				  nowit--;now.erase(tit);
				  if(nowit==now.end())break;
				}
				else { nowit->second++; }
			}
			//�ǉ�����
			Keyboard::KeyMap::const_iterator kmit = km.begin();
			for(;kmit!=km.end();kmit++)
			{
				if( now.find(kmit->first) == now.end() ) now[kmit->first]=1;
			}

			//���ŁB
			//���S���ǂ����̊m�F�J�E���g
			if(player[i].base->hp <= 0)deadcount++;
		}


		//�S�����͈�l�ȊO����ł�����A�J�E���g�Ŏ��ɐi�ށB
		if(deadcount + 1 >= player.size())
		{
			//���v���C��~
			int rnd = rand();
			replayfile.write((char*)&rnd,4);
			replayfile.close();

			//�I���܂ł̃J�E���g�����ȂǁB
			if(forEnd_cnt==0){
				isNeed = false;
				systemmain->setResult(this);
				systemmain->setFadeIn(30);
				systemmain->sound_bgm.stop();
			}
			else if(forEnd_cnt==29){
				systemmain->setFadeOut(30);
				systemmain->sound_bgm.fadeout(480);
			}
			forEnd_cnt--;
		}

		//�����łȂ���Έȉ��̃Q�[�����������s����B
		else{

			//���ۂ̑���ɂ���
			for(unsigned int i=0;i<player.size();i++)
			{
				Input ctrinp = *player[i].input;

				//�E�N���b�N���ꂽ���̓���
				if(player[i].pushtime_right == 1)
				{
					//�A�C�e���I���͉����B
					player[i].selected_item = 0;
					//�t�B�[���h�ゾ�����Ƃ�
					Matrix21<int> pushpos = ctrinp.mouse.getright().getPushpos();
					if( field_rect.Col_Dot( pushpos ) ){
						std::list<Object*>::iterator it = player[i].selected.begin();
						for(;it!=player[i].selected.end();it++){
							if((*it)->get_class_id() == Object::PEOPLE){
								People* temp = pointer_cast<People*>(*it);
								temp->dstpos.x = pushpos.x-field_rect.x-player[i].drawoffset.x;
								temp->dstpos.y = pushpos.y-field_rect.y-player[i].drawoffset.y;
							}
						}
					}
				}

				//���N���b�N�ŏ��ɉ�����铮��
				if(player[i].pushtime_left == 1)
				{
					//�R���g���[������������
					if( control_rect.Col_Dot( ctrinp.mouse.getleft().getPushpos() ) )
					{
						for(int j=0;j<12;j++)
						{
							//�A�C�e��������������
							if(items_rect[j].Col_Dot( ctrinp.mouse.getleft().getPushpos() ) ){
								player[i].selected_item = player[i].shownitems[j];
							}
						}
					}

					//�t�B�[���h���Ȃ��
					else if( field_rect.Col_Dot( ctrinp.mouse.getleft().getPushpos() ) )
					{
						//�I�����Ă���A�C�e�������������̑���B
						if(player[i].selected_item != 0)
						{
							//�������ʒu
							double x = (double)ctrinp.mouse.getleft().getPushpos().x - (double)field_rect.x - player[i].drawoffset.x;
							double y = (double)ctrinp.mouse.getleft().getPushpos().y - (double)field_rect.y - player[i].drawoffset.y;
							bool setaccept = false;

							//������͈͂��ǂ���
							for(_it=objects.begin();_it!=objects.end();_it++){
								if((*_it)->get_player_id()-1==i){
									Matrix21<double> r( (*_it)->fieldpos.x,(*_it)->fieldpos.y);
									if( (r.x-x)*(r.x-x)+(r.y-y)*(r.y-y) < (*_it)->powerdist*(*_it)->powerdist){
										setaccept=true;
										break;
									}
								}
							}

							//������Ȃ炨��
							if(setaccept){
								Item temp = item[player[i].selected_item];
								//��������ł����Ȃ�
								if(temp.obj && temp.obj->price>0 && player[i].spendMoney(temp.obj->price)){
									//�ݒu
									switch(player[i].selected_item)
									{
									case 1:
										setNormalBuilding(i+1,x,y);
										break;
									case 2:
										setFortress(i+1,x,y);
										break;
									case 3:
										setShop(i+1,x,y);
										break;
									case 4:
										setSmith(i+1,x,y);
										break;
									case 17:
										setBomb(i+1,x,y);
										break;
									case 18:
										setGroundBomb(i+1,x,y);
										break;
									}
								}


							}
						}
						
					}

					//��ޕύX�������ꂽ�Ƃ�
					{
						if( flippos.Col_Dot( ctrinp.mouse.getleft().getPushpos() ) ){
							//���ɐi�߂�
							player[i].sortofitems++;
							if(player[i].sortofitems >= GameMain::sortofitems_drawpict.size())player[i].sortofitems=0;
							//�����̎�
							if(player[i].sortofitems==0){
								for(int k=0;k<12;k++) player[i].shownitems[k]=k+1;
							}
							//�A�C�e���̎�
							else if(player[i].sortofitems==1){
								for(int k=0;k<12;k++) player[i].shownitems[k]=k+17;
							}
						}
					}

				}			
				//���N���b�N�������ꂽ�Ƃ��B
				else if(player[i].ispull_left)
				{
					//�N���b�N�����ꏊ���t�B�[���h���Ȃ��
					if( field_rect.Col_Dot( ctrinp.mouse.getleft().getPushpos() ) )
					{
						//����܂łɑI�����Ă��������
						player[i].selected.clear();
						//�I�u�W�F�N�g�S���Ɠ����蔻��
						std::list<Object*>::iterator it = objects.begin();
						for(;it!=objects.end();it++){
							Matrix21<int> tps = ctrinp.mouse.getleft().getPushpos();
							Matrix21<int> tpl = ctrinp.mouse.getleft().getPullpos();
							WinRect temp(
								tpl.x-field_rect.x-(int)player[i].drawoffset.x,
								tpl.y-field_rect.y-(int)player[i].drawoffset.y,
								tps.x-tpl.x,
								tps.y-tpl.y
							);
							//�������Ă���I��
							if(
								temp.Col_Dot(Matrix21<double>((*it)->fieldpos.x,(*it)->fieldpos.y)) &&
								(*it)->get_player_id()-1 == i
							){
								player[i].selected.push_back(*it);
							}
						}
					}
				}

				//�E�N���b�N�ƍ��N���b�N���������A���N���b�N�������ꂽ�Ƃ�
				if( (player[i].pushtime_left>=1 && player[i].pushtime_right>=1) ||
					(player[i].pushtime_middle>=1) ){
					//������͂���
					//�I�t�Z�b�g���牟�����ʒu�����������̂�ۑ�����B
					//�I�t�Z�b�g�Čv�Z�̎��͂��̂Ƃ��̃}�E�X�̈ʒu�𑫂������ł��ށB
					if(	player[i].pushtime_left==1){
						player[i].drawoffset_b = player[i].drawoffset - (Matrix21<double>)player[i].input->mouse.getleft().getPushpos();
					}
					else if( player[i].pushtime_right==1){
						player[i].drawoffset_b = player[i].drawoffset - (Matrix21<double>)player[i].input->mouse.getright().getPushpos();
					}
					else if( player[i].pushtime_middle==1){
						player[i].drawoffset_b = player[i].drawoffset - (Matrix21<double>)player[i].input->mouse.getmiddle().getPushpos();
					}
					//����ȊO
					else{
						player[i].drawoffset = player[i].drawoffset_b + (Matrix21<double>)player[i].input->mouse.getpos();
					}
				}

			}

			//=======================
			//�I�u�W�F�N�g�S�̂̏���
			//=======================

			//move�S�����s
			_it = objects.begin();
			for(;_it!=objects.end();_it++) (*_it)->move();
			
			//collision�S�����s
			Object** opp;
			int num=CollisionAllObjects(&opp);
			for(int i=0;i<num;i+=2){
				(opp[i]->get_class_id() < opp[i+1]->get_class_id()) ?
				opp[i+1]->collision(opp[i]) : opp[i]->collision(opp[i+1]);
			}

			//after_collision�S�����s
			_it = objects.begin();
			for(;_it!=objects.end();_it++) (*_it)->after_collision();

		}

		//�I�������I�u�W�F�N�g����s�v�Ȃ��̂��폜
		for(unsigned int i=0;i<player.size();i++){
			_it = player[i].selected.begin();
			while(_it!=player[i].selected.end()){
				if((*_it)->isNeed==false) _it=player[i].selected.erase(_it);
				else _it++;
			}
		}

		//�{�̂�����ǂ����K�v�Ȃ����ۂ����̂��폜���܂�
		_it = objects.begin();
		while(_it!=objects.end()){
			if((*_it)->isNeed==false){
				delete *_it;
				_it=objects.erase(_it);
			}
			else _it++;
		}

		//�S�͂Ń\�[�g
		objects.sort(ObjectSortFuncByPos());
		//std::stable_sort(objects.begin(),objects.end(),ObjectSortFuncByPos());

		counter++;
	}

	void GameMain::draw()
	{
		//��ԑ厖�Ȃ���
		std::list<Object*>::iterator _it= objects.begin();

		//===============//
		//===============//
		//SDL�ɂ��`��  //
		//===============//
		//===============//

		//================
		//�Q�[���֌W�̕`��
		//================
		char _str[256];
		//�w�i�̔w�i
		{
			SDL_Rect _sr = {field_rect.x,field_rect.y,field_rect.w,field_rect.h};
			SDL_FillRect(systemmain->screen,NULL,0xff004488);
		}
		//�w�i
		SDL_Rect yr = {
			(Sint16)((double)field_rect.x+field_rect.w/2.0+player[player_ctrl].drawoffset.x)+10,
			(Sint16)((double)field_rect.y+field_rect.h/2.0+player[player_ctrl].drawoffset.y)-45,
			field_rect.w ,field_rect.h
		};
		SDLFILESRFS::inst().get("image/stage.gif").Draw(systemmain->screen,&yr,NULL,systemmain->screen->h);

		//������͈͕\��
		for(_it=objects.begin();_it!=objects.end();_it++){
			if((*_it)->get_player_id()-1==player_ctrl && (*_it)->powerdist>=5.0){
				//�͈͂̂���
				SDL_Rect r = {
					(Sint16)((*_it)->fieldpos.x+(double)field_rect.x+player[player_ctrl].drawoffset.x),
					(Sint16)((*_it)->fieldpos.y+(double)field_rect.y+player[player_ctrl].drawoffset.y),
					0,0
				};
				aacircleRGBA(systemmain->screen,r.x,systemmain->screen->h-r.y,(Sint16)((*_it)->powerdist),128,64,192,255);
				filledCircleRGBA(systemmain->screen,r.x,systemmain->screen->h-r.y,(Sint16)((*_it)->powerdist),128,64,192,32);
			}
		}

		//�S���`��(2D)
		_it= objects.begin();
		for(;_it!=objects.end();_it++)
		{
			//�ʏ�`��
			(*_it)->draw2D();
		}

		//�I�������I�u�W�F�N�g�̕`��
		{
			_it=player[player_ctrl].selected.begin();
			for(;_it!=player[player_ctrl].selected.end();_it++){
				//�ʒu
				SDL_Rect yr = {
					(Sint16)((*_it)->fieldpos.x+(double)field_rect.x+player[player_ctrl].drawoffset.x+(*_it)->maincircle.d/2.0+5.0),
					(Sint16)((*_it)->fieldpos.y+(double)field_rect.y+player[player_ctrl].drawoffset.y+(*_it)->maincircle.d/2.0+5.0),
					0,0
				};
				//�`��
				SDLFILESRFS::inst().get("image/ordermark.gif").Draw(systemmain->screen,&yr,NULL,systemmain->screen->h);
			}
		}


		//�I��g�̕\��
		if( field_rect.Col_Dot(player[player_ctrl].input->mouse.getleft().getPushpos()) && player[player_ctrl].input->mouse.getleft().isPush() )
		{
			Matrix21<int> _psps = player[player_ctrl].input->mouse.getleft().getPushpos();
			Matrix21<int> _plps = player[player_ctrl].input->mouse.getleft().getPullpos();

			if(player[player_ctrl].input->mouse.getleft().isPush())
				_plps=player[player_ctrl].input->mouse.getpos();
			
			unsigned char rgba[4][4] = {
				{0xff,0x00,0x00,0xff},
				{0x00,0x00,0xff,0xff},
				{0x00,0xff,0x00,0xff},
				{0xff,0xff,0x00,0xff}
			};

			rectangleRGBA(systemmain->screen,
				      _psps.x, systemmain->screen->h-_psps.y, _plps.x, systemmain->screen->h-_plps.y,
				      rgba[player_ctrl][0], rgba[player_ctrl][1], rgba[player_ctrl][2], rgba[player_ctrl][3]);
		}
		//�Q�[����ʂ̘g��
		//field_rect.Draw(systemmain->screen,0x44,0xff,0xff,0xff,systemmain->screen->h);

		//===================
		//���̃Q�[���֌W�`��
		//===================
		//�S�̘̂g��
		{
			SDL_Rect temprect = {systemmain->screen->w/2,systemmain->screen->h/2,0,0};
			SDLFILESRFS::inst().get("image/mainframe.gif").Draw(systemmain->screen,&temprect,NULL,systemmain->screen->h);
		}

		//�f�[�^��ʂ̘g��
		//data_rect.Draw(systemmain->screen,0x22,0xaa,0x22,0xff,systemmain->screen->h);
		//�f�[�^��ʂ̔w�i
		{
			SDL_Rect temprect = {data_rect.x+data_rect.w/2-1,data_rect.y+data_rect.h/2+5,0,0};
			SDLFILESRFS::inst().get("image/data.gif").Draw(systemmain->screen,&temprect,NULL,systemmain->screen->h);
			//���_�ɂ���ĉ摜��������
			SDL_Rect ra = {150,0,150,150};
			temprect.y -= 20;
			if(player[player_ctrl].typeofBase==BASETYPE_LOVE)
				SDLFILESRFS::inst().get("image/love.gif").Draw(systemmain->screen,&temprect,&ra,systemmain->screen->h);
			else if(player[player_ctrl].typeofBase==BASETYPE_MONEY)
				SDLFILESRFS::inst().get("image/moneybase.gif").Draw(systemmain->screen,&temprect,&ra,systemmain->screen->h);
			else if(player[player_ctrl].typeofBase==BASETYPE_POWER)
				SDLFILESRFS::inst().get("image/power.gif").Draw(systemmain->screen,&temprect,&ra,systemmain->screen->h);
		}
		//�Ƃ肠�����I���I�u�W�F�N�g���̕\��
		{
			sprintf(_str,"(%d,%d)",player[player_ctrl].input->mouse.getpos().x,player[player_ctrl].input->mouse.getpos().y);
			//sprintf(_str,"selects = %d",player[player_ctrl].selected.size());
			//sprintf(_str,"item = %d",player[player_ctrl].selected_item);
			SDL_Rect _rect= {
				data_rect.x+8,
				data_rect.y+data_rect.h-25,
				0,0};
			systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
		}
		//�Ƃ肠�����I�u�W�F�N�g���̕\��
		{
			sprintf(_str,"objects = %d",objects.size());
			SDL_Rect _rect= {
				data_rect.x+8,
				data_rect.y+data_rect.h-40,
				0,0};
			systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
		}
		//�Ƃ肠�����v���C���[�ԍ�
		{
			sprintf(_str,"ctrl_num = %d",player_ctrl);
			SDL_Rect _rect= {
				data_rect.x+8,
				data_rect.y+data_rect.h-55,
				0,0};
			systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
		}
		//�����\��
		{
			Object* _dscobj;
			Item temp = item[player[player_ctrl].selected_item];
			_dscobj = temp.obj;

			if(_dscobj){
				SDL_Rect _rect= { data_rect.x+8, data_rect.y+data_rect.h-85, 0,0};
				systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,"[ITEM]",systemmain->screen->h);
				_rect.x = data_rect.x + 60;
				//�����\��
				_rect.y-=15; sprintf(_str,"COST:%d",_dscobj->price);
				systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
				_rect.y-=15; sprintf(_str,"LIFE:%d",_dscobj->hp);
				systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
				_rect.y-=15; sprintf(_str," DEF:%d",_dscobj->def);
				systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
				//�A�C�R���\��
				temp.showpic->Draw(
					systemmain->screen,
					&(SDL_Rect)WinRect(data_rect.x+30,data_rect.y+100,0,0),
					&(SDL_Rect)temp.r,
					systemmain->screen->h);
			}
		}		

		//�R���g���[����ʂ̘g��
		//control_rect.Draw(systemmain->screen,0x44,0xff,0x44,0xff,systemmain->screen->h);


		//�I������ĂȂ����̂̂���
		{
			//�A�C�e���̎�ޕ\������Ƃ��̉摜�̔z��
			std::vector<const SDLSURFACE*> &sv = sortofitems_drawpict;
			int j=1;
			for(unsigned int i=0;i<sv.size();i++){
				//�I������ĂȂ��͔̂�΂�
				if(i == player[player_ctrl].sortofitems)continue;
	
				//�R���g���[����ʂ̔w�i(�I�����Ă��Ȃ�����
				SDL_Rect temprect = {control_rect.x+control_rect.w/2+j*3,control_rect.y+control_rect.h/2+j*7,0,0};
				SDL_Rect temprectsrc = {0,155,162,155};
				SDLFILESRFS::inst().get("image/board_bs.gif").Draw(systemmain->screen,&temprect,&temprectsrc,systemmain->screen->h);
				
				//�R���g���[����ʂ̃A�C�e���̎��(�I�����Ă��Ȃ�����)
				SDL_Rect rc = {0,sv[i]->surface->h/2,sv[i]->surface->w,sv[i]->surface->h/2};
				SDL_Rect rd = {control_rect.x+rc.w/2-1+j*3,control_rect.y+control_rect.h-2+rc.h/2+j*7-8,0,0};
				sv[i]->Draw(systemmain->screen,&rd,&rc,systemmain->screen->h);
				
				j++;
			}
		}

		//�R���g���[����ʂ̔w�i
		{
			SDL_Rect temprect = {control_rect.x+control_rect.w/2,control_rect.y+control_rect.h/2,0,0};
			SDL_Rect temprectsrc = {0,0,162,155};
			SDLFILESRFS::inst().get("image/board_bs.gif").Draw(systemmain->screen,&temprect,&temprectsrc,systemmain->screen->h);
		}
		//�R���g���[����ʂ̃A�C�e���`��
		for(int i=0;i<12;i++)
		{

			//�K���ɕϊ�
			SDL_Rect r = {
				items_rect[i].x+items_rect[i].w/2,
				items_rect[i].y+items_rect[i].h/2,
				0,0
			};
			SDL_Rect ra = { 0,0,24,24 };
			SDL_Rect rb = { 0,36,36,36 };
			//�A�C�e���̔w�i
			if( items_rect[i].Col_Dot( player[player_ctrl].input->mouse.getpos() ) ) rb.y = 0;
			if( player[player_ctrl].selected_item != 0 &&
				player[player_ctrl].selected_item == player[player_ctrl].shownitems[i])
			{ rb.y = 0;}

			SDLFILESRFS::inst().get("image/chipboard_red.gif").Draw(systemmain->screen,&r,&rb,systemmain->screen->h);
			//�A�C�e��
			const SDLSURFACE * temp = item[player[player_ctrl].shownitems[i]].showpic;
			SDL_Rect rim = item[player[player_ctrl].shownitems[i]].r;
			temp->Draw(systemmain->screen,&r,&rim,systemmain->screen->h);
		}
		
		//�R���g���[����ʂ̃A�C�e���̎��(�I�����Ă������)
		{
			//�A�C�e���̎�ޕ\������Ƃ��̉摜�̔z��
			std::vector<const SDLSURFACE*> &sv = sortofitems_drawpict;
			//���I�΂�Ă�A�C�e���̕`��
			{
				int now = player[player_ctrl].sortofitems;
				//���ʂ̕`��
				SDL_Rect rc = {0,0,sv[now]->surface->w,sv[now]->surface->h/2};
				SDL_Rect rd = {control_rect.x+rc.w/2-1,control_rect.y+control_rect.h-2+rc.h/2-8,0,0};
				sv[now]->Draw(systemmain->screen,&rd,&rc,systemmain->screen->h);
			}
		}
		
		//��ޕύX�̂Ƃ��ɂ������ꂽ��
		{
			if( flippos.Col_Dot( player[player_ctrl].input->mouse.getpos() ) ){
				SDL_Rect temprect = {control_rect.x+control_rect.w-26,control_rect.y+control_rect.h+5,0,0};
				SDLFILESRFS::inst().get("image/circle_arrow.gif").Draw(systemmain->screen,&temprect,NULL,systemmain->screen->h);
			}
		}

		//�������
		//������ʂ̘g��
		//money_rect.Draw(systemmain->screen,0xff,0x55,0x55,0xff,systemmain->screen->h);
		//������ʂ̔w�i
		{
			SDL_Rect temprect = {money_rect.x+money_rect.w/2-1,money_rect.y+money_rect.h/2+5,0,0};
			SDLFILESRFS::inst().get("image/money.gif").Draw(systemmain->screen,&temprect,NULL,systemmain->screen->h);
		}
		//�����̕`��
		{
			sprintf(_str,"%6d;:",player[player_ctrl].money);
			SDL_Rect _rect= {
				money_rect.x+9,
				money_rect.y+5,
				0,0};
			moneyfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
		}


		//���܂�Ă�A�C�e�������鎞�̕`��
		if(item[player[player_ctrl].selected_item].obj)
		{
			//�K���ɕϊ�
			SDL_Rect r = {
				player[player_ctrl].input->mouse.getpos().x,
				player[player_ctrl].input->mouse.getpos().y,
				0,0
			};
			//�A�C�e��
			const SDLSURFACE * temp = item[player[player_ctrl].selected_item].showpic;
			SDL_Rect rim = item[player[player_ctrl].selected_item].r;
			temp->Draw(systemmain->screen,&r,&rim,systemmain->screen->h);
		}
		//�A�C�e���͂���ł��Ȃ����A�A�C�e����Ƀ}�E�X���������Ă��鎞�B
		else{
			for(int i=0;i<12;i++){
				Object* shownobj = item[player[player_ctrl].shownitems[i]].obj;
				if(shownobj && items_rect[i].Col_Dot(player[player_ctrl].input->mouse.getpos())){
					SDL_Rect r = {
						items_rect[i].x - 40 + 30,
						items_rect[i].y+items_rect[i].h/2+items_rect[i].h - 5,
						0,0
					};
					char str[256];
					sprintf(str,"%4dG",shownobj->price);
					SDLFILESRFS::inst().get("image/popup_balloon.gif").Draw(systemmain->screen,&r,NULL,systemmain->screen->h);
					if(shownobj->price <= player[player_ctrl].money)
						systemmain->bmpfont.draw(systemmain->screen,r.x-30,r.y-2,str,systemmain->screen->h);
					else
						systemmain->bmpfontred.draw(systemmain->screen,r.x-30,r.y-2,str,systemmain->screen->h);
					break;
				}
			}
		}

		//�e�v���C���[�̃X�e�[�^�X��ʂ̘g��
		//states_rect.Draw(systemmain->screen,0xff,0xff,0x44,0xff,systemmain->screen->h);
		//�e�I�u�W�F�N�g���J�E���g
		std::vector<int> ienum(player.size(),0);
		std::vector<int> peoplenum(player.size(),0);
		_it= objects.begin();
		for(;_it!=objects.end();_it++)
		{
			if((*_it)->get_class_id() == Object::PEOPLE)
				peoplenum[(*_it)->get_player_id()-1]++;
			else if ((*_it)->get_class_id() == Object::BUILDING)
				ienum[(*_it)->get_player_id()-1]++;
		}
		//�X�e�[�^�X�`��
		for(unsigned int i=0;i<player.size();i++){
			//�Q�[�W�o�[
			{
				SDL_Rect _rect= {
					states_rect.x+111*i+58-(int)(100.0-(double)player[i].base->hp/(double)PlayerBase().hp*100.0)/2,
					states_rect.y+23,
					0,0};
				SDL_Rect ra= {
					0,
					8*i,
					(int)((double)player[i].base->hp/(double)PlayerBase().hp*100.0),8};
				SDLFILESRFS::inst().get("image/life_bar.gif").Draw(systemmain->screen,&_rect,&ra,systemmain->screen->h);
			}
			//�Q�[�W�g
			{
				SDL_Rect _rect= {
					states_rect.x+111*i+54,
					states_rect.y+23,
					0,0};
				SDLFILESRFS::inst().get("image/life_frame.gif").Draw(systemmain->screen,&_rect,NULL,systemmain->screen->h);
			}
			//���_HP�̕\��
			{
				sprintf(_str,"%d",player[i].base->hp);
				SDL_Rect _rect= {
					states_rect.x+111*i+40,
					states_rect.y+19,
					0,0};
				statusfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
			}
			//�v���C���[
			{
				SDL_Rect _rect= {
					states_rect.x+111*i+13,
					states_rect.y+states_rect.h,
					0,0};
				SDL_Rect ra= {
					0,
					20*i,
					34,20};
				SDLFILESRFS::inst().get("image/player.gif").Draw(systemmain->screen,&_rect,&ra,systemmain->screen->h);
			}
			//�Ƃ̃A�C�R���ƕ\��
			{
				SDL_Rect _rect= {
					states_rect.x+111*i+10,
					states_rect.y+9,
					0,0};
				SDL_Rect ra= {
					0,
					16*i,
					16,16};
				SDLFILESRFS::inst().get("image/ie_icon_all.gif").Draw(systemmain->screen,&_rect,&ra,systemmain->screen->h);
				sprintf(_str,"%3d",ienum[i]);
				_rect.x += 10;
				_rect.y -= 4;
				statusfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
			}
			//���m�̃A�C�R���ƕ\��
			{
				SDL_Rect _rect= {
					states_rect.x+111*i+61,
					states_rect.y+9,
					0,0};
				SDL_Rect ra= {
					0,
					16*i,
					16,16};
				SDLFILESRFS::inst().get("image/heishi_icon_all.gif").Draw(systemmain->screen,&_rect,&ra,systemmain->screen->h);
				sprintf(_str,"%3d",peoplenum[i]);
				_rect.x += 10;
				_rect.y -= 4;
				statusfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
			}

		}

		//��ʑS�̂Ɋւ��邱��
		//�ǂ܂�Ȃ�
		//�S���I����Ď������I����Ƃ��Ɏ��̎w���������B
		if(deadcount + 1 >= player.size()){
			//���ꂼ�ꏟ�s���t���Ă鎞�Ɍ��ʂ�����
			//�S������ł�Ƃ�
			if(deadcount == player.size()){
				//��������
				SDL_Rect _rect= {systemmain->screen->w/2,systemmain->screen->h/2,0,0};
				systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,"draw",systemmain->screen->h);
			}
			//�����͎���ł�Ƃ�
			else if(player[player_ctrl].base->hp <= 0){
				//����
				SDL_Rect _rect= {systemmain->screen->w/2,systemmain->screen->h/2,0,0};
				SDLFILESRFS::inst().get("image/looser.gif").Draw(systemmain->screen,&_rect,NULL,systemmain->screen->h);
			}
			//�����S������ł�Ƃ�
			else if(deadcount + 1 == player.size()){
				//����
				SDL_Rect _rect= {systemmain->screen->w/2,systemmain->screen->h/2,0,0};
				SDLFILESRFS::inst().get("image/winner.gif").Draw(systemmain->screen,&_rect,NULL,systemmain->screen->h);
			}
			//���ʂ̎w��
			if(counter%52 < 40)
			{
				sprintf(_str,"Please wait...");
				SDL_Rect _rect= {
					systemmain->screen->w/2-(strlen(_str)*11)/2,
					systemmain->screen->h/2-100,
					0,0};
				systemmain->bmpfont.draw(systemmain->screen,_rect.x,_rect.y,_str,systemmain->screen->h);
			}
		}
		//�ŏ��̂Ƃ�
		else if(counter < 200){
			SDL_Rect r = {systemmain->screen->w/2,systemmain->screen->h/2,0,0};
			SDL_Rect ra= {0,0,640,0};
			int temp = counter/10;
			if(counter<140){
				ra.y = 82*(14-temp);
				ra.h = 82;
				r.x += xor128()%(temp+1)-(temp+1)/2;
				r.y += xor128()%(temp+1)-(temp+1)/2;
				SDLFILESRFS::inst().get("image/areyouready.gif").Draw(systemmain->screen,&r,&ra,systemmain->screen->h);
			}
			else if(counter<150){
				r.y = (counter-140)*systemmain->screen->h/10 - systemmain->screen->h/2;
				ra.y=0; ra.h=120;
				SDLFILESRFS::inst().get("image/start.gif").Draw(systemmain->screen,&r,&ra,systemmain->screen->h);
				r.y += systemmain->screen->h/2 + systemmain->screen->h/2;
				 ra.y=0; ra.h=82;
				SDLFILESRFS::inst().get("image/areyouready.gif").Draw(systemmain->screen,&r,&ra,systemmain->screen->h);
			}
			else if(counter%4<2){
				ra.y=0; ra.h=120;
				SDLFILESRFS::inst().get("image/start.gif").Draw(systemmain->screen,&r,&ra,systemmain->screen->h);
			}
		}



	}

}
