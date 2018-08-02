#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <stdlib.h>
#include <assert.h>

template < typename T >
class Singleton
{
public:
	static T * Instance()
	{
		if (instance == nullptr)
		{
			instance = new T;
			if (!instance->Init())
			{
				assert(0 && "instance init failed.");
			}
		}
			
		return instance;
	};

	// call when app gonna finish.
	static void Destroy() 
	{
		if (instance){
			delete instance;
			instance = nullptr;
        }
	};

	// abstract init method. must override this!
	virtual bool Init() = 0; 
    
private:
	static T * instance;
};

template <typename T> 
T* Singleton<T>::instance = 0;

#endif // !__SINGLETON_H__