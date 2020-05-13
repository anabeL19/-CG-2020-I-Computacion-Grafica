#include <iostream>
#include <math.h>
#define PI 3.14159265359

using namespace std;

class Vector {
    public:
    int r=0;    int c=0;
    float V[4];
    
    Vector(float *v, int _r, int _c){
        this->r = _r;
        this->c = _c;
        for (int i=0; i<this->r; i++) {
            this->V[i] = v[i];
        }

    };
};

class Matrix{
    public:
    int r=0;    int c=0;
    float **M;

    Matrix(){};
    Matrix(int _r, int _c){
        this->r=_r;
        this->c=_c;
        
        M = new float*[this->r];
        for (int i=0; i<_r; ++i) {
            M[i] = new float[this->c];
        }

        for(int i=0; i<this->r; i++) {
            for (int j=0; j<this->c; j++) {
                M[i][j] = 0.0f;
            }
        }
    };
    
    ~Matrix() {
        for (int i=0; i< this->r; i++) 
            delete [] M[i];
        delete [] M;
    } 
    void Identity();   
};
void Matrix::Identity() {
    for (int i=0; i<r; i++) {
        for (int j=0; j<c; j++) {
            if (i==j) M[i][j] = 1.0f;
            else M[i][j] = 0.0f;
        }
    }
}
class Op{
    public:
    Matrix *mtz;
    void Rotation_x(double);
    void Rotation_y(double);
    void Rotation_z(double);
    void Scale(Vector &);
    void Traslation(Vector &);
    void MultMatrixVector(Vector , Matrix &);
    Op(Matrix *m){
        this->mtz = m; 
    };
};

void Op::Rotation_x(double angle){
    double angle_radian = angle * (PI / 180.0);
    mtz->M[1][1] = cos(angle_radian); 
    mtz->M[1][2] = -sin(angle_radian);
    mtz->M[2][1] = sin(angle_radian);
    mtz->M[2][2] = cos(angle_radian);
}

void Op::Rotation_y(double angle){
    double angle_radian = angle * (PI / 180.0);
    mtz->M[0][0] = cos(angle_radian); 
    mtz->M[0][2] = sin(angle_radian);
    mtz->M[2][0] = -sin(angle_radian);
    mtz->M[2][2] = cos(angle_radian);
}

void Op::Rotation_z(double angle){
    double angle_radian = angle * (PI / 180.0);
    mtz->M[0][0] = cos(angle_radian); 
    mtz->M[0][1] = -sin(angle_radian);
    mtz->M[1][0] = sin(angle_radian);
    mtz->M[1][1] = cos(angle_radian);
}

void Op::Scale(Vector &v){
    mtz->M[0][0] = v.V[0]; 
    mtz->M[1][1] = v.V[1];
    mtz->M[2][2] = v.V[2];
}

void Op::Traslation(Vector &v){
    mtz->M[0][3] = mtz->M[0][3] + v.V[0]; 
    mtz->M[1][3] = mtz->M[1][3] + v.V[1];
    mtz->M[2][3] = mtz->M[2][3] + v.V[2];
}

void Op::MultMatrixVector(Vector v, Matrix &r){
    int k=0;
    // std::cout<<"c-r: "<<mtz->r<<" "<<mtz->c<<"\n";
    for (int i=0; i<mtz->r; i++) {
        for (int j=0; j<mtz->c; j++) {
            for (int k=0; k<v.r; k++) {
                r.M[i][j] = r.M[i][j] + mtz->M[i][k] * v.V[k];
            }
        }
    }
}

void p_rint_m (Matrix &mtz) {
    std::cout<<"Print Matrix! \n";
    std::cout<<"dimensions>> "<<mtz.r<<" "<<mtz.c<<"\n";
    for (int i=0; i<mtz.r; i++) {
        for ( int j=0; j<mtz.c; j++) {
            std::cout<<mtz.M[i][j]<<" ";
        }
        std::cout<<"\n";
    }
}
void p_rint (Vector v){
    std::cout<<"Print Vector! \n";
    std::cout<<"dimensions>> "<<v.r<<" "<<v.c<<"\n";
    for (int i=0; i<v.r; i++) {
        std::cout<<v.V[i]<<" ";
    }
    std::cout<<"\n";
}