#include <iostream>
#include <fstream>
#include <string> 
#include <vector>
using namespace std;

struct page_table
{
	vector<int> process_id;
	vector<int> virtual_addr;
	vector<int> physical_addr;
	vector<bool> dirty_bit;
	vector<bool> swapped; 
};

int rows = 100;
int columns = 3;
vector<int> physical; 

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

	// for checking purposes
	//cout << "Original count: " << count << endl; 
	cout << "Number of rows: " << count-1 << endl; 

	for (int i = 0; i < count-1; i++)
	{
		cout << process[i] << " " << action[i] << " " << page[i] << endl;
	}

	page_table pg; 

	// put values in page table
	for (int i = 0; i < count-1; i++)
	{
		if (action[i] == 'A')
		{
			physical.push_back(process[i]); 
			pg.virtual_addr.push_back(page[i]); 
		}
	}

	// for checking purposes
	cout << "Physical pages allocated: " << endl; 
	for (int i = 0; i < physical.size(); i++)
	{
		cout << "At index " << i << endl;
		cout << physical[i] << " ";
		cout << pg.virtual_addr[i] << endl; 
		
	}
	
	// check if physical pages are full
	if (physical.size() == 20)
	{
		cout << "Physical pages are full" << endl; 
	}

	infile.close(); 
}

