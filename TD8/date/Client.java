// Java 2  Client.java
package hello; 

public class Client 
{  public static void main(String args[]) 
   {  // Same as the server
      java.util.Properties props = System.getProperties(); 
      props.put("org.omg.CORBA.ORBClass", 
    		  "com.ooc.OBServer.ORB"); 
      props.put("org.omg.CORBA.ORBSingletonClass",
    		  "com.ooc.CORBA.ORBSingleton");
      
      int status = 0; 
      org.omg.CORBA.ORB orb = null; 
      try 
      {   orb = org.omg.CORBA.ORB.init(args, props); 
    	  status = run(orb); 
      } 
      catch(Exception ex) 
      {   ex.printStackTrace(); 
    	  status = 1; 
      } 
      
      if(orb != null) 
      { try 
    	{  orb.destroy(); }
    	catch(Exception ex)
    	{  ex.printStackTrace(); 
    	   status = 1;   } 
      }     
      System.exit(status);  
   } 
   
//----- run() -----
   
   static int run(org.omg.CORBA.ORB orb) 
   {	org.omg.CORBA.Object obj = null; 
   	try
	{  String refFile = "Date.ref"; 
	   java.io.BufferedReader in = new java.io.BufferedReader( 
	   			new java.io.FileReader(refFile)); 
	   String ref = in.readLine(); 
	   obj = orb.string_to_object(ref); 
	} 
	catch(java.io.IOException ex) 
	{  ex.printStackTrace(); 
	   return 1; } 
	
	Date date = DateHelper.narrow(obj); 
	System.out.println(date.getDate()); 
	
	return 0; 
   }  
}
