#ifndef _GAMEMAINOBJECT_H_
#define _GAMEMAINOBJECT_H_

#include "gamemain.h"
#include "ColTrees.h"

namespace space
{
	//�O���錾
	class GameMain;

	//============================
	//�A�u�X�g���N�g
	//GameMain::�I�u�W�F�N�g�Ɋւ������
	//============================
	class Object
	{
	//���ʂɑ�����`
	public:
		//�K����typedef
		typedef short STATUS;
		typedef unsigned short PLAYERID;
		//enum�Ƃ��萔�Ƃ�
		enum CLASSID {
			NONE = 0,
			PEOPLE,
			BULLET,
			BUILDING,
			IMPORTANCE,
			FIELD,
			EFFECT
		};
		enum ID {
			PEOPLE_NULL = 0,
			PEOPLE_PAWN,
			PEOPLE_GUNNER,
			BULLET_NULL = 0,
			BULLET_NORMAL,
			BULLET_BOMBFIRE,
			BULLET_METEO,
			BULLET_BOMB,
			BULLET_GROUNDBOMB,
			BUILDING_NULL = 0,
			BUILDING_NORMAL,
			BUILDING_FORTRESS,
			BUILDING_SHOP,
			BUILDING_SMITH,
			IMPORTANCE_NULL = 0,
			IMPORTANCE_GOD,
			IMPORTANCE_BASE,
			FIELD_NULL = 0,
			FIELD_VOLCANO,
			FIELD_ROCK,
			FIELD_TREE,
			FIELD_SABOTEN,
			EFFECT_NULL = 0,
			EFFECT_EXPLOSION,
			EFFECT_METEOPOINT,
			EFFECT_WARNING
		};
		static const PLAYERID PLAYER_NULL;

	//�����Ă����Ȃ��ƕs�ւ�����p�����[�^
	public:
		//====================
		//���ʃp�����[�^
		//====================
		//�e
		GameMain *gamemain;
		//�����蔻��p
		IKD::OBJECT_FOR_TREE<Object> formorton;

		//�K�v�Ȃ��Ȃ����̂��ǂ��Ȃ̂��A�Ƃ����t���O
		bool isNeed;

		//�e��p�����[�^
		//�d�v�Ȃ̂́A�����̃p�����[�^�͎g����ꍇ�������
		//�g���Ȃ��ꍇ�����|�I�ɂ���Ƃ������ƁB
		STATUS atk;	//�U��
		STATUS def;	//�h��
		double rpd;	//�ړ����x
		STATUS luk;	//�^�̋���
		STATUS hp;		//�q�b�g�|�C���g
		STATUS maketime;//��鎞��(�t���[��)
		STATUS price;	//�l�i

		double powerdist; //���͋���

		//�t�B�[���h��ł̌��݈ʒu
		Matrix21<double> fieldpos;
		//�t�B�[���h��ł̑��x
		Matrix21<double> fieldspd;
		//�\�����C���[(�f�t�H���g��0)
		int zpos;

		//�����蔻��p�̃I�u�W�F�N�g
		CollisionDetective2D colobj;
		//��{�I�ɉ~�Ȃ̂ł����ɂ����Ă����B
		Circle2<double> maincircle;
		//�����蔻��p�B
		Circle2_COL2D colcircle;
		//���[�g���p
		Matrix21<double> mrect_lt,mrect_rb;
		//�~�̂��Ƃ��낢��
		void revisionPosByCircle(Object *p);

		//�A�j���p�ƌ����Ă�킯�ł͂Ȃ����B
		int animecount;

	//�B���p�����[�^
	protected:
		//�ǂ̃N���X�̃I�u�W�F�N�g�Ȃ̂����w������
		CLASSID class_id;
		//�I�u�W�F�N�g�����Ȃ̂����w������
		ID object_id;
		//�ǂ̃v���C���[�ɑ�����I�u�W�F�N�g�Ȃ̂�������
		PLAYERID player_id;

	public:

		//�R���X�g���N�^
		Object();
		Object(GameMain *p, CLASSID _id = NONE);
		virtual ~Object() { formorton.Remove(); }

		//ID_get
		inline Object::CLASSID get_class_id(){ return class_id; }
		inline Object::ID get_object_id(){ return object_id; }
		inline Object::PLAYERID get_player_id(){ return player_id; }
		//�U������
		virtual void attackTo(Object* src);

		//�f�t�H���g�͗p�ӂ��邯�Ǌm���Ɏ������Ă��������B
		virtual void draw2D(){}
		virtual void draw3D(){}

		//�蓮�œ��������Ɏg���܂��B�Ƃ������ŏI�I�ɂ͉���������œ������悤�ɁB
		virtual void move_manual();
		virtual void move(){animecount++;move_manual();}

