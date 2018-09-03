#include <iostream>
#include <vector>
#include "Compress.cpp"

int main()
{
    string inputPath = "/Users/yunsu/CLionProjects/DailyProgramming/Compressor/text.txt";
    string compressPath = "/Users/yunsu/CLionProjects/DailyProgramming/Compressor/compressed.bin";
    string decompressPath = "/Users/yunsu/CLionProjects/DailyProgramming/Compressor/decompress.txt";
    Huffman coding = Huffman(inputPath, compressPath, decompressPath);
    coding.countText();

    // print frequency
    vpii :: iterator iterV;
    int count = 0;

    coding.detail();
    coding.makeTree();
    coding.makeDictionary();
    coding.showDict();
    cout << coding.text;
    coding.compress();
    coding.deCompress();
}