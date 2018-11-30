#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<limits.h>
#include<vector>
#include<stdio.h>
#include<stack>
#include<queue>
using namespace std;
class node{
    public:
        int x;
        int y;
};
class position{
    public:
        int x;
        int y;
        int intersection_count; 
};
class cleanbot{
    public:
        void read_data_file(string input_path);
        void go_back_to_charge(int x, int y);
        void clean(int x , int y);
        void print();
        cleanbot(string str):inputpath(str),do_once(1),num_vertex(0),total_steps(0),step_to_return(0){};
    private:
        int row_size;
        int col_size;
        int B;
        int total_steps;
        int step_to_return;
        int B_fixed;
        int do_once;
        string inputpath;
        position start;
        position intersection;
        vector<vector< int > > matrix;
        node Node;
        node critical_point;
        queue<node> steps_by_steps;
        queue<int> records_to_point;
        stack<int> steps_back_to_charge;
        stack< position > position_of_intersection;
        stack<node> critical_stack;
        bool inspect_still_intersect(int x,int y);
        //以下為graph所用之成員函式
        int num_vertex,count;
        vector<int> distance ;
        vector<vector<int> > adjmatrix;
        vector<int> predecessor;     // vector<vector<int> > predecessor;
        vector<vector<int> > msp;
        vector<bool> visited;
        int index_every_node();
        void initialize_source(int start);
        void inspect_intersection(int x,int y);
        void inspect_still_going(int x,int y);
        void back_to_node(int x, int y);
        void dijkstra(int start);
        void get_across_critical_point(int x,int y);
        int min_distance();
        void get_steps(int index);
        queue<int> steps_to_certain_point;
        stack<int> steps_to_certain_point2;
};
void cleanbot::get_steps(int index){
    int par_x=0,par_y=0,x=0,y=0;
    if(predecessor[index]==-1)  //predecessor[index][0]==-1
        return ;
    // if(predecessor[index][1]!=-1){
    //     //count++;
    //     get_steps(predecessor[index][1]);
    //     for(int i=0;i<row_size;i++){
    //         for(int j=0;j<col_size;j++){
    //             if(msp[i][j]==index){
    //                 x=i; y=j;
    //             }
    //             if(msp[i][j]==predecessor[index][0]){
    //                 par_x=i; par_y=j;
    //             }
    //         }
    //     }

    //     if((x-par_x)==-1 && (y-par_y)==0){
    //         steps_to_certain_point2.push(1);
    //     }
    //     else if((x-par_x)==1 && (y-par_y)==0){
    //         steps_to_certain_point2.push(2);
    //     }
    //     else if((x-par_x)==0 && (y-par_y)==-1){
    //         steps_to_certain_point2.push(3);
    //     }
    //     else{
    //         steps_to_certain_point2.push(4);
    //     }   
    // }
    // else{
        get_steps(predecessor[index]);       //get_steps(predecessor[index][0])
        for(int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                if(msp[i][j]==index){
                    x=i; y=j;
                }
                if(msp[i][j]==predecessor[index]){  //if(msp[i][j]==predecessor[index][0])
                    par_x=i; par_y=j;
                }
            }
        }
        if((x-par_x)==-1 && (y-par_y)==0){
            steps_to_certain_point.push(1);
            steps_to_certain_point2.push(2);
        }
        else if((x-par_x)==1 && (y-par_y)==0){
            steps_to_certain_point.push(2);
            steps_to_certain_point2.push(1);
        }
        else if((x-par_x)==0 && (y-par_y)==-1){
            steps_to_certain_point.push(3);
            steps_to_certain_point2.push(4);
        }
        else{
            steps_to_certain_point.push(4);
            steps_to_certain_point2.push(3);
        }
    // }
}
int cleanbot::index_every_node(){
    msp.resize(row_size);
    num_vertex=0;
        for(int i=0;i<row_size;i++)
            msp[i].resize(col_size);
        //初始化index為-1
        for(int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                msp[i][j]=-1;
            }
        }
        for(int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                if(matrix[i][j]==0){
                    msp[i][j]=num_vertex;
                    num_vertex++;
                }
            }
        }
        return msp[start.x][start.y];
        //matrix[start.x][start.y]=1;
}
void cleanbot::initialize_source(int start){
    count=0;
    
    adjmatrix.resize(num_vertex);
    distance.resize(num_vertex);
    predecessor.resize(num_vertex);
    visited.resize(num_vertex);
    // for(int i=0; i<num_vertex; i++){
    //     predecessor[i].resize(2);
    // }
    for(int i=0;i<num_vertex;i++){
        // for(int j=0;j<2;j++){
            predecessor[i]=-1;    //predecessor[i][j]=-1;
    
        // }
    }

    for(int i=0;i<num_vertex;i++){
        adjmatrix[i].resize(num_vertex);
        distance[i]=INT_MAX;
        visited[i]=false;
    }
    for(int i=0;i<num_vertex;i++){
        for(int j=0;j<num_vertex;j++){
            adjmatrix[i][j]=INT_MAX;
        }
    }
    distance[start]=0;
}
void cleanbot::dijkstra(int start){
    initialize_source(start);

    //建立adjmatrix
    for(int i=0;i<row_size;i++){
        for(int j=0;j<col_size;j++){
            if(i==0 && j==0){
                if(matrix[i][j]==0 && matrix[i][j+1]==0){
                    adjmatrix[ msp[i][j]][ msp[i][j+1] ]=1;
                    adjmatrix[ msp[i][j+1]][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i+1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i+1][j] ]=1;
                    adjmatrix[ msp[i+1][j] ][ msp[i][j] ]=1;
                }
            }
            else if(i==0 && j==col_size-1){
                if(matrix[i][j]==0 && matrix[i][j-1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j-1] ]=1;
                    adjmatrix[ msp[i][j-1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i+1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i+1][j] ]=1;
                    adjmatrix[ msp[i+1][j] ][ msp[i][j] ]=1;
                }
            }
            else if(i==row_size-1 && j==0){
                if(matrix[i][j]==0 && matrix[i-1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i-1][j] ]=1;
                    adjmatrix[ msp[i-1][j] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i][j+1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j+1] ]=1;
                    adjmatrix[ msp[i][j+1] ][ msp[i][j] ]=1;
                }
            }
            else if(i==row_size-1 && j==col_size-1){
                if(matrix[i][j]==0 && matrix[i-1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i-1][j] ]=1;
                    adjmatrix[ msp[i-1][j] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i][j-1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j-1] ]=1;
                    adjmatrix[ msp[i][j-1] ][ msp[i][j] ]=1;
                }
            }
            //邊
            else if(i==0 && j!=0 && j!=col_size-1){
                if(matrix[i][j]==0 && matrix[i][j+1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j+1] ]=1;
                    adjmatrix[ msp[i][j+1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i][j-1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j-1] ]=1;
                    adjmatrix[ msp[i][j-1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i+1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i+1][j] ]=1;
                    adjmatrix[ msp[i+1][j] ][ msp[i][j] ]=1;
                }
            }

            else if(i==row_size-1 && j!=0 && j!=col_size-1){
                if(matrix[i][j]==0 && matrix[i][j+1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j+1] ]=1;
                    adjmatrix[ msp[i][j+1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i][j-1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j-1] ]=1;
                    adjmatrix[ msp[i][j-1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i-1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i-1][j] ]=1;
                    adjmatrix[ msp[i-1][j] ][ msp[i][j] ]=1;
                }
            }

            else if(j==0 && i!=0 && i!=row_size-1){
                if(matrix[i][j]==0 && matrix[i-1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i-1][j] ]=1;
                    adjmatrix[ msp[i-1][j] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i][j+1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j+1] ]=1;
                    adjmatrix[ msp[i][j+1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i+1][j]==0){
                    adjmatrix[msp[i][j] ][ msp[i+1][j] ]=1;
                    adjmatrix[ msp[i+1][j] ][ msp[i][j] ]=1;
                }
            }

            else if(j==col_size-1 && i!=0 && i!=row_size-1){
                if(matrix[i][j]==0 && matrix[i-1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i-1][j] ]=1;
                    adjmatrix[ msp[i-1][j] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i][j-1]==0){
                    adjmatrix[ msp[i][j] ][ msp[i][j-1] ]=1;
                    adjmatrix[ msp[i][j-1] ][ msp[i][j] ]=1;
                }
                if(matrix[i][j]==0 && matrix[i+1][j]==0){
                    adjmatrix[ msp[i][j] ][ msp[i+1][j] ]=1;
                    adjmatrix[ msp[i+1][j] ][ msp[i][j] ]=1;
                }
            }
            else{
                if(matrix[i][j]==0 && matrix[i-1][j]==0)  {  adjmatrix[ msp[i][j] ][ msp[i-1][j] ]=1; adjmatrix[ msp[i-1][j] ][ msp[i][j] ]=1;  }
                if(matrix[i][j]==0 && matrix[i+1][j]==0)  {  adjmatrix[ msp[i][j] ][ msp[i+1][j] ]=1; adjmatrix[ msp[i+1][j] ][ msp[i][j] ]=1;  }
                if(matrix[i][j]==0 && matrix[i][j-1]==0)  {  adjmatrix[ msp[i][j] ][ msp[i][j-1] ]=1; adjmatrix[ msp[i][j-1] ][ msp[i][j] ]=1;  }
                if(matrix[i][j]==0 && matrix[i][j+1]==0)  {  adjmatrix[ msp[i][j] ][ msp[i][j+1] ]=1; adjmatrix[ msp[i][j+1] ][ msp[i][j] ]=1;  }
                    
            }
        }
    }
    for(int count=0; count<num_vertex; count++){
        int u=min_distance();
        visited[u]=true;
        for(int v=0; v<num_vertex; v++ )
            if(!visited[v] && adjmatrix[u][v]!=INT_MAX && distance[u]+adjmatrix[u][v]<distance[v] ){     //if(!visited[v] && adjmatrix[u][v]!=INT_MAX && distance[u]+adjmatrix[u][v]<=distance[v] )
                // if(distance[u]+adjmatrix[u][v]==distance[v]){
                //     predecessor[v][1]=u;
                //     distance[v]=distance[u]+adjmatrix[u][v];
                // }
                // else{
                    predecessor[v]=u;    //predecessor[v][0]=u;
                    distance[v]=distance[u]+adjmatrix[u][v];
                // }
            }
    }
 
}
int cleanbot::min_distance(){
    int min=INT_MAX,min_index;
    for(int v=0; v<num_vertex; v++)
        if(visited[v]==false && distance[v]<=min)
            min=distance[v],min_index=v;
        
    return min_index;
}
int main(){
    //int row_size=0,col_size=0,B=0;
    //postion start;
    //vector<vector<bool> > matrix;
    string input_path, output_path;
    input_path="floor.data";
    cleanbot robot(input_path);
    robot.read_data_file(input_path);    
    robot.print();  
        
        
    return 0;
}

