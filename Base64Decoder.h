/*
 * MIT License
 * 
 * Copyright (c) 2020 devrangel
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
*/

#ifndef _BASE64DECODER_H_
#define _BASE64DECODER_H_

#include <iostream>
#include <bitset>
#include <string>
#include <array>
#include <unordered_map>
#include <vector>

namespace Base64Tools
{
    const std::unordered_map<unsigned char, std::string> table = {
    {'A', "000000"}, {'B', "000001"}, {'C', "000010"}, {'D', "000011"}, 
    {'E', "000100"}, {'F', "000101"}, {'G', "000110"}, {'H', "000111"}, 
    {'I', "001000"}, {'J', "001001"}, {'K', "001010"}, {'L', "001011"}, 
    {'M', "001100"}, {'N', "001101"}, {'O', "001110"}, {'P', "001111"}, 
    {'Q', "010000"}, {'R', "010001"}, {'S', "010010"}, {'T', "010011"}, 
    {'U', "010100"}, {'V', "010101"}, {'W', "010110"}, {'X', "010111"}, 
    {'Y', "011000"}, {'Z', "011001"},
    {'a', "011010"}, {'b', "011011"}, {'c', "011100"}, {'d', "011101"}, 
    {'e', "011110"}, {'f', "011111"}, {'g', "100000"}, {'h', "100001"}, 
    {'i', "100010"}, {'j', "100011"}, {'k', "100100"}, {'l', "100101"}, 
    {'m', "100110"}, {'n', "100111"}, {'o', "101000"}, {'p', "101001"}, 
    {'q', "101010"}, {'r', "101011"}, {'s', "101100"}, {'t', "101101"}, 
    {'u', "101110"}, {'v', "101111"}, {'w', "110000"}, {'x', "110001"}, 
    {'y', "110010"}, {'z', "110011"},
    {'0', "110100"}, {'1', "110101"}, {'2', "110110"}, {'3', "110111"}, 
    {'4', "111000"}, {'5', "111001"}, {'6', "111010"}, {'7', "111011"}, 
    {'8', "111100"}, {'9', "111101"}, {'+', "111110"}, {'/', "111111"}
    };

    // Base64 Decoder
    std::vector<unsigned char> Decoder(std::string encodedStr)
    {
        std::string binaryCode;
        std::vector<unsigned char> decodedVec;

        unsigned int counterByte = 0;
        unsigned int counterStr = 0;

        unsigned int lengthDivided = encodedStr.size() / 4;
        unsigned int counter = 0;

        while(counterStr < encodedStr.size())
        {
            if((encodedStr.size() % 4) == 0)
            {
                if(encodedStr.at(encodedStr.size() - 1) != '=' && encodedStr.at(encodedStr.size() - 2) != '=')
                {
                    // The encodedStr variable holds the complete sequence of characters
                    // Each subString variable holds 24 bits
                    // Each character of the subString is parsed into to the corresponding binary value in Base64
                    std::string subString = encodedStr.substr(counterStr, 4);
                    for(const unsigned char &value : subString)
                    {
                        auto valueFound = table.find(value);
                        binaryCode.append(valueFound->second);
                    }

                    // Condition: i < 3
                    // Each binaryCode variable has 24 bits
                    // So, it needs to be divided into 3 groups of 8 bits
                    for(unsigned int i = 0; i < 3; i++)
                    {
                        std::bitset<8> byteDecoded(binaryCode.substr(counterByte, 8));
                        decodedVec.push_back(byteDecoded.to_ulong());
                        counterByte += 8;
                    }

                    binaryCode = "";
                    counterStr += 4;
                    counterByte = 0;
                }
                else if(encodedStr.at(encodedStr.size() - 1) == '=' && encodedStr.at(encodedStr.size() - 2) != '=')
                {
                    // The counter < lengthDivided - 1 is used for checking if the current chunck of bits (8 bits) is the last one
                    // If so, the '=' character must be deleted.
                    if(counter < lengthDivided - 1)
                    {
                        std::string subString = encodedStr.substr(counterStr, 4);
                        for(const unsigned char &value : subString)
                        {
                            auto valueFound = table.find(value);
                            binaryCode.append(valueFound->second);
                        }

                        for(unsigned int i = 0; i < 3; i++)
                        {
                            std::bitset<8> byteDecoded(binaryCode.substr(counterByte, 8));
                            decodedVec.push_back(byteDecoded.to_ulong());
                            counterByte += 8;
                        }

                        binaryCode = "";
                        counterStr += 4;
                        counterByte = 0;
                        counter++;
                    }
                    else
                    {
                        std::string subString = encodedStr.substr(counterStr, 4);
                        subString.pop_back();

                        for(const unsigned char &value : subString)
                        {
                            auto valueFound = table.find(value);
                            binaryCode.append(valueFound->second);
                        }

                        // Condition: i < 2
                        // Each binaryCode variable has 18 bits
                        // In this case, the padding character '=' was deleted remaining 18 bits
                        // So, it needs to be divided into 2 groups of 8 bits
                        // The last two bits are discarded
                        for(unsigned int i = 0; i < 2; i++)
                        {
                            std::bitset<8> byteDecoded(binaryCode.substr(counterByte, 8));
                            decodedVec.push_back(byteDecoded.to_ulong());
                            counterByte += 8;
                        }

                        binaryCode = "";
                        counterStr += 4;
                        counterByte = 0;
                    }
                }
                else if(encodedStr.at(encodedStr.size() - 1) == '=' && encodedStr.at(encodedStr.size() - 2) == '=')
                {
                    if(counter < lengthDivided - 1)
                    {
                        std::string subString = encodedStr.substr(counterStr, 4);
                        for(const unsigned char &value : subString)
                        {
                            auto valueFound = table.find(value);
                            binaryCode.append(valueFound->second);
                        }

                        for(unsigned int i = 0; i < 3; i++)
                        {
                            std::bitset<8> byteDecoded(binaryCode.substr(counterByte, 8));
                            decodedVec.push_back(byteDecoded.to_ulong());
                            counterByte += 8;
                        }

                        binaryCode = "";
                        counterStr += 4;
                        counterByte = 0;
                        counter++;
                    }
                    else
                    {
                        std::string subString = encodedStr.substr(counterStr, 4);
                        subString.pop_back();
                        subString.pop_back();

                        for(const unsigned char &value : subString)
                        {
                            auto valueFound = table.find(value);
                            binaryCode.append(valueFound->second);
                        }

                        // Condition: i < 1
                        // Each binaryCode variable has 12 bits
                        // In this case, all padding characters '=' was deleted remaining 12 bits
                        // So, it needs to be divided into 1 groups of 8 bits
                        // The last four bits are discarded
                        for(unsigned int i = 0; i < 1; i++)
                        {
                            std::bitset<8> byteDecoded(binaryCode.substr(counterByte, 8));
                            decodedVec.push_back(byteDecoded.to_ulong());
                            counterByte += 8;
                        }

                        binaryCode = "";
                        counterStr += 4;
                        counterByte = 0;
                    }
                }
            }
            else
            {
                std::cout << "ERROR - Encoded string may be corrupted" << std::endl;
                counterStr = encodedStr.size() + 1;
            }
        }

        return decodedVec;
    }
}

#endif //_BASE64DECODER_H_