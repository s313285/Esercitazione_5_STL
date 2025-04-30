#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary
{
bool ImportMesh(PolygonalMesh& mesh)
{

    if(!ImportCell0Ds(mesh))
        return false;

    if(!ImportCell1Ds(mesh))
        return false;

    if(!ImportCell2Ds(mesh))
        return false;

    return true;

}
// ***************************************************************************
bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
	
	// memorizzo tutto il file in una lista
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell0Ds = listLines.size();

    if (mesh.NumCell0Ds == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DsId.reserve(mesh.NumCell0Ds);
    mesh.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, mesh.NumCell0Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
		char c;
        unsigned int marker;

        converter >> id >> c >> marker >> c >> mesh.Cell0DsCoordinates(0, id) >> c >> mesh.Cell0DsCoordinates(1, id);
        mesh.Cell0DsId.push_back(id);

		// Memorizza i marker
		if (marker != 0){
			const auto it = mesh.MarkerCell0Ds.find(marker);
			if (it == mesh.MarkerCell0Ds.end()){
				mesh.MarkerCell0Ds.insert({marker, {id}});
			}
			else{
				mesh.MarkerCell0Ds[marker].push_back(id);
			}
		}
    }
	
	
	/* test (all markers are correctly stored) 
	for (unsigned int j=0; j<mesh.Cell0DsId.size(); j++){
		if (mesh.MarkerCell0Ds[j]<0 || mesh.MarkerCell0Ds[j]>8)
			cerr << "the marker with id = " << j << " is not correctly stored" << endl;
	}
	*/

    return true;
}
// ***************************************************************************
bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file("./Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell1Ds = listLines.size();

    if (mesh.NumCell1Ds == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DsId.reserve(mesh.NumCell1Ds);
    mesh.Cell1DsExtrema = Eigen::MatrixXi(2, mesh.NumCell1Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);
		
		char c;
        unsigned int id;
        unsigned int marker;
		
        converter >> id >> c >> marker >> c >> mesh.Cell1DsExtrema(0, id) >> c >> mesh.Cell1DsExtrema(1, id);
		
        mesh.Cell1DsId.push_back(id);
		
		// Memorizza i marker
		if(marker != 0){
            const auto it = mesh.MarkerCell1Ds.find(marker);
            if(it == mesh.MarkerCell1Ds.end()){
                mesh.MarkerCell1Ds.insert({marker, {id}});
            }
            else{
                mesh.MarkerCell1Ds[marker].push_back(id);
            }
        }

    }
	
	
	/* test (all markers are correctly stored)  
	for (unsigned int j=0; j<mesh.Cell1DsId.size(); j++){
		if (mesh.MarkerCell1Ds[j] != 0 && mesh.MarkerCell1Ds[j] != 5 && mesh.MarkerCell1Ds[j] != 6 && mesh.MarkerCell1Ds[j] != 7 && mesh.MarkerCell1Ds[j] != 8)
			//cerr << "the marker in 'MarkerCell1Ds' with id = " << j << " is not correctly stored" << endl;
			cout << mesh.MarkerCell1Ds[j]<< " ";
	}	
	*/
	
    return true;
}
// ***************************************************************************
bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("./Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    // remove header
    listLines.pop_front();

    mesh.NumCell2Ds = listLines.size();

    if (mesh.NumCell2Ds == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }
	
    mesh.Cell2DsId.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsVertices.reserve(mesh.NumCell2Ds);
    mesh.Cell2DsEdges.reserve(mesh.NumCell2Ds);

    for (const string& line : listLines)
    {
        istringstream converter(line);
		
		unsigned int n;
        unsigned int id;
		char c;
		unsigned int marker;

        converter >> id >> c >> marker >> c >> n >> c;
		
		vector<unsigned int> vertices(n ,0);
        vector<unsigned int> edges(n, 0);
		edges.reserve(8);
		vertices.reserve(8);
		
        for(unsigned int i = 0; i < vertices.size(); i++)
            converter >> vertices.at(i) >> c;
		converter >> n >> c;
        for(unsigned int i = 0; i < edges.size()-1; i++)
            converter >> edges.at(i) >> c;
		converter >> edges.at(edges.size()-1);
		
        mesh.Cell2DsId.push_back(id);
        mesh.Cell2DsVertices.push_back(vertices);
        mesh.Cell2DsEdges.push_back(edges);
    }
	
    return true;
}

}