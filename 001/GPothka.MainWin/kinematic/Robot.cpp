#include "StdAfx.h"
#include "Robot.h"
#include <assert.h>

bool CJoint::ComputeTransMatrix(void)
{
	/*  �任�������ʽ���£�
	    |R L|
		|0 1|
	    
		�������£�
		R: 3*3����ת����
		L��3*1��ƽ��ʸ��
		0: 1*3����ʸ��
		1: 1*1�ı���
	*/
	_trans.SetToIdentity();
	CVector3 offset_trans( _offsetx, _offsety, _offsetz );
	switch( _channel )
	{
	case XPOSITION:
		{
			assert(0);
			CVector3 addedtrans( _position, 0, 0 );	
			offset_trans = offset_trans + addedtrans;
			_trans.SetTranslate(offset_trans);
			break;
		}
	case YPOSITION:
		{
			if( _positivedirec == Right )      //ƽ�ƹؽ�����Ϊ��
			{
				CVector3 addedtrans( 0, _position, 0 );	
			    offset_trans = offset_trans + addedtrans;
			    _trans.SetTranslate(offset_trans); 
			}
			else if( _positivedirec == Left ) //ƽ�ƹؽ�����Ϊ��
			{
				CVector3 addedtrans( 0, -1.0*_position, 0 ); 	
			    offset_trans = offset_trans + addedtrans;
			    _trans.SetTranslate(offset_trans); 
			}
			else
			{
				assert(0);
				return false;
			}
			
			break;
		}
	case ZPOSITION:
		{
			assert(0);
			CVector3 addedtrans( 0, 0, _position );	
			offset_trans = offset_trans + addedtrans;
			_trans.SetTranslate(offset_trans);
			break;
		}
	case XROTATION:
		{
			//����R
			if( _positivedirec == AntiClockWise ) //�ؽ���ת����ʱ��Ϊ��
			{
				_trans.FromEulerAngle( XYZ, CVector3( _position, 0, 0 ) ); 
			}
			else                                 //�ؽ���ת��˳ʱ��Ϊ��
			{
				_trans.FromEulerAngle( XYZ, CVector3( -1.0*_position, 0, 0 ) ); 
			}

			_trans.SetTranslate(offset_trans);
			break;
		}
	case YROTATION:
		{
			//����R
			if( _positivedirec == AntiClockWise ) //�ؽ���ת����ʱ��Ϊ��
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, _position, 0 ) );
			}
			else                                //�ؽ���ת��˳ʱ��Ϊ��
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, -1.0*_position, 0 ) );
			}

			_trans.SetTranslate(offset_trans);
			break;
		}
	case ZROTATION:
		{
			//����R
			if( _positivedirec == AntiClockWise ) //�ؽ���ת����ʱ��Ϊ��
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, 0, _position ) );
			}
			else                                //�ؽ���ת��˳ʱ��Ϊ��
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, 0, -1.0*_position ) );
			}
			_trans.SetTranslate(offset_trans);
			break;
		}
	default:
		{
			return false;
		}
	}
	return true;
}


bool CJoint::setPosition( double pos )
{
	if( pos  < _dlimit || pos > _ulimit )
	{
		return false;
	}
	else 
	{
		_position = pos;
		return true;
	}
}

Robot::Robot(void)
{
	_figure = NULL;	
}


Robot::~Robot(void)
{
	Release();
}


