#pragma once

#include "WindowJudge.h"
#include<fstream>

//测试类
/*
	 从txt文件中读取文件


*/

class  ReconstrutionTest {

public:
	bool get_data(string filepath);        //从txt文件中获取数据
	void get_reconstruction_result();      //获取序列重构结果(打印序列重构结果和原序列内容)

	void pirnt_data_size();
	int  cluster_size;    //聚类数目

	//重构方法
	WindowJudgeReconstruction    windowJudge;

private:
	vector<string>  origin_seqs;
	vector<vector<string>> copy_seqs;
};