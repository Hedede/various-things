package java1;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import static java.lang.Double.parseDouble;
//import static java.lang.Math.pow
//import static java.lang.Math.sqrt;
import static java.lang.System.in;
import static java.lang.System.out;

public class Java1 {

    public static void main(String[] args) throws IOException {
        BufferedReader rdr = new BufferedReader(new InputStreamReader(in));
        System.out.println("This application calculates volume and surface area of a cube, using given edge length.");
        System.out.println("Enter edge length:");
        System.out.print("x = ");
        
        double x = parseDouble(rdr.readLine());
        double vol = x*x*x;
        double sarea = 6*x*x;
        
        System.out.println("Here's your result:");
        System.out.println("Surface area: " + sarea + " units^2; " + " Volume: " + vol + " units^3.");
        System.out.println();
        System.out.println("Press any key to exit.");
        System.in.read();
    }
}
