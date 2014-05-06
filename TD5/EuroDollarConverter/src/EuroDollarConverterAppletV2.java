import java.applet.Applet;
import java.awt.*;
import java.awt.event.*;
import java.net.*;
import java.text.DecimalFormat;
import java.util.regex.Pattern;


public class EuroDollarConverterAppletV2 extends Applet implements MouseListener {
	Button convertButton;
	TextField exchangeRateField;
	Label exchangeRateLabel;
	TextField valueField;
	Label valueLabel;
	Label resultLabel;
	
	public void init() {
		setLayout(null);
		addNotify();
		
		resize(512, 256);
		
		convertButton = new Button("Convert");
		convertButton.setBounds(216, 200, 80, 20);
		convertButton.addMouseListener(this);
		add(convertButton);
		
		exchangeRateLabel = new Label("Exchange rate: ");
		exchangeRateLabel.setBounds(50, 50, 100, 20);
		add(exchangeRateLabel);
		
		exchangeRateField = new TextField();
		exchangeRateField.setBounds(150, 50, 50, 20);
		add(exchangeRateField);
		
		valueLabel = new Label("Value: ");
		valueLabel.setBounds(50, 100, 100, 20);
		add(valueLabel);
		
		valueField = new TextField();
		valueField.setBounds(150, 100, 50, 20);
		add(valueField);
		
		resultLabel = new Label();
		resultLabel.setBounds(0, 230, 512, 20);
		add(resultLabel);
	}

	@Override
	public void mouseClicked(MouseEvent event) {
		String exchangeRateStr = exchangeRateField.getText();
		float exchangeRate = 0.0f;
		try {
			exchangeRate = Float.parseFloat(exchangeRateStr);
		}
		catch(NumberFormatException ex) {
			resultLabel.setText("Invalid exchange rate, it has to be a number");
			return;
		}
		
		EuroDollarConverter converter = new EuroDollarConverter(exchangeRate);
		DecimalFormat formatter = new DecimalFormat("#.00");
		Pattern pattern = Pattern.compile("\\d+(\\.\\d{1,2}){0,1} [\\$€]");
		String valueStr = valueField.getText();
		if(!pattern.matcher(valueStr).matches()) {
			resultLabel.setText("Invalid value entered, has to follow the format 12.0 $");
		} else {
			if(valueStr.split(" ")[1].equals("$")) {
				resultLabel.setText("In euros: " + formatter.format(Float.parseFloat(valueStr.split(" ")[0]) / converter.getExchangeRate()));
			} else {
				resultLabel.setText("In dollars: " + formatter.format(Float.parseFloat(valueStr.split(" ")[0]) * converter.getExchangeRate()));
			}
		}
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mouseExited(MouseEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mousePressed(MouseEvent arg0) {
		// TODO Auto-generated method stub

	}

	@Override
	public void mouseReleased(MouseEvent arg0) {
		// TODO Auto-generated method stub

	}

}
