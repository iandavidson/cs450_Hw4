/*
Authors: Ian Davidson, John Salman, Ryan Yu
File: p1.cpp
Assignment: HW 4
Date: Spring '18
*/



//instructions make ZERO sense at all so we are going to use a command line argument
//as means to how we are using the resource page table.



#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>
#include <math.h>


using namespace std;

class TableRep
{
public:


	class TableEntry{
	public:
		TableEntry()
		{
			valid = false;
			permission = false;
			physicalPageNumber = -1; //so we know its a bad instance
			useBit = false;//whatever the fuck that it
		}

		TableEntry(bool v, bool p, int pageNum, bool use)
		{
			valid =  v;
			permission = p;
			physicalPageNumber = pageNum;
			useBit = use;

		}
		~TableEntry()
		{}

		TableEntry(const TableEntry & t)
		{
			valid = t.valid;
			permission = t.permission;
			physicalPageNumber = t.physicalPageNumber;
			useBit = t.useBit;
		}

		void printEntry(ostream & o) {
			o << valid << " " << permission << " " << physicalPageNumber << " " << useBit << endl;
		}

		int getPhysicalPageNumber() {
			return physicalPageNumber;
		}

	private:
		bool valid;
		bool permission;
		int physicalPageNumber;
		bool useBit;//whatever the fuck that it
		//whatever else we need to keep track of this information.
	};

	TableRep()
	{
		virtBits = 0;
		physBits = 0;
		bytesInPage = 0;
		offset = 0;
		// bitsInPage
	}

	TableRep(int v, int p, int b){
		virtBits = v;
		physBits = p;
		bytesInPage = b;
		offset = log2(bytesInPage);
		// bitsInpage = log2(bytesInPage);
		// TableRecords = other;
	}

	~TableRep(){
		//delete TableRecords; //only declaring this on the heeep
		for(int i = 0; i < TableRecords.size(); i++ ){
				delete TableRecords[i];
		}
		//need to actually run through array and delete each instane of table entry
	}

	void insertTableEntry(bool v, bool p, int pageNum, bool use) {
		TableEntry* te = new TableEntry( v,  p,  pageNum,  use);
		TableRecords.push_back(te);
	}

	vector<TableEntry *> getTableRecords()
	{
		return TableRecords;
	}

	int getSize(){
		return TableRecords.size();
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

	void printTable(ostream & o) {
		for (int i = 0; i < TableRecords.size(); i++) {
			TableRecords[i]->printEntry(o);
		}
	}

	int translateAddress(int virtualAddress) {
			// calculate VPN
			// index the table
			// offset to find specific address within PPN
			// return PPN

			int mask = pow(2, virtBits) - 1; // mask of 1s
			cout << "mask: " << mask << endl;
			virtualAddress &= mask;
			cout << "virtual address: " << virtualAddress << endl; // off by 1. 48 instead of 96
			int VPN = virtualAddress >> offset;
			cout << "VPN: " << VPN << endl;
			int PPN = TableRecords[VPN]->getPhysicalPageNumber();
			int physicalAddress = PPN << offset;
			mask = pow(2, offset) - 1;
			physicalAddress &= mask;
			return physicalAddress;
	}

private:
	int virtBits; // bitwidths of the addresses
	int physBits;
	int bytesInPage;
	int offset; // bitwidth of the offset
	// bitsInpage = log2(bytesInPage);
	vector<TableEntry*> TableRecords;
	//TableEntry* TableRecords[];
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

TableRep * ReadFile(ifstream & input);

//physAddress parseVirtAddress(int address, bool hex, TableRep * & T);

int main(int argc, char* argv[])
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

	tableRep->printTable(cout);

	//now we need to read in from cin, from the file redirected in

	string address = "";
	int addr = -1;
	int physicalAddress = 0;
	while(cin >> address)//each result should be a virtual address we need to interpret based off tableRep,
	{
		//int addrSize = address.length();
		addr = -1;
		physicalAddress = 0;
		if(address.length() > 1){
			//either hex or decimal addr
			 	if(address[1] == 'x'){ //expecting the "0x" in front of the word
					addr = stoi(address, nullptr, 16);
		 		}else {
					addr = stoi(address);
				}
		}else{
			//we know this is specifically a newline char OR a single digit decimal based address
			if(address[0] != '\n' || address != " ") {
				addr = stoi(address);
			}
		}
		cout << "virtual addr: " << addr << endl;
		if(addr < 0) {
			cout << "invalid address" << endl;
			return 0;
		}
		// process input
		physicalAddress = tableRep->translateAddress(addr);
		cout << "phsyical address: " << physicalAddress << endl << endl;
	}

	return 0;
}

// parseVirtAddress(int address, bool hex, TableRep * & T){}
// should return the phsyical address or 0(if permission is )

//should probably use bit shifts maybe shift off the bits of the page to determine value in addr.


TableRep * ReadFile(ifstream & input)
{

	int w = 0;

	//first 3 items are important
	input >> w;

	//virtBits after casted from string
	int v = w;


	//first 3 items are important
	input >> w;

	//physBits after casted from string
	int p = w;


	//first 3 items are important
	input >> w;

	//bytesInPage after casted from string
	int b = w;

	int s = 0; //used an accumulator when figuring out how much input is left from file.
	string word = "";
	int valid;
	int per;
	int phy;
	int use;
	TableRep* TR = new TableRep(v, p, b);

	while(input >> word) {
		valid = stoi(word);

		input >> word;
		per = stoi(word);

		input >> word;
		phy = stoi(word);

		input >> word;
		use = stoi(word);

		TR->insertTableEntry(valid, per, phy, use);
		word = "";
	}

	return TR;
}