Robot& Robot::operator =(const Robot& robot)
{
	_version = robot._version; //������ģ���ļ��İ汾��
	_model = robot._model;     //����������
	
	// �ͷŹؽڵĴ洢�ռ�
	int size = _index.size();
	for( int i = 0; i < size; ++i )
	{
		delete _index[i];
	}	
	
	_index.clear();
	_name2index.clear();
	_figure = NULL;

	size = robot._index.size();
	for( int i = 0; i < size; ++i )
	{
		CJoint* pj = new CJoint("noName");
		*(pj) = *(robot._index[i]);
		_index.push_back( pj );
	}
	
	// ����ָ���ӵ�ָ��
	for( int i = 0; i < size; ++i )
	{
		CJoint* pSon = robot._index[i]->GetSon();
		if( pSon != NULL )
		{
			// Ѱ�Һ��ӵĵ�ַ
			for( int j = 0; j < size; ++j )
			{
				if( _index[j]->GetName() == pSon->GetName() )
				{
					_index[i]->SetSon( _index[j] );
					break;
				}
			}
		}
	}

	// ������ؽ�ָ��
	if( robot._figure != NULL )
	{
		string rootName = (robot._figure)->GetName();
		for( int i = 0; i < size; ++i )
		{
			if( _index[i]->GetName() == rootName )
			{
				_figure = _index[i];
				break;
			}
		}
	}

	BuildingMap();

	return *this;
}

void Robot::Release()
{
	if( _figure )
	{
		for(unsigned int i = 0; i < _index.size(); i++ ) delete _index[i];
		_index.clear();
		_figure = NULL;
	}
	
}

bool Robot::Import( string fname )
{
	//_filename = fname;
	_infile.open(fname.data());
	if( !_infile )  return false;
	
	bool result = Parse();
	_infile.close();
	return result;
}

bool Robot::Parse()
{
	/* ��״̬����state��¼�����ݵĽ��̣�state = 0��ʾ��ʼ�����������ݣ�state = 6��ʾ��ȷ���������ģ�ͣ�state = -1 ��ʾ�����̷������� */
	int state = 0;
	string token;
	CJoint *joint = NULL;
	
	stack <CJoint *> _stack; //������¼���ӹ�ϵ

	while( state != 6 && state != -1)
	{
		token = GetToken();
		switch(state) {
		case 0:
			if( token == "%%" ) std::getline(_infile,token);
			else if( token == "VERSION" ) state = 1; 
			break;
		case 1:
			_version = token;   
			state = 2;
			break;
		case 2:
			if( token != "MODEL") state = -1;
			else state = 3;
			break;
       case 3:
		    _model = token; 
			state = 4;
			break;
	  case 4:
			if( token != "ROOT") state = -1;
			joint = new CJoint( GetToken() );
			_index.push_back(joint);
			if( GetToken() != "{" )		state = -1;
			if( !ReadJoint( joint ) )	state = -1;
			_stack.push( joint );
			state = 5;
			break;
		case 5:
			if( token == "JOINT") 
			{
				joint = new CJoint( GetToken() );
				_index.push_back(joint);
				if( GetToken() != "{" )		state = -1;
				if( !ReadJoint( joint ) )	state = -1;
				_stack.push( joint );
				state = 5;
			}
			else if( token == "}" )
			{
				joint = _stack.top();
				_stack.pop();
				state = 5;
				if( _stack.size() ) 
				{
					if( _stack.top()->GetSon() != NULL) //�����һ������
						state = -1;
					else
						_stack.top()->SetSon(joint);    //���ú���ָ��
				}
				else 
				{
					_figure = joint;
					state = 6;				
				}
			}
			else state = -1;
			break;
		default: 
			state = -1;
		}
	}

	if( state == 6 )  
	{
		cout<<"Success!";
		return true;
	}
	else  
	{
		Release();
		cout<<"Error";
		return false;
	}
}

string Robot::GetToken()
{
	string word;
	_infile >> word;
	
	return word;
}

