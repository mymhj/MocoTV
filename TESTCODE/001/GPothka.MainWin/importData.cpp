#include "stdafx.h"
#include "importData.h"

#include <fstream>
#include <vector>
#include <string>
using namespace std; 


bool importData( char* fileName )
{
	ifstream ifile;
	ifile.open( string(fileName) ); 
	
	if( ifile.is_open() == false ) 
		return false;

	vector<robotRunCommand> data;
	
	// ��ȡ�ļ���ͷ��������
	string container;
	std::getline( ifile, container );
	std::getline( ifile, container );
	std::getline( ifile, container );

	while( !ifile.eof() )
	{
		robotRunCommand rc;
		rc.A = 0;
		rc.B = 0;
		rc.C = 0;
		rc.X = 0;
		rc.Y = 0;
		rc.Z = 0;
		rc.E2 = 0;
		rc.IPOC = 0;
		
		// A1 A2 A3 A4 A6 ��ͬ��
		// A5 E1 �᷽��
		ifile >> rc.E1;
		rc.E1 = rc.E1*(-1);
		ifile >> rc.A1;
		ifile >> rc.A2;
		ifile >> rc.A3;
		ifile >> rc.A4;
		ifile >> rc.A5;
		rc.A5 = rc.A5*(-1);
		ifile >> rc.A6;
		//ifile >> container; // ���˻��з�

		data.push_back(rc);
	}
    ifile.close();
	return true;
}

