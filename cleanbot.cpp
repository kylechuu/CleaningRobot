#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<limits.h>
#include<vector>
#include <stdio.h>
#include<stack>
using namespace std;

class postion{
    public:
        int x;
        int y; 
};

class cleanbot{
    public:
        postion current_position;
        void move_up();
        void move_down();
        void move_left();
        void move_right();
        void back_to_intersection();
        void back_to_charge();
        void clean();
        cleanbot():current_position.x(0),current_position.y(0){};
        cleanbot(int x , int y):(current_position.x(x),current_position.y(y)){};
    private:
        stack<char> back_to_intersection;
        stack<int> postion_of_intersection; 
};


int main(){
    int row_size=0,col_size=0,B=0;
    postion start;
    vector<vector<bool> > matrix;
    string input_path, output_path;
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
                    matrix[i][j]=false;
                    start.x=i;
                    start.y=j;
                }
            }
        }
        matrixfile.close();            
        postion current_postion;    
        cleanbot robot(start.x , start.y);


    }
    
    


    return 0;
}