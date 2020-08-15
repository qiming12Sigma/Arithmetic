#pragma once
class BilinearScale
{
public:
	BilinearScale();
	~BilinearScale();

	void SetInputData(int width,int height,unsigned char*,float ratio);
	void Execute();
	void GetOutputData(int& width,int& height,unsigned char**);
};

