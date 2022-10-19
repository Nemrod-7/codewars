struct Sudoku {
    data: Vec<Vec<u32>>,
}

impl Sudoku {
    fn form (&self, x: usize, y: usize) -> usize {
        if self.data[x].len() != self.data.len() { return 0 }
        let dig = self.data[x][y] as usize;
        if dig > self.data.len() { return 0 } else { return dig }
    }
    fn is_valid (&self) -> bool {

        let size = self.data.len();
        let sq = (size as f64).sqrt() as usize;

        for i in 0..size {
            let mut row = vec![0;size+1];
            let mut col = vec![0;size+1];
            let mut inn = vec![0;size+1];

            for j in 0..size {
                let x = i / sq * sq + j / sq;
                let y = i % sq * sq + j % sq;

                row[self.form(i,j)] += 1;
                col[self.form(j,i)] += 1;
                inn[self.form(x,y)] += 1;
            }

            for j in 1..=size {
                if row[j] != 1 || col[j] != 1 || inn[j] != 1 { return false }
            }
        }

        true
    }
}
