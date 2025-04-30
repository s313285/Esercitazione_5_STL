#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main()
{
	PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
	
	Gedim::UCDUtilities utilities;
    {
        vector<Gedim::UCDProperty<double>> cell0Ds_properties(1);

        cell0Ds_properties[0].Label = "Marker";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        vector<double> cell0Ds_marker(mesh.NumCell0Ds, 0.0);
        for(const auto &m : mesh.MarkerCell0Ds)
            for(const unsigned int id: m.second)
                cell0Ds_marker.at(id) = m.first;

        cell0Ds_properties[0].Data = cell0Ds_marker.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               mesh.Cell0DsCoordinates,
                               cell0Ds_properties);
    }

    {
        vector<Gedim::UCDProperty<double>> cell1Ds_properties(1);

        cell1Ds_properties[0].Label = "Marker";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        vector<double> cell1Ds_marker(mesh.NumCell1Ds, 0.0);
        for(const auto &m : mesh.MarkerCell1Ds)
            for(const unsigned int id: m.second)
                cell1Ds_marker.at(id) = m.first;

        cell1Ds_properties[0].Data = cell1Ds_marker.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 mesh.Cell0DsCoordinates,
                                 mesh.Cell1DsExtrema,
                                 {},
                                 cell1Ds_properties);
    }
	
	
	// test (each edge has non-zero length)
	for (unsigned int j=0; j<mesh.Cell1DsExtrema.cols(); j++)
		if (mesh.Cell1DsExtrema(0, j) == mesh.Cell1DsExtrema(1, j))
			cerr << "the edge with id = " << j << " has zero length" << endl;
	
	
	// test (each polygon has a non-zero area)
	for (unsigned int i = 0; i < mesh.Cell2DsId.size(); i++){ 
		double area = 0.0;
		for (unsigned int j = 0; j < mesh.Cell2DsVertices.at(i).size()-1; j++){
			unsigned int id1 = mesh.Cell2DsVertices.at(i).at(j);
			unsigned int id2 = mesh.Cell2DsVertices.at(i).at(j+1);
			Vector3d coord1 = mesh.Cell0DsCoordinates.col(id1);
			Vector3d coord2 = mesh.Cell0DsCoordinates.col(id2);
			area = area + 0.5*(coord1.cross(coord2)).norm();
		}
		if (abs(area - 0.0) <= 1e-16)
			cerr << "a polygon has area zero" << endl;
	}
	
	
	/*
		unsigned int k = 0;
		for (unsigned int j=1; j<vertices.size(); j++)
			if (vertices[j-1] == vertices[j])
				k = k+1;
		if (k == vertices.size()-1)
			cout << "the polygon with id = " << id << " has area zero" << endl;
	*/

    return 0;
}