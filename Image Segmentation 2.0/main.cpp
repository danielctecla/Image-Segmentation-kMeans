#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>

#include "../include/struct.h"
#include "../include/import.h"
#include "../include/kmeans.h"
#define endl "\n"

using namespace std;

void clearScreen();

int main(){
    clearScreen();

    int sizeV,sizeH, numClust;
    int type = 0, show = 0;
    string img_name;
    vector<cluster> centroids;
    readImgData(sizeV, sizeH, img_name);
    vector<vector<bitRGB>> matRGB(sizeV, vector<bitRGB>(sizeH));
    ifstream file (img_name, ios::binary );

        //check if the .bmp file is not open
    if(!file.is_open()){ cout<<"Error opening file"<<endl; return -1; }

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

        //clustering
    cout<<"   Clustering..."<<endl<<endl;
    kmeans_pp(matRGB, centroids, numClust, type);
    //showClustering(matRGB);
    kmeans(matRGB, centroids, type, show);

        //export the image
    exportImg(matRGB, buffer, sizeF);
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
 // char csv[]="data.csv";
    // FILE *pOutput = fopen(csv,"w");

    //     //print the matrix
    //     for(int i = 0; i < sizeV; i++){
    //         for(int j = 0; j < sizeH; j++){
    //             cout<<"("<<matRGB[i][j].ble<<", "<<matRGB[i][j].grn<<", "<<matRGB[i][j].red<<")"<<matRGB[i][j].min_dist<<" "<<matRGB[i][j].clust<<endl;
    //             fprintf(pOutput,"(%d,%d,%d)\n",matRGB[i][j].ble,matRGB[i][j].grn,matRGB[i][j].red);
    //         }
    //         cout<<endl;
    //     }

    //     pOutput = fopen(csv,"r");
    //     fclose(pOutput);