#include <Windows.h>
#include <iostream>
#include <ostream>

unsigned int getDiskBlockSize() {
    DWORD sectorsPerCluster, bytesPerSector, freeClusters, totalClusters;

    // Convert narrow-character string to wide-character string

    if (GetDiskFreeSpace(".", &sectorsPerCluster, &bytesPerSector, &freeClusters, &totalClusters)) {
        return bytesPerSector * sectorsPerCluster;
    } else {
        // Handle error
        return -1;
    }

}

int main(){
    unsigned int blockSize = getDiskBlockSize();
    if (blockSize != -1) {
        std::cout << "Tamaño del bloque de disco: " << blockSize << " bytes" << std::endl;
    } else {
        std::cerr << "Error al obtener el tamaño del bloque de disco." << std::endl;
    }
}