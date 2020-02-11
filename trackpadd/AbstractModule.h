#pragma once
class AbstractModule
{
public:
	AbstractModule();
	~AbstractModule();

public:
	virtual int Start() = 0;
	virtual int Stop() = 0;
};

