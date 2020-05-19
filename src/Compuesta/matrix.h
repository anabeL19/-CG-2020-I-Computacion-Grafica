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
    int num_el;

    Matrix(){};
    Matrix(int _r, int _c){
        this->r=_r;
        this->c=_c;
        int cant = 0;
        M = new float*[this->r];
        for (int i=0; i<_r; ++i) {
            M[i] = new float[this->c];
        }

        for(int i=0; i<this->r; i++) {
            for (int j=0; j<this->c; j++) {
                M[i][j] = 0.0f;
                cant++;
                this->num_el = cant;
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
    void MultMatrixMatrix(Matrix &m, Matrix &r);

    int Determinant(Matrix &m, int cant_el); 
    void GetCofactor(Matrix &m, Matrix &tmp, int p, int q, int n);
    void Adjoint(Matrix &m, Matrix &adj);  
    void MatrixInverse(Matrix &m, Matrix &inverse);
    Op(Matrix *m){
        this->mtz = m; 
    };
    Op(){};
};

void Op::Rotation_x(double angle)
{
    double angle_radian = angle * (PI / 180.0);
    // double angle_radian = angle;
    mtz->M[1][1] = cos(angle_radian); 
    mtz->M[1][2] = -sin(angle_radian);
    mtz->M[2][1] = sin(angle_radian);
    mtz->M[2][2] = cos(angle_radian);
}

void Op::Rotation_y(double angle)
{
    double angle_radian = angle * (PI / 180.0);
    // double angle_radian = angle;
    mtz->M[0][0] = cos(angle_radian); 
    mtz->M[0][2] = sin(angle_radian);
    mtz->M[2][0] = -sin(angle_radian);
    mtz->M[2][2] = cos(angle_radian);
}

void Op::Rotation_z(double angle)
{
    double angle_radian = angle * (PI / 180.0);
    // double angle_radian = angle;
    mtz->M[0][0] = cos(angle_radian); 
    mtz->M[0][1] = -sin(angle_radian);
    mtz->M[1][0] = sin(angle_radian);
    mtz->M[1][1] = cos(angle_radian);
}

void Op::Scale(Vector &v)
{
    mtz->M[0][0] = v.V[0]; 
    mtz->M[1][1] = v.V[1];
    mtz->M[2][2] = v.V[2];
}

void Op::Traslation(Vector &v)
{
    mtz->M[0][3] = mtz->M[0][3] + v.V[0]; 
    mtz->M[1][3] = mtz->M[1][3] + v.V[1];
    mtz->M[2][3] = mtz->M[2][3] + v.V[2];
}

void Op::MultMatrixVector(Vector v, Matrix &r)
{
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

void Op::MultMatrixMatrix(Matrix &m, Matrix &r)
{
    int k=0;
    for (int i=0; i<mtz->r; i++) {
        for (int j=0; j<mtz->c; j++) {
            for (int k=0; k<m.r; k++) {
                r.M[i][j] = r.M[i][j] + mtz->M[i][k] * m.M[k][j];
            }
        }
    }
}

// --------------------------------     Inverse 

// Function to get cofactor of m in tmp. n is current 
void Op::GetCofactor(Matrix &m, Matrix &tmp, int r, int c, int n) 
{ 
	int i1 = 0, j1 = 0; 

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) { 
			// Copying into tmp matrix only those element  which are not in given row and column 
			if (i != r && j != c) { 
				tmp.M[i1][j1] = m.M[i][j]; 
                j1++;
                
				// Row is filled, so increase row index and reset col 
                if (j1 >= n - 1) { 
					j1 = 0; 
					i1++; 
				} 
			} 
		} 
    }
} 

int Op::Determinant(Matrix &m, int r) 
{ 
	int D = 0; 

	// Base case: matrix contains single element 
	if (r == 1) 
		return m.M[0][0]; 

    Matrix tmp(m.r, m.c);
	int sign = 1; // To store sign multiplier 

	for (int i = 0; i < r; i++) { 
		GetCofactor(m, tmp, 0, i, r); 
		D += sign * m.M[0][i] * Determinant(tmp, r - 1); 
		sign = -sign; 
	} 

	return D; 
} 

// Get adjoint of m in adj. 
void Op::Adjoint(Matrix &m, Matrix &adj) 
{ 
	if (m.r == 1) { 
		adj.M[0][0] = 1; 
		return; 
	} 

	// tmp is used to store cofactors of m 
	int sign = 1;
    Matrix tmp(m.r, m.c); 
	for (int i=0; i<m.r; i++) {
		for (int j=0; j<m.c; j++) { 
			GetCofactor(m, tmp, i, j, m.r); // Get cofactor of m
			// sign of adj positive if sum of row and column indexes is even. 
			sign = ((i+j)%2==0)? 1: -1; 
			// Interchanging rows and columns to get the transpose of the cofactor matrix 
            adj.M[j][i] = (sign)*(Determinant(tmp, m.c-1));
		} 
	} 
} 

void Op::MatrixInverse(Matrix &m, Matrix &inverse)
{
    // Determinant of 
	int det = Determinant(m, m.r); 
    cout<<"det: "<<det<<"\n";
	if (det == 0) 
	{ 
		cout << "Singular matrix, can't find its inverse"; 
		return; 
	} 

	// Find adjoint 
	Matrix adj(m.r, m.c); 
	Adjoint(m, adj);
	// Find Inverse using formula "inverse(A) = adj(A)/det(A)" 
	for (int i=0; i<m.r; i++) 
		for (int j=0; j<m.c; j++) {
			inverse.M[i][j] = adj.M[i][j]/float(det); 
        }
}
// -----------------------------------------------------------------------------

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