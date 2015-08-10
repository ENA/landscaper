#ifndef _SYSTEMSTATE_H_
#define _SYSTEMSTATE_H_

#include "common.h"
#include "collision.h"
#include "graphic.h"
#include "input.h"
#include "matrixobj.h"

#include "programsystemmain.h"

//�K���Ȗ��O���
namespace space
{

	struct WinRect
	{
		int x,y,w,h;
		WinRect(){}
		WinRect(int _x,int _y,int _w,int _h){
			if(_w>=0){x=_x;w=_w;}else{x=_x+_w;w=-_w;}
			if(_h>=0){y=_y;h=_h;}else{y=_y+_h;h=-_h;}
		}
		
	        template<class T>
		bool Col_Dot(T _d) const{
			return(
				(int)_d.x > x &&
				(int)_d.x < x + w &&
				(int)_d.y > y &&
				(int)_d.y < y + h
				);
		}

		void Draw(SDL_Surface *target,
			unsigned char r,unsigned char g,unsigned char b,unsigned char a,
			unsigned int wh=0)
		{
			int x1=x; int y1=y; int x2=x+w-1; int y2=y+h-1;
			if(wh!=0){ y1=wh-y; y2=wh-y2;}
			rectangleRGBA(
				target,
				x1,y1,x2,y2,
				r,g,b,a
			);
		}

		operator SDL_Rect(){SDL_Rect r={(Sint16)x,(Sint16)y,(Uint16)w,(Uint16)h}; return r;}
	};

	//===========================
	//�h���A�u�X�g���N�g
	//�Q�[�����̏�ʂɊւ������
	//===========================
	//�N���X
	class SystemState : public DrawAndUpdate
	{
	public:
		//��ʂ̒萔
		enum STATE{
			NONE = 0,
			TITLE,
			GAMESETTING,
			GAMENETWORK,
			BASESETTING,
			FIELDSETTING,
			GAMEMAIN,
			RESULT,
			FADEOUT,
			FADEIN,
			GAMEDEMO,
			DISPLAYDEMO,
			TEXTINPUT,
			STATEDUMMY
		};

	protected:
		STATE systemstate;
	public:
		ProgramSystemMain *systemmain;
		bool isNeed;
		//�ւڂ�����
		unsigned int counter;

	public:
		SystemState(ProgramSystemMain *_system, STATE _st=NONE);
		virtual ~SystemState();

		STATE getSystemState() const;
		virtual void DisplayReload();

		//�X�e�[�g����p�֐��I�u�W�F�N�g
		class stateof : public std::binary_function<SystemState*,STATE,bool>{
			public:
				result_type operator() (first_argument_type f,second_argument_type s) const{
					return (f->getSystemState()==s);
				}
		};
	};


}


#endif
