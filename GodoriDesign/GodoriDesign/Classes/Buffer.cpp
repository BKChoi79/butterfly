/**
	Buffer
 
	bkchoi79
*/
#include "Buffer.h"

Buffer::Buffer()
{
	
}

Buffer::~Buffer()
{

}

Buffer* Buffer::create()
{
	Buffer* pRet = new Buffer();
	if (nullptr != pRet && true == pRet->Init())
	{
		return pRet;
	}

	delete pRet;
	return nullptr;
}

bool Buffer::Init()
{
	Clear(q);

	return true;
}

void Buffer::Push(Json::Value v)
{
	if (v[PT].isNull())
	{
		assert( 0 && "not allowed format.( missing protocol field.)" );
	}

	std::string data = v.toStyledString();
	q.push(data.c_str());
}

Json::Value Buffer::Pop()
{
	Json::Value err;
	Json::Value v;
	Json::Reader r;

	if (0 == q.size())
	{
		err["success"] = -1;
		err["msg"] = "size is zero.";
		return err;
	}

	std::string data = q.front();
	q.pop();

	bool bReadJson = r.parse(data.c_str(), v);
	if (false == bReadJson)
	{
		err["success"] = -2;
		err["msg"] = "not json format.";
		return err;
	}

	return v;
}

void Buffer::Clear(std::queue<std::string> &q)
{
	std::queue<std::string> empty;
	std::swap(q, empty);
}

int Buffer::Size()
{
	return q.size();
}