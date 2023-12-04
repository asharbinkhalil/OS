#include<iostream>
#include<unistd.h>
#include<fstream>
#include<sys/types.h>
#include<sys/wait.h>
#include<iomanip>
using namespace std;
int GenerateRandomNumber()
{
	// roll number ---> 20k-1724
	int num = (rand() % 90 + 10)  *  4;  // creating a random number and multiplying it with the last digit of the roll number
	num  = (1724 / num) %25;
	if(num < 10)
		num+=15;
	return num;
	
}
//multiplies the particular row 
void multiply(int** matrix, int size, int processNumber)
{
	int *rowOut = new int [size]{};
	for(int i=0;i<size;i++)
	{
		for(int j=0;j<size;j++)
			rowOut[i] += matrix[processNumber][j] * matrix[j][i];
	}
	
	fstream file("files//row_"+to_string(processNumber)+".txt",ios::out);
	file<< getpid()<<endl;
	for( int i=0;i<size;i++)
		file<< left<<setw(5)<<rowOut[i];
	file<<endl;
	file.close();
	delete[] rowOut;
}
//writes the whole matrix to the file
void WriteMatToFile(int ** mat, int size,string fname)
{
	fstream file("files//"+fname+".txt",ios::out);
	for( int i=0;i<size;i++)
	{
		for( int j=0;j<size;j++)
		{
			file<< left<<setw(4)<< mat[i][j];
		}
		file<<endl;
	}
}
//Reading the matrix from the files
int ** readMatrix(int size)
{
	int ** outmatrix= new int*[size];
	for( int i =0;i<size;i++)
	{
		outmatrix[i]= new int[size];
		fstream file("files//row_"+to_string(i)+".txt",ios::in);
		int temp=0;
		file >> temp;
		for(int j=0;j<size;j++)
		{
			file>>outmatrix[i][j];
		}
	}
	return outmatrix;
}
//multiplying the whole matrix, calling fork() creating n child processes for the N x N matrix
void MultiplyMatrix(int **matrix,int sizeMatrix)		
{
	int i=0;
	pid_t pid;
	for(i=0;i<sizeMatrix;i++)
	{
		pid = fork();	
		if(pid == 0 )
			break;
	}
	if(pid == 0)
	{
		multiply(matrix,sizeMatrix,i);
		exit(0);
	}
	else if(pid>0)
	{
		for(int i=0;i<sizeMatrix;i++)
			wait(0);
	}
}
// main function
int main()
{
	//random seed for the rand() fucntion
	srand(time(0));
	int sizeMatrix = GenerateRandomNumber();
	int ** matrix = new int*[sizeMatrix];
	// initializing the matrix
	for( int i =0;i<sizeMatrix;i++)
	{
		matrix[i]= new int[sizeMatrix];
		for(int j=0;j<sizeMatrix;j++)
			matrix[i][j] = rand()%10;
	}
	//writing the input matrix to the file
	WriteMatToFile(matrix,sizeMatrix,"input_mat");
	MultiplyMatrix(matrix,sizeMatrix);
	int ** result = readMatrix(sizeMatrix);
	// writing the final matrix to the file
	WriteMatToFile(result,sizeMatrix,"output");
	// displaying on terminal
	for(int i=0;i<sizeMatrix;i++)
	{
		for(int j=0;j<sizeMatrix;j++)
			cout<< left<<setw(5)<<result[i][j];
		cout<<endl;
	}
	//free the memory in heap
	for( int i=0;i<sizeMatrix;i++)
		delete[] matrix[i],result[i];
	delete []matrix,result;
	return 0;
}
