#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
const int Max = 2147483647;
//const int n = 15;
const int n = 1024 * 1024 * 1024/ 4;// 1GB
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
void PrintArray(vector<int> &array){
	for (int i = 0; i < n; i++) {
		cout << array[i] << " ";
	}
	cout << endl;
}
const char* NumtoStr(int t)
{
	ostringstream ss;
	ss << t;
	return ss.str().c_str();	
}

int main() {
	FILE *in = fopen("input.txt", "r");
	FILE *out = fopen("output.txt", "w");
	vector<int> array(n);
	int t = 0;
	int k = 1;
	int c = 0;
	while(fscanf(in, "%d", &(array[t++])) != EOF){
		if(t == n)
		{	
			MergeSort(array, 0, n-1);
			FILE * fp = fopen(NumtoStr(k++), "w");
			for(int i = 0; i < n; i++){
				cout << c++ << " " << array[i] << endl;
				fprintf(fp, "%d\n", array[i]);
			}
			fclose(fp);
			t = 0;
		}
	}
	MergeSort(array, 0, t - 2);
	FILE * fp = fopen(NumtoStr(k), "w");
	for(int i = 0; i < t - 1; i++){
		cout << c++ << " " << array[i] << endl;
		fprintf(fp, "%d\n", array[i]);
	}
	fclose(fp);

	FILE* fps[k];
	for(int i = 0; i < k; i++)
		fps[i] = fopen(NumtoStr(i + 1), "r");
	int bf_size = n / 1024;

	int bf[k][bf_size];
	//vector< vector<int>> bf(k);
	vector<int> end_position(bf_size);
	for(int i = 0; i < k; i++)
	{
		t = 0;
		int tmp;
		end_position[i] = bf_size;
		while(fscanf(fps[i], "%d", &tmp) != EOF)
		{
			bf[i][t++] = tmp;
			if(t == bf_size)
				break;
		}
		if(t < bf_size)
			end_position[i] = t;
	}
	int position[k] = {0};
	int min = bf[0][0];
	int min_position = 0;
	printf("test\n");
	while(1)
	{
		bool finished = true;
		for(int i = 0; i < k; i++)
		{
			if(end_position[i] != 0)
			{
				finished = false;
				break;
			}
		}
		if(finished)
		{
			fclose(out);
			break;
		}
		for(int i = 0; i < k; i++)
		{
			if(position[i] < end_position[i] && min > bf[i][position[i]])
			{
				min = bf[i][position[i]];
				min_position = i;
			}
		}
		fprintf(out, "%d\n", min);
		position[min_position]++;
		if(end_position[min_position] < bf_size && position[min_position] == end_position[min_position])
			end_position[min_position] = 0;
		else if(position[min_position] == end_position[min_position])
		{
			position[min_position] = 0;
			t = 0;
			int tmp;
			end_position[min_position] = bf_size;
			while(fscanf(fps[min_position], "%d", &tmp) != EOF)
			{
				bf[min_position][t++];
				if(t == bf_size)
					break;
			}
			if(t < bf_size)
				end_position[min_position] = t;
		}
		for(int i = 0; i < k; i++)
		{
			if(end_position[i] != 0)
			{
				min = bf[i][position[i]];
				min_position = i;
				break;
			}
		}
	}

	out = fopen("output.txt", "r");
	fscanf(out, "%d", &t);
	int counter = 0;
	int last_t = 0;
	while(fscanf(out, "%d", &t) != EOF)
	{
		counter++;
		if(counter != 1 && last_t > t)
		{
			cout << "ERROR!" << endl;
			break;
		}
		last_t = t;
	}
	for(int i = 1; i < k; i++)
		remove(NumtoStr(i));
	fclose(out);
	cout << counter << endl;
	return 0;
}
