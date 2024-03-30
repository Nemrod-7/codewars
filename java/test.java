import java.util.Collections;
import java.util.Arrays;
import java.util.ArrayList;

class hash implements Comparable<hash> {
    public int val, pos;

    public hash(int val, int pos) {
        this.val = val;
        this.pos = pos;
    }

    /*
    public hash(hash p) {
        this.val = p.val;
        this.pos = p.pos;
    }
*/
    @Override
    public int compareTo(hash p) {
        return this.val > p.val ? 1 : -1;
    }
}


class Main {

    static void backtrack(hash[] arr, int end) {
        Arrays.sort(arr, 0, end);
    }

    public static void main (String[] args) {

        int size = 0;
        hash[] base = new hash[81];

        base[size++] = (new hash(4,3));
        base[size++] = (new hash(9,3));
        base[size++] = (new hash(6,3));
        base[size++] = (new hash(8,3));

        base[size++] = (new hash(3,1));
        base[size++] = (new hash(5,3));
        base[size++] = (new hash(2,0));


        backtrack(base, size);

        for (int i = 0; i < size ; i++) {
            var curr = base[i];
            System.out.printf( "%d ", curr.val );

        }
    }
}
