import java.io.IOException;
import java.io.PrintWriter;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


public class DollarEuroConverter extends HttpServlet {
	
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
		
		float dollar = Float.parseFloat(request.getParameter("dollar"));
		
		PrintWriter out = response.getWriter();
		out.println("<html>");
		out.println("<meta charset='utf-8'>");
        out.println("<head>");
        out.println("<title>Dollar to Euro</title>");
        out.println("</head>");
        out.println("<body>");
        out.printf("<h1>%.3f$ = %.3f€</h1>" ,dollar ,dollar / edb.getTaux());
        out.println("</body>");
        out.println("</html>");
	}
}
