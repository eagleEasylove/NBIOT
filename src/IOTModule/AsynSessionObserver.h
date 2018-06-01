
#ifndef _C_ASYNSESSION_OBSERVER_H_
#define _C_ASYNSESSION_OBSERVER_H_


class CAsynSessionObserver
{
public:
	
	CAsynSessionObserver();
	virtual ~CAsynSessionObserver();
	
public:
	
	virtual void OnInput(char *data, int len) = 0;
	virtual void handle_time(int time = 0) = 0;

};


#endif


