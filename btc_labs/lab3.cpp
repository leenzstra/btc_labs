#include <omp.h>
#include <chrono>
#include <iostream>

void mat_fill(int** mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        mat[i] = new int[cols];
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = rand() % 100 + 1;
        }
    }
    return;
}

void mat_reset(int** mat, int rows, int cols) {
    for (int i = 0; i < rows; i++)
    {
        mat[i] = new int[cols];
        for (int j = 0; j < cols; j++)
        {
            mat[i][j] = 0;
        }
    }

}

void mat_print(int** mat, int rows, int cols)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            std::cout << mat[i][j] << "\t";
        }
        std::cout << std::endl;
    }
}

long long mat_multiply_singlethread(int** mat_a, int** mat_b, int** mat_result, int rows, int cols)
{
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            for (int k = 0; k < cols; k++)
            {
                mat_result[i][j] += (mat_a[i][k] * mat_b[k][j]);
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return elapsed.count();
}


long long mat_multiply_signethread_optimal(int** mat_a, int** mat_b, int** mat_result, int rows, int cols)
{
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < rows; i++)
    {
        for (int k = 0; k < cols; k++)
        {
            for (int j = 0; j < cols; j++)
            {
                mat_result[i][j] += (mat_a[i][k] * mat_b[k][j]);
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return elapsed.count();
}

long long mat_multiply_multithread(int** mat_a, int** mat_b, int** mat_result, int rows, int cols)
{
    auto start = std::chrono::steady_clock::now();

    int i, j, k;
#pragma omp parallel for shared(mat_a, mat_b, mat_result) 
    for (i = 0; i < rows; i++)
    {
        for (k = 0; k < cols; k++)
        {
            for (j = 0; j < cols; j++)
            {
                mat_result[i][j] += (mat_a[i][k] * mat_b[k][j]);
            }
        }
    }
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return elapsed.count();
}

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    int m_size = 1000;
    long long operation_time = 0;

    int** mat_a = new int* [m_size];
    int** mat_b = new int* [m_size];
    int** mat_result = new int* [m_size];

    mat_fill(mat_a, m_size, m_size);
    mat_fill(mat_b, m_size, m_size);
    mat_reset(mat_result, m_size, m_size);

    if (m_size <= 10)
    {
        std::cout << "Матрица А" << std::endl;
        mat_print(mat_a, m_size, m_size);

        std::cout << "Матрица B" << std::endl;
        mat_print(mat_b, m_size, m_size);
    }

    operation_time = mat_multiply_singlethread(mat_a, mat_b, mat_result, m_size, m_size);
    std::cout << "\nПоследовательное умножение \t" << operation_time << " мс" << std::endl;
    if (m_size <= 10)
    {
        std::cout << "Результат" << std::endl;
        mat_print(mat_result, m_size, m_size);
    }
    mat_reset(mat_result, m_size, m_size);

    operation_time = mat_multiply_signethread_optimal(mat_a, mat_b, mat_result, m_size, m_size);
    std::cout << "\nОптимальное последовательное умножение (i k j) \t" << operation_time << " мс" << std::endl;
    if (m_size <= 10)
    {
        std::cout << "Результат" << std::endl;
        mat_print(mat_result, m_size, m_size);
    }
    mat_reset(mat_result, m_size, m_size);

    operation_time = mat_multiply_multithread(mat_a, mat_b, mat_result, m_size, m_size);
    std::cout << "\nПараллельное умножение \t" << operation_time << " мс" << std::endl;
    if (m_size <= 10)
    {
        std::cout << "Результат" << std::endl;
        mat_print(mat_result, m_size, m_size);
    }
    mat_reset(mat_result, m_size, m_size);
}
