#include "det.hpp"

namespace lab2 {

static SizeType GetSize(const Matrix& mat) {
    if (mat.size() == 0) {
        return SizeType(0, 0);
    }
    return SizeType(mat.size(), mat[0].size());
}

static Matrix Mult(const Matrix& mat1, const Matrix& mat2) {
    SizeType s1 = GetSize(mat1), s2 = GetSize(mat2);
    if (s1.second != s2.first) {
        throw std::invalid_argument("Bad matrices size");
    }
    size_t n = s1.first, m = s2.second;
    size_t s = s1.second;
    Matrix result = Matrix(n, Row(m, 0));
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < m; j++) {
            for (size_t k = 0; k < s; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
    return result;
}

static Matrix Pow(const Matrix& mat, size_t n) {
    Matrix result = mat;
    for (size_t i = 1; i < n; i++) {
        result = Mult(result, mat);
    }
    return result;
}

static Matrix Toeplitz(const Matrix& a) {
    SizeType s = GetSize(a);
    size_t n = s.first;
    if (s.first != s.second) {
        throw std::invalid_argument("Bad matrix size");
    }
    Matrix R(1, Row(n - 1, 0)),
           C(n - 1, Row(1, 0)),
           A1(n - 1, Row(n - 1, 0));
    for (size_t i = 0; i < n - 1; i++) {
        R[0][i] = a[0][i + 1];
        C[i][0] = a[i + 1][0];
        for (size_t j = 0; j < n - 1; j++) {
            A1[i][j] = a[i + 1][j + 1];
        }
    }

    Matrix result(n + 1, Row(n, 0));
    for (size_t i = 0; i < n + 1; i++) {
        ll val;

        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = -a[0][0];
        } else if (i == 2) {
            val = -(Mult(R, C))[0][0];
        } else {
            val = -(Mult(Mult(R, Pow(A1, i - 2)), C))[0][0];
        }

        for (size_t j = 0; j < std::min(n, n + 1 - i); j++) {
            result[i + j][j] = val;
        }
    }

    return result;
}

ll Det(const Matrix &a) {
    SizeType s = GetSize(a);
    size_t n = s.first;
    if (s.first != s.second) {
        throw std::invalid_argument("Bad matrix size");
    }
    std::vector<Matrix> source(n);
    std::vector<Matrix> t(n);
    source[0] = a;
    Matrix a1;
    for (size_t i = 1; i < n; i++) {
        a1 = Matrix(n - i, Row(n - i, 0));
        for (size_t j = 0; j < n - i; j++) {
            for (size_t k = 0; k < n - i; k++) {
                a1[j][k] = a[j + i][k + i];
            }
        }
        source[i] = a1;
    }
    for (size_t i = 0; i < n; i++) {
        t[i] = Toeplitz(source[i]);
    }

    Matrix res = t[0];
    for (size_t i = 1; i < n; i++) {
        res = Mult(res, t[i]);
    }

    int sign = (n % 2 == 0 ? 1 : -1);
    return res[n][0] * sign;
}

struct ThreadArgs {
    std::vector<size_t> task;
    std::vector<Matrix> * source;
    std::vector<Matrix> * result;
};

static void *ThreadRoutine(void *args) {
    ThreadArgs* data = (ThreadArgs*)args;
    for (auto i : data->task) {
        (*data->result)[i] = Toeplitz((*data->source)[i]);
    }
    return nullptr;
}

ll ParallelDet(const Matrix& a, size_t threadNum) {
    SizeType s = GetSize(a);
    size_t n = s.first;
    if (s.first != s.second) {
        throw std::invalid_argument("Bad matrix size");
    }
    std::vector<Matrix> source(n);
    std::vector<Matrix> t(n);
    source[0] = a;
    Matrix a1;
    for (size_t i = 1; i < n; i++) {
        a1 = Matrix(n - i, Row(n - i, 0));
        for (size_t j = 0; j < n - i; j++) {
            for (size_t k = 0; k < n - i; k++) {
                a1[j][k] = a[j + i][k + i];
            }
        }
        source[i] = a1;
    }

    // Thread stuff
    std::vector<pthread_t> threads(threadNum);
    std::vector<ThreadArgs> args(threadNum);
    for (size_t i = 0; i < threadNum; i++) {
        args[i] = { {}, &source, &t };
        for (size_t j = i; j < n; j += threadNum) {
            args[i].task.push_back(j);
        }
        pthread_create(&threads[i], nullptr, ThreadRoutine, (void*)&(args[i]));
    }
    for (size_t i = 0; i < threadNum; i++) {
        pthread_join(threads[i], nullptr);
    }
    //

    Matrix res = t[0];
    for (size_t i = 1; i < n; i++) {
        res = Mult(res, t[i]);
    }

    int sign = (n % 2 == 0 ? 1 : -1);
    return res[n][0] * sign;
}

}
