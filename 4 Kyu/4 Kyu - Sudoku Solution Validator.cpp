bool validSolution (unsigned int board[9][9]) {

    for (int i = 0; i < 9; i++) {
        int line = 0, col = 0;
        for (int j = 0; j < 9; j++) {
            line += board[i][j];
            col += board[j][i];
        }
        if (line != 45 || col != 45) return false;
    }

    for (int k = 0; k < 9; k++) {
        int centx = k / 3 * 3, centy = k % 3 * 3, cnt = 0;

        for (int i = centx; i < centx + 3; i++) {
            for (int j = centy; j < centy + 3; j++) {
                cnt += board[i][j];
            }
        }
        if (cnt != 45) return false;
    }

    return true;
}
