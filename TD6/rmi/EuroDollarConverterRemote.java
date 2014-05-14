import java.io.IOException;
import java.io.PrintWriter;
import java.rmi.Naming;
import java.rmi.NotBoundException;
import java.rmi.RMISecurityManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


public class EuroDollarConverterRemote extends HttpServlet {
	
	private EuroDollarConverterBean edb;
	
	@Override
	public void init(){
		try {
			super.init();
		} catch (ServletException e) {
			// TODO Bloc catch généré automatiquement
			e.printStackTrace();
		}
		edb = new EuroDollarConverterBean();
		edb.setTaux(1.39f);
	}
	
	@Override
	public void doGet(HttpServletRequest request,
						HttpServletResponse response)
						throws IOException, ServletException{
		System.setSecurityManager(new RMISecurityManager()); 
	    String partOfUrl = "rmi://:10912/"; 
	    
	    float euro = Float.parseFloat(request.getParameter("euro"));
	    float dollar = 0.0f;
	    
	    try {
			EuroDollarConverterInterface cl = (EuroDollarConverterInterface)Naming.lookup(partOfUrl + "euroDollarConverter");
			dollar = cl.euroDollarConverter(euro);
		} catch (NotBoundException e) {
			// TODO Bloc catch généré automatiquement
			e.printStackTrace();
		}
	    
		
		PrintWriter out = response.getWriter();
		out.println("<html>");
		out.println("<meta charset='utf-8'>");
        out.println("<head>");
        out.println("<title>Euro to dollar</title>");
        out.println("</head>");
        out.println("<body>");
        out.printf("<h1>%.3f\u20ac = %.3f$</h1>" ,euro ,dollar);
        out.println("</body>");
        out.println("</html>");
      
	}
}
