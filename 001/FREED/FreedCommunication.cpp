#pragma once
#include "FreedCommunication.h"
#include "serialport.h"

CSerialPort *pSerial;

BOOL FCom::InitPortF(int port)
{

	pSerial = new CSerialPort(NULL);

	m_COMWport = port;
	dwBaudrate = CBR_38400;//38.4 k


	bool a = pSerial->InitPort(m_COMWport,dwBaudrate);

	return a;
}
void FCom::Close()
{

	pSerial->close();
	//pSerial->~CSerialPort();
}
int convert_angle_to_byte(float data)
{
	int output;
	int x;
	int y;
	x= (int)data;//整数部分
	y=(int)((data-(float)x)*32768);//二进制部分转为10进制
	//  0(二进制部分转为10进制)  = y*32768
	output= (x<<15) + y;
	
	
	return output;
}
int convert_position_to_byte(float data)
{
	int output,x,y;
	x= (int)data;//整数部分
	y=(int)((data-(float)x)*64);//二进制部分转为10进制
	//  0(二进制部分转为10进制)  = y*64
	output= (x<<6)+y;
	
	return output;
}
int convert_camera_to_byte(float data)
{
	int output, x, y;
	x = (int)data;//整数部分
	y = (int)((data - (float)x) * 64);//二进制部分转为10进制
	//  0(二进制部分转为10进制)  = y*64
	output = (x << 6) + y;

	return output;
}

