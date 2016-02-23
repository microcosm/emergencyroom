#include "erSeaLevelViz.h"

void erSeaLevelViz::setup(){
    source = "sea-level/surging-seas-BK_2.6_3.csv";
    numCols = 2;
    stream.openReadStream(source);
    readData();
}

void erSeaLevelViz::readData(){
    while(!stream.eof()){
        stream.readNextLine();
        for(int i = 0; i < stream.getCurrentTokenSize(); i += numCols){
            data[stream.getValue<int>(i)] = stream.getValue<float>(i+1);
        }
    }
    for(auto const& row : data){
        cout << row.first << " " << row.second << endl;
    }
}