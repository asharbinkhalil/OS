#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/select.h>
#include<vector>
#include<iostream>
#include<mutex>
#include<numeric>
#include<sstream>
#include<string.h>
#include <limits>
#include<fstream>
using namespace std;

#define ITERATIONS 5
mutex CSizemtx;
int Layers = 4;
int LAYER_COUNT = 4;
int COMP_Count = 0;
int SIZE = 0;
const float MAX_VAL  = std::numeric_limits<float>::max();
vector<int> neurons;
vector<float> currvals, prevvals;
vector<vector<vector<float>>> all;
vector<vector<float>>currLayerWeights;
void* NeuronsFunc(void* args)
{
    int* ptr = (int*)args;
    //locks the mutex
    while (1)
    {
	CSizemtx.lock();
	if (SIZE==-1)
        {
            CSizemtx.unlock();
            break;
        }
        if (ptr[0] == 0)
        {
         	CSizemtx.unlock();
         	continue;
        }
        if (COMP_Count == SIZE || SIZE == 0 || currvals[ptr[1]] !=-999.0f )
        {
            CSizemtx.unlock();
            usleep(100000);
            continue;
        }

        float dot_product=0;
        for(int i=0;i<prevvals.size();i++)
            dot_product+= prevvals[i]*currLayerWeights[i][ptr[1]];
        currvals[ptr[1]] = dot_product;
        COMP_Count++;
        CSizemtx.unlock();
    }
    pthread_exit(NULL);
}
void strToList(char arr[], vector<float> &vt)
{

    stringstream ss(arr); // create a stringstream from the char array
    vt.clear();
    int size;
    ss >> size; // extract the size from the stringstream
    float val;
    for(int i = 0; i < size; i++) {
        ss>>val;
        vt.push_back((val)); // add the value to the vector
    }
    // print the vector to verify the values were stored correctly
}
void convert_toString(char arr[],vector<float> vt)
{
    string str;
    if(vt.size()>20)
        return;
    str+=to_string(vt.size()) +" ";
    for(auto i : vt)
        str+=to_string(i)+" ";
    strcpy(arr, str.c_str());
}
float fX1(float X)
{
    float res = ((X*X) +X + 1)/2;
    return res;
}
float fX2(float X)
{
    float res = ((X*X) - X)/2;
    return res;
}
int CreateLayer(int* prevWrite = NULL, int* prevRead = NULL)
{
    //clsoing reading for fd1, writing for fd2
    LAYER_COUNT--;
    //getting the number of nuerons for the current layer    
    int size = neurons[Layers - LAYER_COUNT - 1];
    //thread ids for neurons threads
    pthread_t* tids = new pthread_t[size];
    // pipes for the communication between current layer and the next
    int* nextRead = new int[2] {};
    int* nextWrite = new int[2] {};
    pipe(nextRead);
    pipe(nextWrite);

    if (LAYER_COUNT)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            //creating required number of processes as layers
            // cout << "layer " << Layers - LAYER_COUNT << "created";
            close(nextWrite[1]);
            close(nextRead[0]);
            CreateLayer(nextRead, nextWrite);
            // cout <<"exit called\n";
            exit(0);
        }
    }
    //closing the pipes for this layer
    close(nextWrite[0]);
    close(nextRead[1]);
    currLayerWeights = all[Layers - LAYER_COUNT - 1];



    currvals.resize(size);
    // cout <<currvals.size()<<" = size"<<endl;
    std::fill(currvals.begin(), currvals.end(), -999.0f);  // set all values to -----
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setstacksize(&attr, 1024 * 1024); // 512kb
    for (int i = 0; i < size; i++)
    {
        int* args = new int[2];
        args[0] = Layers - LAYER_COUNT - 1; // layer number
        args[1] = i;                        //neurons number
        pthread_create(&tids[i], &attr, NeuronsFunc, args);
    }

    int i = 0;
        char buffer[1024];
    while (i < ITERATIONS)
    {
        i++;
        CSizemtx.lock();
        std::fill(currvals.begin(), currvals.end(), -999.0f);  // set all values to -1
        CSizemtx.unlock();
        // cout<<"entered\n";
        // if layer first layer skip 
        if (Layers != (LAYER_COUNT + 1)) {

            int readResult = read(prevRead[0], buffer, 1024);
            if (readResult == -1) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            strToList(buffer,prevvals);
            CSizemtx.lock();
            SIZE = size;
            while(COMP_Count!=SIZE)
            {
                CSizemtx.unlock();
                
                CSizemtx.lock();
            }
            SIZE=0;
            COMP_Count=0;
            CSizemtx.unlock();
        }
        // if first layer
        else 
        {
            if(i==1)
                currvals = all[0][0];
            else
                strToList(buffer,currvals);
        }
        convert_toString(buffer,currvals);
        

        // next step is to compute some stuff and send it to next layer (if not the ouptut layer)

        if (LAYER_COUNT > 0)
        {
            int writeResult = write(nextWrite[1], buffer, 1024);
            if (writeResult == -1) {
                perror("write error");
                exit(EXIT_FAILURE);
            }
            if(i==ITERATIONS)
            {
                wait(0);
                if((Layers - LAYER_COUNT - 1 ) == 0)
                                       exit(0);

             /* for (int i = 0; i < size; i++){
                    pthread_cancel(tids[i]);
                    pthread_join(tids[i], NULL);
             }*/

                                   exit(0);

            }
            // now this layer will wait for the backward feed from the next layer
            int readResult = read(nextRead[0], buffer, 1024);
            if (readResult == -1) {
                perror("read error");
                exit(EXIT_FAILURE);
            }
            cout << Layers - LAYER_COUNT - 1 << " received from next  layer = " << buffer << endl;
        }
        else
        {
            cout << "ITERATION   ["<<i<<"]      output = "<<currvals[0]<<endl<<endl;
            if(i==ITERATIONS)
             {
                CSizemtx.lock();
                SIZE=-1;
                CSizemtx.unlock();

                for (int i = 0; i < size; i++){
                    
                    pthread_join(tids[i], NULL);
                }
                exit(0);
                }
            // yeh calculate krni hai abhi
            float x=currvals[0];
            vector<float> vt = {fX1(x),fX2(x)};
            convert_toString(buffer,vt);
        }
        // then this layer will send the feed to the previous layer (if not the first layer)
        if (Layers != (LAYER_COUNT + 1)) {
            int writeResult = write(prevWrite[1], buffer, 1024);
            if (writeResult == -1) {
                perror("write error");
                exit(EXIT_FAILURE);
            }
        }

    }
    return 1;
}

void ReadConfig()
{
    vector<vector<float>> weights;
    ifstream input_file("file.txt");
    if (input_file.is_open()) {
        string line;
        int layer = -1;
        while (getline(input_file, line)) {
            if(line.size()==0)
                continue;
            if(line.size()==1 || line == "input")
            {
                if(layer>=0)
                    all.push_back(weights);
                layer++;
                if(line=="input")
                    layer=-1;
                weights.clear();
                continue;
            }
            stringstream ss(line);
            vector<float> row;
            float value;
            while (ss >> value) {
                row.push_back(value);
                if (ss.peek() == ',') ss.ignore();
            }
            
            weights.push_back(row);
            if(layer==-1)
            {
                    all.insert(all.begin(),weights);
                    break;
            }
        }
        all.push_back(weights);
        input_file.close();
    }
    all.erase(all.begin()+all.size()-1);
    for(auto i:all)
        neurons.push_back(i[0].size());
    Layers=LAYER_COUNT=neurons.size();
}

int main()
{
    ReadConfig();
    srand(time(0));
    pid_t pid = fork();
    if (pid == 0)
    {
        //creatin the first hidden layer.... input layer
        CreateLayer();
        exit(0);
    }
    wait(0);
    return 0;
}