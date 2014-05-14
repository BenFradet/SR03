import java.rmi.Remote;
import java.rmi.RemoteException;


public interface EuroDollarConverterInterface extends Remote{
	float euroDollarConverter(float euro) throws RemoteException;
}
