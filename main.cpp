// 3D_CAD_ZC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "solidModeling.h"
#include <GL/glut.h> 
#include <GL/freeglut.h>
#include <iostream>

void display(int argc, char ** argv, vector<Solid *>_solidlist, int _eyePos[3], int _centerPos[3]);

int main(int argc, char **argv){
	EularOperation eular_op;
	solidModeling solidmodel("input.txt", "output.txt");
	solidmodel.Modeling(eular_op);
	solidmodel.output();
	int eyePos[3] = { 0, 20, 50 };
	int centerPos[3] = { 0, 12, 0 };
	display(argc, argv, solidmodel.getsolidList(), eyePos, centerPos);
	return 0;
}
