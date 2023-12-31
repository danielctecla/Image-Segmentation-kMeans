#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include "struct.h"

#define endl "\n"

using namespace std;

void readImgData(int &sizeV, int &sizeH, string &img_name){
    string temp_name;
    img_name = "../images/";
    cout<<"Image name: ";
    cin>>temp_name;
    img_name += temp_name;
    img_name += ".bmp";

    cout<<"Image size: ";
    cin>>sizeV>>sizeH;
}

void createMatrixRGB(vector<vector<bitRGB>> &matrixRGB, vector<char> &buff, int sizeV, int sizeH){
    //create a matrix to store the image data
    for(int i = 0, pointer = 54; i < matrixRGB.size() ; i++){
        for(int j = 0; j < matrixRGB[0].size() ; j++,pointer += 3){
            matrixRGB[i][j].ble = static_cast<int>(static_cast<unsigned char>(buff[pointer]));
            matrixRGB[i][j].grn = static_cast<int>(static_cast<unsigned char>(buff[pointer + 1]));
            matrixRGB[i][j].red = static_cast<int>(static_cast<unsigned char>(buff[pointer + 2]));

        }
    }
}

void exportImg(vector<vector<bitRGB>> &matrixRGB, vector<char> &buff, streampos &sizeF){
    int pointer = 54;
    char* new_image = new char[sizeF];
    for(int i = 0; i < pointer ; i++) new_image[i] = buff[i];

    for(int i = 0; i < matrixRGB.size() ; i++){
        for(int j = 0; j < matrixRGB[0].size() ; j++, pointer += 3){
            new_image[pointer] = static_cast<char>(matrixRGB[i][j].ble);
            new_image[pointer + 1] = static_cast<char>(matrixRGB[i][j].grn);
            new_image[pointer + 2] = static_cast<char>(matrixRGB[i][j].red);
        }
    }
    
    ofstream outputFile("output.bmp", ios::binary);
    outputFile.write(new_image, sizeF);
    outputFile.close();

    delete[] new_image;

}