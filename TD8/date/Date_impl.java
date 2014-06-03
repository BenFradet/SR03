package date;

public class Date_impl extends DatePOA {
    public String getDate() {
        return new java.text.SimpleDateFormat("yyyyMMdd_HHmmss")
            .format(java.util.Calendar.getInstance().getTime());
    }
}
