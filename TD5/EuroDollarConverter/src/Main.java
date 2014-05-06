import java.io.*;
import java.util.regex.*;
import java.text.*;

public class Main {

	public static void main(String[] args) {
		BufferedReader reader = null;
		String input;
		boolean cont = true;
		float exchangeRate = 0.0f;
		
		try {
			try {
				while(cont) {
					reader = new BufferedReader(new InputStreamReader(System.in));
					System.out.print("exchange rate (dollars -> euro): ");
					input = reader.readLine();
					
					try {
						exchangeRate = Float.parseFloat(input);
						cont = false;
					}
					catch(NumberFormatException ex) {
						System.out.println("exchange rate has to be a float.");
						cont = true;
					}
				}
			}
			catch (EOFException ex) {
				reader.close();
			}
		}
		catch (IOException ex) {
			System.out.println("io exception");
		}
		
		char c = 'c';
		EuroDollarConverter converter = new EuroDollarConverter(exchangeRate);
		DecimalFormat formatter = new DecimalFormat("#.00");
		Pattern pattern = Pattern.compile("\\d+(\\.\\d{1,2}){0,1} [\\$€]");
		while(c != 'q') {
			try {
				try {
					reader = new BufferedReader(new InputStreamReader(System.in));
					System.out.print("value: ");
					input = reader.readLine();
					if(!pattern.matcher(input).matches() && !input.equals("q")) {
						System.out.println("Invalid input, the input has to be like '12.0 $', type q to quit");
					} else {
						if(input.equals("q")) {
							c = 'q';
						} else {
							if(input.split(" ")[1].equals("$")) {
								System.out.println("In euros: " + formatter.format(Float.parseFloat(input.split(" ")[0]) / converter.getExchangeRate()));
							} else {
								System.out.println("In dollars: " + formatter.format(Float.parseFloat(input.split(" ")[0]) * converter.getExchangeRate()));
							}
						}
					}
				}
				catch (EOFException ex) {
					reader.close();
				}
			}
			catch (IOException ex) {
				System.out.println("io exception");
			}
		}
	}
}
