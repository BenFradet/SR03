// Java  hello/Server.java
package hello;

public class Server
{ 
  public static void main(String args[])
  { 
    java.util.Properties props = System.getProperties(); 
    props.put("org.omg.CORBA.ORBClass", 
    		"com.ooc.OBServer.ORB"); 
    props.put("org.omg.CORBA.ORBSingletonClass",
    		"com.ooc.CORBA.ORBSingleton");
    
    int status = 0; 
    org.omg.CORBA.ORB orb = null; 
    try 
    {	orb = org.omg.CORBA.ORB.init(args, props); 
	status = run(orb); 
    } 
    catch(Exception ex) 
    { 	ex.printStackTrace(); 
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
	throws org.omg.CORBA.UserException
   { 
	org.omg.PortableServer.POA rootPOA = 
	org.omg.PortableServer.POAHelper.narrow( 
	orb.resolve_initial_references("RootPOA")); 
	
	org.omg.PortableServer.POAManager manager = 
	rootPOA.the_POAManager(); 
	
	Hello_impl helloImpl = new Hello_impl(); 
	Hello hello = helloImpl._this(orb); 
	
	try 
	{  String ref = orb.object_to_string(hello); 
	   String refFile = "Hello.ref"; 
	   java.io.PrintWriter out = new java.io.PrintWriter( 
	   	new java.io.FileOutputStream(refFile)); 
	   out.println(ref); 
	   out.close(); 
	} 
	catch(java.io.IOException ex) 
	{  ex.printStackTrace(); 
	   return 1; } 
	
	manager.activate(); 
	orb.run(); 
	return 0; 
   } 
}
