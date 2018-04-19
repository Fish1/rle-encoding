#include <iostream>

#include <string>

#include <fstream>

void decompress(std::string source, std::string destination);

void compress(std::string source, std::string destination);

void createFile(std::string name, unsigned int bytes);

void error(std::string message, int code);

int main(int argc, char ** argv)
{
	srand(time(NULL));

	if(argc != 4)
	{
		error("incorrect argument count", 1);
	}

	if(std::string(argv[1]).compare("-c") == 0)
	{
		compress(argv[2], argv[3]);
	}
	else if(std::string(argv[1]).compare("-d") == 0)
	{
		decompress(argv[2], argv[3]);
	}
	else if(std::string(argv[1]).compare("-cf") == 0)
	{
		createFile(argv[2], std::stoi(argv[3]));
	}
	else
	{
		error("incorrect flags", 2);
	}

	return 0;
}

void decompress(std::string source, std::string destination)
{
	std::cout << "decompressing file " << source << "..." << std::endl;
	
	std::ifstream input(source, std::ifstream::binary);
	input.seekg(0, std::ifstream::end);
	size_t size = input.tellg();
	input.seekg(0, std::ifstream::beg);

	uint8_t data[size];
	input.read((char*)data, size);

	std::ofstream output(destination, std::ofstream::binary);

	for(size_t index = 0; index < size; ++index)
	{
		uint16_t count = data[index] | (data[index + 1] << 8);
		uint8_t byte = data[index + 2];

		index += 2;

		for(uint16_t byteCount = 0; byteCount < count; ++byteCount)
		{
			output.write((char*)&byte, sizeof(byte));
		}
	}

	input.close();
	output.close();
}

void compress(std::string source, std::string destination)
{
	std::cout << "compressing file " << source << "..." << std::endl;

	std::ifstream input(source, std::ifstream::binary);
	input.seekg(0, std::ifstream::end);
	size_t size = input.tellg();
	input.seekg(0, std::ifstream::beg);

	uint8_t data[size];
	input.read((char*)data, size);

	std::ofstream output(destination, std::ofstream::binary);

	uint8_t byte = data[0];
	uint8_t last_byte = data[0];
	uint16_t count = 0;

	for(size_t index = 0; index < size; ++index)
	{
		byte = data[index];

		if(byte != last_byte)
		{
			output.write((char*)&count, sizeof(uint16_t));
			output.write((char*)&last_byte, sizeof(uint8_t));
			count = 0;
		}

		last_byte = byte;
		++count;
	}

	output.write((char*)&count, sizeof(uint16_t));
	output.write((char*)&last_byte, sizeof(uint8_t));

	input.close();
	output.close();
}

void createFile(std::string name, unsigned int bytes)
{
	std::ofstream output(name);

	uint8_t c = 32 + (rand() % 94);

	for(unsigned int index = 0; index < bytes; ++index)
	{
		if(index > 10 && (rand() % 5) == 0)
		{
			c = 32 + (rand() % 94);
		}

		output.write((char*)&c, sizeof(c));
	}

	output.close();
}

void error(std::string message, int code)
{
	std::cout << "Error: " << message << std::endl;

	exit(code);
}
