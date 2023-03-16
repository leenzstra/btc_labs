#include <iostream>
#include <chrono>
#include <omp.h>
#include <vector>


std::vector<double> gauss(std::vector<double> coef_mat, std::vector<double> b, int n) {
	std::vector<double> x(n);

	auto start = std::chrono::steady_clock::now();

#pragma omp parallel
	{
#pragma omp for
		for (int k = 0; k < n - 1; k++) {

			double pivot = coef_mat[k * n + k];
			for (int i = k + 1; i < n; i++) {

				double lik = coef_mat[i * n + k] / pivot;
				for (int j = k; j < n; j++)
					coef_mat[i * n + j] -= lik * coef_mat[k * n + j];
				b[i] -= lik * b[k];
			}
		}
#pragma omp for
		for (int k = n - 1; k >= 0; k--)
		{
			double s = 0;

			for (int i = k + 1; i < n; i++)
				s += coef_mat[k * n + i] * x[i];
			x[k] = (b[k] - s) / coef_mat[k * n + k];
		}
	}
	auto end = std::chrono::steady_clock::now();

	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Время решения СЛАУ методом Гаусса\t" << elapsed.count() << " мс\n";
	return x;
}



int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Russian");

	omp_set_num_threads(12);

	int eq_count = 5000;
	std::vector<double> coef_matrix(eq_count * eq_count);
	std::vector<double> b(eq_count); //свободные члены

	for (int i = 0; i < eq_count * eq_count; i++)
	{
		coef_matrix[i] = rand() % 100 + 1;
	}
	for (int i = 0; i < eq_count; i++)
	{
		b[i] = rand() % 100 + 1;
	}

	std::vector<double> x = gauss(coef_matrix, b, eq_count);

	/*for (int i = 0; i < eq_count; i++)
	{
		std::cout << "\nx" << i + 1 << " = " << x[i];
	}*/
}
