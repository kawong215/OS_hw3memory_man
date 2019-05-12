#include <iostream>
#include <fstream>
#include <string> 
using namespace std;


int rows = 100;
int columns = 3;

int create();

int main() 
{
  	ifstream infile;
	infile.open("memory.dat");

	int process[rows]; 
	char action[rows];
	int page[rows]; 

	if (!infile)
	{
		cout << "Cannot read file";
	}

	int count = 0; 	

	while (!infile.eof())
	{
		infile >> process[count] >> action[count];
		if (action[count] == 'C' || action[count] == 'T')
		{
			page[count] = 0; 
		}
		
		else
		{
			infile >> page[count]; 
		}
		count++; 
	}

	cout << "Number of rows: " << count-1 << endl; 

	for (int i = 0; i < count-1; i++)
	{
		cout << process[i] << " " << action[i] << " " << page[i] << endl;
	}

	infile.close(); 
}
