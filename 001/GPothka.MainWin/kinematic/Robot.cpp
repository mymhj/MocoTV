#include "StdAfx.h"
#include "Robot.h"
#include <assert.h>

bool CJoint::ComputeTransMatrix(void)
{
	/*  变换矩阵的形式如下：
	    |R L|
		|0 1|
	    
		含义如下：
		R: 3*3的旋转矩阵
		L：3*1的平移矢量
		0: 1*3的零矢量
		1: 1*1的标量
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
			if( _positivedirec == Right )      //平移关节向右为正
			{
				CVector3 addedtrans( 0, _position, 0 );	
			    offset_trans = offset_trans + addedtrans;
			    _trans.SetTranslate(offset_trans); 
			}
			else if( _positivedirec == Left ) //平移关节向左为正
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
			//计算R
			if( _positivedirec == AntiClockWise ) //关节旋转是逆时针为正
			{
				_trans.FromEulerAngle( XYZ, CVector3( _position, 0, 0 ) ); 
			}
			else                                 //关节旋转是顺时针为正
			{
				_trans.FromEulerAngle( XYZ, CVector3( -1.0*_position, 0, 0 ) ); 
			}

			_trans.SetTranslate(offset_trans);
			break;
		}
	case YROTATION:
		{
			//计算R
			if( _positivedirec == AntiClockWise ) //关节旋转是逆时针为正
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, _position, 0 ) );
			}
			else                                //关节旋转是顺时针为正
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, -1.0*_position, 0 ) );
			}

			_trans.SetTranslate(offset_trans);
			break;
		}
	case ZROTATION:
		{
			//计算R
			if( _positivedirec == AntiClockWise ) //关节旋转是逆时针为正
			{
				_trans.FromEulerAngle( XYZ, CVector3( 0, 0, _position ) );
			}
			else                                //关节旋转是顺时针为正
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
	_version = robot._version; //机器人模型文件的版本号
	_model = robot._model;     //机器人类型
	
	// 释放关节的存储空间
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
	
	// 处理指向孩子的指针
	for( int i = 0; i < size; ++i )
	{
		CJoint* pSon = robot._index[i]->GetSon();
		if( pSon != NULL )
		{
			// 寻找孩子的地址
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

	// 处理根关节指针
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
	/* 用状态变量state记录读数据的进程，state = 0表示开始读机器人数据，state = 6表示正确读入机器人模型，state = -1 表示读过程发生错误 */
	int state = 0;
	string token;
	CJoint *joint = NULL;
	
	stack <CJoint *> _stack; //辅助记录父子关系

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
					if( _stack.top()->GetSon() != NULL) //最多有一个孩子
						state = -1;
					else
						_stack.top()->SetSon(joint);    //设置孩子指针
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
		assert( chname == "Yposition" );    // 目前不支持Xposition和Zposition类型
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

    if( joint->GetDownLimit() > joint->GetUpLimit() )	//下限必须小于上限
		return false;
	
	joint->ComputeTransMatrix(); //初始化变换矩阵，此时关节的位置数据为零  
	
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
	_outfile << "%% 奥视佳摄像运动控制系统机器人模型文件 1.0版本" << endl
             << "%% VERSION     版本标识符" << endl
             << "%% MODEL       机器人类型" << endl
			 << "%% ROOT        机器人系统的根关节" << endl
             << "%% JOINT       机器人系统的关节" << endl
             << "%% CHANNEL     关节运动的方向，可取值：Xposition  Yposition  Zposition  Xrotation  Yrotation  Zrotation" << endl
             << "%%             对旋转运动，ClockWise表示顺时针为正，AntiClockWise表示逆时针为正;" << endl
             << "%%             对于平移关节，当所有关节位置设置为0时，人正对机器人，Right：右手方向为正，Left：左手方向为正" << endl
             << "%% LIMIT       关节运动自由度的下限与上限" << endl
             << "%% 机器人模型度量单位约定：     长度（米）；旋转（度）  ；时间（毫秒）" << endl
             << "%% 奥视佳系统内部度量单位约定： 长度（米）；旋转（弧度）；时间（毫秒），在数据转换时注意单位变换" << endl
             << "%% 奥视佳系统内部以逆时针（AntiClockWise）为正，因此顺时针（ClockWise）类型关节在计算旋转矩阵时，关节运动数据会自动被乘-1" << endl
             << "%% 旋转关节运动自由度的下限与上限按关节旋转的正方向确定" << endl;

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

	 //世界坐标系"world"在_index中并不存在，但是虚拟的将其下标设为-1
	 //以方便函数TransMatrixBetweenTwoCoordinates的调用。
	_name2index.insert( pair<string, int>(string("world"), -1) ); 
}

//设置各个关节的运动数据，并更新变换矩阵
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

//计算从坐标系coor1到坐标系coor2的坐标变换,如果coord1或者coord2 为 'world'，其表示的是世界坐标系
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

	if( id1 < -1 ||  id2 < -1 )  // "world" 对应的下标为-1，是最小值，比-1还小的值是非法
	{
		assert( 0 );
		return false;
	}

	if( id1 > int(_index.size()) || id2 > int(_index.size()) ) // 下标值不能大于_index的容量
	{
		assert( 0 );
		return false;
	}
	
	//确定子孙与先辈节点
	int descendant    = max( id1, id2 );
	int predecessor = min( id1, id2 );

	//计算从子孙节点到先辈节点的变换矩阵
	matrix.SetToIdentity();
	
	for( int i = descendant; i > predecessor; --i ) // 因为是单链结构，因此能利用 --i 进行从子孙到先辈的遍历
	{
		matrix = _index[i]->GetTransMatrix() * matrix;		//Add by JerryHe 关注的是变换过程，因此初始矩阵设为单位矩阵，并且因为矩阵是列向量，所以是左乘。
	}

	//如果id1不是子孙节点，则需将坐标变换矩阵求逆
	if( id1 != descendant ) 
	{
		matrix = matrix.GetInverse();
	}
	return true;
}

//获取机器人关节的列表，其中关节顺序的排列是[根关节,子关节,子子关节,...]
void Robot::GetJointsList(vector<CJoint>& jointslist)
{
	jointslist.clear();

	CJoint* p = _figure;
	while( p != NULL )
	{
		jointslist.push_back(*p);
		p = p->GetSon();
	}
	//将每个关节的孩子指针设置为NULL，以防止错误的指针访问
	int size = int( jointslist.size() );
	for( int i = 0; i < size; ++i )
	{
		jointslist[i].SetSon(NULL);
	}
}