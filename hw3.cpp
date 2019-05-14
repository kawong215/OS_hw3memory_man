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
	int pg_count = 0; 

	// put values in page table
	for (int i = 0; i < count-1; i++)
	{
		if (physical.size() > 20)
		{
			cout << "Physical pages are full" << endl; 
			cout << "SWAP NECESSARY" << endl; 
			break; 
		}

		if (action[i] == 'A')
		{
			physical.push_back(process[i]); 
			pg.virtual_addr.push_back(page[i]); 
			pg.physical_addr.push_back(pg_count);
			pg.dirty_bit.push_back(false); 
			pg.swapped.push_back(false); 
			pg_count++;
		}
	}

	// for checking purposes
	cout << endl <<  "Page table: " << endl << endl; 
	for (int i = 0; i < physical.size(); i++)
	{
		cout << "---At index " << i << " ---" << endl;
		cout << "ID: " << physical[i] << endl;
		cout << "VIRTUAL: " << pg.virtual_addr[i] << endl;
		cout << "PHYSICAL: " << pg.physical_addr[i] << endl;
		cout << "DIRTY BIT: " << pg.dirty_bit[i] << endl;
		cout << "SWAPPED: " << pg.swapped[i] << endl;
		cout << endl;  	
	}

	infile.close(); 
}

