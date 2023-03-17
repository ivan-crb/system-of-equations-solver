#include <iostream>
#include <vector>

#define endl "\n" 

using namespace std;

void printMatrix(vector<vector<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

vector<vector<double>> swapRows(vector<vector<double>> matrix, int row1, int row2) {
    
    vector<double> aux = matrix[row1];
    matrix[row1] = matrix[row2];
    matrix[row2] = aux;

    return matrix;
}

vector<vector<double>> multiplyRow(vector<vector<double>> matrix, int row, double scalar) {

    for (int i = 0; i < matrix[row].size(); i++) {
        if (abs(matrix[row][i] * scalar) < 1e-10) matrix[row][i] = 0;
        else matrix[row][i] *= scalar;
    }

    return matrix;
}

vector<vector<double>> addMultipliedRow(vector<vector<double>> matrix, int row1, int row2, double scalar) {

    for (int i = 0; i < matrix[row1].size(); i++) {
        if (abs(matrix[row1][i] + scalar * matrix[row2][i]) < 1e-10) matrix[row1][i] = 0;
        else matrix[row1][i] = matrix[row1][i] + scalar * matrix[row2][i];
    }

    return matrix;
}

vector<vector<double>> reducedEchelonForm(vector<vector<double>> matrix) {
    int columns = matrix[0].size();
    int rows = matrix.size();

    int pivotRow = 0;
        for (int c = 0; c < columns - 1; c++) {
            
            bool allZeros = true;
            for (int r = pivotRow; r < rows && allZeros; r++) {
                if (matrix[r][c] != 0) allZeros = false;
            }
            if (allZeros) continue;
            
            // The current column is not all zeros
            if (matrix[pivotRow][c] == 0) {
                // Switch for a non-zero row
                int nonZeroRow = -1;
                for (int r = pivotRow; r < rows && nonZeroRow == -1; r++) {
                    if (matrix[r][c] != 0) nonZeroRow = r;
                }
                matrix = swapRows(matrix, pivotRow, nonZeroRow);
            }
            // Make the pivot 1
            matrix = multiplyRow(matrix, pivotRow, 1 / matrix[pivotRow][c]);

            // Make zeros above and below the pivot
            for (int r = 0; r < rows; r++) {
                if (matrix[r][c] == 0 || r == pivotRow) continue;
                matrix = addMultipliedRow(matrix, r, pivotRow, -matrix[r][c]);
            }
            
            pivotRow++;
        }
        return matrix;
}

int matrixRange(vector<vector<double>> matrix) {
    int columns = matrix[0].size();
    int rows = matrix.size();

    int range = matrix.size() - 1;
    bool allZeros = true;
    while (range >= 0 && allZeros) {
        for (int c = 0; c < columns && allZeros; c++) {
            if (matrix[range][c] != 0) allZeros = false;
        }
        if (allZeros) range--;
        else range++;
    }
    return range;
}


int main() {
// 89 121
// 78 110
// 32
    //char a = 'N';
    //char b = 'n';

    //cout << (int) a << " " << (int) b;
    
    cout << "Starting calculator..." << endl;     //       "type end in the console"

    bool doProcedure = true;
    
    while (doProcedure) {

        confirmation:
        cout << "Proceed to introduce a system of equations? (Y/N): ";

        char boo;
        cin >> boo;
        if (boo == 'N' || boo == 'n') {
            doProcedure = false;
            continue;
        } else if (boo != 'Y' && boo != 'y') goto confirmation;

        cout << "> Enter the number of equations: ";

        int equationNum;
        cin >> equationNum;

        cout << "> Enter the number of variables, followed by their names (separating everything with a space): ";

        int varNum;
        cin >> varNum;

        vector<string> varNames;
        for (int i = 0; i < varNum; i++) {
            string v;
            cin >> v;
            varNames.push_back(v);
        }
        
        // Create matrix with the data
        vector<vector<double>> equations; // equations[row][column]
        vector<vector<double>> equationsB;
        for (int i = 1; i <= equationNum; i++) {
            vector<double> eq;
            vector<double> eqB;
            cout << "> Enter the coefficients of the " << i << " equation:" << endl;
            for (int j = 0; j < varNum; j++) {
                cout << "  " << varNames[j] + " = ";
                double coeff;
                cin >> coeff;
                eq.push_back(coeff);
                eqB.push_back(coeff);
            }
            cout << "  " << "Constant = ";
            double coeff;
            cin >> coeff;
            eq.push_back(coeff);

            equations.push_back(eq);
            equationsB.push_back(eqB);
        }

         /*
        equations = {
            {3, 6, 1 , 4, 8},
            {1, 2, 1, 2, 4},
            {4, 8, 2, 6, 12},
            {2, 4, 0, 2, 4}
        };
        equationsB = {
            {3, 6, 1 , 4},
            {1, 2, 1, 2},
            {4, 8, 2, 6},
            {2, 4, 0, 2}
        };
        varNames = {"x", "y", "z", "t"};
        */
        int rows = equationNum;
        int columns = varNum + 1;
        /*
        rows = 4;
        columns = 5;
         */
        

        // Obtaining the reduced echelon form
        
        equations = reducedEchelonForm(equations);
        equationsB = reducedEchelonForm(equationsB);

        //printMatrix(equations);
        //cout << endl;
        //printMatrix(equationsB);

        // Obtaining the range

        int range = matrixRange(equations);
        int rangeB = matrixRange(equationsB);

        // cout << "Ranges: " << range << " | " << rangeB;

        if (range != rangeB) {
            cout << "- Incompatible system, the system has no solution." << endl;
            continue;
        }
        else if (range != varNum) {
            cout << "- Indeterminate compatible system, the system has infinitely many solutions:" << endl;
        }
        else {
            cout << "- Determined compatible system, the system has a single unique solution:" << endl;
        }

        printMatrix(equations);

        // Find pivots
        vector<pair<int, int>> pivots;
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < columns - 1; c++) {
                if (equations[r][c] != 0) { 
                    pivots.push_back({r, c});
                    r++;
                    cout << "numberOFpivots=" << pivots.size();
                
                    cout << " go" << endl;
                }
            }
            cout << "   dasda " << endl;
        }

        cout << "CHECK " << endl;

        printMatrix(equations);

        // Print the regression substitution
        for (int i = 0; i < varNames.size(); i++) {
            // Finds whether the current variable is pivot
            int pivotPos = -1;
            for (int j = 0; j < pivots.size() && pivotPos == -1; j++) {
                if (pivots[j].second == i) pivotPos = j;
            }

            if (pivotPos != -1) { // Pivot
                int r = pivots[pivotPos].first;
                cout << varNames[i] << " = ";
                bool first = true;
                for (int c = pivots[pivotPos].second + 1; c < columns; c++) {
                    
                    if (equations[r][c] == 0) continue;

                    if (!first) cout << "+ ";
                    if (first) first = false;

                    if (c == varNames.size()) cout << equations[r][c] << " ";
                    else cout << -equations[r][c] << varNames[c] << " ";
                }
                cout << endl;
            }
            else { // Not pivot
                cout << varNames[i] << " = " << varNames[i] << endl;
            }


        }





    }
}
