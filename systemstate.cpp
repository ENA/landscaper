#include "systemstate.h"

namespace space
{
	//===========================
	//�h���A�u�X�g���N�g
	//�Q�[�����̏�ʂɊւ������
	//===========================
	SystemState::SystemState(ProgramSystemMain *_system, STATE _st)
		: systemstate(_st), systemmain(_system), isNeed(true)
	{counter = 0;}
	SystemState::~SystemState(){}

	SystemState::STATE SystemState::getSystemState() const {return systemstate;}
	void SystemState::DisplayReload(){}

}
