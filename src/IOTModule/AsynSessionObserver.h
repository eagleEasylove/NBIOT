
#ifndef _C_ASYNSESSION_OBSERVER_H_
#define _C_ASYNSESSION_OBSERVER_H_


class CAsynSessionObserver
{
public:
	
	CAsynSessionObserver();
	virtual ~CAsynSessionObserver();
	
public:
	
	virtual void OnInput(char *data, int len);
	virtual void handle_time(int time = 0);

};


#endif


