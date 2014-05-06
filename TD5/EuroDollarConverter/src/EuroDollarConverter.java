public class EuroDollarConverter {
	private float exchangeRate;

	/**
	 * Constructor for the EuroDollarConverter class.
	 * @param exchangeRate
	 */
	public EuroDollarConverter(float exchangeRate) {
		super();
		this.exchangeRate = exchangeRate;
	}
	
	/**
	 * Getter for the exchangeRate field.
	 * @return the exchangeRate
	 */
	public float getExchangeRate() {
		return exchangeRate;
	}

	/**
	 * Setter for the exchangeRate field.
	 * @param exchangeRate
	 */
	public void setExchangeRate(float exchangeRate) {
		this.exchangeRate = exchangeRate;
	}
}
