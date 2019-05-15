#include <iostream>
#include <fstream>
#include <string> 
#include <algorithm>    // std::replace, std::find, std::distance
#include <vector>
using namespace std;

int oldest_access_time(vector<int>times); // finds oldest accessed time for LRU

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
	cout << "Number of page tables: " << id.size() - 1 << endl; 
	
	int pg_count = 0; 
	int time = 0; 
	int index = 0;
	int location = 0; 
	bool swap_needed = false;
	vector<int>::iterator freed; 

	// put values in page table
	for (int i = 0; i < count-1; i++)
	{
		if (action[i] == 'A' && physical.size() == 20)
		{
			/* for checking purposes
			cout << "Physical pages are full" << endl; 
			cout << "SWAP NECESSARY" << endl; 
			*/

			// call swap function: FIFO, LRU, RANDOM instead of break
			swap_needed = true; 
			break; 
		}

		result = find(id.begin(), id.end(), process[i]); // find page table for specific process
		index = distance(id.begin(), result);

		if (action[i] == 'A' && physical.size() < 20)
		{
			physical.push_back(process[i]); 

			pg_table[index].process_id.push_back(process[i]); 
			pg_table[index].virtual_addr.push_back(page[i]); 
			pg_table[index].physical_addr.push_back(pg_count);
			pg_table[index].dirty_bit.push_back(0); 
			pg_table[index].swapped.push_back(0); 
			pg_table[index].accessed.push_back(0); 
			pg_count++;
		}

		if (action[i] == 'T')
		{
			replace(physical.begin(), physical.end(), process[i], 0); 
		}
		
		if (action[i] == 'F')
		{
			freed = find(pg_table[index].virtual_addr.begin(), pg_table[index].virtual_addr.end(), page[i]);
			location = distance(pg_table[index].virtual_addr.begin(), freed); // get location of virtual address

			if (freed != pg_table[index].virtual_addr.end())
			{
				pg_table[index].virtual_addr.erase(pg_table[index].virtual_addr.begin()+location); 
				pg_table[index].process_id.erase(pg_table[index].process_id.begin()+location); 
			}	

			// find location of physical to free
			freed = find(pg_table[index].physical_addr.begin(), pg_table[index].physical_addr.end(), location);

			if (freed != pg_table[i].physical_addr.end())
			{
				physical.at(location) = 0; // empty physical page
				pg_table[index].physical_addr.erase(pg_table[index].physical_addr.begin()+location); 
				pg_table[index].accessed.erase(pg_table[index].accessed.begin()+location); 
				pg_table[index].dirty_bit.erase(pg_table[index].dirty_bit.begin() + location); 
			    	pg_table[index].swapped.erase(pg_table[index].swapped.begin() + location); 
			}	

			else
			{
				cout << "Cannot free unallocated page. Process killed." << endl; 
				break; 
			}		
		}
		
		else if (action[i] == 'W')
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

	vector<int> combined;
	int total_size = 0; 

	// call swap functions
	if (swap_needed == true)
	{
		for (int i = 0; i < id.size(); i++)
		{
			total_size += pg_table[i].accessed.size();
			
		}
		combined.reserve(total_size); 
		cout << "reserved size: " << combined.capacity() << endl; 
		for (int i = 0; i < id.size(); i++)
		{
			combined.insert(combined.end(), pg_table[i].accessed.begin(), pg_table[i].accessed.begin()); 
		}
	}

	vector<int>::iterator current; 
	vector<int>::iterator print_virt;
	vector<int>::iterator print_phys;
	vector<int>::iterator swap; 

	for (int i = 0; i < id.size() - 1; i++)
	{
		cout << "Page Table for Process " << id[i] << endl;
		
		print_virt = pg_table[i].virtual_addr.begin();
		print_phys = pg_table[i].physical_addr.begin(); 
		while (print_virt < pg_table[i].virtual_addr.end() && print_phys < pg_table[i].physical_addr.end())
		{
			cout << "VIRTUAL     " << *print_virt << "     PHYSICAL     ";
			
			// check if any swap flags were set true 
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
		cout << i << "     ";
		if (physical[i] == 0)
		{
			cout << "FREE" << endl;
		}  	
		else
		{
			cout << "Process " << physical[i] << endl; 
		}
	}

	infile.close(); 
}



int oldest_access_time(vector<int>times)
{
	vector<int> sorted;
	sorted = times;
	sort(sorted.begin(), sorted.end(), greater<int>()); 
	return sorted.front(); 
}
