#include <iostream>
#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <vector>

const int WIDTH = 1200, HEIGHT = 900;

struct vect{
    float x,y,z;
};

struct tri{
    vect arr[3];
};

struct mesh{
    std::vector<tri> tmsh;
};

struct matrix_4x4{
    float mtx[4][4] = {};
};

void matrixVector(vect &vi, vect &vo, matrix_4x4 &mx){

    vo.x = vi.x * mx.mtx[0][0] + vi.y * mx.mtx[1][0] + vi.z * mx.mtx[2][0] + mx.mtx[3][0];
    vo.y = vi.x * mx.mtx[0][1] + vi.y * mx.mtx[1][1] + vi.z * mx.mtx[2][1] + mx.mtx[3][1];
    vo.z = vi.x * mx.mtx[0][2] + vi.y * mx.mtx[1][2] + vi.z * mx.mtx[2][2] + mx.mtx[3][2];
    float z = vi.x * mx.mtx[0][3] + vi.y * mx.mtx[1][3] + vi.z * mx.mtx[2][3] + mx.mtx[3][3];

    if(z != 0.0f){
        vo.x /= z;
        vo.y /= z;
        vo.z /= z;
    }
}

char A[HEIGHT][WIDTH];
int drawn = 254;

/*void print(){
    for(int j = WIDTH - 1; j >= 0; j--){
        for(int i = 0; i < HEIGHT; i++){
            if(A[i][j]=='#'){

                SDL_RenderDrawPoint(renderer, i, j);

                //std::cout<<char(drawn);
            //} else if(A[i][j]=='@'){
                //std::cout<<"@";
            //} else {
                //std::cout<<" ";
            }
        }
        //std::cout<<"\n";
    }
    SDL_RenderPresent(renderer);
}*/

void clear(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < WIDTH; j++){
            A[i][j] = ' ';
        }
    }
}

void line(int x1, int y1, int x2, int y2){
    
    double a;
    double fy;
    double fx;
    int carry;

    if((x2-x1)!=0){
        a = ((double) y2- (double) y1)/((double) x2- (double) x1);
        if(a >= 1){ 

            for(int i = std::min(y1,y2); i <= std::max(y1,y2); i++){
                fx = ((double)i - (double)y1)/a + (double) x1;
                fx = std::round(fx);
                carry = fx;
                A[carry][i] = '#';
            }
            
        //} else if(a == 1){ 
            
            //for(int i = std::min(x1, x2); i <= std::max(x1, x2); i++){
            //    A[i][i] = '#';
            //}


        } else if(a < 1 && a > 0){ 

            for(int i = std::min(x1,x2); i <= std::max(x1,x2); i++){
                fy = a*(i-x1) + y1;
                fy = std::round(fy);
                carry = fy;
                A[i][carry] = '#';
            }

        } else if(a == 0){ 

            for(int i = std::min(x1, x2); i <= std::max(x1,x2); i++){
                A[i][y1] = '#';
            }

        } else if(a < 0 && a > -1) { 
                
                for(int i = std::max(x1,x2); i >= std::min(x1,x2); i--){
                    fy = a*((double) i- (double) x1) + (double) y1;
                    fy = std::round(fy);
                    carry = fy;
                    A[i][carry] = '#';
                }

        //} else if(a == -1){ 

            //for(int i = std::max(x1, x2); i >= std::min(x1, x2); i--){
            //    A[std::max(x1,x2)-i+1][i] = '#';
            //}

        } else if(a <= -1){ 

            for(int i = std::min(y1,y2); i <= std::max(y1,y2); i++){
                fx = ((double)i - (double)y1)/a + (double) x1;
                fx = std::round(fx);
                carry = fx;
                A[carry][i] = '#';
            }

        }

    } else { 
        for(int i = std::min(y1,y2); i <= std::max(y1,y2); i++){
            A[x1][i] = '#';
        }
    }
}

