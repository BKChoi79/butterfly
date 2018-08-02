#ifndef __SCEME_H__
#define  __SCEME_H__

class Scene
{
public:
	Scene();
	virtual ~Scene();

	static Scene* create();

	virtual bool Init();
	virtual void Proc();
	virtual void ProcServer();
	virtual void KeyPressed(int c);
};

#endif