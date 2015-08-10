#ifndef _INPUT_H_
#define _INPUT_H_

#include <map>
#include "collision.h"

namespace space{

	//�L�[�{�[�h���
	//�L�[�{�[�h�̒l�͐����ɕϊ������B�ǂ������ϊ����͎g���l���l���邱�ƁB
	class Keyboard
	{
	public:
		typedef std::unordered_map<unsigned short,unsigned short> KeyMap;
	protected:
		KeyMap keys;
	public:

		void clear(){keys.clear();}

		Keyboard(){clear();}
		~Keyboard(){}

		//�Z�b�g
		void pushkey(unsigned short _k) { keys[_k] = _k; }
		//�A���Z�b�g
		void pullkey(unsigned short _k) { 
			KeyMap::iterator it = keys.find(_k);
			if( it != keys.end() ) keys.erase(it); 
		}
		//�������炤
		bool ispush(unsigned short _k) const{
			
			if( keys.find(_k) == keys.end() ) return false;
			return true;
		}
		//�L�[��S���ۂ��ƃv���[���g����
		const KeyMap& getkeys() const{ return keys;}
	};


	//�W���C�X�e�B�b�N���
	//�{�^������13�Ɖ���B
	class Joystick
	{
		bool button[13];
		int x,y,z;
	public:
		void clear(){
			x=0;y=0;z=0;
			for(int i=0;i<16;i++) button[i]=false;
		}

		Joystick(){clear();}
		~Joystick(){}

		//�Z�b�g(unset�̕K�v�Ȃ�)
		void setx(int _k) { x=_k; }
		void sety(int _k) { y=_k; }
		void setz(int _k) { z=_k; }
		//�{�^���n�Z�b�g
		void pushbutton(unsigned int _b){ if(_b>=13)return; button[_b]=true; }
		//�{�^���n�A���Z�b�g
		void pullbutton(unsigned int _b){ if(_b>=13)return; button[_b]=false; }

		//�������炤
		int getx() const { return x; }
		int gety() const { return y; }
		int getz() const { return z; }
		bool ispush(unsigned int _b) const { if(_b>=13)return false; return button[_b]; }

	};

	//�}�E�X
	//4�{�^���ȏ�Ƃ��m���B
	class Mouse
	{
		
	public:
		struct Button
		{
		private:
			bool ispush;
			Matrix21<int> pushpos;
			Matrix21<int> pullpos;
		public:
			void clear(){
				ispush=false;
				pushpos = Matrix21<int>(0,0);
				pullpos = Matrix21<int>(0,0);
			}
			Button(){ clear(); }
			void push(const Matrix21<int>& _pos){ispush=true; pushpos=_pos;}
			void pull(const Matrix21<int>& _pos){ispush=false;pullpos=_pos;}
			const Matrix21<int>& getPushpos() const {return pushpos;}
			const Matrix21<int>& getPullpos() const {return pullpos;}
			bool isPush() const {return ispush;}

			std::string output() const{
				std::stringstream ss("");
				ss << "::" << ispush << ":ps(" << pushpos.x << "," << pushpos.y << "):pl(" << pullpos.x << "," << pullpos.y << ")";	
				return ss.str();
			}
		};
	protected:
		Button left,right,middle;
		Matrix21<int> nowpos;
	public:
		void clear(){
			left.clear();
			right.clear();
			middle.clear();
			nowpos = Matrix21<int>(0,0);
		}

		Mouse(){clear();}

		//�Z�b�g(�A���Z�b�g�K�v�Ȃ�)
		void setpos(const Matrix21<int> &_pos){ nowpos=_pos; }
		//�Q�b�g
		const Matrix21<int>& getpos() const { return nowpos; }
		
		//�Ԃ�����
		const Button &getleft() const {return left;}
		const Button &getmiddle() const {return middle;}
		const Button &getright()const {return right;}

		Button &setleft() {return left;}
		Button &setmiddle() {return middle;}
		Button &setright() { return right;}

		std::string output() const{
			std::stringstream ss("");
			ss << "*l" << left.output() << "::m" << middle.output() << "::r" << right.output() << "::p(" << nowpos.x << "," << nowpos.y << ")";	
			return ss.str();
		}
		
	};

	struct Input
	{
		Keyboard keyboard;
		Mouse mouse;
		Joystick joystick;
		
		void clear(){
			keyboard.clear();
			mouse.clear();
			joystick.clear();
		}

		std::string output() const{
			std::stringstream ss("");
			ss << "**ms" << mouse.output();
			return ss.str();
		}
	};

}

#endif
