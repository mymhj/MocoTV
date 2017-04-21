#pragma once
#include <vector>
#include <map>



//�켣�ظ�����
class CTrackRepeatModule
{
public:
	CTrackRepeatModule(void);
	~CTrackRepeatModule(void);

	
	//�ؼ�֡���ϲֿ�
	static map<string,vector<KeyFrame>> m_TrackKeyFrameDepository;

	//�ؼ�֡�����ڲֿ��е�����
	static string m_TrackIndex;

	//��ǰ�۽��Ĺؼ�֡����
	static int m_keyFrameIndex;

	
	//�ظ��˶���֡ǰ���޸�ָ����Ŀ
	static float commandNum;

	//�ؼ�֡����
	static vector<KeyFrame> m_KeyFrameColl;

	//�켣�ظ�ָ���뼯��
	static vector<char*> m_TrackRepeatCommandData;

	//��λָ���
	static vector<char*> m_RestorationCommandData;

	//����ָ���
	static vector<char*> m_SingleStepCommandData;


	//���ָ���켣�����ĵĹؼ�֡����
	vector<KeyFrame> GetKeyFrameCollToIndexTrack(string indexTrack);

	//����һ���µĹ켣�ؼ�֡����
	bool AddNewTrackToIndexTrack(string indexTrack);

	//ɾ��ָ�������Ĺ켣�ؼ�֡����
	bool DeleteTrackToIndexTrack(string indexTrack);

	//���  �ؼ�֡
	bool AddKeyFrame(robotlocation rbLocation,string indexTrack);
	bool AddKeyFrame(robotlocation rbLocation,string indexTrack,int waitTime,float partSpeed);

	//����  �ؼ�֡
	bool InsertKeyFrame(robotlocation rbLocation, string indexTrack, int m_nIndex, int waitTime, float partSpeed);

	//ɾ�� ָ�������Ĺؼ�֡
	bool DeleteKeyFrame(int keyFrameIndex,string indexTrack);

	//���� ����ָ�������Ĺؼ�֡
	bool UpdateKeyFrame(robotlocation rbLocation,int keyFrameIndex,string indexTrack);
	bool UpdateKeyFrame(robotlocation rbLocation,int keyFrameIndex,string indexTrack,int waitTime,float partSpeed);
	bool UpdataKeyTime(int keyFrameIndex,string indexTrack,float runTime,float waitTime);

	//���� ����ָ�������Ĺؼ�֡����
	bool UpdataKeyName(int keyFrameIndex, string indexTrack, CString name);

	//���뱾�عؼ�֡
	bool LoadLocalKeyFrames(char* filePath,string keyFrameCollIndex);

	//����  �ؼ�֡����
	bool SaveKeyFramesToLocal(string saveFullPath,string keyFrameCollIndex);

	//���� ��õ�������ָ��
	bool ObtainSingleMotionCommandData(robotlocation rbLocation,int keyFrameIndex,string indexTrack,vector<robotRunCommand> *CommandColl);

	//��λ ��ø�λ�˶�ָ��
	bool ObtainRestorationMotionCommandData(robotlocation rbLocation,string indexTrack,vector<robotRunCommand> *CommandColl);

	//�ظ�  ��ȡ�켣�ظ�ָ��
	bool ObtainRepeatMotionCommandData(string indexTrack,vector<robotRunCommand> *CommandColl);

	//Roll������ͷ��ˮƽ
	bool ObtainRollLockMotionCommandData(robotlocation rbLocation,vector<robotRunCommand> *CommandColl);

	//��ͷ��λ�͹켣�ظ�
	//WORD ObtainCameraRestorationMotionCommandData(string indexTrack);
	//bool ObtainCameraRepeatorationMotionCommandData(string indexTrack);
	
	//CR����
	bool ObtainCubicSplineCommandData(string indexTrack);
	bool SendCubicSplineCommandData(string indexTrack, vector<robotRunCommand> *CommandColl);

	//����B��������
	bool ObtainCubicBSplineCommandData(string indexTrack,vector<robotRunCommand> *CommandColl);


	///ͨ��������֡�����������˶�ָ��
	vector<robotRunCommand> CalculateCommandFrameFromTwoKeyFrame3(KeyFrame startFrame,KeyFrame endFrame);
	vector<robotRunCommand> CalculateCommandFrameFromTwoKeyFrame4(KeyFrame startFrame,KeyFrame endFrame,float comdnum);

	//�Ż���켣�ظ�ʱ�ؼ�֮֡��Ĳ�ֵ�㷨
	vector<robotRunCommand> CalculateCommandFrameFromTwoKeyFrame5(KeyFrame startFrame, KeyFrame endFrame,bool lastFrame);//lastFrame�����ж��ǹ��ǹ켣���һ֡

	//���ùؼ�֡����ʱ��
	double setFrameRunTime(KeyFrame startFrame,KeyFrame endFrame);
	//����ÿ֡����ʱ��
	int frameRunTime(KeyFrame startFrame,KeyFrame endFrame);
	//����ÿ֡������Сʱ��
	bool frameMinTime(string indexTrack);

	//�������������λ��
	void CalculateCameraLocation();

	void test();

	//�������У׼
	void camCalibration(double x,double y,double z,double pan,double tilt,double roll);

	bool SpeedBuffer(vector<robotRunCommand>* sendDataEntityColl, int Sum);

	static map<string,string> m_TrackDataPath;
	
	GP_Lens_Fujinon m_Lens_Fujinon;
};