void FCom::SendOMsg()
{
	BYTE bXorCal, i;
	if(msg.CMD==PollForPositionUpdate)//获得位置信息更新数据
	{
		cTxBuffer[0] = msg.CMD;						//<D1>    Message type 
		cTxBuffer[1] = msg.CameraID;				//<CA>    Camera ID 
		msg.CameraPanAngle_2 = convert_angle_to_byte(msg.CameraPanAngle);
		cTxBuffer[2] = msg.CameraPanAngle_2 >>16 ;	//<PH><PM><PL>  Camera Pan Angle 
		cTxBuffer[3] = msg.CameraPanAngle_2 >>8;  
		cTxBuffer[4] = msg.CameraPanAngle_2 ; 
		msg.CameraTiltAngle_2 = convert_angle_to_byte(msg.CameraTiltAngle);
		cTxBuffer[5] = msg.CameraTiltAngle_2 >>16 ; //<TH><TM><TL>  Camera Tilt Angle
		cTxBuffer[6] = msg.CameraTiltAngle_2 >>8;   
		cTxBuffer[7] = msg.CameraTiltAngle_2 ;  
		msg.CameraRollAngle_2 = convert_angle_to_byte(msg.CameraRollAngle);
		cTxBuffer[8] = msg.CameraRollAngle_2 >>16 ;	//<RH><RM><RL>  Camera Roll Angle 
		cTxBuffer[9] = msg.CameraRollAngle_2 >>8;  
		cTxBuffer[10] = msg.CameraRollAngle_2 ;  
		msg.CameraXPosition_2 = convert_position_to_byte(msg.CameraXPosition);
		cTxBuffer[11] = msg.CameraXPosition_2 >>16 ; //<XH><XM><XL>  Camera X-Position 
		cTxBuffer[12] = msg.CameraXPosition_2 >>8;   
		cTxBuffer[13] = msg.CameraXPosition_2 ;  
		msg.CameraYPosition_2 = convert_position_to_byte(msg.CameraYPosition);
		cTxBuffer[14] = msg.CameraYPosition_2 >>16 ; //<YH><YM><YL>  Camera Y-Position 
		cTxBuffer[15] = msg.CameraYPosition_2 >>8;   
		cTxBuffer[16] = msg.CameraYPosition_2 ;  
		msg.CameraZPosition_2 = convert_position_to_byte(msg.CameraZPosition);
		cTxBuffer[17] = msg.CameraZPosition_2 >>16 ; //<HH><HM><HL>  Camera Height (Z-Position) 
		cTxBuffer[18] = msg.CameraZPosition_2 >>8;   
		cTxBuffer[19] = msg.CameraZPosition_2 ;  

		cTxBuffer[20] = msg.CameraZoom >>16;							 //<ZH><ZM><ZL>  Camera Zoom 
		cTxBuffer[21] = msg.CameraZoom >> 8;
		cTxBuffer[22] = msg.CameraZoom ;
		cTxBuffer[23] = msg.CameraFocus >> 16;							 //<FH><FM><FL>  Camera Focus 
		cTxBuffer[24] = msg.CameraFocus >> 8;
		cTxBuffer[25] = msg.CameraFocus;
		cTxBuffer[26] = 0xff;							 //<SH><SL>   Spare / User Defined (16 bits) 
		cTxBuffer[27] = 0xff;

		BYTE cal=0x40;
		for(int i=0;i<28;i++){  cal = cal-cTxBuffer[i];	}//note:The checksum is calculated by subtracting (modulo 256) each byte of the message, including the message type, from 40 (hex). 
		cTxBuffer[28] = cal;						 //<CK>    Checksum    

		//cTxBuffer[0] = 0xd1;						//<D1>    Message type 
		//cTxBuffer[1] = 0x00;				//<CA>    Camera ID 
		//cTxBuffer[2] = 0x06;	//<PH><PM><PL>  Camera Pan Angle 
		//cTxBuffer[3] = 0x24;
		//cTxBuffer[4] = 0x78;
		//cTxBuffer[5] = 0xfe; //<TH><TM><TL>  Camera Tilt Angle
		//cTxBuffer[6] = 0xc8;
		//cTxBuffer[7] = 0x13;
		//cTxBuffer[8] = 0x00;	//<RH><RM><RL>  Camera Roll Angle 
		//cTxBuffer[9] = 0x8d;
		//cTxBuffer[10] = 0xb9;
		//cTxBuffer[11] = 0xfe; //<XH><XM><XL>  Camera X-Position 
		//cTxBuffer[12] = 0x40;
		//cTxBuffer[13] = 0x5b;
		//cTxBuffer[14] = 0xf8; //<YH><YM><YL>  Camera Y-Position 
		//cTxBuffer[15] = 0x7a;
		//cTxBuffer[16] = 0x09;
		//cTxBuffer[17] =0x03; //<HH><HM><HL>  Camera Height (Z-Position) 
		//cTxBuffer[18] = 0x76;
		//cTxBuffer[19] = 0xeb;
		//cTxBuffer[20] = 0x09;							 //<ZH><ZM><ZL>  Camera Zoom 
		//cTxBuffer[21] = 0x8a;
		//cTxBuffer[22] = 0x7e;
		//cTxBuffer[23] = 0x13;							 //<FH><FM><FL>  Camera Focus 
		//cTxBuffer[24] = 0x55;
		//cTxBuffer[25] = 0x78;
		//cTxBuffer[26] = 0xff;							 //<SH><SL>   Spare / User Defined (16 bits) 
		//cTxBuffer[27] = 0xff;

		//
	    //note:The checksum is calculated by subtracting (modulo 256) each byte of the message, including the message type, from 40 (hex). 
		//cTxBuffer[28] = 0x47;

		//BYTE cal=0x40;
		//for(int i=0;i<28;i++){  cal = cal-cTxBuffer[i];	}//note:The checksum is calculated by subtracting (modulo 256) each byte of the message, including the message type, from 40 (hex). 
		//cTxBuffer[28] = cal;						 //<CK>    Checksum 

		pSerial->WriteBYTE(cTxBuffer,29);
	}
	else if(1)
	{
	}
	wMsgSent++;
}


void FCom::RecMsg()
{
	BYTE bXorCal, i;
	unsigned char cRecved = 0x00; 
	for(int temp=0;temp<25;temp++)
	{
		pSerial->ReadChar(cRecved);
		cRxBuf[temp]=cRecved;
	}
}