#ifndef MEMORYMANAGER_HPP
#define MEMORYMANAGER_HPP

class MemoryManager
{
    unsigned int currentAddress;
    //vector<unsigned int> memory;
    unsigned int nextAvailableAddress;
public:
    MemoryManager():currentAddress(0),nextAvailableAddress(0) {}
    const unsigned int getCurrentAddress() { return nextAvailableAddress++; }

};

#endif // MEMORYMANAGER_HPP
