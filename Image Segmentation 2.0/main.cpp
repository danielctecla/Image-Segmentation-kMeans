#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>

#include "../include/struct.h"
#include "../include/import_export.h"
#include "../include/kmeans.h"
#define endl "\n"

using namespace std;

void clearScreen();

int main(){
    clearScreen();

    int sizeV,sizeH, numClust;
    int type = 0, show = 0, kernel = 3;
    string img_name;
    vector<cluster> centroids;
    readImgData(sizeV, sizeH, img_name);
    ifstream file (img_name, ios::binary );
    if(!file.is_open()){ cout<<"Error opening file"<<endl; return -1; } //check if the .bmp file is not open
    sizeImg(sizeV, sizeH, file);
    vector<vector<bitRGB>> matRGB(sizeV, vector<bitRGB>(sizeH));

        //get information about the image and read the data into a buffer
    file.seekg(0, ios::end); streampos sizeF = file.tellg();
    file.seekg(0, ios::beg);
    vector<char> buffer(sizeF);
    file.read(buffer.data(), sizeF);
    if (file.fail()){ cout << "Error reading file" << endl; return -1; }

    createMatrixRGB(matRGB, buffer, sizeV, sizeH);

        //get the number of clusters
    cout<<"Type: "; cin>>type;
    if(type){ cout<<"Number of clusters: "; cin>>numClust; } 
    else numClust = 3;

        //show the clustering process
    cout<<"Show clustering? (1/0): "; cin>>show;
    clearScreen();

        //box blur
    Box_Blur(matRGB, kernel);
    exportImg(matRGB, buffer, sizeF);

        //clustering
    cout<<"   Clustering..."<<endl<<endl;
    kmeans_pp(matRGB, centroids, numClust, type);
    //showClustering(matRGB);
    kmeans(matRGB, centroids, type, show);

        //export the image
    ClusteredImg(matRGB, buffer, sizeF, centroids);
    revomeBackground(matRGB, centroids, buffer, sizeF);
    file.close();
    
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
 // char csv[]="data.csv"