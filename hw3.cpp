#include <iostream>
#include <fstream>
#include <string> 
#include <algorithm>    // std::replace, std::find
#include <vector>
using namespace std;

struct page_table
{
	vector<int> process_id;
	vector<int> virtual_addr;
	vector<int> physical_addr;
	vector<int> dirty_bit;
	vector<int> swapped;
	vector<int> accessed;  
} pg_table[100]; // used to create page tables per process

int rows = 100;
int columns = 6;

vector<int> physical; // physical pages -- 20 in total

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

	int key = process[0]; 

	vector<int> id; 
	id.push_back(process[0]); 
	vector<int>::iterator result; 

	for (int i = 1; i < count; i++)
	{
		if (key != process[i])
		{
			result = find(id.begin(), id.end(), process[i]); 
			
			if (result == id.end())
			{
				id.push_back(process[i]); 
			}	
			key = id[i];
		}
	}
	
	// for checking purposes
	cout << "Number of page tables: " << id.size() << endl; 
	
	int pg_count = 0; 
	int time = 0; 
	int index = 0;

	// put values in page table
	for (int i = 0; i < count-1; i++)
	{
		if (physical.size() > 20)
		{
			cout << "Physical pages are full" << endl; 
			cout << "SWAP NECESSARY" << endl; 
			// call swap function: FIFO, LRU, RANDOM instead of break
			break; 
		}

		result = find(id.begin(), id.end(), process[i]);
		index = distance(id.begin(), result);

		if (action[i] == 'A' && physical.size() < 20)
		{
			physical.push_back(process[i]); 

			pg_table[index].process_id.push_back(process[i]); 
			pg_table[index].virtual_addr.push_back(page[i]); 
			pg_table[index].physical_addr.push_back(pg_count);
			pg_table[index].dirty_bit.push_back(0); 
			pg_table[index].swapped.push_back(0); 
			pg_count++;
		}
 
		if (action[i] == 'T') // terminate
		{
			replace(physical.begin(), physical.end(), process[i], 0); 
		}
		
		else if (action[i] == 'W') // write and set dirty bit
		{
			pg_table[index].dirty_bit.push_back(1); 
			time++; 
			pg_table[index].accessed.push_back(time); 
		}
		
		else if (action[i] == 'R')
		{
			time++; 
			pg_table[index].accessed.push_back(time); 
		}	
	}

	vector<int>::iterator current; 
	vector<int>::iterator print_virt;
	vector<int>::iterator print_phys;
	vector<int>::iterator swap; 

	for (int i = 0; i < id.size(); i++)
	{
		cout << "Page Table for Process " << id.at(i) << endl;
		
		print_virt = pg_table[i].virtual_addr.begin();
		print_phys = pg_table[i].physical_addr.begin(); 
		while (print_virt < pg_table[i].virtual_addr.end() && print_phys < pg_table[i].physical_addr.end())
		{
			cout << "VIRTUAL     " << *print_virt << "     PHYSICAL     ";
			
			swap = find(pg_table[i].swapped.begin(), pg_table[i].swapped.end(), 0); 
			if (result != pg_table[i].swapped.end())
			{
				cout << *print_phys << endl;
			}	
			else
			{
				cout << "SWAPPED" << endl; 
			}

			print_virt++;
			print_phys++; 
		}
	}

	cout << endl <<  "--PHYSICAL PAGES --" << endl << endl; 
	for (int i = 0; i < physical.size(); i++)
	{
		cout << i << "     " << physical[i] << endl;  	
	}

	infile.close(); 
}
