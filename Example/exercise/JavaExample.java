import java.util.ArrayList;
import java.util.Collections;
import java.util.Scanner;

public class JavaExample {

    public static void main(String[] args) {
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
