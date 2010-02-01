#include "gamemainobject.h"

namespace space
{
	//��؂Ȓ�`
	const Object::PLAYERID Object::PLAYER_NULL = 0;
	//GameMain* Object::gamemain = NULL;

	//�R���X�g���N�^
	Object::Object(){}
	Object::Object(GameMain *p, CLASSID _id)
	{
		gamemain = p;
		class_id = _id;
		isNeed=true;
		colcircle.circle = &maincircle;
		colobj.objs.push_back(&colcircle);
		animecount = 0;
		powerdist = 0.0;
		zpos = 0;

		// OFT�ɓo�^
		formorton.m_pObject = this;
	}

	inline void Object::move_manual(){
			fieldpos += fieldspd;
			maincircle.p = fieldpos;
			formorton.Remove();
			gamemain->mortontree.Regist(fieldpos.x-maincircle.d,fieldpos.y+maincircle.d,fieldpos.x+maincircle.d,fieldpos.y-maincircle.d,&formorton);
	}

	//�U������
	void Object::attackTo(Object* src){
		//hp��0�̂�͕��u���邱�Ƃɂ���
		if(src->hp<=0)return;

		//�_���[�W�v�Z��
		//���
		int dmg = (int)( (7.0*atk - 2.0*src->def) / 5.0 );

		//�_���[�W�␳
		//���_�Ƃ��ŕ␳���������肷��̂���
		//�͋��_�{�[�i�X
		if(player_id>0 && gamemain->player[player_id-1].typeofBase==BASETYPE_POWER)dmg+=std::max(2.0,dmg*0.6);
		if(src->get_player_id()>0 && gamemain->player[src->get_player_id()-1].typeofBase==BASETYPE_POWER)dmg-=std::max(1.0,dmg*0.12);

		//�Ō�ɂ���
		dmg = (dmg<=0) ? 1 : dmg;

		//���̑����ꔻ��
		//���ʂ̒e�����͓����`�[�����m�Ȃ�_���[�W����u����
		if( ( (src->get_class_id()==BULLET && src->get_object_id()==BULLET_NORMAL) ||
			(         class_id  ==BULLET &&          object_id  ==BULLET_NORMAL) )
			&& src->get_player_id()==player_id){
				dmg = 0;
		}

		//���ʂɃ_���[�W��^����
		src->hp -= dmg;

		//�_���[�W������Ƃ�
		if(dmg>0){
			//�e�͍U���ɉ��Ή󂷁B
			if(class_id==BULLET){hp=0;}
			//�e�͍U������Ă���
			if(src->get_class_id()==BULLET){src->hp=0;}
		}

		//�Ȃɂ�����|������̂��b
		if(src->hp<=0 && player_id!=0){
			int addmoney=0;

			if(src->get_player_id()>0){gamemain->player[src->get_player_id()-1].bekilled_cnt++;}
			if(player_id>0){gamemain->player[player_id-1].killed_cnt++;}

			//�l�̏ꍇ
			if(src->get_class_id()==PEOPLE){
				addmoney += rand()%5 + 6;
			}
			//�e�͉󂵂Ă��Ȃɂ��Ȃ���
			else if(src->get_class_id()==BULLET){}
			//�����̏ꍇ
			else if(src->get_class_id()==BUILDING){
				addmoney += ((rand()%5 + 9)*pointer_cast<Building*>(src)->poolnum+rand()%10+21);
			}
			//���_�j��
			else if(src->get_class_id()==IMPORTANCE && src->get_object_id()==IMPORTANCE_BASE){
				addmoney += 250;
			}

			//�������_�{�[�i�X	
			if(player_id>0 && gamemain->player[player_id-1].typeofBase==BASETYPE_MONEY) addmoney+=std::max(1.0,addmoney*0.05);
			gamemain->player[player_id-1].money += addmoney;
		}
	}

	//�����蔻��␳
	void Object::revisionPosByCircle(Object *p){
		//�~�̓z
		double theta = atan2(
			p->fieldpos.y - fieldpos.y,
			p->fieldpos.x - fieldpos.x);
		p->fieldpos.x = fieldpos.x + (maincircle.d/2+p->maincircle.d/2)*cos(theta);
		p->fieldpos.y = fieldpos.y + (maincircle.d/2+p->maincircle.d/2)*sin(theta);
		Matrix21<double> temp = p->fieldspd;
		p->fieldspd.x = 0.0; p->fieldspd.y = 0.0;
		p->move_manual();
		p->fieldspd = temp;
	}

	//���s����
	//move -> collision -> after_collision -> draw2D,draw3D


	//�l�Ԃ̃A�u�X�g���N�g
	People::People(GameMain* p,ID _oid) : Object(p,PEOPLE)
	{
		object_id = _oid;
	}
	


	//�Փ˔���
	void People::collision(Object* op){
		//if(op==this)return;

		switch( op->get_class_id() )
		{
		//�l�Ɛl
		case PEOPLE:
			if( colobj.getCollision(&op->colobj) )
			{
				//����������ǂ����B
				revisionPosByCircle(op);
			}
			break;
		}
	}



	//����
	
	//�p�����[�^�Z�b�g	
	void PawnPeople::setParam(){
		atk = 13; def = 1; luk = 10; hp = 10; rpd = 1.0;
		maketime = 150; price = 10;
		maincircle.d = 10;
	}

