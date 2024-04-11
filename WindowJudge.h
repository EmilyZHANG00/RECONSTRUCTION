#pragma once
/*
论文：Trace reconstruction from noisy polynucleotide sequencer reads
作者：Parikshit S . Gopalan , Palo Alto , CA( US ) ; Sergey Yekhanin . Redmond WA ( US ) ; Siena Dumas Ang , Seattle , WA( US ) ; Nebojsa Jojic , Redmond , WA( US ) ; Miklos

算法基本思想: 对每个序列维护一个指针，对每个序列中当前指针所指向的内容进行执行多数投票，选择出共识碱基。
当前指针指向内容与该共识碱基不同的序列被看作是包含错误的序列，根据后续窗口内容进行判断该错误序列的内容是插入、删除、替换，对于混合类型的错误，将该序列进行丢弃

*/


#include<iostream>
#include<vector>
#include <string>
#include<map>
#include<unordered_map>

#define  WINDOW_SIZE   10      //窗口序列大小
#define  MAX_SET_SIZE  50     //最大可接受的序列集合大小
#define  BURST_SIZE    1       //每次只考虑插入1 删除1的情况

using namespace std;
class WindowJudgeReconstruction
{
public:    //对外接口
	WindowJudgeReconstruction();             //构造函数
	~WindowJudgeReconstruction();            //析构函数

	string  reconstruction_one_sequences(vector<string> & sequences_set);   //从一个序列集合中重构出一个序列

private:    //序列重构过程中需要实现的内部调用以及相关参数



	string res_seq = "";       //最终返回的结果序列
	char consensus_base;
	string  consensus_seq = "";

	int  seq_pointer[MAX_SET_SIZE]; 
	bool valid[MAX_SET_SIZE]; 

	int all_seq_cnt;
	int valid_seq_cnt;
	vector<vector<int>>  base_class_set;   

	map<char, int>   base_index_map;
	char index_base_map[4];
	int subsequent_window_size;



	void  para_clear();
	string  get_consensus_seq(vector<string>& sequences_set,int no_error_seqs_index);
	vector<int> judge_error_class(vector<string>& sequences_set, int no_error_seqs_index);

	bool is_substitution(string error_seq, int seq_pointer,int & pointer_change);
	bool is_deletion(string error_seq, int seq_pointer,  int& pointer_change);
	bool is_insertion(string error_seq, int seq_pointer, int& pointer_change);

};
