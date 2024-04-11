#include "WindowJudge.h"

WindowJudgeReconstruction::WindowJudgeReconstruction()
{

	base_index_map.insert({ 'A',0 });
	base_index_map.insert({ 'C',1 });
	base_index_map.insert({ 'G',2 });
	base_index_map.insert({ 'T',3 });

	index_base_map[0] = 'A';
	index_base_map[1] = 'C';
	index_base_map[2] = 'G';
	index_base_map[3] = 'T';

	base_class_set.resize(4);

	para_clear();

}


/*
	 每次进行reconstruction_one_sequences时需要进行参数初始化工作
*/
void  WindowJudgeReconstruction::para_clear()
{
	res_seq = "";       //最终返回的结果序列
	consensus_seq = "";
	for (int i = 0; i < MAX_SET_SIZE; i++)
	{
		seq_pointer[i] = 0;
		valid[i] = true;
	}
	subsequent_window_size = WINDOW_SIZE;
}


WindowJudgeReconstruction:: ~WindowJudgeReconstruction()            //析构函数
{

}
string WindowJudgeReconstruction::reconstruction_one_sequences(vector<string>& sequences_set)
{
	all_seq_cnt = sequences_set.size();
	valid_seq_cnt = sequences_set.size();             //记录初始状态有效序列的个数，当有效序列为0时跳出循环
	para_clear();

	while (valid_seq_cnt>0)
	{
		// 0.准备工作
		for (int i = 0; i < 4; i++)
			base_class_set[i].clear();
		
		//1.选出共识碱基并把序列根据指针指向的ACGT值进行分类 
		for (int i = 0; i < all_seq_cnt; i++)
		{
			if (!valid[i])  continue;
			char c = sequences_set[i][seq_pointer[i]];
			base_class_set[base_index_map[c]].push_back(i);
		}
		
		int max_size = 0, max_size_index = 0;
		for (int i = 0; i < 4; i++)
		{
			if (base_class_set[i].size() > max_size)
			{
				max_size = base_class_set[i].size();
				max_size_index = i;
			}
		}
		consensus_base = index_base_map[max_size_index];
		res_seq += consensus_base;


		vector<int> pos(all_seq_cnt, 1);
		if (max_size != valid_seq_cnt)
		{
		 	//cout << "find a error position, now res ="<< res_seq << endl;
			//2.选出共识窗口序列  参数:所有序列、共识碱基标记     返回值: 共识窗口序列
			consensus_seq = get_consensus_seq(sequences_set, max_size_index);

			//3.判断错误序列的错误类型   参数：所有序列、共识碱基标记  返回参数:所有序列的指针移动位置
			pos = judge_error_class(sequences_set, max_size_index);

		}

		//4.移动指针并检查指针合法性
		for (int i = 0; i < all_seq_cnt; i++)
		{
			if (!valid[i])  continue;
			if (pos[i]== -1 || seq_pointer[i]+ pos[i] >= sequences_set[i].size())   //已经到序列结尾处
			{
				valid[i] = false;
				valid_seq_cnt -= 1;
			}
			else
				seq_pointer[i] += pos[i];
		}
	}
	//cout << "【重构结果】:" << res_seq << endl;
	return res_seq;
}





/*
    函数功能:从多个序列的窗口中找出共识序列
*/
string WindowJudgeReconstruction::get_consensus_seq(vector<string>& sequences_set,int  no_error_seqsets_index)
{
	unordered_map<string, int>  window_seqs;
	string win_seq;
	for (auto e:base_class_set[no_error_seqsets_index])
	{
		win_seq = sequences_set[e].substr(seq_pointer[e]+1, subsequent_window_size);
		window_seqs[win_seq] += 1;
	}
	string consensus_seq = window_seqs.begin()->first;
	int seq_cnt = window_seqs.begin()->second;

	for (auto it = window_seqs.begin(); it != window_seqs.end(); it++)
	{
		if (it->second > seq_cnt)
		{
			seq_cnt = it->second;
			consensus_seq = it->first;
		}
	}
	return consensus_seq;
}



vector<int> WindowJudgeReconstruction::judge_error_class(vector<string>& sequences_set,int no_error_seqsets_index) {

	bool no_error[MAX_SET_SIZE] = { false };

	//对于所有的无错误序列进行标记，并且返回其指针应该移动的位置
	vector<int>  pointer_change(all_seq_cnt, -1);
	for (auto e:base_class_set[no_error_seqsets_index])
	{
		no_error[e] = true;      
		pointer_change[e] = 1;
	}

	for (int i = 0; i < all_seq_cnt; i++)
	{
		if (!valid[i] || no_error[i])  continue;

		//接下来依次判断错误类型:替换 or 删除 or 插入 or 混合类型错误(丢弃序列  用-（窗口大小+1）来在返回值中表现出混合类型错误 )     指针位移说明: 1 替换错误   2 插入错误  0 删除错误  -1 混合类型错误
		string  error_seq = sequences_set[i];
		if (is_substitution(error_seq, seq_pointer[i],  pointer_change[i]))      //判断为替换错误
			continue;
		else if (is_deletion(error_seq, seq_pointer[i], pointer_change[i]))
			continue;
		else if (is_insertion(error_seq, seq_pointer[i], pointer_change[i]))
			continue;
		else
			continue;
	}
	return pointer_change;
}





bool  WindowJudgeReconstruction::is_substitution(string error_seq, int seq_pointer, int& pointer_change) {
	int win_size = consensus_seq.size();  //考虑到达序列末尾时后续窗口大小不足subsequent_window_size的情况，比较时实际窗口长度取共识序列长度;
	return error_seq.substr(seq_pointer + 1, win_size) == consensus_seq;
}
bool  WindowJudgeReconstruction::is_deletion(string error_seq, int seq_pointer,  int& pointer_change)
{
	int win_size = consensus_seq.size();
	return error_seq.substr(seq_pointer, win_size) == consensus_seq;
}
bool  WindowJudgeReconstruction::is_insertion(string error_seq, int seq_pointer, int& pointer_change)
{
	int win_size = consensus_seq.size();
	return error_seq[seq_pointer+1] == consensus_base && error_seq.substr(seq_pointer + 2, win_size - 1) == consensus_seq.substr(0, win_size - 1);
}