#pragma once
#ifndef EULAR_OPERATION_H
#define EULAR_OPERATION_H

#include "HalfEdgeDataStructure.h"
#include<vector>
class EularOperation {
public:
	EularOperation() : solid(nullptr) {}
	Solid *mvfs(Point p, Vertex * &v);	//���壺����һ���壬һ���棬һ���⻷��һ���㡣
	HalfEdge *mev(Point p2, Vertex *v1, Loop *lp);	//��� : ����һ���µ㣬ͬʱ����һ�������µ�����һ��������±ߡ�
	Loop *mef(Vertex *v1, Vertex *v2, Loop *lp);	//���� : ����������������һ���±ߣ�ͬʱ����һ���µĻ����µ��档
	Loop *kemr(Vertex *v1, Vertex *v2, Loop *lp);	//�컷:ɾ��һ���ߣ����ɸñ�ĳһ�����ϵ��µ��ڻ�
	void kfmrh(Face *f1, Face *f2);	//���
	Solid *sweep(double dist, double dir[3]);
	Solid *getSolid() {
		return solid;
	}
	Solid *setSolid(Solid *_solid) {
		solid = _solid;
		return solid;
	}
	bool isSweepFace;

private:
	Solid *solid;
};





#endif // !EULAR_OPERATION_H
