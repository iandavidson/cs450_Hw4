/*
Authors: Ian Davidson, John Salman, Ryan Yu
File: p1.cpp
Assignment: HW 4
Date: Spring '18
*/



//instructions make ZERO sense at all so we are going to use a command line argument
//as means to how we are using the resource page table.



#include <iostream>
#include <cstdlib>
#include <string>
#include <cctype>

using namespace std;

class TableRep;

TableRep & ReadFile(ifstream & input);

//physAddress parseVirtAddress(int address, bool hex, TableRep * & T);

int main(int argc, char* [] argv)
{
	if(argc != 2){ //exe + inputfile
		cerr << "needs input file as arguement. exiting" << endl;
		exit(1);
	}
	
	ifstream input (argv[1]);
	if(!input.good()){
		cerr << "input is not a file" << endl;
		exit(1);
	}

	//make instance of TableRep
	TableRep * tableRep =  ReadFile(input);

	
	//now we need to read in from cin, from the file redirected in

	string address = "";

	while(cin >> word)//each result should be a virtual address we need to interpret based off tableRep, 
	{
		//address can be binary or hex
		if(word[1] == 'x'){ //expecting the "0x" in front of the word
			//hex address

		}else{
			//decimal address

		}
		//call parseVirtAddress

	}


	return 0;
}

// parseVirtAddress(int address, bool hex, TableRep * & T){}
// should return the phsyical address or 0(if permission is )

//should probably use bit shifts maybe shift off the bits of the page to determine value in addr.


TableRep & ReadFile(ifstream & input)
{
	
	string word = "";

	//first 3 items are important
	input >> word;

	//virtBits after casted from string
	int v = stoi(word);


	//first 3 items are important
	input >> word;

	//physBits after casted from string
	int p = stoi(word);


	//first 3 items are important
	input >> word;

	//bytesInPage after casted from string
	int b = stoi(word);


	int s = -1; //used an accumulator when figuring out how much input is left from file.

	input.clear();
	input.seekg(0, ios::beg);

	string line = "";

	//get line
	while(getline(input,line)){
		if(!isspace(word)) // a line consisting of information we WANT
			s++;
	}

	int ** table;


	table = new int * [4];

	for(int i = 0; i < 4; i++){
		table[i] = new int[s]; 
	}
	
	getline(input, line)//the first 


	for(int i = 0; i < s; i++){
		getline(input, line);

		input >> word;
		table[0][i] = stoi(word);

		input >> word;
		table[1][i] = stoi(word);

		input >> word;
		table[2][i] = stoi(word);

		input >> word;
		table[3][i] = stoi(word);
	}


	TableRep * tableRep = new TableRep(v, p, b, s, table);

	return *tableRep;
}


class TableRep
{
public: 
	TableRep()
	{
		virtBits = 0;
		physBits = 0;
		bytesInPage = 0;
		size = 0;
		TableRecords = NULL;
	}

	TableRep(int v, int p, int b, int s, int ** T){
		virtBits = v;
		physBits = p;
		bytesInPage = b;
		size = s;
		TableRecords = T;
	}

	~TableRep(){
		delete TableRecords; //only declaring this on the heeep
	}

	int ** getTableRecords()
	{
		return  TableRecords;
	}

	int getSize(){
		return size;
	}

	int getBytesInPage(){
		return bytesInPage;
	}

	int getVirtBits(){
		return virtBits;
	}

	int getPhysBits(){
		return physBits;
	}

private:
  int virtBits;
  int physBits;
  int bytesInPage;
  int size;
  int** TableRecords;
/*
Number of bits in virtual address
Number of bits in physical address
Number of BYTES in a page
The lines of a flat page table for the current process, in the following format:
Valid?
Permission (1 bit)
Physical page number
Use bit (see Problem 2) 
*/
  





};
