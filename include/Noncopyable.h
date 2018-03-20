#ifndef _SM_NONCOPYABLE_H__
#define _SM_NONCOPYABLE_H__

namespace SM
{
class Noncopyable
{
public:
	Noncopyable(){}
	~Noncopyable(){}
private:
	Noncopyable(const Noncopyable &);
	Noncopyable & operator=(const Noncopyable &);
};
}//end of namespace SM

#endif
