#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <iomanip>

class Block
{
private:
    int index;
    int nonce = 0;
    std::string hash;
    std::string previousHash;
    std::tm time;
    std::string data;

    int getIndex()
    {
        return index;
    }

    std::string calculateHash()
    {
        std::hash<std::string> hasher;
        std::string allString = std::to_string(nonce) + previousHash + std::to_string(time.tm_year + 1900) + std::to_string(time.tm_mon + 1) + std::to_string(time.tm_mday) + std::to_string(time.tm_hour) + std::to_string(time.tm_min) + std::to_string(time.tm_sec) + data;
        return int_to_hex(hasher(allString));
    }

    std::string int_to_hex(int number) {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << number;
        return ss.str();
    }


public:

    Block(std::string data, Block* previousBlock) : data(data)
    {

        if (previousBlock == nullptr)
        {
            previousHash = "0";
            index = 0;
        }
        else
        {
            previousHash = previousBlock->getHash();
            index = previousBlock->getIndex()+1;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        localtime_s(&time, &currentTime);
        hash = calculateHash();
    }

    ~Block() 
    {
        std::cout << "block " << hash << " supprime" << std::endl << std::endl;
    }

    void print()
    {
        std::cout << "Bloc " << index << ": ";
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

    void update(Block* previousBlock)
    {
        if (previousBlock == nullptr)
        {
            index = 0;
            previousHash = "0";
        }
        else
        {
            previousHash = previousBlock->getHash();
            index = previousBlock->getIndex() + 1;
        }

        auto now = std::chrono::system_clock::now();
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
        localtime_s(&time, &currentTime);
    }

    std::string mine(int difficulty)
    {
        std::string target(difficulty, 'f');
        std::string newHash;
        do 
        {
            nonce++;
            newHash = calculateHash();
        } while (newHash.substr(0, difficulty) != target);

        std::cout << "Bloc mine : " << hash << std::endl;
        nonce = 0;
        return newHash;
    }

    void setData(std::string newData)
    {
        data = newData;
    }

    void setHash(std::string newHash)
    {
        hash = newHash;
    }

    std::string getPreviousHash()
    {
        return previousHash;
    }

    std::string getHash()
    {
        return hash;
    }

    void changePreviousHash(std::string newPreviousHash)
    {
        previousHash = newPreviousHash;
    }
};

class Blockchain
{
public:
    static Blockchain& getInstance() 
    {
        static Blockchain instance;
        return instance;
    }

    void addBlock(std::string data) 
    {
        int index = blocks.size();
        Block* previousHash = (index == 0) ? nullptr : blocks[index - 1];
        Block* newBlock = new Block(data, previousHash);
        newBlock->setHash(newBlock->mine(difficulty));
        blocks.emplace_back(newBlock);
        std::cout << "Bloc " << index << " ajoute : " << data << std::endl;

    }

    void delBlock(int index)
    {
        if (index == blocks.size())
        {
            blocks[index+1]->changePreviousHash(blocks[index]->getPreviousHash());
        }

        delete blocks[index];
        blocks.erase(blocks.begin() + index);

        for (int i = index; i < blocks.size(); i++)
        {
            Block* previousBlock = nullptr;
            if (i != 0)
            {
                previousBlock = blocks[i - 1];

            }
            blocks[i]->update(previousBlock);
            blocks[i]->setHash(blocks[i]->mine(difficulty));
        }
    }

    void printBlockchain() 
    {
        for (Block* &block : blocks) 
        {
            block->print();
        }
        std::cout << std::endl;
    }

    void corrompreBlock(int index, std::string newData)
    {
        blocks[index]->setData(newData);
    }

    bool verificationIntegrity()
    {
        for (int i = 1; i < blocks.size(); i++)
        {
            if (blocks[i]->getHash() != blocks[i]->mine(difficulty))
            {
                
                return false;
            }
            if (blocks[i-1]->getHash() != blocks[i]->getPreviousHash())
            {
                std::cout << blocks[i - 1]->getHash() << "   " << blocks[i]->getPreviousHash() << std::endl;
                return false;
            }
        }
        return true;
    }

    Blockchain(const Blockchain&) = delete;
    Blockchain& operator=(const Blockchain&) = delete;

private:
    Blockchain() {}

    ~Blockchain()
    {
        for (Block* &block : blocks)
        {
            delete block;
        }
    }

    std::vector<Block*> blocks;
    int difficulty = 3;
};


int main()
{
    Blockchain::getInstance().addBlock("test");
    Blockchain::getInstance().addBlock("baba");
    Blockchain::getInstance().addBlock("sisi");
    Blockchain::getInstance().addBlock("sisi");
    Blockchain::getInstance().addBlock("sisi");
    Blockchain::getInstance().printBlockchain();

    Blockchain::getInstance().delBlock(1);

    Blockchain::getInstance().printBlockchain();
    Blockchain::getInstance().corrompreBlock(3, "testqdf");
    std::cout << std::endl << std::endl << std::endl;
    if (Blockchain::getInstance().verificationIntegrity())
    {
        std::cout << "integre" << std::endl;
    }
    else
    {
        std::cout << "pas integre" << std::endl;
    }
    

    return 0;
}