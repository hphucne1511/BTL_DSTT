#include <iostream>
#include <vector>
#include <cmath>


using namespace std;


//Thiet lap class ma tran
class matrix{
    protected:
        int rows;
        int cols;
        vector<vector<double>> mat;
    public:
        matrix(int, int);
        matrix(int, int, double);
        void setMatrix();
        vector<vector<double>> getMatrix();
        double det();
        matrix subMat(int, int);
        vector<pair<int, int>> maximinIndex();
        vector<pair<int, int>> minimaxIndex();
        vector<double>& operator[](int);
        friend void mult(vector<double>&, double);
        friend void elemRowTran(vector<double>&, vector<double>, double);
        void makePositive();
        matrix makeSimplexMatrix();
        void solve2x2();
        void solvemxn();
};




//Constructor 1
matrix::matrix(int rows, int cols){
    this->rows = rows;
    this->cols = cols;
    mat.assign(rows, vector<double>(cols, 0));
}
//Constructor 2
matrix::matrix(int rows, int cols, double val){
    this->rows = rows;
    this->cols = cols;
    mat.assign(rows, vector<double>(cols, val)); 
}
//Thiet lap gia tri ma tran
void matrix::setMatrix(){
    for(int i = 0;i < this->rows;i++){
        for(int j = 0;j < this->cols;j++){
            double val;
            cin >> val;
            this->mat[i][j] = val;
        }
    }
}
//Lay ma tran
vector<vector<double>> matrix::getMatrix(){
    return this->mat;
}
//Ham tinh dinh thuc
double matrix::det(){
    if(this->cols != this->rows) return 0.0;
    if(this->cols == 1) return (double)this->mat[0][0];
    if(this->cols == 2) return (double)this->mat[0][0] * this->mat[1][1] - (double)this->mat[0][1] * this->mat[1][0];
    double res = 0;
    for(int i = 0;i < this->cols;i++){
        if(i % 2 == 0){
            res += this->mat[i][0] * (this->subMat(i, 0)).det();
        }
        else{
            res -= this->mat[i][0] * (this->subMat(i, 0)).det();
        }
    }
    return res;
}
//Ma tran bu
matrix matrix::subMat(int row, int col) {
    matrix res(this->rows - 1, this->cols - 1);
    for (int i = 0; i < this->rows; i++) {
        if (i == row) continue;
        int ri = (i < row) ? i : i - 1;
        for (int j = 0; j < this->cols; j++) {
            if (j == col) continue;
            int rj = (j < col) ? j : j - 1;
            res.mat[ri][rj] = this->mat[i][j];
        }
    }
    return res;
}
//Ham tim maximin
vector<pair<int, int>> matrix::maximinIndex(){
    vector<pair<int, int>> indices;
    double maximin = -10e20;
    for(int i = 0;i < this->rows;i++){
        double rowMin = 10e20;
        for(int j = 0;j < this->cols;j++){
            rowMin = this->mat[i][j] < rowMin?this->mat[i][j]:rowMin;
        }
        maximin = rowMin > maximin?rowMin:maximin;
    }
    for(int i = 0;i < this->rows;i++){
        for(int j = 0;j < this->cols;j++){
            if(this->mat[i][j] == maximin) indices.push_back({i,j});
        }
    }
    return indices;
}
//Ham tim minimax
vector<pair<int, int>> matrix::minimaxIndex(){
    vector<pair<int, int>> indices;
    double minimax = 10e20;
    for(int j = 0;j < this->cols;j++){
        double colMax = -10e20;
        for(int i = 0;i < this->rows;i++){
            colMax = this->mat[i][j] > colMax?this->mat[i][j]:colMax;
        }
        minimax = colMax < minimax?colMax:minimax;
    }
    for(int i = 0;i < this->rows;i++){
        for(int j = 0;j < this->cols;j++){
            if(this->mat[i][j] == minimax) indices.push_back({i,j});
        }
    }
    return indices;
}
//Phep lay gia tri
vector<double>& matrix::operator[](int row){
    return this->mat[row];
}
//Nhan vector voi mot hang so
void mult(vector<double>& vec, double n){
    int size = vec.size();
    for(int i = 0;i < size;i++){
        vec[i] *= n;
    }
}
//Bien doi so cap tren hang
void elemRowTran(vector<double>& vec1, vector<double> vec2, double n){
    int size1 = vec1.size();
    int size2 = vec2.size();
    if(size1 != size2) return;
    for(int i = 0;i < size1;i++){
        vec1[i] = vec1[i] + n * vec2[i];
    }
}
//Duong hoa moi phan tu
void matrix::makePositive(){
    double minMat = 10e30;
    for(int i = 0;i < this->rows;i++){
        for(int j = 0;j < this->cols;j++){
            if(minMat > (*this)[i][j]) minMat = (*this)[i][j];
        }
    }
    if(minMat < 0){
        for(int i = 0;i < this->rows;i++){
            for(int j = 0;j < this->cols;j++){
                (*this)[i][j] += minMat;
            }
        }
    }
}
//Chuyen thanh ma tran de dung thuat toan simplex
matrix matrix::makeSimplexMatrix(){
    matrix simplexMat(this->rows + 1, this->cols + this->rows + 1);
    for(int i = 0;i < this->rows;i++){
        for(int j = 0;j < this->cols;j++){
            simplexMat[i][j] = (*this)[i][j];
            simplexMat[i][this->cols + i] = 1;
            simplexMat[i][this->cols + this->rows] = 1;
        }
    }
    return simplexMat;
}


