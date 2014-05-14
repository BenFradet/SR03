<jsp:useBean id="edc" class="JSP.EuroDollarConverterBean" scope="session">
<jsp:setProperty name="edc" property="taux" value="1.39f"/>
<html>
	<head>
		<meta charset='utf-8'>
		<title>Euro to dollar</title>
	</head>");
	<body>
	<%
		float dollar = Float.parseFloat(request.getParameter("dollar"));
		out.println(String.format("%.3f $ = %.3f€", dollar, dollar / edc.getTaux()));
	%>
	</body>
</html>
</jsp:useBean>
