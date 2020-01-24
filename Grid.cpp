#include "Grid.h"

	
using namespace std;
using Dict=map<pair<int,int>,int>;
using Pair = pair<int,int>;

int distance(int x1, int y1, int x2, int y2){
    return ceil(100*sqrt((x1-x2)*(x1-x2)+(y1 -y2)*(y1-y2)));
};
Grid::Direction getDirection(pair<int,int> source, pair<int,int> target){
    if(target.first>source.first){
        if(target.second>source.second){
            return Grid::SE;
        }else if(target.second<source.second){
            return Grid::NE;
        }else{
            return Grid::E;
        }
    }else if(target.first<source.first){
        if(target.second>source.second){
            return Grid::SW;
        }else if(target.second<source.second){
            return Grid::NW;
        }else{
            return Grid::W;
        }

    }else{
        if(target.second>source.second){
            return Grid::S;
        }else if(target.second<source.second){
            return Grid::N;
        }else{
            // default
            return Grid::N;
        }
    }
};
void reconstruct_path(std::map<Pair,Pair> came_from,Pair endNode, std::vector<Grid::Direction> &path){
    
    if(came_from.count(endNode)){
        Pair source = came_from[endNode];
        // get parent move
        reconstruct_path(came_from, came_from[endNode],path);
        path.push_back(getDirection(source,endNode));
    }
};

Grid::Grid(int width, int height):width(width),
height(height)
{
    mapInfo = new Grid::Tile*[height];
    for (int i = 0; i < height; i++)
    {
        mapInfo[i] = new Grid::Tile[width]();
    }
    connectedCache = new int* [height*width];
    for (int i = 0; i < height*width; i++){
        connectedCache[i] = new int[height*width]();
    }
};
Grid::~Grid(){
    for(int i = 0; i < height; i++)
    {
        delete[] mapInfo[i];
    }
    for(int i = 0; i < height*width; i++)
    {
        delete[] connectedCache[i];
    }   
    delete[] connectedCache;

    delete[] mapInfo;
};
int Grid::getWidth() const{
    return width;
};
int Grid::getHeight() const{
    return height;
};
Grid::Tile Grid::getTile(int x, int y) const{
    return mapInfo[y][x];
};
void Grid::setTile(int x, int y, Grid::Tile tile){
    mapInfo[y][x] = tile;
};

bool Grid::isConnected(int size, int x1, int y1, int x2, int y2) const{    


    bool visted[width*height] = {0};
    return innerConnected(size, x1, y1,x2,y2,visted);

};

int Grid::findShortestPath(int size, int x1, int y1, int x2, int y2, vector<Direction> &path) const{

    if(x1==x2 && y1==y2){
        // start and end at the same point
        return 0;
    }
    Dict gScore, hScore,fScore;
    // init g h f score 
    gScore[make_pair(x1,x2)] = 0;
    hScore[make_pair(x1,x2)] = distance(x1,y1,x2,y2);
    fScore[make_pair(x1,x2)] = hScore[make_pair(x1,x2)];
    map<Pair,Pair> came_from;
    // store the parent tuple

    bool closedList[height][width] = {false};
    list<Pair> openList;    
    
    
    openList.push_back(make_pair(x1,y1));
    // insert start point

    while (openList.size())
    {   
        Pair p = *openList.begin();
        // find the point has lowest f score 
        for (auto itr = openList.begin(); itr != openList.end(); itr++){
            if (fScore[*itr]!=0 && fScore[*itr]<fScore[p]){
                p = *itr;
            }
        }
        // arrive dest
        if(p.first==x2 && p.second==y2){

            reconstruct_path(came_from,make_pair(x2,y2),path);   
            return fScore[p];
        }

        openList.remove(p); 
        closedList[p.first][p.second] = true; 

        int gNew; 
        vector<Pair> direction;
        // find all possiable moves 
        legalMoves(p.first,p.second,size,direction);
        
        for (Pair var: direction)
        {   

            if (closedList[var.first][var.second]){
                continue;
            }
            // get new g socre
            gNew = gScore[p]+distance(var.first,var.second,p.first,p.second);
            // check if it is better
            if(gScore[var] == 0||gScore[var]>gNew){

                openList.push_back(var);
                gScore[var] = gNew;
                hScore[var] = distance(var.first,var.second,x2,y2);
                fScore[var] = hScore[var] + gScore[var];
                came_from[var] = p;

            }
            
        }
    }
    
    return -1;

};
