#include <bits/stdc++.h>

using namespace std;

//-----------------------STRUCTS-----------------------
struct bitRGB{
    int red, green, blue;
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

void writeRGBtoFile(vector<bitRGB> &vectRGB, ofstream &NewImg);

//-----------------------MAIN-----------------------
int main() {
    int sizeV,sizeH, numClust;
    sizeV = sizeH = 512;

    vector<bitRGB> vectRGB(sizeV*sizeH);
    ifstream file ("input_test.bmp", ios::binary );
    
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

        writeRGBtoFile(vectRGB, NewImg);
        
        file.close();
    }else cout << "Unable to open file" << endl;

    return 0;
}

void vectorRGB(vector<bitRGB> &vectRGB, vector<char> &buff,int sizeV, int sizeH){
    vectRGB.resize(sizeV * sizeH);

    int i = 0;

    for(int header = 54; i < sizeV*sizeH; i++, header+=3){
        vectRGB[i].red = static_cast<int>(static_cast<unsigned char>(buff[header]));
        vectRGB[i].green = static_cast<int>(static_cast<unsigned char>(buff[header + 1]));
        vectRGB[i].blue = static_cast<int>(static_cast<unsigned char>(buff[header + 2]));
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
    uniform_int_distribution<> distrib(0, vectRGB.size() - 1);
    int randomIndex = distrib(gen);
    cout << "First centroid: " << randomIndex << endl;
    centroid c = {static_cast<float>(vectRGB[randomIndex].red),static_cast<float>(vectRGB[randomIndex].green),static_cast<float>(vectRGB[randomIndex].blue)};
    centroids.push_back(c); 
    
    while(centroids.size()<numClust){
        vector<float> dist(vectRGB.size(),0);
        
        for(int i = 0; i < vectRGB.size(); i++){
            float minDist = FLT_MAX;
            for(auto &ctroid: centroids){
                float d = distance(vectRGB[i], ctroid);
                if(d < minDist){
                    minDist = d;
                }
            }
            dist[i] = minDist;
        }

        discrete_distribution<> weightedDist(dist.begin(), dist.end());
        randomIndex = weightedDist(gen);
        centroid c = {static_cast<float>(vectRGB[randomIndex].red),static_cast<float>(vectRGB[randomIndex].green),static_cast<float>(vectRGB[randomIndex].blue)};
        centroids.push_back(c);
    }
    
    for(int i = 0;i<centroids.size();i++){
        cout << "Centroid " << i << ": " << centroids[i].x << " " << centroids[i].y << " " << centroids[i].z << endl;
    }
}

void writeRGBtoFile(vector<bitRGB> &vectRGB, ofstream &NewImg) {
    for (const auto &pixel : vectRGB) {
        NewImg.put(static_cast<char>(pixel.red));
        NewImg.put(static_cast<char>(pixel.green));
        NewImg.put(static_cast<char>(pixel.blue));
    }
}