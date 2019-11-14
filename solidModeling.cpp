#include "pch.h"
#include "solidModeling.h"
#include<iostream>

void solidModeling::Modeling(EularOperation & eular_op){
	fstream input;
	input.open(input_file);
	if (!input.is_open()) {
		cerr << "Fail to open the input file ' " << input_file << " '" << endl;
		system("pause");
	}

	int pointCount, inloopCount = 0;
	Vertex *v, *inv;
	v = nullptr;
	inv = nullptr;
	HalfEdge *he;
	
	Loop *loop;
	loop = nullptr;
	int solidCount;
	int solidNo = 0;
	int inloopNo = 0;
	pointList.clear();
	int inputflag = 0;	//未输入：0   已输入solidnum: 1  已输入v: 2
	string head;
	while (input >> head) {
		if (head[0] == '#') {
			string comment;
			getline(input, comment);
			continue;
		}
		// solid modeling:
		//1. model solid and outloop
		if (head == "SolidNum") {
			input >> solidCount;
			for (int i = 0; i < solidCount; i++) {
				Solid *solid = new Solid(i);
				solidList.push_back(solid);
			}
			inputflag = 1;
		}
		else if(head == "v"){	//v代表开始输入一个新的实体的点
			if (inputflag == 0) {
				cerr << "You should input the number of solid first. Please check your input file ' " << input_file << " '" << endl;
				system("pause");
			}
			if (solidNo > solidCount - 1) {	//检查实体序号是否越界
				cerr << "The number of solid is not consistent. Please check your input file ' " << input_file << " '" << endl;
				system("pause");
			}
			eular_op.setSolid(solidList[solidNo]);	//指向相应实体
			input >> pointCount;	
			pointList.clear();
			for (int i = 0; i < pointCount; i++) {
				Point p;
				input >> p;
				pointList.push_back(p);
			}
			checkPoints(pointCount);

			Point firstpoint(pointList[0]);
			solidList[solidNo] = eular_op.mvfs(firstpoint, v);	//造体
			loop = solidList[solidNo]->sfaces->floops;
			he = eular_op.mev(pointList[1], v, loop);	//造边

			for (int i = 2; i < pointCount; i++) {
				he = eular_op.mev(pointList[i], he->endv, loop);	//造边
			}

			eular_op.mef(he->endv, v, loop);	//造面
			solidNo++;	//	实体编号
		}
		else if (head == "inloop") {
			inputflag = 2;
			inloopNo = 0;
			//2. model inner loops
			input >> inloopCount;
		}
		else if (head == "inv") {
			if (inputflag != 2) {
				cerr << "You should input the number of inner loops first. Please check your input file ' " << input_file << " '" << endl;
				system("Pause");
			}
			if (inloopNo == inloopCount) {
				cerr << "The number of inner loops is out of range. Please check your input file ' " << input_file << " '" << endl;
				system("Pause");
			}
			pointList.clear();
			input >> pointCount;
			inloopNo++;
			for (int j = 0; j < pointCount; j++) {
				Point p;
				input >> p;
				pointList.push_back(p);
			}
			checkPoints(pointCount);

			he = eular_op.mev(pointList[0], v, loop);	
			inv = he->endv;//内环的第一个点

			for (int j = 1; j < pointCount; j++) {
				he = eular_op.mev(pointList[j], he->endv, loop);
			}
			eular_op.mef(he->endv, inv, loop);
			eular_op.kemr(v, inv, loop);
		}
		else if (head == "sweep") {
			std::cout << "head is sweep." << endl;
			eular_op.isSweepFace = true;
			double dist;
			double dir[3];
			input >> dist >> dir[0] >> dir[1] >> dir[2];
			if (pointList.size() == 0) {
				cerr << "The sweep operation cannot be performed for too few points. Please check your input file ' " << input_file << " '" << endl;
				system("Pause");
			}
			eular_op.sweep(dist, dir);

			vector<Face *> sweepList = solidList[solidNo - 1]->sweepList;
			for (unsigned int i = 1; i < sweepList.size(); i++) {
				std::cout << "kfmrh" << endl;
				eular_op.kfmrh(solidList[solidNo - 1]->sfaces, sweepList[i]);
			}
			eular_op.isSweepFace = false;
		}
		else if(head == "end"){
			if (inloopNo < inloopCount){
				cerr << "The number of inner loops is not enough. Please check your input file ' " << input_file << " '" << endl;
			}
			input.close();
		}
		else{
			cerr << "Wrong format. Please check your input file "<< input_file 
				<< ". For more details, please read the reference sample input file : sample_input.txt/n" << endl;
			system("pause");
		}
	}
}


