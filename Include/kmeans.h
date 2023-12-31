#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include "struct.h"
#include "config.h"

#define stc_cstD(x) static_cast<double>(x)
#define endl "\n"
#define mspace "\n\n"
#define hspace "\n\n\n"

const double precision = 1e-9;
int iterations = MAX_ITERARIONS;

using namespace std;

void ClustersInfo(vector<cluster> &centroids, int knd){
    if(knd) cout<<setw(12)<<"Centroid"<<setw(10)<<"Blue(x)"<<setw(10)<<"Green(y)"<<setw(10)<<"Red(z)"<<endl;
    else cout<<setw(12)<<"Centroid"<<setw(10)<<"Blue(x)"<<setw(10)<<"Green(y)"<<setw(10)<<"Red(z)"<<setw(15)<<"Total pixels"<<endl;
    for(int i = 0; i < centroids.size(); i++){
        if(knd) cout<<setw(12)<<i<<setw(10)<<centroids[i].b<<setw(10)<<centroids[i].g<<setw(10)<<centroids[i].r<<endl;
        else cout<<setw(12)<<i<<setw(10)<<centroids[i].b<<setw(10)<<centroids[i].g<<setw(10)<<centroids[i].r<<setw(15)<<centroids[i].numPixels<<endl;
    }
}

void header(){
    cout<<setw(12)<<"Centroid"
    <<setw(10)<<"Blue(x)"
    <<setw(10)<<"Green(y)"
    <<setw(10)<<"Red(z)"
    <<setw(15)<<"Total blue"
    <<setw(15)<<"Total green"
    <<setw(15)<<"Total red"
    <<setw(15)<<"Total pixels" << endl;
}

void info(cluster &c, int &k){
    cout<<setw(12)<<k
    <<setw(10)<<c.b
    <<setw(10)<<c.g
    <<setw(10)<<c.r
    <<setw(15)<<c.median.b
    <<setw(15)<<c.median.g
    <<setw(15)<<c.median.r
    <<setw(10)<<c.numPixels << endl;
}

void kmeans_pp(vector<vector<bitRGB>> &matRGB, vector<cluster> &centroids, int &numClust, int &type){
    vector<pair<int,int>> indices(matRGB.size()*matRGB[0].size(),{0,0});
    cluster c;
    int fNonZero;
    random_device rd;
    mt19937 gen(rd());

    cout<<" kmeans++"<<mspace;
    if(type){
        uniform_int_distribution<> distrib(0, matRGB.size());
        uniform_int_distribution<> distrib2(0, matRGB[0].size()-1);
        int i = distrib(gen);
        int j = distrib2(gen);
        c = {stc_cstD(matRGB[i][j].ble), stc_cstD(matRGB[i][j].grn), stc_cstD(matRGB[i][j].red)};
    }else{ c.b = 155; c.g = 155; c.r = 155; }
    centroids.push_back(c);
    // cout<<"first centroid: "<<centroids[0].b<<" "<<centroids[0].g<<" "<<centroids[0].r<<endl;

    while(centroids.size() <= numClust){
        vector<double> distance_distrib(matRGB.size()*matRGB[0].size(),0);
        int k = 0; fNonZero = 0;
        for(int i = 0; i < matRGB.size(); i++){
            for(int j = 0; j < matRGB[0].size(); j++, k++){
                for(int l = 0; l < centroids.size(); l++){ 
                    double d = matRGB[i][j].distance(centroids[l]); 
                    if(d < matRGB[i][j].min_dist){ matRGB[i][j].min_dist = d; matRGB[i][j].clust = l; }
                }
                if(matRGB[i][j].min_dist > precision) fNonZero = 1;
                distance_distrib[k] = matRGB[i][j].min_dist;
                if(!(centroids.size()-1)) indices[k] = {i,j};
                // cout<<"d = "<<matRGB[i][j].min_dist<<" ";
            }

            // cout<<endl;
        }

        if(!fNonZero){ cout<<"All distances are zero, only create "<<centroids.size()<<" centorids"<<endl; break; }

        discrete_distribution<> weightedDist(distance_distrib.begin(), distance_distrib.end());
        int rndI = weightedDist(gen);
        // cout<<"rndI: "<<rndI<<endl;
        c = {stc_cstD(matRGB[indices[rndI].first][indices[rndI].second].ble), 
             stc_cstD(matRGB[indices[rndI].first][indices[rndI].second].grn), 
             stc_cstD(matRGB[indices[rndI].first][indices[rndI].second].red)};
        centroids.push_back(c);
    }

    cout<<" Initial Centroids"<<endl;
    ClustersInfo(centroids,1);
}

void kmeans(vector<vector<bitRGB>> &matRGB, vector<cluster> &centroids, int &type, int &show){
    bool CentroidUpdated = true;

    if(show) cout<<hspace<<" kmeans"<<endl;
    while(CentroidUpdated && iterations--){
        CentroidUpdated = false;

        if(show){ cout << string(102, '-') << endl; cout<<" Iteration: "<<MAX_ITERARIONS - iterations<<endl; }
        for(int i = 0; i < matRGB.size(); i++){
            for(int j = 0; j < matRGB[0].size(); j++){
                for(int k = 0; k < centroids.size(); k++){
                    double d = matRGB[i][j].distance(centroids[k]);
                    if(d < matRGB[i][j].min_dist){ matRGB[i][j].min_dist = d; matRGB[i][j].clust = k; }
                }
                
                centroids[matRGB[i][j].clust].numPixels++;
                centroids[matRGB[i][j].clust].median.b += matRGB[i][j].ble;
                centroids[matRGB[i][j].clust].median.g += matRGB[i][j].grn;
                centroids[matRGB[i][j].clust].median.r += matRGB[i][j].red;
            }
        }

        if(show){ cout<<" Position centroids: "<<endl; ClustersInfo(centroids,0); cout<<endl<<" New Centroids: "<<endl; header();}
        vector<cluster> oldCentroids = centroids;
        for(int k = 0; k < centroids.size(); k++){
            if(centroids[k].numPixels > 0){
                centroids[k].b = centroids[k].median.b / centroids[k].numPixels;
                centroids[k].g = centroids[k].median.g / centroids[k].numPixels;
                centroids[k].r = centroids[k].median.r / centroids[k].numPixels;
            }

            if(centroids[k].oldCentroidDistance(oldCentroids[k]) > EPSILON) CentroidUpdated = true;
            if(show) info(centroids[k], k);
        }

        for(auto &ctroid: centroids){
                ctroid.numPixels = 0;
                ctroid.median = medianClust();
        }  
        
    }
    
    cout<<hspace<<" Final Centroids"<<endl;
    ClustersInfo(centroids,1);    
}

void showClustering(vector<vector<bitRGB>> &matRGB){
    for(int i = 0; i < matRGB.size(); i++){
        for(int j = 0; j < matRGB[0].size(); j++){
            cout<<matRGB[i][j].clust<<" ";
        }
        cout<<endl;
    }
}