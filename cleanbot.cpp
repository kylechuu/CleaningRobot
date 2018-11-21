#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<limits.h>
#include<vector>
#include <stdio.h>
#include<stack>
using namespace std;
class graph;


class postion{
    public:
        int x;
        int y;
        int intersection_count; 
};

class cleanbot{
    public:
        void read_data_file(string input_path);
        // void move_up();
        // void move_down();
        // void move_left();
        // void move_right();
        // void back_to_intersection();
        void back_to_charge(int x, int y);
        void clean(int x , int y);
        cleanbot():current_position.x(0),current_position.y(0){};
        cleanbot(int x , int y):(start.x(x),start.y(y)){};
    private:
        int row_size;
        int col_size;
        int B;
        position start;
        postion current_position;
        vector<vector< bool > > matrix;
        stack<char> back_to_intersection;
        stack< position > postion_of_intersection; 
        friend class graph;
};

class graph{
    matrix[]
};


int main(){
    //int row_size=0,col_size=0,B=0;
    //postion start;
    //vector<vector<bool> > matrix;
    string input_path, output_path;
    cleanbot robot(input_path);
    robot.read_data_file(input_path);    
        


    
    
    


    return 0;
}

void cleanbot::read_data_file(string input_path){
    ifstream matrixfile (input_path);
    if(!matrixfile.is_open())
        cout<<"failed to open the file"<<"\n";
    else{
        //讀檔
        matrixfile>>row_size;
        matrixfile>>col_size;
        matrixfile>>B;
        matrix.resize(row_size);
        //初始化地圖(都設為1)
        for(int i=0;i<row_size;i++){
            matrix[i].resize(col_size);
        }
        for(int i=0;i<=row_size;i++)
            for(int j=0;j<=col_size;i++)
                matrix[i][j]=true;


        for(int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                matrixfile>>matrix[i][j];
                if(matrix[i][j]!=true || matrix[i][j]!=false){
                    matrix[i][j]=true;
                    start.x=i;
                    start.y=j;
                }
            }
        }
        matrixfile.close();
        clean(start.x , start.y);
        //cleanbot robot(start.x , start.y);
    }
}

void cleanbot::clean(int x , int y){
    position intersection;
    int step_to_teturn=0;
    back_to_intersection.push(string "R");
    while( !postion_of_intersection.empty() ){
        if(matrix[x-1][y]==false){
            matrix[x-1][y]=true;
            //move up
            x=x-1;
            //每走一格電力少一
            B--;
            step_to_teturn++;
            back_to_charge.push_back(string "down");
            //定義該點是否為岔路
            if(matrix[x-1][y]==false)
                intersection.intersection_count++;
            if(matrix[x][y+1]==false)
                intersection.intersection_count++
            if(matrix[x+1][y]==false)
                intersection.intersection_count++
            if(matrix[x][y-1]==false)
                intersection.intersection_count++
            if(intersection.intersection_count>=2){
                intersection.x=x;
                intersection.y=y;
                postion_of_intersection.push(intersection);
            }

            if(B<=step_to_teturn+1){
                back_to_charge(x,y);
                back_to_node();
            }

        }
        else if(matrix[x][y+1]==false){
            matrix[x][y+1]=true;
            y=y+1;
            B--;
        }
        else if(matrix[x+1][y]==false){
            matrix[x+1][y]=true;
            x=x+1;
            B--;
        }
        else if(matrix[x][y-1]==false){
            matrix[x][y-1]=true;
            y=y-1;
            B--;
        }
        else{
            back_to_charge(x,y);
            back_to_node();
        }
    }
}

void cleanbot::back_to_charge(int x, int y){
    B=B+back_to_charge.size();
    while(!back_to_charge.empty()){
        if(back_to_charge.top()=="up"){
            x=x-1;
            back_to_charge.pop();
        }
        else if(back_to_charge.top()=="right"){
            y=y+1;
            back_to_charge.pop();
        }
        else if(back_to_charge.top()=="down"){
            x=x+1;
            back_to_charge.pop();                
        }
        else{
            y=y-1;
            back_to_charge.pop();
        }
    }
}

void cleanbot::back_to_intersection(){

}