void solidModeling::checkPoints(int pointCount) {
	//1. check if points count is right
	if (pointList.size() != pointCount) {
		std::cerr << "Wrong: The number of points is not consistant. Please check your input file.\n";
		system("pause");
	}
	if (pointCount < 1) {
		std::cerr << "Wrong: There are not enough point to model a solid. Please check your input file.\n";
		system("pause");
	}
}


void solidModeling::output(){
	ofstream output;
	output.open(output_file);

	if (!output.is_open()) {
		cerr << "Fail to write to the output file ' " << output_file << " ', please check your output file name." << endl;
		system("pause");
	}

	//输出所有实体的信息
	for (int i = 0; i < solidList.size(); i++) {
		output << "----------------------- Solid " << i << " -----------------------\n";
		printVertexes(solidList[i]->vList, output);
		printEdges(solidList[i]->sedges, solidList[i]->edgeCount, output);
		printFaces(solidList[i]->sfaces, solidList[i]->faceCount, output);
		printLoops(solidList[i]->lpList, output);
		output << endl;
	}
	output.close();
}

void solidModeling::printVertexes(vector<Vertex *> vList, ofstream &output){
	output << "Vertexes count: " << vList.size() << endl;
	output << "ID\tCoord\n";
	for (int i = 0; i < vList.size(); i++) {
		output << vList[i]->vertex_id << ".\t" << vList[i]->point << endl;
	}
}

void solidModeling::printEdges(Edge *edge, int edgeCount, ofstream &output){
	output << "Edges count: " << edgeCount << endl;
	Edge *eg = edge;
	int egnum = 0;
	while (eg != nullptr) {
		output << egnum << ".\t" << eg->he1->startv->vertex_id << " — " << eg->he1->endv->vertex_id << endl;
		eg = eg->nexte;
		egnum++;
	}
}

void solidModeling::printFaces(Face *face, int faceNum, ofstream &output){
	output << "Faces count: " << faceNum << endl;
	//vector<int> faceid;
	Face *tmpf = face;
	while (tmpf != nullptr) {
		//faceid.push_back(tmpf->face_id);
		output << tmpf->face_id << ".\t outer loop ID : " << tmpf->floops->loop_id << endl;
		if (tmpf->inloops.size() > 0) {
			output << "inner loop number: " << tmpf->inloops.size() << "\t inner loop ID: ";
			for (unsigned int i = 0; i < tmpf->inloops.size(); i++) {
				output << tmpf->inloops[i]->loop_id;
				if (i != tmpf->inloops.size() - 1) {
					output << ", ";
				}
			}
			output << endl;
		}
		tmpf = tmpf->nextf;
	}
}

void solidModeling::printLoops(vector<Loop *> loopList, ofstream &output){
	output << "Loops count: " << loopList.size() << endl;
	for (int i = 0; i < loopList.size(); i++) {
		output << loopList[i]->loop_id << ".\t";
		HalfEdge *he = loopList[i]->ledge;
		Vertex *firstV = he->startv;
		vector<int> vid;
		 do{
			vid.push_back(he->startv->vertex_id);
			he = he->nxt;
		 } while (he->startv != firstV);
		output << vid.size() << " Points : ";
		for (unsigned int j = 0; j < vid.size(); j++) {
			output << vid[j];
			if (j != vid.size() - 1) {
				output << "->";
			}
		}
		output << endl;
	}
}