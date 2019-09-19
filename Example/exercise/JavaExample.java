package Test.Test;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

public class JavaExample {

    public static void main(String[] args) {
        Algo algo = new Algo();
        algo.Texto = "2";
        Scanner in = new Scanner(System.in);
        ArrayList<Integer> a = new ArrayList<>();
        in.nextInt();
        while(in.hasNextInt()) {
            a.add(in.nextInt());
        }
        Collections.sort(a);
        for(Integer i : a) {
            System.out.println(i);
        }
    }
    
}

class Algo {
    public String Texto;
}
