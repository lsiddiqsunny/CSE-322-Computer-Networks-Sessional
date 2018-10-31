import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.Socket;

public class Server {
    private Socket smtpSocket=null;
    private BufferedReader in=null;
    private PrintWriter pr=null;
    private String state,prevstate;
    public Server(){
        prevstate=state="closed";

    }

    public Server(InetAddress mailHost, int port) {
        prevstate="closed";
        state="begin";
        try {
            smtpSocket=new Socket(mailHost,25);
        } catch (IOException e) {
            System.out.println(e.toString());
            state="closed";

        }

        try {
            if(state.equals("begin"))
            in =  new BufferedReader(new InputStreamReader(smtpSocket.getInputStream()));
        } catch (IOException e) {
            System.out.println(e.toString());
            state="closed";

        }
        try {
            if(state.equals("begin"))
            pr = new PrintWriter(smtpSocket.getOutputStream(),true);
        } catch (IOException e) {
            System.out.println(e.toString());
            state="closed";



        }


    }
    String getState(){
        return  state;
    }

    void Reply(){
       // System.out.println(state);
        try {
            long startTime = System.currentTimeMillis();
            String reply;
            while((reply=in.readLine())==null && startTime+20000>System.currentTimeMillis()){

            }
            if(reply!=null) {

                System.out.println(reply);
                if(state.equals("deliver") && reply.charAt(0)=='2'){
                    System.out.println("Mail sent successfully.");
                    state="wait";
                }
                if(state.equals("deliver") && reply.charAt(0)!='2'){
                    System.out.println("Mail sending failed.");
                    state="wait";
                }

                if(reply.charAt(0)=='4'){
                if(state.equals("wait") && prevstate.equals("begin")){
                    state="begin";
                    prevstate="closed";
                }
                else if(state.equals("from")){
                    state="wait";
                    System.out.println("Error in your mail address.");
                }
                else if(state.equals("rcpt") && prevstate.equals("from")){
                    System.out.println("Error in recipient mail address.");
                    state="from";
                    prevstate="wait";

                }else if(state.equals("rcpt") && prevstate.equals("rcpt")){
                    System.out.println("Error in recipient mail address.");
                }else
                System.out.println("Error "+reply);
                }
              }
             else System.out.println("Server timeout.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    void Request(String req){

            if(state.equals("data")){
                pr.println(req);
                if(req.equals(".")){
                    state="deliver";
                }
                return;
            }
            if(req.length()<4){
                pr.println(req);
                return;
            }

            String check= req.substring(0,4);

            if(check.equals("RSET")){
                state.equals("wait");
            }
            if(check.equals("QUIT")){
                state="closed";
            }

            if(state.equals("begin") && check.equals("HELO")){
                prevstate="begin";
                state="wait";
            }
            if(state.equals("wait") && check.equals("MAIL")){
                prevstate="wait";
                state="from";
            }
            if(state.equals("from") && check.equals("RCPT")){
                prevstate="from";
                state="rcpt";
            }
            if(state.equals("rcpt") && check.equals("RCPT")){
                prevstate="rcpt";
                state="rcpt";
            }
            if(state.equals("rcpt") && check.equals("DATA")){
                prevstate="rcpt";
                state="data";
            }



            pr.println(req);

    }


}
