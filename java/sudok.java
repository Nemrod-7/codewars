import java.util.Collections;
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

        int cnt = 0;

        for (int y = 0; y < N; y++) {
            for (int x = 0; x < N; x++) {
                if (source[y][x] != 0) {
                    if (source[y][x] < 0 || source[y][x] > 9) {
                        return false;
                    } 

                    cnt++;
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
                int z = y / 3 * 3 + x / 3;

                if (grid[y][x] != 0) {
                    col[x] |= 1 << grid[y][x]; 
                    row[y] |= 1 << grid[y][x]; 
                    sub[z] |= 1 << grid[y][x]; 
                }
            }
        }

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
        backtrack( hist, size - 1);

        return grid;
    }

    public SudokuSolver(int[][] src) {

        if (check(src) == false) {
            throw new IllegalArgumentException("Invalid board.");
        }

        grid = src;
    }

}


class Main {
    static public void main(String[] args) {

        int[][] puzzle1   =
        {{0, 0, 6, 1, 0, 0, 0, 0, 8}, 
            {0, 8, 0, 0, 9, 0, 0, 3, 0}, 
            {2, 0, 0, 0, 0, 5, 4, 0, 0}, 
            {4, 0, 0, 0, 0, 1, 8, 0, 0}, 
            {0, 3, 0, 0, 7, 0, 0, 4, 0}, 
            {0, 0, 7, 9, 0, 0, 0, 0, 3}, 
            {0, 0, 8, 4, 0, 0, 0, 0, 6}, 
            {0, 2, 0, 0, 5, 0, 0, 8, 0}, 
            {1, 0, 0, 0, 0, 2, 5, 0, 0}},

            solution = {{3, 4, 6, 1, 2, 7, 9, 5, 8}, 
                {7, 8, 5, 6, 9, 4, 1, 3, 2}, 
                {2, 1, 9, 3, 8, 5, 4, 6, 7}, 
                {4, 6, 2, 5, 3, 1, 8, 7, 9}, 
                {9, 3, 1, 2, 7, 8, 6, 4, 5}, 
                {8, 5, 7, 9, 4, 6, 2, 1, 3}, 
                {5, 9, 8, 4, 1, 3, 7, 2, 6},
                {6, 2, 4, 7, 5, 9, 3, 8, 1},
                {1, 7, 3, 8, 6, 2, 5, 9, 4}};

        int[][] puzzle2 =
        {{0, 3, 4, 6, 7, 8, 9, 0, 2},
            {6, 7, 2, 1, 9, 5, 3, 4, 8},
            {0, 9, 8, 3, 4, 2, 5, 6, 7},
            {8, 5, 9, 7, 6, 1, 4, 2, 3},
            {4, 2, 6, 8, 5, 3, 7, 9, 1},
            {7, 1, 3, 9, 2, 4, 8, 5, 6},
            {9, 6, 1, 5, 3, 7, 2, 8, 4},
            {2, 8, 7, 4, 1, 9, 6, 3, 5},
            {3, 4, 5, 2, 8, 6, 1, 7, 0}};

        var result = new SudokuSolver(puzzle1).solve();

        SudokuSolver.display(result);

    }
}
