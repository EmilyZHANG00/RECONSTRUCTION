#pragma once

#include "WindowJudge.h"
#include<fstream>

//������
/*
	 ��txt�ļ��ж�ȡ�ļ�


*/

class  ReconstrutionTest {

public:
	bool get_data(string filepath);        //��txt�ļ��л�ȡ����
	void get_reconstruction_result();      //��ȡ�����ع����(��ӡ�����ع������ԭ��������)

	void pirnt_data_size();
	int  cluster_size;    //������Ŀ

	//�ع�����
	WindowJudgeReconstruction    windowJudge;

private:
	vector<string>  origin_seqs;
	vector<vector<string>> copy_seqs;
};