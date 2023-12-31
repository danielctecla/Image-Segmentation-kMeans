#include <bits/stdc++.h>

#define endl "\n"

using namespace std;

//-----------------------STRUCTS-----------------------
struct bitRGB{
    int red, green, blue;
    int cluster;
    double dist_c;
};

struct centroid{
    double x, y, z;
};

struct sumCluster{
    long double x, y, z;
    int numPixels;
};
//-----------------------FUNCTIONS-----------------------

void vectorRGB(vector<bitRGB> &vectRGB, vector<char> &buff , int sizeV, int sizeH);
void kMeansPlusPlusInit(vector<bitRGB> &vectRGB, vector<centroid> &centroids, int &numClust);
double distance(bitRGB &pRGB, centroid &centr);

double distanceC(centroid &centrOld, centroid &centrNew);
bool converged(vector<centroid> &previousCentroids, vector<centroid> &centroids, double epsilon);
void convergeCentroids(vector<bitRGB> &vectRGB, vector<centroid> &centroids);

void writeRGBtoFile(vector<bitRGB> &vectRGB, vector<centroid> &centroids, vector<char> &buff);

//-----------------------MAIN-----------------------
int main() {
    int sizeV,sizeH, numClust;
    cout<<"Image size: ";
    cin>>sizeV>>sizeH;
    string name;
    cout<<"Image name: ";
    cin>>name;
    name += ".bmp";
    vector<bitRGB> vectRGB(sizeV*sizeH);
    ifstream file (name, ios::binary );
    
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
        convergeCentroids(vectRGB, centroids);

        //-----------------------Export IMG-----------------------
        //create a new file to store the new image
        
        writeRGBtoFile(vectRGB, centroids, buffer);

        ofstream NewImg("output.bmp", ios::binary);
        NewImg.write(buffer.data(), sizeF);
        file.close();
    }else cout << "Unable to open file" << endl;

    return 0;
}

void vectorRGB(vector<bitRGB> &vectRGB, vector<char> &buff,int sizeV, int sizeH){
    //vectRGB.resize(sizeV * sizeH);

    int i = 0;

    for(int header = 54; i < sizeV*sizeH; i++, header+=3){
        vectRGB[i].blue = static_cast<int>(static_cast<unsigned char>(buff[header]));
        vectRGB[i].green = static_cast<int>(static_cast<unsigned char>(buff[header + 1]));
        vectRGB[i].red = static_cast<int>(static_cast<unsigned char>(buff[header + 2]));
    }
}

double distance(bitRGB &pRGB, centroid &centr){
    double dx = pRGB.red - centr.x;
    double dy = pRGB.green - centr.y;
    double dz = pRGB.blue - centr.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

void kMeansPlusPlusInit(vector<bitRGB> &vectRGB, vector<centroid> &centroids, int &numClust){
    //first centroid - random initialization
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, vectRGB.size() - 1);
    int randomIndex = distrib(gen);
    cout << "First centroid: " << randomIndex << endl;
    centroid c = {static_cast<double>(vectRGB[randomIndex].blue),
                static_cast<double>(vectRGB[randomIndex].green),
                static_cast<double>(vectRGB[randomIndex].red)};
    centroids.push_back(c); 
    
    while(centroids.size()<numClust){
        vector<double> dist(vectRGB.size(),0);
        
        //calculate the distance of each pixel from the previous centroids
        for(int i = 0; i < vectRGB.size(); i++){
            double minDist = 1e9;
            for(auto &ctroid: centroids){
                double d = distance(vectRGB[i], ctroid);
                if(d < minDist){
                    minDist = d;
                }
            }
            dist[i] = minDist;
        }

        //select the next centroid with probability proportional to the distance from the previous centroids
        discrete_distribution<> weightedDist(dist.begin(), dist.end());
        randomIndex = weightedDist(gen);
        centroid c = {static_cast<double>(vectRGB[randomIndex].blue),
                    static_cast<double>(vectRGB[randomIndex].green),
                    static_cast<double>(vectRGB[randomIndex].red)};
        centroids.push_back(c);
    }
    
    for(int i = 0;i<centroids.size();i++){
        cout << "Centroid " << i << ": " << centroids[i].x << " " << centroids[i].y << " " << centroids[i].z << endl;
    }
}

double distanceC(centroid &centrOld, centroid &centrNew){
    double dx = centrOld.x - centrNew.x;
    double dy = centrOld.y - centrNew.y;
    double dz = centrOld.z - centrNew.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}

bool converged(vector<centroid> &previousCentroids, vector<centroid> &centroids, double epsilon){
    //check if the centroids have converged
    for(int i = 0; i < centroids.size(); i++){
        if(distanceC(previousCentroids[i], centroids[i]) > epsilon) return false; 
    }

    //return true if the centroids have converged
    return true;
}

void convergeCentroids(vector<bitRGB> &vectRGB, vector<centroid> &centroids){
    double epsilon = 0.01;
    int iteration = 0;
    vector<centroid> previousCentroids(centroids.size(), {0,0,0});
    
    cout << "Starting K-Means++" << endl;
    vector<sumCluster> sumC(centroids.size(), {0,0,0,0});
    while(!converged(previousCentroids, centroids, epsilon) && iteration < 10){
        previousCentroids = centroids;
        for(int i=0 ; i < vectRGB.size(); i++){
            int xB = vectRGB[i].blue;
            int yG = vectRGB[i].green;
            int zR = vectRGB[i].red;
            double minDist = 1e9;
            for(int j=0; j < centroids.size(); j++){
                double d = distance(vectRGB[i], centroids[j]);
                if(d < minDist){
                    minDist = d;
                    vectRGB[i].cluster = j;
                    vectRGB[i].dist_c = d;
                    minDist = d;
                }
            }
            sumC[vectRGB[i].cluster].x += xB;
            sumC[vectRGB[i].cluster].y += yG;
            sumC[vectRGB[i].cluster].z += zR;
            sumC[vectRGB[i].cluster].numPixels++;
        }

        for(int i=0; i < centroids.size(); i++){
            if(sumC[i].numPixels != 0){
                centroids[i].x = sumC[i].x / sumC[i].numPixels;
                centroids[i].y = sumC[i].y / sumC[i].numPixels;
                centroids[i].z = sumC[i].z / sumC[i].numPixels;
            }
        }

        cout << "\nIteration " << iteration++ << endl;
        for (int i = 0; i < sumC.size(); i++) cout<< "Cluster " << i << ": " << centroids[i].x << " " << centroids[i].y << " " << centroids[i].z << " " << sumC[i].numPixels << endl;
        

        for(int i=0; i < centroids.size(); i++){
            if(sumC[i].numPixels != 0){
                sumC[i].x = 0;
                sumC[i].y = 0;
                sumC[i].z = 0;
                sumC[i].numPixels = 0;
            }
        }
    }
    cout << "\nFinal centroids" << endl;
    for(int i = 0;i<centroids.size();i++)   cout << "Centroid " << i << ": " << centroids[i].x << " " << centroids[i].y << " " << centroids[i].z << endl;
}

void writeRGBtoFile(vector<bitRGB> &vectRGB, vector<centroid> &centroids, vector<char> &buff) {
    for(int i = 54, j = 0; j < vectRGB.size(); i+=3, j++){
        buff[i] = static_cast<int>(centroids[vectRGB[j].cluster].x);
        buff[i+1] = static_cast<int>(centroids[vectRGB[j].cluster].y);
        buff[i+2] = static_cast<int>(centroids[vectRGB[j].cluster].z);
    }
}