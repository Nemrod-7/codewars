import java.util.HashMap;
import java.util.Collections;
import java.util.Arrays;
import java.util.ArrayList;

class hash implements Comparable<hash> {
    public int val, pos;

    public hash(int val, int pos) {
        this.val = val;
        this.pos = pos;
    }   
    public hash(hash p) {
        this.val = p.val;
        this.pos = p.pos;
    }

    @Override
    public int compareTo(hash p) {
        //return Integer.bitCount(this.val) < Integer.bitCount(p.val) ? 1 : -1;    
        return this.val > p.val ? 1 : -1;
    }
}


class Main {

    static void backtrack(ArrayList<hash> src, int end) {
        ArrayList<hash> base = new ArrayList<>();

        for (int i = 0; i < end; i++) {
            base.add( src.get(i));
        }

        Collections.sort(base);

    }
    public static void main (String[] args) {

        ArrayList<hash> base = new ArrayList<>();


        base.add(new hash(4,3));
        base.add(new hash(6,3));
        base.add(new hash(9,3));
        base.add(new hash(8,3));

        base.add(new hash(3,1));
        base.add(new hash(5,3));
        base.add(new hash(2,0));

        backtrack(base, 3);

        for (var curr : base) {
            System.out.printf( "%d ", curr.val ); 
        }
        /*
        */

    }
}
