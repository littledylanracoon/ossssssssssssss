#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime>
using namespace std;
const int Max = 2147483647;
const int n = 1024 * 1024 * 1024 / 4;// 1GB

void Merge(vector<int>&Array, int front, int mid, int end){
	vector<int>Left(Array.begin()+front, Array.begin()+mid+1),Right(Array.begin()+mid+1, Array.begin()+end+1);
	Left.insert(Left.end(),Max);      
	Right.insert(Right.end(),Max);    
	int leftindex = 0, rightindex = 0;
	for (int i = front; i <= end; i++) {
		if (Left[leftindex] <= Right[rightindex] ) {
			Array[i] = Left[leftindex];
			leftindex++;
		}
		else{
			Array[i] = Right[rightindex];
			rightindex++;
		}
	}
}
void MergeSort(vector<int>&array, int front, int end){
	if (front < end) {                  
		int mid = (front+end)/2;        
		MergeSort(array, front, mid);    
		MergeSort(array, mid+1, end);    
		Merge(array, front, mid, end);   
	}
}
/*
void PrintArray(vector<int> &array){
	for (int i = 0; i < n; i++) {
		cout << array[i] << " ";
	}
	cout << endl;
}*/
const char* NumtoStr(int t) // file 
{
	ostringstream ss;
	ss << t;
	return ss.str().c_str();	
}

int main(void) {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");
	clock_t st, fin;
	float time;
	vector<int> array(n);
	int t = 0; //count num of data read in 
	int k = 1; //num of file 
	int c = 0;
	st = clock();
	while(fscanf(in, "%d", &(array[t++])) != EOF){ //read file and sort
		if(t == n) //fill 
		{	
			MergeSort(array, 0, n-1);

			FILE * fp = fopen(NumtoStr(k++), "w");
			for(int i = 0; i < n; i++){
				//cout << c++ << " " << array[i] << endl;
				fprintf(fp, "%d\n", array[i]);
			}
			fclose(fp);
			t = 0;
		}
	}
	MergeSort(array, 0, t - 2); //sort remain data
	FILE * fp = fopen(NumtoStr(k), "w");
	for(int i = 0; i < t - 1; i++){
		//cout << c++ << " " << array[i] << endl;
		fprintf(fp, "%d\n", array[i]);
	}
	fclose(fp);
	//printf("k = %d\n", k);
	FILE* fps[k];
	for(int i = 0; i < k; i++)
		fps[i] = fopen(NumtoStr(i + 1), "r");
	int bf_size = n / 1024;

	int bf[k][bf_size]; //k buffers with respectively bf_size
	//vector< vector<int>> bf(k);
	vector<int> endpos(bf_size);
	for(int i = 0; i < k; i++) //read data from file
	{
		t = 0;
		int tmp;
		endpos[i] = bf_size;
		while(fscanf(fps[i], "%d", &tmp) != EOF)
		{
			bf[i][t++] = tmp;
			if(t == bf_size)
				break;
		}
		if(t < bf_size) // remain data
			endpos[i] = t;
	}
	int pos[k] = {0};
	int min = bf[0][0]; //record min num in compare procss
	int minpos = 0;
	//printf("test\n");
	while(1)
	{
		bool finished = true;
		for(int i = 0; i < k; i++)
		{
			if(endpos[i] != 0)
			{
				finished = false;
				break;
			}
		}
		if(finished) //k-way merge finished
		{
			fclose(out);
			fin = clock();
			time = (double)(fin- st) / CLOCKS_PER_SEC;
			printf("%f s\n", time);
			break; //sort end!!!!!!!!! :)
		}
		for(int i = 0; i < k; i++)
		{
			if(pos[i] < endpos[i] && min > bf[i][pos[i]])
			{
				min = bf[i][pos[i]];
				minpos = i;
			}
		}
		fprintf(out, "%d\n", min);
		pos[minpos]++;
		if(endpos[minpos] < bf_size && pos[minpos] == endpos[minpos]) //this file sorted!
			endpos[minpos] = 0;
		else if(pos[minpos] == endpos[minpos]) //buffer sorted but file not ended 
		{
			pos[minpos] = 0;
			t = 0;
			int tmp;
			endpos[minpos] = bf_size;
			while(fscanf(fps[minpos], "%d", &tmp) != EOF)
			{
				bf[minpos][t++] = tmp;
				if(t == bf_size)
					break;
			}
			if(t < bf_size)
				endpos[minpos] = t;
		}
		for(int i = 0; i < k; i++)
		{
			if(endpos[i] != 0)
			{
				min = bf[i][pos[i]];
				minpos = i;
				break;
			}
		}
	}

	out = fopen("output.txt", "r");
	int counter = 0; //count number
	int last_t = 0; 
	while(fscanf(out, "%d", &t) != EOF)
	{
		counter++;
		if(counter != 1 && last_t > t) // compare and check if it's all sorted
		{
			cout << "ERROR!" << endl;
			break;
		}
		last_t = t;
	}
	for(int i = 1; i < k; i++) //remove temporary files
		remove(NumtoStr(i));
	fclose(out);
	cout << counter << endl;
	return 0;
}
