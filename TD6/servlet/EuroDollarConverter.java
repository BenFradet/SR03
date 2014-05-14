import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


public class EuroDollarConverter extends HttpServlet {
	
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
		
		float euro = Float.parseFloat(request.getParameter("euro"));
		
		PrintWriter out = response.getWriter();
		out.println("<html>");
		out.println("<meta charset='utf-8'>");
        out.println("<head>");
        out.println("<title>Euro to dollar</title>");
        out.println("</head>");
        out.println("<body>");
        out.printf("<h1>%.3f\u20ac = %.3f$</h1>" ,euro ,euro * edb.getTaux());
        out.println("</body>");
        out.println("</html>");
      
	}
}
