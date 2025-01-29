#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Simplex
{
private:
    vector<vector<double>> tableau;
    int numConstraints, numVariables;

public:
    Simplex(vector<vector<double>> &A, vector<double> &b, vector<double> &c)
    {
        numConstraints = b.size();
        numVariables = c.size();
        tableau.resize(numConstraints + 1, vector<double>(numVariables + numConstraints + 1, 0));

        for (int i = 0; i < numConstraints; i++)
        {
            for (int j = 0; j < numVariables; j++)
            {
                tableau[i][j] = A[i][j];
            }
            tableau[i][numVariables + i] = 1;
            tableau[i].back() = b[i];
        }

        for (int j = 0; j < numVariables; j++)
        {
            tableau[numConstraints][j] = -c[j];
        }
    }

    void printTableau()
    {
        cout << "\nCurrent Simplex Tableau:" << endl;
        for (const auto &row : tableau)
        {
            for (double val : row)
            {
                cout << setw(10) << fixed << setprecision(2) << val << " ";
            }
            cout << endl;
        }

        vector<double> zj(numVariables + numConstraints, 0);
        vector<double> cjMinusZj(numVariables + numConstraints, 0);

        for (int j = 0; j < numVariables + numConstraints; j++)
        {
            for (int i = 0; i < numConstraints; i++)
            {
                zj[j] += tableau[i][j] * tableau[numConstraints][numVariables + numConstraints];
            }
            cjMinusZj[j] = tableau[numConstraints][j] - zj[j];
        }

        cout << "Zj Row: " << endl;
        for (double val : zj)
        {
            cout << setw(10) << fixed << setprecision(2) << val << " ";
        }
        cout << endl;

        cout << "Cj - Zj Row: " << endl;
        for (double val : cjMinusZj)
        {
            cout << setw(10) << fixed << setprecision(2) << val << " ";
        }
        cout << endl;
    }

    bool isOptimal()
    {
        for (int j = 0; j < numVariables + numConstraints; j++)
        {
            if (tableau[numConstraints][j] < 0)
                return false;
        }
        return true;
    }

    void pivot(int pivotRow, int pivotCol)
    {
        double pivotValue = tableau[pivotRow][pivotCol];
        for (double &val : tableau[pivotRow])
            val /= pivotValue;

        for (int i = 0; i <= numConstraints; i++)
        {
            if (i != pivotRow)
            {
                double factor = tableau[i][pivotCol];
                for (int j = 0; j <= numVariables + numConstraints; j++)
                {
                    tableau[i][j] -= factor * tableau[pivotRow][j];
                }
            }
        }
    }

    void solve()
    {
        while (!isOptimal())
        {
            int pivotCol = 0;
            for (int j = 1; j < numVariables + numConstraints; j++)
            {
                if (tableau[numConstraints][j] < tableau[numConstraints][pivotCol])
                    pivotCol = j;
            }

            int pivotRow = -1;
            double minRatio = 1e9;
            for (int i = 0; i < numConstraints; i++)
            {
                if (tableau[i][pivotCol] > 0)
                {
                    double ratio = tableau[i].back() / tableau[i][pivotCol];
                    if (ratio < minRatio)
                    {
                        minRatio = ratio;
                        pivotRow = i;
                    }
                }
            }

            if (pivotRow == -1)
            {
                cout << "Unbounded solution." << endl;
                return;
            }

            pivot(pivotRow, pivotCol);
            printTableau();
        }

        cout << "Optimal solution found." << endl;
    }
};

int main()
{
    vector<vector<double>> A = {{2, 1}, {1, 2}};
    vector<double> b = {20, 20};
    vector<double> c = {3, 5};

    Simplex simplex(A, b, c);
    simplex.printTableau();
    simplex.solve();

    return 0;
}
