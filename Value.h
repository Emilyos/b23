#ifndef Value_H
#define Value_H


class Value
{
public:
	virtual ~Value(){};
	virtual Value* clone() const = 0;
	virtual void print() const = 0;
};

#endif