bool Robot::ReadJoint( CJoint *joint )
{   
	string token;
    token = GetToken();
	if( token != "OFFSET" )  return false;
	double ofx = atof(GetToken().data());
	double ofy = atof(GetToken().data());
	double ofz = atof(GetToken().data());
	joint->SetOffset( CVector3(ofx, ofy, ofz) ); 
	
	token = GetToken();
	if( token != "CHANNEL") return false;
	
	string chname = GetToken();
	if     ( chname == "Xposition" ) { joint->SetChannelName(XPOSITION);}
	else if( chname == "Yposition" ) { joint->SetChannelName(YPOSITION);}
	else if( chname == "Zposition" ) { joint->SetChannelName(ZPOSITION);}
	else if( chname == "Xrotation" ) { joint->SetChannelName(XROTATION);}
	else if( chname == "Yrotation" ) { joint->SetChannelName(YROTATION);}
	else if( chname == "Zrotation" ) { joint->SetChannelName(ZROTATION);}
	else { assert(0); return false;} 
	
	if( chname == "Xposition" ||  chname == "Yposition" ||  chname == "Zposition" )
	{
		assert( chname == "Yposition" );    // Ŀǰ��֧��Xposition��Zposition����
		string positiveDirec =  GetToken();
		if     (positiveDirec == "Right")   { joint->SetPositiveDirec(Right); }
		else if(positiveDirec == "Left")    { joint->SetPositiveDirec(Left); }
		else { assert(0); return false;} 
	}
	else if( chname == "Xrotation" ||  chname == "Yrotation" ||  chname == "Zrotation" )
	{
		string positiveDirec =  GetToken();
		if     (positiveDirec == "ClockWise")     { joint->SetPositiveDirec(ClockWise); }
		else if(positiveDirec == "AntiClockWise") { joint->SetPositiveDirec(AntiClockWise); }
		else { assert(0); return false;} 
	}
	else
	{
		assert(0);
		return false;
	}

	token = GetToken();
	if( token != "LIMIT") return false;

	if( chname == "Xrotation" ||  chname == "Yrotation" ||  chname == "Zrotation" )
	{
		joint->SetDownLimit( atof(GetToken().data()) * 3.1415/180.0 );
		joint->SetUpLimit  ( atof(GetToken().data()) * 3.1415/180.0 );
	}
	else 
	{
		joint->SetDownLimit( atof(GetToken().data()) );
		joint->SetUpLimit  ( atof(GetToken().data()) );
	}

    if( joint->GetDownLimit() > joint->GetUpLimit() )	//���ޱ���С������
		return false;
	
	joint->ComputeTransMatrix(); //��ʼ���任���󣬴�ʱ�ؽڵ�λ������Ϊ��  
	
	return true;
}

bool Robot::Export(string fname )
{
	//Is there data or not?
	if( _figure == NULL ) return false;

	//open the output file
	_outfile.open(fname.data());
	if( !_outfile ) return false;
	Generate();
	_outfile.close();
	return true;
}