void cleanbot::read_data_file(string input_path){
    ifstream matrixfile (input_path);
    if(!matrixfile.is_open())
        cout<<"failed to open the file"<<"\n";
    else{
        //讀檔
        char iterater;
        char row_size_char;
        char col_size_char;
        char B_char;
        matrixfile>>row_size_char;
        matrixfile>>col_size_char;
        matrixfile>>B_char;
        row_size=(int)row_size_char-48;
        col_size=(int)col_size_char-48;
        B=(int)B_char-48;
        B_fixed=B;
        matrix.resize(row_size);
        //初始化地圖(都設為1)
        for(int i=0;i<row_size;i++){
            matrix[i].resize(col_size);
        }
        for(int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                matrix[i][j]=1;
            }
        }
        for(int i=0;i<row_size;i++){
            for(int j=0;j<col_size;j++){
                matrixfile>>iterater;
                if(iterater=='1')
                    matrix[i][j]=1;
                else if(iterater=='0'){
                    matrix[i][j]=0;
                    num_vertex++;
                }
                else{
                    matrix[i][j]=0;
                    num_vertex++;
                    start.x=i;
                    start.y=j;
                }
            }
        }
        int s=index_every_node();
        dijkstra(s);
        matrixfile.close();
        matrix[start.x][start.y]=1;
        
        clean(start.x , start.y);
        
        //cleanbot robot(start.x , start.y);
    }
}
void cleanbot::print(){
    vector<node> print_node;
    int counter=0;
    print_node.resize(total_steps);
    while(!steps_by_steps.empty()){
        print_node[counter]=steps_by_steps.front();
        steps_by_steps.pop();
        counter++;
    }
    cout<<total_steps<<"\n";
    for(int i=0;i<total_steps;i++){
        cout<<print_node[i].x<<" "<<print_node[i].y<<"\n";
    }
}
void cleanbot::inspect_intersection(int x,int y){
    int intersection_counter=0;
    if(matrix[x-1][y]==0)
        intersection_counter++;
    if(matrix[x][y+1]==0)
        intersection_counter++;
    if(matrix[x+1][y]==0)
        intersection_counter++;
    if(matrix[x][y-1]==0)
        intersection_counter++;
    if(intersection_counter>=2){
        intersection.x=x;
        intersection.y=y;
        position_of_intersection.push(intersection);
    }
}
void cleanbot::get_across_critical_point(int x, int y){
    get_steps(msp[x][y]);
    while(!steps_to_certain_point2.empty()){
        if(steps_to_certain_point2.top()==1){
            x=x-1;
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(2);
        }
        else if(steps_to_certain_point2.top()==2){
            x=x+1;
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(1);
        }
        else if(steps_to_certain_point2.top()==3){
            y=y-1;
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(4);
        }
        else{
            y=y+1;
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(3);
        }
        inspect_intersection(x,y);
        steps_to_certain_point2.pop();                  
    }
    step_to_return=0;
    B=B_fixed;
    //while(!steps_back_to_charge.empty()){ steps_back_to_charge.pop(); }
    while(!steps_to_certain_point.empty()){ steps_to_certain_point.pop(); }
    while(!records_to_point.empty()){ records_to_point.pop(); }
    while(!steps_to_certain_point2.empty()){ steps_to_certain_point2.pop(); }
    back_to_node(start.x , start.y);
}
void cleanbot::clean(int x , int y){
    inspect_still_going(x , y);
    while(!critical_stack.empty() || !position_of_intersection.empty() ||  do_once==1){
      
        do_once=0;
        if(B<=step_to_return+1){//檢查電力
           
                if(distance[msp[x][y]]==B && (matrix[x-1][y]==0 || matrix[x+1][y]==0 || matrix[x][y-1]==0 || matrix[x][y+1]==0 ) ){
                    critical_point.x=x;
                    critical_point.y=y;
                    critical_stack.push(critical_point);
                    //queue_to_critical_point=records_to_point;
                    if(position_of_intersection.empty()){
                        if(matrix[x-1][y]==0)
                            get_across_critical_point(x-1,y);
                        else if(matrix[x+1][y]==0)
                            get_across_critical_point(x+1,y);
                        else if(matrix[x][y-1]==0)
                            get_across_critical_point(x,y-1);
                        else
                            get_across_critical_point(x,y+1);

                        critical_stack.pop();
                    }
                    else{
                        //record path
                        go_back_to_charge(x,y);
                        back_to_node(start.x,start.y);
                    }
                    
                }
                else{
                    
                    go_back_to_charge(x,y);
                   
                    back_to_node(start.x,start.y);
                   
                }
        }
        else{
        if(x!=start.x && y!=start.y){ inspect_intersection(x,y);}
        if(matrix[x-1][y]==0){
            //move up
            matrix[x-1][y]=1;
            x=x-1;
            //每走一格電力少一，總步數加一，回到原點需要的步數加一
            B--;
            total_steps++;
            step_to_return++;
            //用node來printout，注意index從1開始
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            //回去充電的話是往下面一格
            steps_back_to_charge.push(2);
            records_to_point.push(1);
            //定義該點是否為岔路
            inspect_intersection(x,y);
            inspect_still_going(x,y);
        }
        else if(matrix[x][y+1]==0){
            matrix[x][y+1]=1;
            y=y+1;
            B--;
            total_steps++;
            step_to_return++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(3);
            records_to_point.push(4);
            inspect_intersection(x,y);
            inspect_still_going(x,y);
        }
        else if(matrix[x+1][y]==0){
            matrix[x+1][y]=true;
            x=x+1;
            B--;
            total_steps++;
            step_to_return++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(1);
            records_to_point.push(2);
            inspect_intersection(x,y);
            inspect_still_going(x,y);
        }
        else if(matrix[x][y-1]==0){
            matrix[x][y-1]=1;
            y=y-1;
            B--;
            total_steps++;
            step_to_return++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
            steps_back_to_charge.push(4);
            records_to_point.push(3);
            inspect_intersection(x,y);
            inspect_still_going(x,y);
        }
        else{
            go_back_to_charge(x,y);
          
            back_to_node(start.x , start.y);
        }
      
    }
    }
  
}

