#include <iostream>
#include <fstream>

int bytes = 0;
int drops = 0;

void write(int c, std::ofstream &ofs){
	bytes++;
	ofs.write((const char*)(&c), sizeof(char));
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
	
	std::cout << "Compile " << bytes << " bytes, " << "Drop " << drops << " bytes" << std::endl;
}