void Robot::Generate( )
{
	_outfile << "%% ���Ӽ������˶�����ϵͳ������ģ���ļ� 1.0�汾" << endl
             << "%% VERSION     �汾��ʶ��" << endl
             << "%% MODEL       ����������" << endl
			 << "%% ROOT        ������ϵͳ�ĸ��ؽ�" << endl
             << "%% JOINT       ������ϵͳ�Ĺؽ�" << endl
             << "%% CHANNEL     �ؽ��˶��ķ��򣬿�ȡֵ��Xposition  Yposition  Zposition  Xrotation  Yrotation  Zrotation" << endl
             << "%%             ����ת�˶���ClockWise��ʾ˳ʱ��Ϊ����AntiClockWise��ʾ��ʱ��Ϊ��;" << endl
             << "%%             ����ƽ�ƹؽڣ������йؽ�λ������Ϊ0ʱ�������Ի����ˣ�Right�����ַ���Ϊ����Left�����ַ���Ϊ��" << endl
             << "%% LIMIT       �ؽ��˶����ɶȵ�����������" << endl
             << "%% ������ģ�Ͷ�����λԼ����     ���ȣ��ף�����ת���ȣ�  ��ʱ�䣨���룩" << endl
             << "%% ���Ӽ�ϵͳ�ڲ�������λԼ���� ���ȣ��ף�����ת�����ȣ���ʱ�䣨���룩��������ת��ʱע�ⵥλ�任" << endl
             << "%% ���Ӽ�ϵͳ�ڲ�����ʱ�루AntiClockWise��Ϊ�������˳ʱ�루ClockWise�����͹ؽ��ڼ�����ת����ʱ���ؽ��˶����ݻ��Զ�����-1" << endl
             << "%% ��ת�ؽ��˶����ɶȵ����������ް��ؽ���ת��������ȷ��" << endl;

	CJoint *joint = _figure;
	string bk = "";
	
	_outfile<< bk << "VERSION  " << _version << endl;
	_outfile<< bk << "MODEL  " << _model << endl;
	_outfile<< bk << "ROOT " << joint->GetName() <<endl;
	_outfile<< bk << '{' << endl;
	
	_outfile<< bk << '\t' << "OFFSET " << joint->GetOffset().GetX() << ' ' << joint->GetOffset().GetY() << ' ' << joint->GetOffset().GetZ() << endl;
		
	_outfile<< bk << '\t' << "CHANNEL  ";
	CHANNELNAME chname = joint->GetChannelName();

	switch( chname )
	{
	case XPOSITION:
		_outfile<< "Xposition  ";
		break;
	case YPOSITION:
		_outfile<< "Yposition  ";
		break;
	case ZPOSITION:
		_outfile<< "Zposition  ";
		break;
	case XROTATION:
		_outfile<< "Xrotation  ";
		break;
	case YROTATION:
		_outfile<< "Yrotation  ";
		break;
	case ZROTATION:
		_outfile<< "Zrotation  ";
		break;
	default:
		break;
	}

	
	POSITIVEDIREC prd = joint->GetPositiveDirec();
	if      ( prd == Right )         _outfile << "Right";
	else if ( prd == Left )          _outfile << "Left";
	else if ( prd == ClockWise )     _outfile << "ClockWise";
	else if ( prd == AntiClockWise ) _outfile << "AntiClockWise";
	else;
	
	_outfile << endl;

	_outfile<< bk << '\t' <<"LIMIT  ";
	if( chname == XROTATION || chname == YROTATION || chname == ZROTATION ) 
		_outfile << joint->GetDownLimit()*180/3.1415 << ' ' << joint->GetUpLimit()*180/3.1415 << endl;
	else
		_outfile << joint->GetDownLimit() << ' ' << joint->GetUpLimit() << endl;
	
	if( joint->GetSon() != NULL ) GenerateHelper( 1, joint->GetSon() );

	_outfile << bk << "}" << endl;
}

void Robot::GenerateHelper( int level, CJoint *joint )
{
	//generate blank
	string bk;
	for( int j = 0; j < level; j++ ) bk += '\t';

	_outfile<< bk << "JOINT " << joint->GetName() << endl;
	_outfile<< bk << '{' << endl;
	_outfile<< bk << '\t' << "OFFSET " << joint->GetOffset().GetX() << ' ' << joint->GetOffset().GetY() << ' ' << joint->GetOffset().GetZ() << endl;
	_outfile<< bk << '\t' <<"CHANNEL  ";
	CHANNELNAME chname = joint->GetChannelName();

	switch( chname )
	{
	case XPOSITION:
		_outfile<< "Xposition  ";
		break;
	case YPOSITION:
		_outfile<< "Yposition  ";
		break;
	case ZPOSITION:
		_outfile<< "Zposition  ";
		break;
	case XROTATION:
		_outfile<< "Xrotation  ";
		break;
	case YROTATION:
		_outfile<< "Yrotation  ";
		break;
	case ZROTATION:
		_outfile<< "Zrotation  ";
		break;
	default:
		break;
	}

	POSITIVEDIREC prd = joint->GetPositiveDirec();
	if      ( prd == Right )         _outfile << "Right";
	else if ( prd == Left )          _outfile << "Left";
	else if ( prd == ClockWise )     _outfile << "ClockWise";
	else if ( prd == AntiClockWise ) _outfile << "AntiClockWise";
	else;

	_outfile << endl;

	_outfile<< bk << '\t' <<"LIMIT  ";
	if( chname == XROTATION || chname == YROTATION || chname == ZROTATION ) 
		_outfile << joint->GetDownLimit()*180/3.1415 << ' ' << joint->GetUpLimit()*180/3.1415 << endl;
	else
		_outfile << joint->GetDownLimit() << ' ' << joint->GetUpLimit() << endl;

	if( joint->GetSon() != NULL ) GenerateHelper( level + 1, joint->GetSon() );

	_outfile << bk << "}" << endl;
}

