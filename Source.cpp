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


int main(){
    //Giai bai toan 2x2
    double gameValue_2x2 = 0;
    matrix beneficialMatrix_2x2(2, 2);
    beneficialMatrix_2x2.setMatrix(); 
    double maximin = beneficialMatrix_2x2[beneficialMatrix_2x2.maximinIndex()[0].first][beneficialMatrix_2x2.maximinIndex()[0].second];
    double minimax = beneficialMatrix_2x2[beneficialMatrix_2x2.minimaxIndex()[0].first][beneficialMatrix_2x2.minimaxIndex()[0].second];
    if(abs(maximin - minimax) < 1e-9){
        gameValue_2x2 = maximin;    
        vector<pair<int, int>> pureStrategy_2x2;
        pureStrategy_2x2 = beneficialMatrix_2x2.maximinIndex();
        int size = pureStrategy_2x2.size();
        for(int i = 0;i < size;i++){
            cout << "Pure strategy #" << i + 1 << ": " << pureStrategy_2x2[i].first << " " << pureStrategy_2x2[i].second << '\n';
        }
    }else{
        double delta = beneficialMatrix_2x2[0][0] - beneficialMatrix_2x2[0][1] - beneficialMatrix_2x2[1][0] + beneficialMatrix_2x2[1][1];
        gameValue_2x2 = beneficialMatrix_2x2.det() / delta;
        double p = (beneficialMatrix_2x2[1][1] - beneficialMatrix_2x2[1][0]) / delta;
        double q = (beneficialMatrix_2x2[1][1] - beneficialMatrix_2x2[0][1]) / delta;
        cout << "First player strategy: " << p << " " << 1 - p << '\n';
        cout << "Second player strategy: " << q << " " << 1 - q << '\n';
    }
    cout << "Game Value: " << gameValue_2x2 << '\n';
}