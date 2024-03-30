import java.util.Arrays;
//import java.lang.Exception;

class hash implements Comparable<hash> {
    public int val, pos;

    public hash(int val, int pos) {
        this.val = val;
        this.pos = pos;
    }

    @Override
    public int compareTo(hash p) {
        return Integer.bitCount(p.val) - Integer.bitCount(this.val);
    }
}

class SudokuSolver {
    int N = 9;
    int[][] grid ;
    int[] col = new int[10], row = new int[10], sub = new int[10];

    boolean validate() {
        for (int r = 0; r < 9; r++) {
            int row = 0, col = 0, box = 0;
            for (int c = 0; c < 9; c++) {
                int i = (r % 3) * 3 + c % 3, j = (r / 3) * 3 + c / 3;
                row ^= 1 << grid[r][c];
                col ^= 1 << grid[c][r];
                box ^= 1 << grid[j][i];
            }
            if (row != 1022 || col != 1022 || box != 1022) {
                return false;
            }
        }
        return true;
    }
    static public void display(int[][] board) {
        System.out.print("\n");

        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++) {
                var dig = board[y][x];

                if (dig == 0) {
                    System.out.print('.');
                } else {
                    System.out.print(dig);
                }
                System.out.print(' ');
            }
            System.out.print("\n");
        }
    }

    boolean exist (int mask, int bit) { return (mask >> bit &1 ) == 1; }

    void showbit (int mask) {
        for (int i = 1; i <= N; i++) {
            System.out.print(mask >> i &1);
        }
    }
    private int getbit (int mask) {
        for (int i = 1; i <= N; i++) {
            if (exist(mask, i) == true) return i;
        }

        return 0;
    }
    private boolean check(int[][] source) {
        int cnt = 0, dig, z;
        if (source.length != 9) return false;

        for (int y = 0; y < N; y++) {
            if (source[y].length != 9) return false;

            for (int x = 0; x < N; x++) {
                dig = source[y][x];
                z = y / 3 * 3 + x / 3;

                if (source[y][x] != 0) {
                    if (source[y][x] < 0 || source[y][x] > 9) {
                        return false;
                    } else {
                        if ((row[y] >> dig &1) == 1 || (col[x] >> dig &1) == 1 || (sub[z] >> dig &1) == 1) {
                            return false;
                        }

                        cnt++;
                        row[y] |= 1 << grid[y][x];
                        col[x] |= 1 << grid[y][x];
                        sub[z] |= 1 << grid[y][x];
                    }
                }
            }
        }

        if (cnt < 17) return false;
        return true;
    }
    private int nconflict (int col, int row, int sub) {
        int mask = 0;

        for (int j = 1; j <= N; j++) {
            int ex = col &1 << j, ey = row &1 << j, bl = sub &1 << j;

            if (ex == 0 && ey == 0 && bl == 0) {
                mask |= 1 << j;
            }
        }

        return mask;
    }
    private boolean backtrack(hash[] tape, int end) {

        if (end == -1) return true;
        int x, y, z, index = tape[end].pos;

        for (int i = 0; i < end; i++) {
            var curr = tape[i];
            x = curr.pos % N; y = curr.pos / N; z = y / 3 * 3 + x / 3;
            curr.val = nconflict (col[x], row[y], sub[z]);
        }

        Arrays.sort(tape, 0, end);
        x = index % N; y = index / N; z = y / 3 * 3 + x / 3;

        for (int dig = 1; dig <= N; dig++) {
            if ( !exist(row[y], dig) && !exist(col[x], dig) && !exist(sub[z], dig)) {
                col[x] ^= 1 << dig; row[y] ^= 1 << dig; sub[z] ^= 1 << dig;
                grid[y][x] = dig;

                if (backtrack(tape, end - 1) == true) {
                    return true;
                }

                grid[y][x] = 0;
                col[x] ^= 1 << dig; row[y] ^= 1 << dig; sub[z] ^= 1 << dig;
            }
        }

        return false;
    }
    public int[][] solve() {

        int size = 0;
        hash[] hist = new hash[81];

        for (int y = 0; y < 9; y++) {
            for (int x = 0; x < 9; x++)  {
                int z = y / 3 * 3 + x / 3, index = y * 9 + x;

                if (grid[y][x] == 0) {
                    int mask = nconflict(col[x], row[y], sub[z]);

                    if (Integer.bitCount(mask) > 1) {
                        hist[size++] = new hash(mask, index);
                    } else {
                        int dig = getbit(mask);
                        grid[y][x] = dig;

                        col[x] |= 1 << dig;
                        row[y] |= 1 << dig;
                        sub[z] |= 1 << dig;
                    }
                }
            }
        }

        //for (var curr : hist) {
        //    System.out.printf("%d %d : ", x, y);
        //    showbit(curr.val);
        //    System.out.print('\n');
        //}

        Arrays.sort(hist, 0, size);
        var status = backtrack( hist, size - 1);

        if (status == false) {
            throw new IllegalArgumentException("No result.");
        } else if (validate() == false) {
            throw new IllegalArgumentException("Invalid.");
        }

        return grid;
    }

    public SudokuSolver(int[][] src) {
        grid = src;

        if (check(src) == false) {
            throw new IllegalArgumentException("Invalid board.");
        }
    }

}


class Main {
    static public void main(String[] args) {

        int[][] puzzle   =
        {{0, 0, 6, 1, 0, 0, 0, 0, 8},
            {0, 8, 0, 0, 9, 0, 0, 3, 0},
            {2, 0, 0, 0, 0, 5, 4, 0, 0},
            {4, 0, 0, 0, 0, 1, 8, 0, 0},
            {0, 3, 0, 0, 7, 0, 0, 4, 0},
            {0, 0, 7, 9, 0, 0, 0, 0, 3},
            {0, 0, 8, 4, 0, 0, 0, 0, 6},
            {0, 2, 0, 0, 5, 0, 0, 8, 0},
            {1, 0, 0, 0, 0, 2, 5, 0, 0}};

        int[][] puzz1 = 
        {  {4, 0, 5, 0, 1, 0, 7, 0, 8},
            {0, 0, 7, 0, 0, 5, 0, 0, 0},
            {0, 3, 0, 7, 0, 0, 0, 5, 0},
            {0, 0, 3, 0, 0, 0, 0, 0, 5},
            {0, 4, 0, 2, 0, 8, 0, 6, 0},
            {5, 0, 0, 0, 0, 0, 1, 0, 0},
            {0, 7, 0, 0, 2, 3, 0, 1, 0},
            {0, 0, 0, 4, 0, 0, 2, 0, 0},
            {9, 0, 6, 0, 7, 0, 4, 0, 3}
        };
        var result = new SudokuSolver(puzz1).solve();

        
        SudokuSolver.display(result);


    }


}