void Robot::BuildingMap(void)
{
	int size = _index.size();
	for( int i = 0; i < size; ++i )
	{
		_name2index.insert( pair<string, int>( _index[i]->GetName(), i ) );
	}

	 //��������ϵ"world"��_index�в������ڣ���������Ľ����±���Ϊ-1
	 //�Է��㺯��TransMatrixBetweenTwoCoordinates�ĵ��á�
	_name2index.insert( pair<string, int>(string("world"), -1) ); 
}

//���ø����ؽڵ��˶����ݣ������±任����
bool Robot::SetJointsMotionData( map<string, double> jointData )
{
	int size = int(_index.size());
	for( int i = 0; i < size; ++ i )
	{
		string name = _index[i]->GetName();
		double data = jointData[name];
		bool isLegal = _index[i]->setPosition( data );
		assert( isLegal == true );
		if( isLegal  == false )
			return false;

		isLegal = _index[i]->ComputeTransMatrix();
		assert( isLegal == true );
		if( isLegal  == false )
			return false;
	}
	return true;
}

//���������ϵcoor1������ϵcoor2������任,���coord1����coord2 Ϊ 'world'�����ʾ������������ϵ
bool Robot::TransMatrixBetweenTwoCoordinates( string coor1, string coor2, CTransMatrix& matrix )
{
	map<string,int>::iterator iter1, iter2;
	iter1 = _name2index.find(coor1);
	iter2 = _name2index.find(coor2);
	if( iter1 == _name2index.end() || iter2 == _name2index.end() )
	{
		assert(0);
		return false;
	}
	
	int id1 = iter1->second;
	int id2 = iter2->second;

	if( id1 < -1 ||  id2 < -1 )  // "world" ��Ӧ���±�Ϊ-1������Сֵ����-1��С��ֵ�ǷǷ�
	{
		assert( 0 );
		return false;
	}

	if( id1 > int(_index.size()) || id2 > int(_index.size()) ) // �±�ֵ���ܴ���_index������
	{
		assert( 0 );
		return false;
	}
	
	//ȷ���������ȱ��ڵ�
	int descendant    = max( id1, id2 );
	int predecessor = min( id1, id2 );

	//���������ڵ㵽�ȱ��ڵ�ı任����
	matrix.SetToIdentity();
	
	for( int i = descendant; i > predecessor; --i ) // ��Ϊ�ǵ����ṹ����������� --i ���д����ﵽ�ȱ��ı���
	{
		matrix = _index[i]->GetTransMatrix() * matrix;		//Add by JerryHe ��ע���Ǳ任���̣���˳�ʼ������Ϊ��λ���󣬲�����Ϊ����������������������ˡ�
	}

	//���id1��������ڵ㣬���轫����任��������
	if( id1 != descendant ) 
	{
		matrix = matrix.GetInverse();
	}
	return true;
}

//��ȡ�����˹ؽڵ��б����йؽ�˳���������[���ؽ�,�ӹؽ�,���ӹؽ�,...]
void Robot::GetJointsList(vector<CJoint>& jointslist)
{
	jointslist.clear();

	CJoint* p = _figure;
	while( p != NULL )
	{
		jointslist.push_back(*p);
		p = p->GetSon();
	}
	//��ÿ���ؽڵĺ���ָ������ΪNULL���Է�ֹ�����ָ�����
	int size = int( jointslist.size() );
	for( int i = 0; i < size; ++i )
	{
		jointslist[i].SetSon(NULL);
	}
}