#ifndef __BUFFER_H__
#define  __BUFFER_H__

#include <queue>
#include <string>
#include <assert.h>
#include "JsonCpp/json/json.h"
#include "Protocols.h"

class Buffer
{
public:
	Buffer();
	~Buffer();
	static Buffer* create();
	bool Init();

	void Push( Json::Value v );
	Json::Value Pop();
	int Size();
	void Clear(std::queue<std::string> &q);

private:
	std::queue<std::string> q;

};
#endif

