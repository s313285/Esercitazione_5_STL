#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace 	PolygonalLibrary {

struct PolygonalMesh
{
	unsigned int NumCell0Ds = 0;
	unsigned int NumCell1Ds = 0;
	unsigned int NumCell2Ds = 0;
	
	vector<unsigned int> Cell0DsId = {};
	vector<unsigned int> Cell1DsId = {};
	vector<unsigned int> Cell2DsId = {};
	
	MatrixXd Cell0DsCoordinates = {};
	MatrixXi Cell1DsExtrema = {};
	vector<vector<unsigned int>> Cell2DsEdges = {}; 
	vector<vector<unsigned int>> Cell2DsVertices = {};
	
	map<unsigned int, list<unsigned int>> MarkerCell0Ds = {};
	map<unsigned int, list<unsigned int>> MarkerCell1Ds = {};
	
};

}

