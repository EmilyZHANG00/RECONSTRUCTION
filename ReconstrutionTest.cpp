#include "ReconstrutionTest.h"

bool ReconstrutionTest::get_data(string filepath)
{
    std::ifstream inputFile(filepath); // 打开文本文件
    std::string line;
    bool NewCluster = true;
    vector<string> empty_vec;

    copy_seqs.push_back(empty_vec);
    if (inputFile.is_open()) {
        cout << "-- [ open file ! ]" << endl;
        while (getline(inputFile, line)) {
       //     cout <<line.length() << line << endl;
            if (line.length() == 0)
            {
                if (!NewCluster)    //读取第一个空行的时候进行处理。
                {
                    cout << "-- [ have read cluster "<< copy_seqs.size() <<" , include "<<copy_seqs.back().size()<<" sequences ]" << endl;
                    copy_seqs.push_back(empty_vec);
                    NewCluster = true;
                }
                else
                    continue;
            }
            else if (line[0] == '=')
            {
                NewCluster = false;
            }
            else    //数据序列
            {
                if (NewCluster)
                    origin_seqs.push_back(line);
                else
                    copy_seqs.back().push_back(line);

            }   
        }

        //如果最后一个簇大小为0的话，弹出最后一个(文件最后存在空行的话，可能会造成这种情况)
        if (copy_seqs.back().size() == 0)
            copy_seqs.pop_back();
        cluster_size = origin_seqs.size();
        pirnt_data_size();
        inputFile.close();
        return true;
    }
    else {
        std::cerr << "Unable to open file." << std::endl;
        return false;
    }
}

void ReconstrutionTest :: get_reconstruction_result() {
	for (int i = 0; i < cluster_size; i++)
	{
        string res_seq = windowJudge.reconstruction_one_sequences(copy_seqs[i]);
        string is_same;
        if (origin_seqs[i] == res_seq)
            is_same = "YES";
        else
            is_same = "NO";
        cout << endl 
            << "reference sequence   :" << origin_seqs[i] << endl
            << "reconstruction result:" << res_seq << endl
            << "is same:"<<is_same<<endl;
	}
}


void ReconstrutionTest::pirnt_data_size()
{
    cout << endl << "numbers of references :" << origin_seqs.size() << endl
        << "numbers of references reads:";
    for (int i = 0; i < copy_seqs.size(); i++)
    {
        cout << "  " << copy_seqs[i].size();
    }
    cout << endl;
}