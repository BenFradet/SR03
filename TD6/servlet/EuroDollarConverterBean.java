
public class EuroDollarConverterBean {
	private float taux;
	private float dollar;
	private float euro;
	
	public EuroDollarConverterBean(){
		super();
		
		taux = 1.20f;
		dollar = 0.0f;
		euro = 0.0f;
	}
	
	//Getter
	public float getTaux(){
		return taux;
	}
	
	public float getDollar(){
		return dollar;
	}
	
	public float getEuro(){
		return euro;
	}
	
	//Setter
	public void setEuro(float e){
		euro = e;
	}
	
	public void setDollar(float d){
		dollar = d;
	}
	
	public void setTaux(float t){
		taux = t;
	}
}
