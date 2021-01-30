//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// p0_starter.h
//
// Identification: src/include/primer/p0_starter.h
//
// Copyright (c) 2015-2020, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>

namespace bustub {

/*
 * The base class defining a Matrix
 */
template <typename T>
class Matrix {
 protected:

  // TODO(P0): Add implementation
  Matrix(int r, int c) {
		this->rows = r;
		this->cols = c;		
		//init the linear
		if(r*c!=0) {
			linear = new T[r*c];
			for(int i=0;i<r*c;i++) {
				linear[i] = 0;
			}		
		}
		else linear = NULL;
	}

  int rows;
  int cols;

  // Flattened array containing the elements of the matrix
  // TODO(P0) : Allocate the array in the constructor. Don't forget to free up
  // the array in the destructor.
  T *linear;

	
 public:
  virtual int GetRows() = 0;//you can rewrite it in the derived classes

  virtual int GetColumns() = 0;

  virtual T GetElem(int i, int j) = 0;

  virtual void SetElem(int i, int j, T val) = 0;

  // Sets the matrix elements based on the array arr
  virtual void MatImport(T *arr) = 0;

  // TODO(P0): Add implementation
  virtual ~Matrix() {
		delete [] linear;
	}
};



template <typename T>//?
class RowMatrix : public Matrix<T> {
 public:

  // TODO(P0): Add implementation
  RowMatrix(int r, int c) : Matrix<T>(r, c) {
		data_ = new T* [r];
		for(int i=0;i<r;i++){
			data_[i] = new T[c];		
		}
		for(int i=0;i<r;i++) {
			for(int j=0;j<c;j++) {
				data_[i][j] = 0;			
			}
		}
		

	}

  // TODO(P0): Add implementation
  int GetRows() override { return this->rows; }

  // TODO(P0): Add implementation
  int GetColumns() override { return this->cols; }

  // TODO(P0): Add implementation
  T GetElem(int i, int j) override { 
		if(i<0||j<0||i>=this->GetRows()||j>=this->GetColumns())return 0;
		return data_[i][j]; 
	}

  // TODO(P0): Add implementation
  void SetElem(int i, int j, T val) override {
  		if(i<0||j<0||i>=this->GetRows()||j>=this->GetColumns())return;
  		int col = this->GetColumns();
  		this->linear[i*col+j] = val;
  		data_[i][j] = val;
  }

  // TODO(P0): Add implementation
  void MatImport(T *arr) override {
  		int tot = 0;
  		int row = this->GetRows();
  		int col = this->GetColumns();
  		for(int i=0;i<row;i++) {
  			for(int j=0;j<col;j++) {
  				this->linear[tot] = arr[tot];
  				data_[i][j] = arr[tot];
  				++tot;
  			}
  		}
  }

  // TODO(P0): Add implementation
  ~RowMatrix() override {
  	int row = this->GetRows();
  	for(int i=0;i<row;i++) {
  		delete [] data_[i];
  	}
  	delete [] data_;
  }

 private:
  // 2D array containing the elements of the matrix in row-major format
  // TODO(P0): Allocate the array of row pointers in the constructor. Use these pointers
  // to point to corresponding elements of the 'linear' array.
  // Don't forget to free up the array in the destructor.
  T **data_;
};

template <typename T>
class RowMatrixOperations {
 public:
  // Compute (mat1 + mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  //gua:why it is unique?
  static std::unique_ptr<RowMatrix<T>> AddMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                   std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
  	if(mat1->GetRows()!=mat2->GetRows()||mat1->GetColumns()!=mat2->GetColumns())
  		return std::unique_ptr<RowMatrix<T>>(nullptr);
  	int row = mat1->GetRows(),col = mat1->GetColumns();
  	std::unique_ptr<RowMatrix<T>> mat(new RowMatrix<T>(row,col));
  	for(int i=0;i<row;i++) {
  		for(int j=0;j<col;j++) {
  			mat->SetElem(i,j,mat1->GetElem(i,j)+mat2->GetElem(i,j));
  		}
  	}
  	return mat;
  }

  // Compute matrix multiplication (mat1 * mat2) and return the result.
  // Return nullptr if dimensions mismatch for input matrices.
  static std::unique_ptr<RowMatrix<T>> MultiplyMatrices(std::unique_ptr<RowMatrix<T>> mat1,
                                                        std::unique_ptr<RowMatrix<T>> mat2) {
    // TODO(P0): Add code
  	if(mat1->GetColumns()!=mat2->GetRows()) {
  	    return std::unique_ptr<RowMatrix<T>>(nullptr);
  	}  
  	//(a*b) (b*c)
  	int a = mat1->GetRows(),b = mat1->GetColumns(),c = mat2->GetColumns();
  	std::unique_ptr<RowMatrix<T>> mat(new RowMatrix<T>(a,c));
  	for(int i=0;i<a;i++) {
  		for(int j=0;j<c;j++) {
  			int ans = 0;
  			for(int k=0;k<b;k++) {
  				ans+=mat1->GetElem(i,k)+mat2->GetElem(k,j);
  			}
  			mat->SetElem(i,j,ans);
  		}
  	}
  	return mat;

  }

  // Simplified GEMM (general matrix multiply) operation
  // Compute (matA * matB + matC). Return nullptr if dimensions mismatch for input matrices
  static std::unique_ptr<RowMatrix<T>> GemmMatrices(std::unique_ptr<RowMatrix<T>> matA,
                                                    std::unique_ptr<RowMatrix<T>> matB,
                                                    std::unique_ptr<RowMatrix<T>> matC) {
    // TODO(P0): Add code
  	std::unique_ptr<RowMatrix<T>>temp = MultiplyMatrices(matA,matB);
  	if(temp==nullptr)
	    return std::unique_ptr<RowMatrix<T>>(nullptr);
	else return AddMatrices(temp,matC);
  }
};
}  // namespace bustub