void cleanbot::go_back_to_charge(int x, int y){
    while(!steps_back_to_charge.empty()){
        //1=上 2=下 3=左 4=右
        if(steps_back_to_charge.top()==1){
            x=x-1;
            steps_back_to_charge.pop();
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
        }
        else if(steps_back_to_charge.top()==2){
            x=x+1; 
            steps_back_to_charge.pop(); 
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node); 
        }
        else if(steps_back_to_charge.top()==3){
            y=y-1; 
            steps_back_to_charge.pop(); 
            total_steps++;
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node); 
        }
        else{
            //(steps_back_to_charge.top()==4)
            y=y+1; 
            steps_back_to_charge.pop(); 
            total_steps++; 
            Node.x=x;
            Node.y=y;
            steps_by_steps.push(Node);
        }
    }
    step_to_return=0;
    B=B_fixed;
    while(!records_to_point.empty()){records_to_point.pop();}
}
void cleanbot::back_to_node(int x, int y){
    if(!position_of_intersection.empty()){
        bool a= false;
        while(!a){
            intersection=position_of_intersection.top();
            a=inspect_still_intersect(intersection.x,intersection.y);
            if(!a)
                position_of_intersection.pop();
            if(position_of_intersection.empty())
                break ;
        }
        if(position_of_intersection.empty())
            return ;
        get_steps(msp[intersection.x][intersection.y]);
        position_of_intersection.pop();
        while(!steps_to_certain_point.empty()){
            if(steps_to_certain_point.front()==1){
                x=x-1;
                total_steps++;
                B--;
                step_to_return++;
                inspect_intersection(x,y);
                steps_back_to_charge.push(2);
                records_to_point.push(1);
                Node.x=x;
                Node.y=y;
                steps_by_steps.push(Node);
            }
            else if(steps_to_certain_point.front()==2){
                x=x+1;
                total_steps++;
                B--;
                step_to_return++;
                inspect_intersection(x,y);
                steps_back_to_charge.push(1);
                records_to_point.push(2);
                Node.x=x;
                Node.y=y;
                steps_by_steps.push(Node);
            }
            else if(steps_to_certain_point.front()==3){
                y=y-1;
                total_steps++;
                B--;
                step_to_return++;
                inspect_intersection(x,y);
                steps_back_to_charge.push(4);
                records_to_point.push(3);
                Node.x=x;
                Node.y=y;
                steps_by_steps.push(Node);
            }
            else{
                y=y+1;
                total_steps++;
                B--;
                step_to_return++;
                inspect_intersection(x,y);
                steps_back_to_charge.push(3);
                records_to_point.push(4);
                Node.x=x;
                Node.y=y;
                steps_by_steps.push(Node);
            }
            steps_to_certain_point.pop();                   
        }
        //while(!steps_to_certain_point.empty()){ steps_to_certain_point.pop(); }
        //while(!records_to_point.empty()){ records_to_point.pop(); }
        while(!steps_to_certain_point2.empty()){ steps_to_certain_point2.pop(); }
    }
    else{
        if(!critical_stack.empty()){
            node key;
            key=critical_stack.top();
            while(!inspect_still_intersect(key.x,key.y)){
                critical_stack.pop();
                key=critical_stack.top();
            }
            if(critical_stack.empty()){
                return;
            }
            go_back_to_charge(x,y);

            //go_to_critical_point();
        }
    }
    clean(x,y);
}
void cleanbot::inspect_still_going(int x,int y){
    if(matrix[x-1][y]==0 || matrix[x+1][y]==0 || matrix[x][y-1]==0 || matrix[x][y+1]==0)
        do_once=1;
    else
         do_once=0;
}
bool cleanbot::inspect_still_intersect(int x,int y){
    if(matrix[x-1][y]==1 && matrix[x+1][y]==1 && matrix[x][y-1]==1 && matrix[x][y+1]==1)
        return false;
    else 
        return true;
}