#pragma once
/*
���ģ�Trace reconstruction from noisy polynucleotide sequencer reads
���ߣ�Parikshit S . Gopalan , Palo Alto , CA( US ) ; Sergey Yekhanin . Redmond WA ( US ) ; Siena Dumas Ang , Seattle , WA( US ) ; Nebojsa Jojic , Redmond , WA( US ) ; Miklos

�㷨����˼��: ��ÿ������ά��һ��ָ�룬��ÿ�������е�ǰָ����ָ������ݽ���ִ�ж���ͶƱ��ѡ�����ʶ�����
��ǰָ��ָ��������ù�ʶ�����ͬ�����б������ǰ�����������У����ݺ����������ݽ����жϸô������е������ǲ��롢ɾ�����滻�����ڻ�����͵Ĵ��󣬽������н��ж���

*/


#include<iostream>
#include<vector>
#include <string>
#include<map>
#include<unordered_map>

#define  WINDOW_SIZE   10      //�������д�С
#define  MAX_SET_SIZE  50     //���ɽ��ܵ����м��ϴ�С
#define  BURST_SIZE    1       //ÿ��ֻ���ǲ���1 ɾ��1�����

using namespace std;
class WindowJudgeReconstruction
{
public:    //����ӿ�
	WindowJudgeReconstruction();             //���캯��
	~WindowJudgeReconstruction();            //��������

	string  reconstruction_one_sequences(vector<string> & sequences_set);   //��һ�����м������ع���һ������

private:    //�����ع���������Ҫʵ�ֵ��ڲ������Լ���ز���



	string res_seq = "";       //���շ��صĽ������
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
