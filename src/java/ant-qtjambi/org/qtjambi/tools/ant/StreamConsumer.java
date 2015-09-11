package org.qtjambi.tools.ant;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;

public class StreamConsumer extends Thread {

    public StreamConsumer(InputStream in, PrintStream out) {
        this.in = in;
        this.out = out;
    }

    @Override
    public void run() {
        BufferedReader reader = null;
        String line;
        try {
            reader = new BufferedReader(new InputStreamReader(in));
            while((line = reader.readLine()) != null) {
                if(out != null)
                    out.println(line);
            }
            reader.close();
            reader = null;
            in.close();
            in = null;
        } catch(IOException e) {
            e.printStackTrace();
        } finally {
            if(reader != null) {
                try {
                    reader.close();
                } catch(IOException eat) {
                }
                reader = null;
            }
            if(in != null) {
                try {
                    in.close();
                } catch(IOException eat) {
                }
                in = null;
            }
        }
    }

    private PrintStream out;
    private InputStream in;
}
