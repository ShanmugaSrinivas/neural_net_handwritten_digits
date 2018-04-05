#include "neural.hpp"

template <class T>
void csv_writer(string fname,vector <T> &list)
{
    fstream fout(fname,ios::out);
    for(int i=0;i<list.size()-1;i++)
        fout << list[i] << ",";
    fout << list[list.size()-1];
}

void mnist()
{
    vector <int> layer_n={28*28,20,10};
    neural_dataset <float> ds_train("0train");
    neural_dataset <float> ds_test("0test");
    neural_dataset <float> ds_dev("0dev");
    neural<float> net(layer_n);
    vector <float> error;
    net.train(ds_train,ds_dev,10,100,error);
    net.train(ds_train,ds_dev,10,100,error);
    int size_final_error=100;
    vector <float> error_final(size_final_error,0);
    for(int i=0;i<size_final_error;i++)
        for(int j=0;j<error.size()/size_final_error;j++)
        {
            error_final[i]+=error[i*(error.size()/size_final_error)+j]/(error.size()/size_final_error);
        }
    csv_writer("error_0_training.csv",error_final);
    cout << "done training" << endl;
    float error_train=net.get_error(ds_train,100);
    cout << "\nerror_train=" << error_train;
    float error_test=net.get_error(ds_test,100);
    cout << "\nerror_test=" << error_test;
    net.write("net_0train_20");
}

void xor_train()
{
    vector <int> layer_n={2,200,2};
    neural_dataset <float> ds_all("xor");
    neural<float> net(layer_n);
    vector <float> error;
    //for(int i=0;i<1;i++)
        //net.train(ds_all,9,10,error);
    cout << "here" << endl;
}

int main()
{
    //xor_train();
    mnist();
    return 0;
}
