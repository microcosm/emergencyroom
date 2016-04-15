#include "erEcgVisualization.h"

void erEcgVisualization::setup(){
    source = "ecg.csv";
    numCols = 2;
    stream.openReadStream(source);
    readData();
}

void erEcgVisualization::readData(){
    while(!stream.eof()){
        stream.readNextLine();
        for(int i = 0; i < stream.getCurrentTokenSize(); i += numCols){
            data[stream.getValue<float>(i)] = stream.getValue<float>(i+1);
        }
    }
    for(auto const& row : data){
        cout << row.first << " " << row.second << endl;
    }
}