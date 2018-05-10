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

	struct TableEntry{
		bool valid;
		bool permission;
		int physicalPageNumber;
		bool useBit;
	};


	TableRep()
	{
		virtBits = 0;
		physBits = 0;
		bytesInPage = 0;
		offset = 0;
		clock = 0;
		// bitsInPage
	}

	TableRep(int v, int p, int b){
		virtBits = v;
		physBits = p;
		bytesInPage = b;
		offset = log2(bytesInPage);
		clock = 0;
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
		TableEntry* te = new TableEntry{v,  p,  pageNum,  use};
		TableRecords.push_back(te);
	}

	void pageReplacement(int vpn) { // valid bit 0 at this vpn
		// decide which entry to replace
		// check the entry the clock points
		if (TableRecords[clock]->useBit) {
			incrementClock(); // after this function call, the clock points to the next valid entry
			pageReplacement(vpn);
		}
		else {
			// if the use bit is zero, do the replacement.
			TableRecords[vpn]->valid = TableRecords[clock]->valid;
			TableRecords[vpn]->permission = TableRecords[clock]->permission;
			TableRecords[vpn]->physicalPageNumber = TableRecords[clock]->physicalPageNumber;
			TableRecords[vpn]->useBit = true;
			TableRecords[clock]->valid = false;
			incrementClock();
		}
	}

	void incrementClock(){//vpn of new virt address as param
		TableRecords[clock]->useBit = false;
		clock = (clock + 1) % TableRecords.size();
		while (!TableRecords[clock]->valid || !TableRecords[clock]->permission) {
			// entry is invalid, move on to the next one.
			clock = (clock + 1) % TableRecords.size();
		}
	}

	vector<TableEntry *> getTableRecords()
	{
		return TableRecords;
	}

	int getSize(){
		return TableRecords.size();
	}

	int getOffSet(){
		return offset;
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
			// TableRecords[i]->printEntry(o);
			cout << TableRecords[i]->valid << " " << TableRecords[i]->permission << " " << TableRecords[i]->physicalPageNumber << " " << TableRecords[i]->useBit << endl;
		}
	}



private:
	int virtBits; // bitwidths of the addresses
	int physBits;
	int bytesInPage;
	int offset; // bitwidth of the offset
	// bitsInpage = log2(bytesInPage);
	vector<TableEntry*> TableRecords;
	int clock;
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
int translateAddress(TableRep * t, int virtualAddress, bool hexFormat);


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

	// tableRep->printTable(cout);

	//now we need to read in from cin, from the file redirected in

	string address = "";
	int addr = -1;
	int physicalAddress = 0;
	bool hex;
	while(cin >> address)//each result should be a virtual address we need to interpret based off tableRep,
	{
		//int addrSize = address.length();
		addr = -1;
		physicalAddress = 0;

		if(address.length() > 1){
			//either hex or decimal addr

				if(address[1] == 'x'){ //expecting the "0x" in front of the word
					addr = stoi(address, nullptr, 16);
					hex = true;
		 		}else {
					addr = stoi(address);
					hex = false;
				}
		}else{
			//we know this is specifically a newline char OR a single digit decimal based address
			if(address[0] != '\n' || address != " ") {
				addr = stoi(address);
			}
		}
		cout << "Virtual Address: " << addr << endl;
		if(addr < 0) {
			cout << "invalid address" << endl;
			return 0;
		}
		// process input
		physicalAddress = translateAddress(tableRep, addr, hex);
		//cout << "phsyical address: " << physicalAddress << endl << endl;
	}

	return 0;
}

// parseVirtAddress(int address, bool hex, TableRep * & T){}
// should return the phsyical address or 0(if permission is )

//should probably use bit shifts maybe shift off the bits of the page to determine value in addr.

int translateAddress(TableRep * table, int virtualAddress, bool hexFormat) {
		// calculate VPN
		// index the table
		// offset to find specific address within PPN
		// return PPN


		int mask = pow(2, table->getVirtBits()) - 1; // mask of 1s
		// cout << "mask: " << mask << endl;

		int maskedVirtualAddress  = virtualAddress &= mask;//on purpose

		// cout << "virtual address: " << maskedVirtualAddress << endl; // off by 1. 48 instead of 96
		int VPN = maskedVirtualAddress >> table->getOffSet();
		// cout << "VPN: " << VPN << endl;
		int PPN = table->getTableRecords()[VPN]->physicalPageNumber;
		// cout << "Respective PPN: " << PPN << endl;
		int physicalAddress = PPN << table->getOffSet();

		// we need to get the actual offset value and  =|
		// it with the shifted ppn (stored in var physicalAddress)


		mask = pow(2, table->getOffSet()) - 1;
		int offsetValue = virtualAddress & mask;


		physicalAddress |= offsetValue;


		if(table->getTableRecords()[VPN]->permission){
			if(table->getTableRecords()[VPN]->valid){

				if(hexFormat){
						//print out in hex
					cout << "printing hex address" << endl;
					cout << "Physical Address: 0x" << hex << physicalAddress << endl;
				}else{
					cout << "Physical Address: " << physicalAddress << endl;
				}
			}else{

				#ifdef PROB1
				//preprocessor directive Prob1
				cout << "DISK" << endl;
				#else
				//preprocessor directive Prob2
				table->pageReplacement(VPN);
				cout << "finished page replacement" << endl;

				return translateAddress(table, virtualAddress, hexFormat);
				#endif
			}
		}else{
			cout << "SEGFAULT" << endl;
		}
		return physicalAddress;
}

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
