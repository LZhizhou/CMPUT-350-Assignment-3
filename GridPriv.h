// list private Grid members here

// ...
Tile** mapInfo;
const int width;
const int height;
int **connectedCache;   //map coord (x,y)-> 0 -> no cache;
                                    // (x,y)-> -1 not connected
                                    // (x,y)-> 1 connected
bool check_cache(int x1, int y1, int x2, int y2, int size)const{
    return connectedCache[x1+y1*width][x2+y2*width]>size;
}
void write_cache(int x1, int y1, int x2, int y2, int size)const {
    connectedCache[x1+y1*width][x2+y2*width] =size+1;
    connectedCache[x2+y2*width][x1+y1*width] =size+1;
}

// recursive call of make 
bool innerConnected(int size, int x1, int y1, int x2, int y2, bool visted[])const{
    if(x1 == x2 && y1 == y2){
        return true;
    }
    
    if(visted[x1+y1*width]){
        return false; 
    }
    visted[x1+y1*width] = true;
    if(check_cache(x1,y1,x2,y2,size)){
        return true;
    }
    Tile tile = mapInfo[y1][x1];
    for (int i = -1 ; i <= 1;i++){
        for (int j = -1; j <= 1;j++){
            if((i == j && i == 0)||(check_cache(x1,y1,x2,y2,size)))
            {
                continue;
            }else{
                if(ifLegal(x1,y1,i,j,size,tile)){
                    if (innerConnected(size,x1+i,y1+j,x2,y2,visted)){
                        write_cache(x1+i, y1+j,x2,y2,size);
                        return true;
                    }
                }

            }
        }
    }
    
    return false;
};
// check all 8 surronding tiles, and add legal to newMove
void legalMoves(int x, int y, int size,std::vector<std::pair<int, int>> &newMove)const{
    Tile tile = mapInfo[y][x];
    for (int i = -1 ; i <= 1;i++){
        for (int j = -1; j <= 1;j++){
            if(i == j && i == 0)
            {
                continue;
            }else
            {
                if(ifLegal(x,y,i,j,size,tile)){
                    newMove.push_back(std::make_pair(x+i,y+j));
                }
            }
        }
    }
};

// return ture if it is legal to move to new location
// x, y is the point came from
// x_diff, y_diff is the offset
bool ifLegal(int x,int y,int x_diff, int y_diff, int size,Tile tile)const{
    int smallX, smallY, bigX, bigY;
    if(x_diff<0){
        smallX = x+x_diff;
        bigX = x;
    }else{
        bigX = x+x_diff;
        smallX = x;
    }
    if(y_diff<0){
        smallY = y+y_diff;
        bigY = y;
    }else{
        bigY = y+y_diff;
        smallY = y;
    }
    if(smallX<0||smallY<0||bigX+size>=width||bigY+size>=height){
        return false;
    } 
    for (int i = smallX; i <= bigX+size; i++)
    {
        for (int j = smallY; j <=bigY+size; j++)
        {

            if (mapInfo[j][i]!=tile)
            {
                
                return false;
            }
        }
    }
    return true;   
};



