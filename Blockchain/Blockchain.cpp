#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>

class Block
{
private:
    std::string hash;
    std::string previousHash;
    std::tm time;
    std::string data;

    std::string getHash()
    {
        return hash;
    }

    void createHash()
    {
        std::hash<std::string> hasher;
        std::string allString = previousHash + std::to_string(time.tm_year + 1900) + std::to_string(time.tm_mon + 1) + std::to_string(time.tm_mday) + std::to_string(time.tm_hour) + std::to_string(time.tm_min) + std::to_string(time.tm_sec) + data;
        hash = int_to_hex(hasher(allString));
    }

    std::string int_to_hex(int number) {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << number;
        std::cout << std::hex << std::endl;
        return ss.str();
    }


public:

    Block(std::string data, Block* previousBlock = 0) : data(data)
    {

        if (previousBlock == nullptr)
        {
            previousHash = "0";
        }
        else
        {
            previousHash = previousBlock->getHash();
        }

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        localtime_s(&time, &currentTime);
        createHash();
    }

    ~Block() {}

    void print()
    {
        std::cout << "hash : " << hash << std::endl;
        std::cout << "previous hash : " << previousHash << std::endl;
        std::cout << "Date actuelle : "
            << std::to_string(time.tm_year + 1900) << '-'
            << std::to_string(time.tm_mon + 1) << '-'
            << std::to_string(time.tm_mday) << " "
            << std::to_string(time.tm_hour) << ':'
            << std::to_string(time.tm_min) << ':'
            << std::to_string(time.tm_sec)
            << std::endl;
        std::cout << "donnee : " << data << std::endl;
    }

};

int main()
{
    std::vector<Block*> blockchain;
    blockchain.push_back(new Block("je suis basil"));
    blockchain[0]->print();



    return 0;
}