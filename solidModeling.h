#pragma once
#ifndef SOLID_MODELING_H
#define SOLID_MODELING_H

#include "pch.h"
#include "EularOperation.h"
#include<string>
#include<iostream>
#include<istream>
#include<fstream>
#include<sstream>

class solidModeling {
public:
	solidModeling(string input_file_name, string output_file_name): input_file(input_file_name), output_file(output_file_name){}
	void Modeling(EularOperation &eular_op);
	void output();
	void printVertexes(vector<Vertex *> vList, ofstream &output);
	void printEdges(Edge *edge, int edgeCount, ofstream &output);
	void printFaces(Face *face, int faceNum, ofstream &output);
	void printLoops(vector<Loop *> loopList, ofstream &output);
	void checkPoints(int pointCount);
	vector<Solid *> getsolidList() {
		return solidList;
	}

private:
	string input_file, output_file;
	vector<Point> pointList;
	vector<Solid *> solidList;
};



#endif // !SOLID_MODELING_H