		//�����蔻��
		virtual void collision(Object* obj){}
		virtual void collision(std::list<Object*> &list){
			std::list<Object*>::iterator it = list.begin();
			for(;it!=list.end();it++){if(*it!=this){collision(*it);}}
		}

		//�Q�Ɗ֌W�̉����Ɏg�����肵�܂��B
		//�܂��g��������Ύg���ł����񂶂�Ȃ��ł��傤���B
		virtual void after_collision(){}

		//�p�����[�^�̃Z�b�g(���̂����N���X�S�ĂŐ錾���邱��)
		virtual void setParam() = 0;

		//���s����
		//move -> collision -> after_collision -> draw2D,draw3D

	};

	//�ʒu�\�[�g�p�̊֐��I�u�W�F�N�g
	class ObjectSortFuncByPos{
	public:
		bool operator()(const Object* lt, const Object* rt) const{
			return (
				//�܂���zpos
				(lt->zpos < rt->zpos) ||
				//zpos�����l�Ȃ炙�Ŕ��f
				(lt->zpos==rt->zpos && lt->fieldpos.y > rt->fieldpos.y)
			);
		}
	};

	//======================
	//�l�Ԃ̃A�u�X�g���N�g
	//======================
	//�܂�����Ȃɒ�`�͂Ȃ��킯����
	class People  : public Object
	{
	public:
		Matrix21<double> dstpos;	//�ړI�n
		int state;	//���̏��
	public:
		People(){}
		People(GameMain* p,ID _oid = PEOPLE_NULL);
		virtual ~People(){}
		
		//�Փ˔���
		virtual void collision(Object* obj);
		virtual void after_collision(){ if(hp<=0){ isNeed=false; hp=0; } }
		//��
		virtual void move(void){Object::move();}

	};


	//����
	class PawnPeople : public People
	{
	public:

		PawnPeople();
		PawnPeople(GameMain* p,PLAYERID _pid,double _x,double _y);

		//2D�`��
		inline void draw2D(void);

		//����(update�݂����Ȃ��́A������s�������)
		inline void move(void);

		//�p�����[�^�Z�b�g
		inline void setParam();
	};

	//�e��
	class Gunner : public People
	{
	public:
		int range;

	public:
		Gunner();
		Gunner(GameMain* p,PLAYERID _pid,double _x,double _y);

		//2D�`��
		inline void draw2D(void);

		//����(update�݂����Ȃ��́A������s�������)
		inline void move(void);

		//�p�����[�^�Z�b�g
		inline void setParam();
	};

	//=====================
	//�e�ۂ̃A�u�X�g���N�g
	//======================
	//�����َq�ł͂Ȃ�
	class Bullet : public Object{
	public:
		Bullet(){}
		Bullet(GameMain* p,ID _oid = BULLET_NULL);
		virtual ~Bullet();

		virtual void collision(Object* op);
		virtual void after_collision();
	};

	//���ʂ̒e
	class NormalBullet : public Bullet
	{
	public:
		double distance;
		Matrix21<double> startpos;
	public:

		NormalBullet();
		NormalBullet(GameMain* p,PLAYERID _pid,double _x,double _y,double vx,double vy);

		virtual void draw2D(void);
		virtual void move(void);
		virtual void after_collision(void);

		//�p�����[�^�Z�b�g
		virtual void setParam(void);
	};

	//����
	class BombFire : public Bullet
	{
	public:
		unsigned int damageframe;
	public:

		BombFire();
		BombFire(GameMain* p,PLAYERID _pid,double _x,double _y,double _d,unsigned int frame);

		virtual void draw2D(void);
		virtual void move(void);
		virtual void after_collision(void);

		//�p�����[�^�Z�b�g
		virtual void setParam(void);
	};

	//���e�I
	class Meteo : public Bullet
	{
	public:
		Matrix21<double> dstpos;
		Matrix21<double> startpos;
		double PBa,PBb,PBc;
		Object* meteopointeffect;
	public:

		Meteo();
		Meteo(GameMain* p,PLAYERID _pid,double _x,double _y,double dx,double dy);

		virtual void draw2D(void);
		virtual void move(void);
		
		virtual void after_collision(void);

		//�p�����[�^�Z�b�g
		virtual void setParam(void);
	};

	//���e
	class Bomb : public Bullet
	{
	public:
		Bomb();
		Bomb(GameMain* p,PLAYERID _pid,double _x,double _y);
		virtual void draw2D(void);

		virtual void after_collision();


		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//�n��
	class GroundBomb : public Bullet
	{
	public:
		GroundBomb();
		GroundBomb(GameMain* p,PLAYERID _pid,double _x,double _y);
		virtual void draw2D(void);

		virtual void after_collision();

