#include <bits/stdc++.h>

using namespace std;

//-----------------------STRUCTS-----------------------
struct bitRGB{
    unsigned int red, green, blue;
    int cluster;
    float dist_c;
};

struct centroid{
    float x, y, z;
};

//-----------------------FUNCTIONS-----------------------

void vectorRGB(vector<bitRGB> &vectRGB, vector<char> &buff , int sizeV, int sizeH);
void kMeansPlusPlusInit(vector<bitRGB> &vectRGB, vector<centroid> &centroids, int &numClust);
int distance(bitRGB &pRGB, centroid &centr);

//-----------------------MAIN-----------------------
int main() {
    int sizeV,sizeH, numClust;
    sizeV = sizeH = 16;

    vector<bitRGB> vectRGB(sizeV*sizeH);
    ifstream file ("input.bmp", ios::binary );
    
    //check if the .bmp file is open
    if(file.is_open()){

        //get the size of the file
        file.seekg(0, ios::end);
        streampos sizeF = file.tellg();
        file.seekg(0, ios::beg);

        //create a buffer to store the file
        vector<char> buffer(sizeF);
        file.read(buffer.data(), sizeF);
        
        if (file.fail()) {
            cout << "Error reading file" << endl;
            return -1;
        }

        vectorRGB(vectRGB, buffer, sizeV, sizeH);
        //-----------------------Clustering-----------------------
        cout << "Number of clusters: ";
        cin >> numClust;
        vector<centroid> centroids;

        kMeansPlusPlusInit(vectRGB, centroids, numClust);



        //-----------------------Export IMG-----------------------
        //create a new file to store the new image
        ofstream NewImg("output.bmp", ios::binary);
        NewImg.write(buffer.data(), 54);

        for(auto pRGB: vectRGB){
            NewImg.put(pRGB.red);
            NewImg.put(pRGB.green);
            NewImg.put(pRGB.blue);
        }
        
        file.close();
    }else cout << "Unable to open file" << endl;

    return 0;
}

void vectorRGB(vector<bitRGB> &vectRGB, vector<char> &buff,int sizeV, int sizeH){
    int i = 0;

    for(int header = 54; i < sizeV*sizeH; i++, header+=3){
        vectRGB[i].red = buff[header];
        vectRGB[i].green = buff[header + 1];
        vectRGB[i].blue = buff[header + 2];
        vectRGB[i].dist_c = FLT_MAX;
    }
}

int distance(bitRGB &pRGB, centroid &centr){
    //distance in R3
    return sqrt(pow(pRGB.red - centr.x, 2) + pow(pRGB.green - centr.y, 2) + pow(pRGB.blue - centr.z, 2));
}

void kMeansPlusPlusInit(vector<bitRGB> &vectRGB, vector<centroid> &centroids, int &numClust){
    //first centroid - random initialization
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);
    centroid c;
    c.x = distrib(gen);
    c.y = distrib(gen);
    c.z = distrib(gen);
    centroids.push_back(c);

    int kClusters = 1;
    vector<pair<float,vector<bitRGB>>> dist(vectRGB.size());
    
    // sort(dist.begin(), dist.end(), [](const pair<float,vector<bitRGB>>& a, const pair<float,vector<bitRGB>>& b) {
    //     return a.first > b.first;
    // });

    while(kClusters < numClust){
        //calculate the distance of each point to the nearest centroid
        
    }
    
    
    
    
    
}