#ifndef _TITLE_H_
#define _TITLE_H_

#include "systemstate.h"
#include "input.h"

#include "gamesetting.h"

namespace space{

class Title : public SystemState
{
public:
	//==========
	//���͊֘A
	//==========
	const Input *input;
	unsigned short pushtime_left;
	unsigned short pushtime_middle;
	unsigned short pushtime_right;
	//key=�{�^���ԍ� , value=�����ꂽ�t���[����
	std::unordered_map<unsigned short,unsigned short> pushtime_key;

public:
	Title(ProgramSystemMain* sys);
	virtual ~Title(void);

	virtual void draw();
	virtual void update();
};

}// end of namespace space

#endif
