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
	 ÿ�ν���reconstruction_one_sequencesʱ��Ҫ���в�����ʼ������
*/
void  WindowJudgeReconstruction::para_clear()
{
	res_seq = "";       //���շ��صĽ������
	consensus_seq = "";
	for (int i = 0; i < MAX_SET_SIZE; i++)
	{
		seq_pointer[i] = 0;
		valid[i] = true;
	}
	subsequent_window_size = WINDOW_SIZE;
}


WindowJudgeReconstruction:: ~WindowJudgeReconstruction()            //��������
{

}
string WindowJudgeReconstruction::reconstruction_one_sequences(vector<string>& sequences_set)
{
	all_seq_cnt = sequences_set.size();
	valid_seq_cnt = sequences_set.size();             //��¼��ʼ״̬��Ч���еĸ���������Ч����Ϊ0ʱ����ѭ��
	para_clear();

	while (valid_seq_cnt>0)
	{
		// 0.׼������
		for (int i = 0; i < 4; i++)
			base_class_set[i].clear();
		
		//1.ѡ����ʶ����������и���ָ��ָ���ACGTֵ���з��� 
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
			//2.ѡ����ʶ��������  ����:�������С���ʶ������     ����ֵ: ��ʶ��������
			consensus_seq = get_consensus_seq(sequences_set, max_size_index);

			//3.�жϴ������еĴ�������   �������������С���ʶ������  ���ز���:�������е�ָ���ƶ�λ��
			pos = judge_error_class(sequences_set, max_size_index);

		}

		//4.�ƶ�ָ�벢���ָ��Ϸ���
		for (int i = 0; i < all_seq_cnt; i++)
		{
			if (!valid[i])  continue;
			if (pos[i]== -1 || seq_pointer[i]+ pos[i] >= sequences_set[i].size())   //�Ѿ������н�β��
			{
				valid[i] = false;
				valid_seq_cnt -= 1;
			}
			else
				seq_pointer[i] += pos[i];
		}
	}
	//cout << "���ع������:" << res_seq << endl;
	return res_seq;
}





/*
    ��������:�Ӷ�����еĴ������ҳ���ʶ����
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

	//�������е��޴������н��б�ǣ����ҷ�����ָ��Ӧ���ƶ���λ��
	vector<int>  pointer_change(all_seq_cnt, -1);
	for (auto e:base_class_set[no_error_seqsets_index])
	{
		no_error[e] = true;      
		pointer_change[e] = 1;
	}

	for (int i = 0; i < all_seq_cnt; i++)
	{
		if (!valid[i] || no_error[i])  continue;

		//�����������жϴ�������:�滻 or ɾ�� or ���� or ������ʹ���(��������  ��-�����ڴ�С+1�����ڷ���ֵ�б��ֳ�������ʹ��� )     ָ��λ��˵��: 1 �滻����   2 �������  0 ɾ������  -1 ������ʹ���
		string  error_seq = sequences_set[i];
		if (is_substitution(error_seq, seq_pointer[i],  pointer_change[i]))      //�ж�Ϊ�滻����
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
	int win_size = consensus_seq.size();  //���ǵ�������ĩβʱ�������ڴ�С����subsequent_window_size��������Ƚ�ʱʵ�ʴ��ڳ���ȡ��ʶ���г���;
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