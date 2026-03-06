#include <iostream>
#include <fstream>

#define IDX_BEG (7)

int bytes = 0;
int drops = 0;
char data = 0;
int idx = IDX_BEG;

void show_bit(char c){
	std::cout << "<<bits>>: ";
	for(int i = 7; i >= 0; i--){
		std::cout << std::dec << static_cast<int>((c >> i) & 1);
	}
	std::cout << std::endl;
}

void write(char c, std::ofstream &ofs){
	data |= (c << idx);
	idx--;
	if(idx == -1){
		ofs.write((const char*)(&data), sizeof(char));
		bytes++;
		data = 0;
		idx = IDX_BEG;
	}
}

void compile_char(char c, std::ofstream &ofs){
	switch(c){
		case '0':
			write(0, ofs);
			break;
		case '1':
			write(1, ofs);
			break;
		default:
			drops++;
	}
}

void complie_str(std::string &str, std::ofstream &ofs){
	decltype(str.size()) idx = 0;
	for(; idx < str.size(); idx++){
		if(str.at(idx) == ';'){
			return;
		}
		
		compile_char(str.at(idx), ofs);
	}
}

int main(int argc, char *argv[]){
	if(argc < 2){
			std::cout << "No file provided" << std::endl;
			return 1;
	}
	
	std::ifstream ifs(argv[1]);
	if(!ifs.is_open()){
		std::cout << "Failed to open " << argv[1] << std::endl;
		return 1;
	}
	
	std::string of(argv[1]);
	of.append(".bin");
	std::ofstream ofs(of, std::ios::binary);
	if(!ofs.is_open()){
		std::cout << "Failed to open " << of << std::endl;
		return 1;
	}
	
	while(ifs){
		std::string str;
		std::getline(ifs, str);
		complie_str(str, ofs);
	}
	
	ifs.close();
	ofs.close();
	
	if(idx != IDX_BEG){
		std::cout << "Warning! Incomplete byte at tail of " << argv[1] << std::endl;
	}
	
	std::cout << "Compile " << bytes << " bytes, " << "Drop " << drops << " bytes" << std::endl;
}