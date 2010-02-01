#ifndef _GAMEMAIN_H_
#define _GAMEMAIN_H_

#include "systemstate.h"
#include "gamemainobject.h"
#include "input.h"
#include "ColTrees.h"

namespace space
{
	class Object;
	class PawnPeople;
	class NormalBullet;
	class Fortress;
	class NormalBuilding;
	class Bomb;
	class GroundBomb;
	class Shop;
	class Smith;

	enum{
		BASETYPE_MONEY = 0,
		BASETYPE_LOVE,
		BASETYPE_POWER
	};

	//�t�B�[���h�̃N���X
	class GameMain : public SystemState
	{
	public:
		unsigned int deadcount;
	public:
		//�v���C���[
		struct Player{

			//==========
			//���_�֘A
			//==========
			unsigned int score;
			unsigned int killed_cnt;
			unsigned int bekilled_cnt;
			int spendmoney;

			Object* base; //���_
			unsigned int typeofBase; //���_�^�C�v

			//============
			//�������֘A
			//============
			int money; //����
			unsigned char items[64]; //�z��ԍ�=�A�C�e���̔ԍ� ���g=�A�C�e���̐�

			//==========
			//���͊֘A
			//==========
			const Input *input;
			Matrix21<int> bmousepos;
			unsigned short pushtime_left;
			unsigned short pushtime_middle;
			unsigned short pushtime_right;
			bool ispull_left;
			bool ispull_middle;
			bool ispull_right;
			//key=�{�^���ԍ� , value=�����ꂽ�t���[����
			std::map<unsigned short,unsigned short> pushtime_key;
			std::map<unsigned short,unsigned short> ispull_key; 

			//==========
			//�I���֘A
			//==========
			//���܁A�I�����Ă���I�u�W�F�N�g�B
			//�����Ă��̔z���delete���Ȃ��悤�ɁB
			std::list<Object*> selected;
			//���܁A�I�����Ă���A�C�e���̔ԍ��B
			//0�őI�����Ă��Ȃ��B
			unsigned int selected_item;
			//�\������Ă���A�C�e��
			unsigned short shownitems[12];
			//�\������Ă���A�C�e���̎��
			unsigned short sortofitems;
			//�\���I�t�Z�b�g
			Matrix21<double> drawoffset;
			Matrix21<double> drawoffset_b;
			//�Ȃ܂�
			std::string name;

			//�������g��
			bool spendMoney(int um){
				if(money >= um){
					money -= um;
					spendmoney += um;
					return true;
				}
				return false;
			}

		};

		//�A�C�e���i�ݒu�ł�����̑S���܂Ƃ߂Ă����������Ƃɂ���j
		struct Item{
			//�A�C�e���̉摜�Ɛ؂���ʒu
			const SDLSURFACE* showpic;
			WinRect r;
			//�ݒu����I�u�W�F�N�g
			Object* obj;

			Item(){
				showpic = &SDLFILESRFS::inst().get("image/default.gif");
				obj = NULL;
				r.x=0; r.y=0; r.w=32; r.h=32;
			}
		};

		//�t�B�[���h��ʂ̑傫��
		WinRect field_rect;
		//�R���g���[����ʂ̑傫��
		WinRect control_rect;
		WinRect items_rect[12];
		//���\����ʂ̑傫��
		WinRect states_rect;
		//�����\����ʂ̑傫��
		WinRect money_rect;
		//�f�[�^�\���E�C���h�E
		WinRect data_rect;

		//�����p�̃t�H���g
		BMPFONT moneyfont;
		//�X�e�[�^�X�̃t�H���g
		BMPFONT statusfont;

		//�A�C�e���̎�ޕ\������Ƃ��̉摜�̔z��
		std::vector<const SDLSURFACE*> sortofitems_drawpict;
		//��ޕύX����Ƃ��B
		WinRect flippos;

		//�I�u�W�F�N�g�S��
		std::list<Object*> objects;
		IKD::CLiner4TreeManager<Object> mortontree;
		IKD::CollisionList<Object> *mortoncollist;

		//�����̎�
		unsigned int randamseed;

		//player�Ɋւ������
		std::vector<Player> player;
		//�����̔ԍ�
		unsigned int player_ctrl;
		
		//���v���C�t�@�C��
		std::fstream replayfile;
		bool setrep;

		//item�Ɋւ������
		std::vector<Item> item;

		//�����p�̃J�E���g
		unsigned int forEnd_cnt;


	public:

		PawnPeople* setPawn(unsigned short _pid, double _x, double _y);
		NormalBullet* setNormalBullet(unsigned short _pid, double _x, double _y, double vx, double vy);
		NormalBuilding* setNormalBuilding(unsigned short _pid, double _x,double _y);
		Fortress* setFortress(unsigned short _pid, double _x,double _y);
		Bomb* setBomb(unsigned short _pid, double _x,double _y);
		GroundBomb* setGroundBomb(unsigned short _pid, double _x,double _y);
		Shop* setShop(unsigned short _pid, double _x,double _y);
		Smith* setSmith(unsigned short _pid, double _x, double _y);

		void changePlayerCtrl(unsigned int);

		GameMain(ProgramSystemMain* _sys, std::vector<Player> _pl,unsigned int _ctrlpl, unsigned int _rands,bool _rep=true);
		GameMain(const GameMain &temp);
		~GameMain();

		void setting3D();

		void update();
		void draw();
		void DisplayReload();

		inline int CollisionAllObjects(Object*** oppp){
			int mortoncolnum = mortontree.GetAllCollisionList(&mortoncollist);
			*oppp = mortoncollist->getRootPtr();
			return mortoncolnum;
		}

	};
}

#endif