		//�p�����[�^�Z�b�g
		virtual void setParam();
	};


	//=====================
	//�����̂���
	//======================
	class Building : public Object
	{
	public:
		int poolnum; //�c���Ă�l�̐�

	public:
		Building(){}
		Building(GameMain* p,ID _oid = BUILDING_NULL);
		virtual ~Building(){}

		virtual void collision(Object* op);
		virtual void after_collision();

	};

	//���ʂ̌���
	class NormalBuilding : public Building
	{
	public:
		NormalBuilding();
		NormalBuilding(GameMain* p,PLAYERID _pid,double _x,double _y);
		virtual void draw2D(void);
		virtual void move(void);
		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//�v��
	class Fortress : public Building
	{
	public:
		Fortress();
		Fortress(GameMain* p,PLAYERID _pid,double _x,double _y);
		virtual void draw2D(void);
		virtual void move(void);
		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//�X
	class Shop : public Building
	{
	public:
		Shop();
		Shop(GameMain* p,PLAYERID _pid,double _x,double _y);
		virtual void draw2D(void);
		virtual void move(void);
		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//�b�艮
	class Smith : public Building
	{
	public:
		Smith();
		Smith(GameMain* p,PLAYERID _pid,double _x,double _y);
		virtual void draw2D(void);
		virtual void move(void);
		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//=============================
	//�d�v�Ȃ���
	//=============================
	//�A�u�X�g���N�g
	class Importance : public Object
	{
	public:
		Importance(){}
		Importance(GameMain* p,ID _oid = IMPORTANCE_NULL);
		virtual ~Importance(){}


	};


	//���_
	class PlayerBase : public Importance
	{
	public:

		PlayerBase();
		PlayerBase(GameMain* p,PLAYERID _pid,double _x,double _y);

		virtual void draw2D(void);

		virtual void move(void);
		virtual void collision(Object* op);
		virtual void after_collision();

		//�p�����[�^�Z�b�g
		void setParam();
	};



	//===============================================
	//�t�B�[���h�ɂ��镨�́A�܂��͑S�̂̂��ƂɊւ��āB
	//===============================================
	class Field : public Object
	{
	public:
		Field(){}
		Field(GameMain* p,ID _oid = FIELD_NULL);
		virtual ~Field();
		
		virtual void after_collision(){
			if(hp<=0){isNeed=false;hp=0;}
		}
		virtual void collision(Object* op);
	};

	//�ΎR
	class FieldVolcano : public Field
	{
	public:
		int distance;
		int warningfinish_cnt;
	public:
		FieldVolcano();
		FieldVolcano(GameMain* p,double _x,double _y);
		virtual void draw2D(void);
		virtual void move(void);

		virtual void collision(Object* op);
		virtual void after_collision(){
			if(hp<=0){hp=0;}
		}


		//�p�����[�^�Z�b�g
		virtual void setParam();
	};


	//��
	class FieldRock : public Field
	{
	public:
		FieldRock();
		FieldRock(GameMain* p,double _x,double _y);
		virtual void draw2D(void);


		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	
	//��
	class FieldTree : public Field
	{
	public:
		FieldTree();
		FieldTree(GameMain* p,double _x,double _y);
		virtual void draw2D(void);

		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//���ڂĂ�
	class FieldSaboten : public Field
	{
	public:
		FieldSaboten();
		FieldSaboten(GameMain* p,double _x,double _y);
		virtual void draw2D(void);

		//�p�����[�^�Z�b�g
		virtual void setParam();
	};


	//==========
	//�G�t�F�N�g
	//==========
	class Effect : public Object
	{
	public:
		Effect(){}
		Effect(GameMain* p,ID _oid) : Object(p,EFFECT){ object_id = _oid; player_id = PLAYER_NULL; }
		virtual ~Effect(){}
		virtual void move_manual(){Object::move_manual();formorton.Remove();}
	};

	//�����G�t�F�N�g
	class ExplosionEffect : public Effect
	{
	public:
		ExplosionEffect();
		ExplosionEffect(GameMain* p,double _x,double _y);

		virtual void draw2D(void);
		virtual void move(void);

		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//�����X�\���G�t�F�N�g
	class MeteoPointEffect : public Effect
	{
	public:
		MeteoPointEffect();
		MeteoPointEffect(GameMain* p,double _x,double _y);

		virtual void draw2D(void);

		//�p�����[�^�Z�b�g
		virtual void setParam();
	};

	//���[�j���O�G�t�F�N�g
	class WarningEffect : public Effect
	{
	public:
		int display_cnt;
	public:
		WarningEffect();
		WarningEffect(GameMain* p,int cnt);

		virtual void draw2D(void);
		virtual void move(void);

		//�p�����[�^�Z�b�g
		virtual void setParam();
	};
}

#endif