//Giai 2x2
void matrix::solve2x2(){
    double gameValue_2x2 = 0;
    double maximin = (*this)[this->maximinIndex()[0].first][this->maximinIndex()[0].second];
    double minimax = (*this)[this->minimaxIndex()[0].first][this->minimaxIndex()[0].second];
    if(abs(maximin - minimax) < 1e-9){
        gameValue_2x2 = maximin;    
        vector<pair<int, int>> pureStrategy_2x2 = this->maximinIndex();
        int size = pureStrategy_2x2.size();
        for(int i = 0; i < size; i++){
            cout << "Pure strategy #" << i + 1 << ": " << pureStrategy_2x2[i].first << " " << pureStrategy_2x2[i].second << '\n';
        }
    } else {
        double delta = (*this)[0][0] - (*this)[0][1] - (*this)[1][0] + (*this)[1][1];
        gameValue_2x2 = this->det() / delta;
        double p = ((*this)[1][1] - (*this)[1][0]) / delta;
        double q = ((*this)[1][1] - (*this)[0][1]) / delta;
        cout << "First player strategy: " << p << " " << 1 - p << '\n';
        cout << "Second player strategy: " << q << " " << 1 - q << '\n';
    }
    cout << "Game Value: " << gameValue_2x2 << '\n';
}


//Giai mxn
void matrix::solvemxn(){
    double gameValue_mxn = 0;
    double maximin = (*this)[this->maximinIndex()[0].first][this->maximinIndex()[0].second];
    double minimax = (*this)[this->minimaxIndex()[0].first][this->minimaxIndex()[0].second];
    if(abs(maximin - minimax) < 1e-9){
        gameValue_mxn = maximin; 
        vector<pair<int, int>> pureStrategy_mxn = this->maximinIndex();
        int size = pureStrategy_mxn.size();
        for(int i = 0; i < size; i++){
            cout << "Pure strategy #" << i + 1 << ": " << pureStrategy_mxn[i].first << " " << pureStrategy_mxn[i].second << '\n';
        }
    }else{
        this->makePositive();
        matrix simplexMat = this->makeSimplexMatrix();
        int newMaxRow = this->rows + 1;
        int newMaxCol = this->rows + this->cols + 1;
    }
}


int main(){
    //Giai bai toan 2x2
    matrix beneficialMatrix_2x2(2, 2);
    beneficialMatrix_2x2.setMatrix(); 
    beneficialMatrix_2x2.solve2x2();
    //Giai bai toan tong quat mxn
    int m,n;
    cin >> m >> n;
    matrix beneficialMatrix_mxn(m, n);
    beneficialMatrix_mxn.setMatrix();

}