	//�R���X�g���N�^
	PawnPeople::PawnPeople() : People(NULL,PEOPLE_PAWN){setParam();}
	PawnPeople::PawnPeople(GameMain* p,PLAYERID _pid,
			   double _x,double _y) : People(p,PEOPLE_PAWN)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		dstpos = fieldpos;
		fieldspd.x=0;fieldspd.y=0;

		player_id = _pid;
		maincircle.p = fieldpos;

		state = 0;

	}

	//2D�`��
	void PawnPeople::draw2D(void)
	{
		//===================
		//SDL�̕`��
		//===================
		
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra= {0,0,24,24};
		ra.x = 24 * ((animecount/30)%2);
		ra.y = 24 * (player_id-1);
		SDLFILESRFS::inst().get("image/heishi_front_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}

	//�A�j���̂��߂����ɍ�����킯�ł͂Ȃ��B
	void PawnPeople::move(){

		//�Փ˂�����U������
		Object* nearobj=NULL;
		for(std::list<Object*>::iterator it=gamemain->objects.begin();it!=gamemain->objects.end();it++){
			//ID
			CLASSID cid=(*it)->get_class_id();
			ID id=(*it)->get_object_id();
			PLAYERID pid=(*it)->get_player_id();
			//�U������l��I��
			bool foratk=false;
			switch(cid){
				//�G�ŁA�l�������Ȃ�
				case PEOPLE: foratk=true;break;
				case BUILDING: foratk=true;break;
				case IMPORTANCE: if(id==IMPORTANCE_BASE){foratk=true;} break;
				default: break;
			}
			//�U������l�Ɍ����đ��鏈���B
			if(foratk){
				if(player_id!=pid && colobj.getCollision(&(*it)->colobj)){
					//�U���^�C�~���O�Ȃ�U��
					if((animecount+1)%(unsigned int)(90.0/rpd) == 0){attackTo(*it);}
					//�����łȂ��Ă�������������A�ڕW��ύX����
					dstpos = (*it)->fieldpos;
				}
			}
		}

		//�����̈ړ���
		if(state == 0){
			//5�t������
			if(animecount % 5 == 0){
				//�s���������ɍs��
				double theta = atan2(dstpos.y-fieldpos.y,dstpos.x-fieldpos.x) + (rand()%120-60)/180.0*M_PI;
				fieldspd.x=0.9*cos(theta); fieldspd.y=0.9*sin(theta);
			}
		}

		//������
		People::move();
	}


	//�e��
	
	//�p�����[�^�Z�b�g	
	void Gunner::setParam(){
		atk = 8; def = 1; luk = 10; hp = 5; rpd = 1.0;
		maketime = 490; price = 40;
		maincircle.d = 10;
		range = 70;
	}

	//�R���X�g���N�^
	Gunner::Gunner() : People(NULL,PEOPLE_GUNNER){setParam();}
	Gunner::Gunner(GameMain* p,PLAYERID _pid, double _x,double _y) : People(p,PEOPLE_GUNNER)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		dstpos = fieldpos;
		fieldspd.x=0;fieldspd.y=0;

		player_id = _pid;
		maincircle.p = fieldpos;

		state = 0;

	}

	//2D�`��
	void Gunner::draw2D(void)
	{
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra= {0,0,24,24};
		ra.x = 24 * ((animecount/30)%2);
		ra.y = 24 * (player_id-1);
		SDLFILESRFS::inst().get("image/zyuhei_front_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}

	//�A�j���̂��߂����ɍ�����킯�ł͂Ȃ��B
	void Gunner::move(){

		//�͈͓��Ȃ�U������
		if((animecount+1)%(unsigned int)(200.0/rpd)==0){
			//��ԋ߂��́B
			Object* nearobj=NULL;
			//�͈͓��ɂ���l���m�F����
			for(std::list<Object*>::iterator it=gamemain->objects.begin();it!=gamemain->objects.end();it++){
				//ID
				CLASSID cid=(*it)->get_class_id();
				ID id=(*it)->get_object_id();
				PLAYERID pid=(*it)->get_player_id();
				
				//player_id�������Ȃ疳���B
				if(player_id==pid)continue;

				//�U������l��I��
				bool foratk=false;
				switch(cid){
					//�G�ŁA�l�������Ȃ�
					case PEOPLE: foratk=true;break;
					case BUILDING: foratk=true;break;
					case IMPORTANCE: if(id==IMPORTANCE_BASE){foratk=true;} break;
					default: break;
				}

				//�U���Ώۂ̂Ƃ�
				if(foratk){
					//�����ŏ��������W�͈͓̔����A
					//���łɑI�񂾂���߂��Ȃ�I�ԁB
					if( ( nearobj==NULL && ((*it)->fieldpos-fieldpos).Abs2() < range*range ) ||
						( nearobj!=NULL && ((*it)->fieldpos-fieldpos).Abs2() < (nearobj->fieldpos-fieldpos).Abs2() ) ){
						//���ꂩ��
						nearobj = (*it);
					}
				}
			}

			//�U������l�Ɍ����čU��
			if(nearobj){
				Matrix21<double> v;
				v = nearobj->fieldpos-fieldpos;
				v *= (0.8/v.Abs());
				NormalBullet* p = new NormalBullet(gamemain,player_id,fieldpos.x,fieldpos.y,v.x,v.y);
				p->distance = range;
				gamemain->objects.push_back(p);
			
			}
			
		}

		//�����̈ړ���,5�t������
		if(animecount % 5 == 0){
			//�s���������ɍs��
			double theta = atan2(dstpos.y-fieldpos.y,dstpos.x-fieldpos.x) + (rand()%120-60)/180.0*M_PI;
			fieldspd.x=0.8*cos(theta); fieldspd.y=0.8*sin(theta);
		}

		//������
		People::move();
	}



	//=========================
	//�e�ۂ̃A�u�X�g���N�g
	//=======================
	Bullet::Bullet(GameMain* p,ID _oid) : Object(p,BULLET){
		object_id = _oid;
	}
	Bullet::~Bullet(){}

	void Bullet::collision(Object* op){
			//if(op==this)return;
			switch( op->get_class_id() )
			{
			case PEOPLE:
				if(colobj.getCollision(&op->colobj) ){
					attackTo(op);
					op->attackTo(this);
				}
				break;
			case BULLET:
				if(colobj.getCollision(&op->colobj) ){
					attackTo(op);
					op->attackTo(this);
				}
				break;
			default:
				break;
			}
	}
	//��������
	void Bullet::after_collision()
	{
		if(hp<=0){
			isNeed=false;
			hp = 0;
		}
	}



	//���ʂ̒e

	//�p�����[�^�Z�b�g	
	void NormalBullet::setParam(){
		atk = 20; def = 1; luk = 0; hp = 1; rpd = 3.5;
		maketime = 120; price = 0;
		maincircle.d = 8;
		distance = 100;
	}

	//�R���X�g���N�^
	NormalBullet::NormalBullet() : Bullet(NULL,BULLET_NORMAL){setParam();}
	NormalBullet::NormalBullet(GameMain* p,PLAYERID _pid,
			   double _x,double _y,double vx,double vy) : Bullet(p,BULLET_NORMAL)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=vx; fieldspd.y=vy;

		player_id = _pid;
		maincircle.p = fieldpos;
		startpos = fieldpos;
	}

	void NormalBullet::draw2D(void)
	{
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra= {0,0,12,12};
		ra.x= 12 * ((animecount/9)%4);
		ra.y= 12 * (player_id-1);
		SDLFILESRFS::inst().get("image/Bullet_all_tamesi.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}

	void NormalBullet::move(void){
		if( (startpos-fieldpos).Abs2() >= distance*distance ) isNeed = false;
		Bullet::move();
	}

	void NormalBullet::after_collision(void){
		Bullet::after_collision();
		//���񂾂甚���B
		if(hp<=0) gamemain->objects.push_back(new ExplosionEffect(gamemain,fieldpos.x,fieldpos.y));
	}



	//����

	//�p�����[�^�Z�b�g	
	void BombFire::setParam(){
		atk = 8; def = 100; luk = 0; hp = 0; rpd = 0;
		maketime = 0; price = 0;
		maincircle.d = 0.0;//��������Ȃ����0�ł��B
	}

	//�R���X�g���N�^
	BombFire::BombFire() : Bullet(NULL,BULLET_BOMBFIRE){setParam();}
	BombFire::BombFire(GameMain* p,PLAYERID _pid,
			   double _x,double _y,double d,unsigned int frame) : Bullet(p,BULLET_BOMBFIRE)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=.0; fieldspd.y=.0;

		player_id = _pid;
		maincircle.p = fieldpos;
		maincircle.d = d;

		damageframe = frame;
	}

	void BombFire::draw2D(void)
	{
	}

	void BombFire::move(void){
		//���O����ԁB�����ʓ|�B
		//�Ƃ����킯�Ŕ������O�B
		if(animecount%15 -1 == 0){
			gamemain->objects.push_back(new ExplosionEffect(gamemain,fieldpos.x,fieldpos.y));
		}

		//�J�E���g�ɓ��B������I���B
		if(  animecount >= (int)damageframe ) isNeed = false;
		Bullet::move();
	}

	void BombFire::after_collision(){
		//�����͎��ȂȂ��B�J�E���g�܂ł́B
		if(hp<=0){hp=0;}
	}

	//�ΎR�e

	//�p�����[�^�Z�b�g	
	void Meteo::setParam(){
		atk = 200; def = 200; luk = 0; hp = 200; rpd = 30.0;
		maketime = 60; price = 0;
		maincircle.d = 48;
	}

	//�R���X�g���N�^
	Meteo::Meteo() : Bullet(NULL,BULLET_METEO){setParam();}
	Meteo::Meteo(GameMain* gp,PLAYERID _pid,
			   double _x,double _y,double dx,double dy) : Bullet(gp,BULLET_METEO)
	{
		setParam();

		if(fabs(_x-dx)<0.001)dx=_x+0.001;

		fieldpos.x=_x; fieldpos.y=_y;
		dstpos.x = dx; dstpos.y = dy;


		player_id = _pid;
		maincircle.p = fieldpos;
		startpos = fieldpos;


		//�\���ʒu�͏�B
		zpos = 127;
		//���������x�͊���Ƃ������ڂɁB
		fieldspd.x = std::min( fabs(dstpos.x-startpos.x)/240.0, 0.5 );
		fieldspd.x *= (dstpos.x>startpos.x)?1.0:-1.0;
		//�������̂��낢��B
		Matrix21<double> p;
		p.y = std::max(dstpos.y,startpos.y)+250.0;
		p.x = startpos.x+(dstpos.x-startpos.x)*0.5;
		double A,B,C,D,E,F,G;
		A=startpos.x*startpos.x-dstpos.x*dstpos.x;
		B=startpos.x-dstpos.x;
		C=startpos.y-dstpos.y;
		D=startpos.x*startpos.x-p.x*p.x;
		E=startpos.x-p.x;
		F=startpos.y-p.y;
		G=B/E;
		//���v�Z
		PBa = (C-G*F)/(A-G*D);
		PBb = (C-A*PBa)/B;
		PBc = startpos.y-PBa*startpos.x*startpos.x-PBb*startpos.x;

		//�ŏ��͓����蔻����Ȃ���
		colobj.objs.clear();

		//�����_�\��
		meteopointeffect = new MeteoPointEffect(gamemain,dstpos.x,dstpos.y);
		gamemain->objects.push_back(meteopointeffect);

	}

	void Meteo::draw2D(void)
	{
		//�ʒu
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		//�G�t�F�N�g
		static SDL_Rect ra = {0,0,184,184};
		ra.x= ( animecount%5 )*ra.w;
		if(fieldspd.y>0.0){
			if(fieldspd.y<0.5){
				ra.y= ( (animecount/5)%1   )*ra.h;
			}
			else if(fieldspd.y<1.0){
				ra.y= ( (animecount/5)%1+1 )*ra.h;
			}
			else if(fieldspd.y<1.5){
				ra.y= ( (animecount/5)%1+2 )*ra.h;
			}
			else{
				ra.y= ( (animecount/5)%2+3 )*ra.h;
			}
			r.y -= 50;
			SDLFILESRFS::inst().get("image/meteo_fire_back.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
			r.y += 50;
		}
		else{
			if(fieldspd.y>-0.5){
				ra.y= ( (animecount/5)%1   )*ra.h;
			}
			else if(fieldspd.y>-1.0){
				ra.y= ( (animecount/5)%1+1 )*ra.h;
			}
			else if(fieldspd.y>-1.5){
				ra.y= ( (animecount/5)%1+2 )*ra.h;
			}
			else{
				ra.y= ( (animecount/5)%2+3 )*ra.h;
			}
			r.y += 50;
			SDLFILESRFS::inst().get("image/meteo_fire.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
			r.y -= 50;
		}
		//�摜�{��
		static SDL_Rect rb = {0,0,48,48};
		SDLFILESRFS::inst().get("image/meteo.gif").Draw(gamemain->systemmain->screen,&r,&rb,gamemain->systemmain->screen->h);
	}

	void Meteo::move(void){
		//�v�Z�����ʒu���瑬�x���t�Z����B
		Matrix21<double> nextpos;
		nextpos.x = fieldpos.x+fieldspd.x;
		nextpos.y = PBa*nextpos.x*nextpos.x+PBb*nextpos.x+PBc;
		fieldspd=nextpos-fieldpos;
		

		//���Ƃ͂����ǂ���B
		Bullet::move();
		//�ڕW�n�_�ɓ��B�����悤�Ȃ瓖���蔻��𕜊�������B
		if( (dstpos-fieldpos).Abs2() < 1.0 ) colobj.objs.push_back(&colcircle);
	}

	void Meteo::after_collision(void){
		Bullet::after_collision();
		//�����蔻����o�����������̃^�[���ɕK���j�󂷂�B
		//���񂾂甚���B
		if(colobj.objs.size()){
			hp=0;
			isNeed=false;
			gamemain->objects.push_back(new ExplosionEffect(gamemain,fieldpos.x,fieldpos.y));
			//�����_����
			meteopointeffect->isNeed = false;
		}
	}


	//���e
	
	//�p�����^
	void Bomb::setParam(void){
		atk = 0; def = 0; luk = 0; hp = 1; rpd = 0.0;
		maketime = 0; price = 120;
		maincircle.d = 20;
	}

	//�R���X�g���N�^
	Bomb::Bomb() : Bullet(NULL,BULLET_BOMB){setParam();}
	Bomb::Bomb(GameMain* p,PLAYERID _pid,double _x,double _y)
		: Bullet(p,BULLET_BOMB)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = _pid;

	}

	void Bomb::draw2D(void){
		//===================
		//SDL�̕`��
		//===================
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect rs= {0,0,32,32};
		rs.x = 32*((animecount/6)%3);
		SDLFILESRFS::inst().get("image/bomb.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}

	//��������
	void Bomb::after_collision()
	{
		if(hp<=0){
			//�łт�
			isNeed=false;
			hp = 0;
			//����
			gamemain->objects.push_back(new BombFire(gamemain,player_id,fieldpos.x,fieldpos.y,32,15));
		}
	}

	//�n���B
	//�܂肻��͌����Ȃ����e

	//�p�����^
	void GroundBomb::setParam(void){
		atk = 0; def = 0; luk = 0; hp = 1; rpd = 0.0;
		maketime = 0; price = 140;
		maincircle.d = 20;
	}

	//�R���X�g���N�^
	GroundBomb::GroundBomb() : Bullet(NULL,BULLET_GROUNDBOMB){setParam();}
	GroundBomb::GroundBomb(GameMain* p,PLAYERID _pid,double _x,double _y)
		: Bullet(p,BULLET_GROUNDBOMB)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = _pid;

	}

	void GroundBomb::draw2D(void){
		//�����ȊO�����Ȃ���
		if(gamemain->player_ctrl==(player_id-1)){
			static SDL_Rect r = {0,0,0,0};
			r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
			r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
			static SDL_Rect rs ={0,0,32,32};
			rs.y= 32*(player_id-1);
			SDLFILESRFS::inst().get("image/groundbomb.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
		}
	}

	//��������
	void GroundBomb::after_collision()
	{
		if(hp<=0){
			//�łт�
			isNeed=false;
			hp = 0;
			//����
			gamemain->objects.push_back(new BombFire(gamemain,player_id,fieldpos.x,fieldpos.y,30,14));
		}
	}



	//=======================
	//�����̂���
	//=======================
	Building::Building(GameMain* p,ID _oid) : Object(p,BUILDING){
		object_id = _oid;
	}

	void Building::collision(Object* op){
			//if(op==this)return;

			switch( op->get_class_id() )
			{
			case PEOPLE:
				//�l�ƌ���
				if( colobj.getCollision(&op->colobj) ){
					//�~�O��łǂ���
					People *p = pointer_cast<People*>(op);
					revisionPosByCircle(p);
				}
				break;

			case BULLET:
				//�e�̓_���[�W���肠��
				if( colobj.getCollision(&op->colobj) ){
					//�Ƃ̓_���[�W
					op->attackTo(this);
				}
				break;

			case BUILDING:
				//���̂�͈ړ����肵���Ȃ��B
				if( colobj.getCollision(&op->colobj) ){
					//�~�̓z
					revisionPosByCircle(op);
				}
				break;
			default:
				break;
			}
	}


	//��������
	void Building::after_collision()
	{
		if(hp<=0){
			isNeed=false;
			hp = 0;
		}
	}


	//���ʂ̌���

	//�p�����[�^�Z�b�g	
	void NormalBuilding::setParam(){
		atk = 0; def = 10; luk = 0; hp = 100; rpd = 1.0;
		maketime = 10; price = 80;
		maincircle.d = 20;

		poolnum = 8;
		powerdist = 60.0;
	}

	//�R���X�g���N�^
	NormalBuilding::NormalBuilding() : Building(NULL,BUILDING_NORMAL){setParam();}
	NormalBuilding::NormalBuilding(GameMain* p,PLAYERID _pid,
			   double _x,double _y) : Building(p,BUILDING_NORMAL)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x= 0; fieldspd.y= 0;

		player_id = _pid;
		maincircle.p = fieldpos;

		//���̋��_�{�[�i�X
		if(gamemain->player[player_id-1].typeofBase==BASETYPE_LOVE){poolnum*=1.5;}
	}

	//�o����I
	void NormalBuilding::move(void){
		//�l���o���܂�
		if((animecount+1)%PawnPeople().maketime == 0){
			if(poolnum > 0){ gamemain->setPawn(player_id,fieldpos.x,fieldpos.y-maincircle.d/2); poolnum--;}
		}
		//�ق��̂���
		Building::move();
	}

	//2D
	void NormalBuilding::draw2D(void)
	{
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra = {0,0,32,32};
		ra.y= 32 * (player_id-1);
		SDLFILESRFS::inst().get("image/ie_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}


	//�v��

	//�p�����[�^�Z�b�g	
	void Fortress::setParam(){
		atk = 3; def = 23; luk = 10; hp = 85; rpd = 1.0;
		maketime = 10; price = 200;
		maincircle.d = 20;

		poolnum = 15;
		powerdist = 80.0;
	}

	//�R���X�g���N�^
	Fortress::Fortress() : Building(NULL,BUILDING_FORTRESS){setParam();}
	Fortress::Fortress(GameMain* p,PLAYERID _pid,
			   double _x,double _y) : Building(p,BUILDING_FORTRESS)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x= 0; fieldspd.y= 0;

		player_id = _pid;
		maincircle.p = fieldpos;

		//���̋��_�{�[�i�X
		if(gamemain->player[player_id-1].typeofBase==BASETYPE_LOVE){poolnum*=1.5;}
	}

	//2D
	void Fortress::draw2D(void)
	{
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra ={0,0,32,32};
		ra.y= 32 * (player_id-1);
		SDLFILESRFS::inst().get("image/yousai_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}

	//�e�ł����肵�܂�����܂��B
	void Fortress::move(){
		//�ł���
		if((animecount+1)%(unsigned int)(120.0/rpd) == 0){
			//�_����͈͂ŁB
			Object* nearobj=NULL;
			for(std::list<Object*>::iterator it=gamemain->objects.begin();it!=gamemain->objects.end();it++){
				double dist = ((*it)->fieldpos-fieldpos).Abs();
				//�������߂��A�N�����߂��A���ꂪ�G�ł���Ȃ�_���B
				if(
					(*it)->get_player_id()!=player_id && (*it)->get_player_id()!=PLAYER_NULL &&
					dist<=120.0 &&
					(!nearobj || dist<=(nearobj->fieldpos-fieldpos).Abs())
					){
					nearobj=(*it);
				}
			}
			//�ł�������
			if(nearobj){
				Matrix21<double> dir=nearobj->fieldpos-fieldpos;
				dir /= dir.Abs();
				dir *= 0.6;
				//�o��
				gamemain->setNormalBullet(player_id,fieldpos.x,fieldpos.y,dir.x,dir.y);
			}
		}
		//�l���o���܂�
		if((animecount+1)%PawnPeople().maketime == 0){
			if(poolnum > 0){ gamemain->setPawn(player_id,fieldpos.x,fieldpos.y-maincircle.d/2); poolnum--;}
		}
		//���̑��̏���
		Building::move();
	}


	//���X

	//�p�����[�^�Z�b�g	
	void Shop::setParam(){
		atk = 0; def = 11; luk = 10; hp = 50; rpd = 0.0;
		maketime = 10; price = 50;
		maincircle.d = 20;

		poolnum = 2;
		powerdist = 20.0;
	}

	//�R���X�g���N�^
	Shop::Shop() : Building(NULL,BUILDING_SHOP){setParam();}
	Shop::Shop(GameMain* p,PLAYERID _pid,
			   double _x,double _y) : Building(p,BUILDING_SHOP)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x= 0; fieldspd.y= 0;

		player_id = _pid;
		maincircle.p = fieldpos;

		//���̋��_�{�[�i�X
		if(gamemain->player[player_id-1].typeofBase==BASETYPE_LOVE){poolnum*=1.5;}
	}

	//2D
	void Shop::draw2D(void)
	{
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra = {0,0,32,32};
		ra.y= 32 * (player_id-1);
		SDLFILESRFS::inst().get("image/shop_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}

	//�����͂����˂��ӂ��܂�
	void Shop::move(){
		//�������������
		if((animecount+1)%720 == 0){
			gamemain->player[player_id-1].money += rand()%8+8;
			//���̋��_�{�[�i�X
			if(gamemain->player[player_id-1].typeofBase==BASETYPE_MONEY){gamemain->player[player_id-1].money += rand()%1+1;}
		}
		//�l���o���܂�
		if((animecount+1)%PawnPeople().maketime == 0){
			if(poolnum > 0){ gamemain->setPawn(player_id,fieldpos.x,fieldpos.y-maincircle.d/2); poolnum--;}
		}
		//�ق��̂���
		Building::move();
	}


	//�b�艮

	//�p�����[�^�Z�b�g	
	void Smith::setParam(){
		atk = 0; def = 10; luk = 10; hp = 100; rpd = 0.0;
		maketime = 10; price = 270;
		maincircle.d = 20;

		poolnum = 10;
		powerdist = 20.0;
	}

	//�R���X�g���N�^
	Smith::Smith() : Building(NULL,BUILDING_SMITH){setParam();}
	Smith::Smith(GameMain* p,PLAYERID _pid,
			   double _x,double _y) : Building(p,BUILDING_SMITH)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x= 0; fieldspd.y= 0;

		player_id = _pid;
		maincircle.p = fieldpos;

		//���̋��_�{�[�i�X
		if(gamemain->player[player_id-1].typeofBase==BASETYPE_LOVE){poolnum*=1.5;}
	}

	//2D
	void Smith::draw2D(void)
	{
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect ra = {0,0,32,32};
		ra.x= 32*((animecount/30)%2);
		ra.y= 32 * (player_id-1);
		SDLFILESRFS::inst().get("image/smith_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
	}

	//�e�����o�܂�
	void Smith::move(){
		//�l���o���܂�
		if((animecount+1)%Gunner().maketime == 0){
			if(poolnum > 0){ gamemain->objects.push_back(new Gunner(gamemain,player_id,fieldpos.x,fieldpos.y-maincircle.d/2)); poolnum--;}
		}
		//�ق��̂���
		Building::move();
	}


	//=====================
	//�d�v�Ȃ���
	//=====================
	Importance::Importance(GameMain* p,ID _oid) : Object(p,IMPORTANCE){
		object_id = _oid;
	}


	//���_�ɂ���
	//�p�����^�ݒ�
	void PlayerBase::setParam(){
		atk = 0; def = 20; luk = 100; hp = 500; rpd = 0.1;
		maketime = 0; price = 0;
		maincircle.d = 28;
		powerdist = 140.0;
	}

	//�R���X�g���N�^
	PlayerBase::PlayerBase() : Importance(NULL,IMPORTANCE_BASE){setParam();}
	PlayerBase::PlayerBase(GameMain* p,PLAYERID _pid,
			   double _x,double _y) : Importance(p,IMPORTANCE_BASE)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		player_id = _pid;
		maincircle.p = fieldpos;
	}

	void PlayerBase::collision(Object* op){
			//if(op==this)return;
			switch( op->get_class_id() )
			{
			//�l�͂Ƃ肠�����ǂ����B
			case PEOPLE:
				if( colobj.getCollision(&op->colobj) ){
					revisionPosByCircle(op);
				}
				break;
			//�e�͍U�����Ď���
			case BULLET:
				if( colobj.getCollision(&op->colobj) ){
					op->attackTo(this);
					attackTo(op);
				}
				break;
			//�����͕␳
			case BUILDING:
				if( colobj.getCollision(&op->colobj) ){
					revisionPosByCircle(op);
				}
				break;
			//���̑��X���[
			case IMPORTANCE:
				break;
			default:
				break;
			}
	}
	//��������
	void PlayerBase::after_collision()
	{
		if(hp<=0){
			hp = 0;
			colobj.objs.clear();
		}
	}

	void PlayerBase::move(void)
	{
		//�����Ă�Ƃ�
		if(hp>0){
			//�������������
			if((animecount+1)%600 == 0){
				gamemain->player[player_id-1].money += rand()%3+8;
			//���̋��_�{�[�i�X
			if(gamemain->player[player_id-1].typeofBase==BASETYPE_MONEY){gamemain->player[player_id-1].money += rand()%2+1;}
			}
		}
		//���Ƃ͂����ǂ���B
		Object::move();
	}

	void PlayerBase::draw2D(void)
	{
		if(hp<=0)return;

		//===================
		//SDL�̕`��
		//===================
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y-4);
		static SDL_Rect ra = {64,64,64,64};
		static int ax;
		ax = animecount%256 - 128;
		ax = ( (ax*ax*ax)/(128*128) + 128 )/16;
		ra.x= 64 * ax;
		ra.y= 64 * (player_id-1);
		//���_�̉摜�\��
		SDLFILESRFS::inst().get("image/bace_all.gif").Draw(gamemain->systemmain->screen,&r,&ra,gamemain->systemmain->screen->h);
		//���O�\���̏ꏊ
		r.x -= gamemain->systemmain->bmpfont.getW()*gamemain->player[player_id-1].name.length()/2;
		r.y -= (gamemain->systemmain->bmpfont.getH()/2+16);
		gamemain->systemmain->bmpfont.draw(gamemain->systemmain->screen,r.x,r.y,gamemain->player[player_id-1].name.c_str(),gamemain->systemmain->screen->h);
	}



	//===============================================
	//�t�B�[���h�ɂ��镨�́A�܂��͑S�̂̂��ƂɊւ���
	//==============================================
	Field::Field(GameMain* p,ID _oid) : Object(p,FIELD){
		object_id = _oid;
		player_id = PLAYER_NULL; //��{�I��NULL
	}
	Field::~Field(){}

	void Field::collision(Object *op){

		switch(op->get_class_id()){
		//�l�͂ǂ���B�������ǂ���B
		case PEOPLE:
		case BUILDING:
			if( colobj.getCollision(&op->colobj) ){
				revisionPosByCircle(op);
			}
			break;
		//�e�ɓ���������U�����󂯂�
		case BULLET:
			if( colobj.getCollision(&op->colobj) ){
				op->attackTo(this);
			}
			break;
		//�d�v���͋��_����������␳
		case IMPORTANCE:
			if( op->get_object_id()==IMPORTANCE_BASE && colobj.getCollision(&op->colobj) ){
				op->revisionPosByCircle(this);
			}
			break;
		//�t�B�[���h�I�u�W�F�N�g�͎�����␳
		case FIELD:
			if( colobj.getCollision(&op->colobj) ){
				op->revisionPosByCircle(this);
			}
			break;
		//���̑��X���[(�����̂���)
		default:
			break;
		}
	}

	//�ΎR

	//�p�����^
	void FieldVolcano::setParam(void){
		atk = 0; def = 30000; luk = 30000; hp = 30000; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 95;
	}

	//�R���X�g���N�^
	FieldVolcano::FieldVolcano() : Field(NULL,FIELD_VOLCANO){setParam();}
	FieldVolcano::FieldVolcano(GameMain* p,double _x,double _y)
		: Field(p,FIELD_VOLCANO)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = PLAYER_NULL;
		distance = 250;
		warningfinish_cnt = -1;

	}

	void FieldVolcano::draw2D(void)
	{
		//�ʒu
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y+20.0);
		static SDL_Rect rs = { 0,0,128,128 };
		rs.x= 128*((animecount/9)%6);
		SDLFILESRFS::inst().get("image/volcano_anim.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}

	void FieldVolcano::collision(Object* op){
		//if(op==this)return;

		switch(op->get_class_id()){
		//�l�͈ʒu�␳
		case PEOPLE:
			if( colobj.getCollision(&op->colobj) ){
				revisionPosByCircle(op);
			}
			break;
		//�e�͔j��
		case BULLET:
			if(colobj.getCollision(&op->colobj)){
				attackTo(op);
			}
			break;
		//�����͕␳
		case BUILDING:
			if( colobj.getCollision(&op->colobj) ){
				revisionPosByCircle(op);
			}
			break;
		//�d�v���̓X���[����
		case IMPORTANCE:
			//���_�����ʁB
			if( op->get_object_id()==IMPORTANCE_BASE && colobj.getCollision(&op->colobj) ){
				revisionPosByCircle(op);
			}
			break;
		//�t�B�[���h�I�u�W�F�N�g�͕␳
		case FIELD:
			if( colobj.getCollision(&op->colobj) ){
				revisionPosByCircle(op);
			}
			break;
		//���̑��X���[
		default:
			break;
		}
	}

	//�ΎR�e��ł炵��
	void FieldVolcano::move(){
		Field::move();

		//��΂��󋵂̂Ƃ�
		if(animecount < warningfinish_cnt){
			//�I���܂ł̃J�E���g
			int c = warningfinish_cnt - animecount;
			//�ł��܂�
			if(c%30==0) gamemain->objects.push_back(new Meteo(gamemain,0,fieldpos.x,fieldpos.y+50,rand()%(distance*2)-distance+fieldpos.x,rand()%(distance*2)-distance+fieldpos.y) );
		}
		//��΂��Ȃ��Ƃ�
		else{
			//�������������΂��Ȃ邩������Ȃ�
			if(rand()%2100==0){
				warningfinish_cnt = animecount + 200;
				gamemain->objects.push_back(new WarningEffect(gamemain,200));
				gamemain->systemmain->sound_warning.play(5);
			}
		}
	}


	//�΂̂悤�ȕ���
	//�p�����^
	void FieldRock::setParam(void){
		atk = 0; def = 1000; luk = 0; hp = 1000; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 20;
	}

	//�R���X�g���N�^
	FieldRock::FieldRock() : Field(NULL,FIELD_ROCK){setParam();}
	FieldRock::FieldRock(GameMain* p,double _x,double _y)
		: Field(p,FIELD_ROCK)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = 0;


	}

	void FieldRock::draw2D(void){
		//�ʒu
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect rs = { 0,0,32,32 };
		SDLFILESRFS::inst().get("image/rock.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}


	//��
	//�p�����^
	void FieldTree::setParam(void){
		atk = 0; def = 1000; luk = 0; hp = 1000; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 20;
	}

	//�R���X�g���N�^
	FieldTree::FieldTree() : Field(NULL,FIELD_TREE){setParam();}
	FieldTree::FieldTree(GameMain* p,double _x,double _y)
		: Field(p,FIELD_TREE)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = 0;

	}

	void FieldTree::draw2D(void){
		//�ʒu
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect rs = { 0,0,32,32 };
		SDLFILESRFS::inst().get("image/tree.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}

	//�T�{�e��
	//�p�����^
	void FieldSaboten::setParam(void){
		atk = 0; def = 1000; luk = 0; hp = 500; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 10;
	}

	//�R���X�g���N�^
	FieldSaboten::FieldSaboten() : Field(NULL,FIELD_SABOTEN){setParam();}
	FieldSaboten::FieldSaboten(GameMain* p,double _x,double _y)
		: Field(p,FIELD_SABOTEN)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = 0;

	}

	void FieldSaboten::draw2D(void){
		//�ʒu
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect rs = { 0,0,24,24 };
		SDLFILESRFS::inst().get("image/saboten.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}


	//===============================================
	//�Ƃ��Ă��G�t�F�N�g
	//==============================================

	//���j�G�t�F�N�g

	//�p�����^
	void ExplosionEffect::setParam(void){
		atk = 0; def = 0; luk = 0; hp = 0; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 0;
	}

	//�R���X�g���N�^
	ExplosionEffect::ExplosionEffect() : Effect(NULL,EFFECT_EXPLOSION){setParam();}
	ExplosionEffect::ExplosionEffect(GameMain* p,double _x,double _y)
		: Effect(p,EFFECT_EXPLOSION)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;

		player_id = PLAYER_NULL;

	}

	void ExplosionEffect::draw2D(void){
		//===================
		//SDL�̕`��
		//===================
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static int c;
		static SDL_Rect rs = { 0,0,72,72 };
		c = (int)((double)animecount/1.5);
		rs.x = 72*(c%8);
		rs.y = 72*(c/8);
		SDLFILESRFS::inst().get("image/exp02.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}
	void ExplosionEffect::move(){
		Effect::move();
		if((double)animecount/1.5>=32.0) isNeed = false;
	}


	//�����X�\���G�t�F�N�g

	//�p�����^
	void MeteoPointEffect::setParam(void){
		atk = 0; def = 0; luk = 0; hp = 0; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 0;
	}

	//�R���X�g���N�^
	MeteoPointEffect::MeteoPointEffect() : Effect(NULL,EFFECT_METEOPOINT){setParam();}
	MeteoPointEffect::MeteoPointEffect(GameMain* p,double _x,double _y)
		: Effect(p,EFFECT_METEOPOINT)
	{
		setParam();

		fieldpos.x=_x; fieldpos.y=_y;
		fieldspd.x=0.0;fieldspd.y=0.0;

		maincircle.p = fieldpos;
		zpos = -256;

		player_id = PLAYER_NULL;

	}

	void MeteoPointEffect::draw2D(void){
		//===================
		//SDL�̕`��
		//===================
		static SDL_Rect r = {0,0,0,0};
		r.x = (Sint16)(fieldpos.x+(double)gamemain->field_rect.x+gamemain->player[gamemain->player_ctrl].drawoffset.x);
		r.y = (Sint16)(fieldpos.y+(double)gamemain->field_rect.y+gamemain->player[gamemain->player_ctrl].drawoffset.y);
		static SDL_Rect rs = { 0,0,96,96 };
		rs.x = 96*((animecount/6)%5);
		SDLFILESRFS::inst().get("image/meteo_point.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}


	//Warning�G�t�F�N�g

	//�p�����^
	void WarningEffect::setParam(void){
		atk = 0; def = 0; luk = 0; hp = 0; rpd = 0.0;
		maketime = 0; price = 0;
		maincircle.d = 0;
	}

	//�R���X�g���N�^
	WarningEffect::WarningEffect() : Effect(NULL,EFFECT_WARNING){setParam();}
	WarningEffect::WarningEffect(GameMain* p,int cnt)
		: Effect(p,EFFECT_WARNING)
	{
		setParam();

		fieldpos.x=0.0; fieldpos.y=0.0;
		fieldspd.x=0.0; fieldspd.y=0.0;

		maincircle.p = fieldpos;
		zpos = 255;

		player_id = PLAYER_NULL;
		display_cnt = cnt;

	}

	void WarningEffect::draw2D(void){
		//===================
		//SDL�̕`��
		//===================
		static SDL_Rect r = { 0,0,0,0};
		r.x = (Sint16)(gamemain->field_rect.w/2 + gamemain->field_rect.x);
		r.y = (Sint16)(gamemain->field_rect.h/2 + gamemain->field_rect.y);
		static SDL_Rect rs = { 0,0,89,78 };

		//���ԓI��2:1�̊����ŕ\��
		if(animecount%39<26)
			SDLFILESRFS::inst().get("image/warning.gif").Draw(gamemain->systemmain->screen,&r,&rs,gamemain->systemmain->screen->h);
	}

	void WarningEffect::move(){
		if(animecount > display_cnt)isNeed=false;
		Effect::move();
	}

}