int main(int argc, char *argv[]){

    SDL_Window *window = nullptr;
    Uint32 flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    //SDL_Window *window = SDL_CreateWindow("3Deng", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, flags);

    SDL_Renderer* renderer = nullptr;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);


    SDL_Event windowEvent;

    mesh mCube;
    mCube.tmsh = {

        { 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 0.0f }, //back
        { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f },

        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f }, //right
        { 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f },

        { 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f }, //front
        { 1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f },

        { 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f, 0.0f }, //left
        { 0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f },

        { 0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 1.0f }, //top
        { 0.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },

        { 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f }, //bottom
        { 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f },
    };

    mesh mRabyte;
    mRabyte.tmsh = {
        {4.0f, 0.0f, 0.0f,   2.0f, 1.0f, 0.0f,   1.0f, 2.0f, 0.0f},
        {1.0f, 2.0f, 0.0f,   0.0f, 4.0f, 0.0f,   0.0f, 8.0f, 0.0f},
        {0.0f, 8.0f, 0.0f,   1.0f, 10.0f, 0.0f,   2.0f, 11.0f, 0.0f},
        {2.0f, 11.0f, 0.0f,   4.0f, 12.0f, 0.0f,   8.0f, 12.0f, 0.0f},
        {8.0f, 12.0f, 0.0f,   10.0f, 11.0f, 0.0f,   11.0f, 10.0f, 0.0f},
        {11.0f, 10.0f, 0.0f,   12.0f, 8.0f, 0.0f,   12.0f, 4.0f, 0.0f},
        {12.0f, 4.0f, 0.0f,   11.0f, 2.0f, 0.0f,   10.0f, 1.0f, 0.0f},
        {10.0f, 1.0f, 0.0f,   8.0f, 0.0f, 0.0f,   9.0f, 1.0f, 0.0f},
        {9.0f, 1.0f, 0.0f,   9.0f, 2.0f, 0.0f,   8.0f, 4.0f, 0.0f},
        {8.0f, 4.0f, 0.0f,   8.0f, 6.0f, 0.0f,   9.0f, 8.0f, 0.0f},
        {9.0f, 8.0f, 0.0f,   10.0f, 7.0f, 0.0f,   11.0f, 7.0f, 0.0f},
        {11.0f, 7.0f, 0.0f,   11.0f, 8.0f, 0.0f,   9.0f, 9.0f, 0.0f},
        {9.0f, 9.0f, 0.0f,   8.0f, 9.0f, 0.0f,   7.0f, 6.0f, 0.0f},
        {7.0f, 6.0f, 0.0f,   7.0f, 5.0f, 0.0f,   5.0f, 5.0f, 0.0f},
        {5.0f, 5.0f, 0.0f,   5.0f, 6.0f, 0.0f,   4.0f, 9.0f, 0.0f},
        {4.0f, 9.0f, 0.0f,   3.0f, 11.0f, 0.0f,   2.0f, 10.0f, 0.0f},
        {2.0f, 10.0f, 0.0f,   2.0f, 9.0f, 0.0f,   3.0f, 8.0f, 0.0f},
        {3.0f, 8.0f, 0.0f,   4.0f, 6.0f, 0.0f,   4.0f, 4.0f, 0.0f},
        {4.0f, 4.0f, 0.0f,   3.0f, 2.0f, 0.0f,   3.0f, 1.0f, 0.0f},
        {3.0f, 1.0f, 0.0f,   4.0f, 0.0f, 0.0f,   2.0f, 1.0f, 0.0f},

        {4.0f, 0.0f, 2.0f,   2.0f, 1.0f, 2.0f,   1.0f, 2.0f, 2.0f},
        {1.0f, 2.0f, 2.0f,   0.0f, 4.0f, 2.0f,   0.0f, 8.0f, 2.0f},
        {0.0f, 8.0f, 2.0f,   1.0f, 10.0f, 2.0f,   2.0f, 11.0f, 2.0f},
        {2.0f, 11.0f, 2.0f,   4.0f, 12.0f, 2.0f,   8.0f, 12.0f, 2.0f},
        {8.0f, 12.0f, 2.0f,   10.0f, 11.0f, 2.0f,   11.0f, 10.0f, 2.0f},
        {11.0f, 10.0f, 2.0f,   12.0f, 8.0f, 2.0f,   12.0f, 4.0f, 2.0f},
        {12.0f, 4.0f, 2.0f,   11.0f, 2.0f, 2.0f,   10.0f, 1.0f, 2.0f},
        {10.0f, 1.0f, 2.0f,   8.0f, 0.0f, 2.0f,   9.0f, 1.0f, 2.0f},
        {9.0f, 1.0f, 2.0f,   9.0f, 2.0f, 2.0f,   8.0f, 4.0f, 2.0f},
        {8.0f, 4.0f, 2.0f,   8.0f, 6.0f, 2.0f,   9.0f, 8.0f, 2.0f},
        {9.0f, 8.0f, 2.0f,   10.0f, 7.0f, 2.0f,   11.0f, 7.0f, 2.0f},
        {11.0f, 7.0f, 2.0f,   11.0f, 8.0f, 2.0f,   9.0f, 9.0f, 2.0f},
        {9.0f, 9.0f, 2.0f,   8.0f, 9.0f, 2.0f,   7.0f, 6.0f, 2.0f},
        {7.0f, 6.0f, 2.0f,   7.0f, 5.0f, 2.0f,   5.0f, 5.0f, 2.0f},
        {5.0f, 5.0f, 2.0f,   5.0f, 6.0f, 2.0f,   4.0f, 9.0f, 2.0f},
        {4.0f, 9.0f, 2.0f,   3.0f, 11.0f, 2.0f,   2.0f, 10.0f, 2.0f},
        {2.0f, 10.0f, 2.0f,   2.0f, 9.0f, 2.0f,   3.0f, 8.0f, 2.0f},
        {3.0f, 8.0f, 2.0f,   4.0f, 6.0f, 2.0f,   4.0f, 4.0f, 2.0f},
        {4.0f, 4.0f, 2.0f,   3.0f, 2.0f, 2.0f,   3.0f, 1.0f, 2.0f},
        {3.0f, 1.0f, 2.0f,   4.0f, 0.0f, 2.0f,   2.0f, 1.0f, 2.0f},

        {4.0f, 0.0f, 0.0f,   4.0f, 0.0f, 2.0f,   4.0f, 0.0f, 0.0f},
        {1.0f, 2.0f, 0.0f,   1.0f, 2.0f, 2.0f,   1.0f, 2.0f, 0.0f},
        {0.0f, 8.0f, 0.0f,   0.0f, 8.0f, 2.0f,   0.0f, 8.0f, 0.0f},
        {2.0f, 11.0f, 0.0f,   2.0f, 11.0f, 2.0f,   2.0f, 11.0f, 0.0f},
        {8.0f, 12.0f, 0.0f,   8.0f, 12.0f, 2.0f,   8.0f, 12.0f, 0.0f},
        {11.0f, 10.0f, 0.0f,   11.0f, 10.0f, 2.0f,   11.0f, 10.0f, 0.0f},
        {12.0f, 4.0f, 0.0f,   12.0f, 4.0f, 2.0f,   12.0f, 4.0f, 0.0f},
        {10.0f, 1.0f, 0.0f,   10.0f, 1.0f, 2.0f,   10.0f, 1.0f, 0.0f},
        {9.0f, 1.0f, 0.0f,   9.0f, 1.0f, 2.0f,   9.0f, 1.0f, 0.0f},
        {8.0f, 4.0f, 0.0f,   8.0f, 4.0f, 2.0f,   8.0f, 4.0f, 0.0f},
        {9.0f, 8.0f, 0.0f,   9.0f, 8.0f, 2.0f,   9.0f, 8.0f, 0.0f},
        {11.0f, 7.0f, 0.0f,   11.0f, 7.0f, 2.0f,   11.0f, 7.0f, 0.0f},
        {9.0f, 9.0f, 0.0f,   9.0f, 9.0f, 2.0f,   9.0f, 9.0f, 0.0f},
        {7.0f, 6.0f, 0.0f,   7.0f, 6.0f, 2.0f,   7.0f, 6.0f, 0.0f},
        {5.0f, 5.0f, 0.0f,   5.0f, 5.0f, 2.0f,   5.0f, 5.0f, 0.0f},
        {4.0f, 9.0f, 0.0f,   4.0f, 9.0f, 2.0f,   4.0f, 9.0f, 0.0f},
        {2.0f, 10.0f, 0.0f,   2.0f, 10.0f, 2.0f,   2.0f, 10.0f, 0.0f},
        {3.0f, 8.0f, 0.0f,   3.0f, 8.0f, 2.0f,   3.0f, 8.0f, 0.0f},
        {4.0f, 4.0f, 0.0f,   4.0f, 4.0f, 2.0f,   4.0f, 4.0f, 0.0f},
        {3.0f, 1.0f, 0.0f,   3.0f, 1.0f, 2.0f,   3.0f, 1.0f, 0.0f},
    };

    float fov = 150.0f;
    float asp = (float) HEIGHT / (float) WIDTH;
    float zprim = 100.0f;
    float zsec = 1000.0f;
    float tanscale = 1.0f / tan(fov * 0.5f / 180.0f * 3.14159f);

    matrix_4x4 mxProjection;
    mxProjection.mtx[0][0] = asp * tanscale; //*asp
    mxProjection.mtx[1][1] = tanscale;
    mxProjection.mtx[2][2] = zsec / (zsec - zprim);
    mxProjection.mtx[3][2] = (-zsec * zprim) / (zsec - zprim);
    mxProjection.mtx[2][3] = 1.0f;

    float t = 0.0f;

    matrix_4x4 mrX, mrZ;
     
    while(true){
        if(SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type){
                break;
            }
        }

        mrX.mtx[0][0] = 1;
        mrX.mtx[1][1] = cosf(t * 0.5f);
        mrX.mtx[1][2] = sinf(t * 0.5f);
        mrX.mtx[2][1] = -sinf(t * 0.5f);
        mrX.mtx[2][2] = cosf(t * 0.5f);
        mrX.mtx[3][3] = 1;

        mrZ.mtx[0][0] = cosf(t);
        mrZ.mtx[0][1] = sinf(t);
        mrZ.mtx[1][0] = -sinf(t);
        mrZ.mtx[1][1] = cosf(t);
        mrZ.mtx[2][2] = 1;
        mrZ.mtx[3][3] = 1;

        clear();
        for(auto tr : mCube.tmsh){
        tri trProj, trTrans, trRX, trRZ;

        matrixVector(tr.arr[0], trRZ.arr[0], mrZ);
        matrixVector(tr.arr[1], trRZ.arr[1], mrZ);
        matrixVector(tr.arr[2], trRZ.arr[2], mrZ);

        matrixVector(trRZ.arr[0], trRX.arr[0], mrX);
        matrixVector(trRZ.arr[1], trRX.arr[1], mrX);
        matrixVector(trRZ.arr[2], trRX.arr[2], mrX);

        float mvz = 5.0f;
        trTrans = trRX;
        trTrans.arr[0].z = trRX.arr[0].z + mvz;
        trTrans.arr[1].z = trRX.arr[1].z + mvz;
        trTrans.arr[2].z = trRX.arr[2].z + mvz;

        matrixVector(trTrans.arr[0], trProj.arr[0], mxProjection);
        matrixVector(trTrans.arr[1], trProj.arr[1], mxProjection);
        matrixVector(trTrans.arr[2], trProj.arr[2], mxProjection);

        float mv = 0.25f;
        float sc = 1.33f;

        trProj.arr[0].x += mv;
        trProj.arr[0].y += mv;
        trProj.arr[1].x += mv;
        trProj.arr[1].y += mv;
        trProj.arr[2].x += mv;
        trProj.arr[2].y += mv;

        trProj.arr[0].x *= sc * WIDTH;
        trProj.arr[0].y *= sc * HEIGHT;
        trProj.arr[1].x *= sc * WIDTH;
        trProj.arr[1].y *= sc * HEIGHT;
        trProj.arr[2].x *= sc * WIDTH;
        trProj.arr[2].y *= sc * HEIGHT;

        //std::cout<<turn<<"\n";

        std::cout<<trProj.arr[0].x<<" "<<trProj.arr[0].y<<"\n";
        std::cout<<trProj.arr[1].x<<" "<<trProj.arr[1].y<<"\n";
        std::cout<<trProj.arr[2].x<<" "<<trProj.arr[2].y<<"\n";

        line(trProj.arr[0].x, trProj.arr[0].y, trProj.arr[1].x, trProj.arr[1].y);
        line(trProj.arr[1].x, trProj.arr[1].y, trProj.arr[2].x, trProj.arr[2].y);
        line(trProj.arr[2].x, trProj.arr[2].y, trProj.arr[0].x, trProj.arr[0].y);

        //std::cout<<trProj.arr[1].x<<" "<<trProj.arr[1].y<<" ";

        }

        t += 0.05f;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for(int j = WIDTH - 1; j >= 0; j--){
            for(int i = 0; i < HEIGHT; i++){
                if(A[i][j]=='#'){
                    SDL_RenderDrawPoint(renderer, i, j);
                }
            }
        }

    SDL_RenderPresent(renderer);

        SDL_Delay(15);
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;

}