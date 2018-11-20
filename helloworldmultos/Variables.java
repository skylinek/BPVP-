/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package helloworldmultos;

import static helloworldmultos.HelloWorldMultOS.createAPDUComm;
import static helloworldmultos.HelloWorldMultOS.sendAPDU;
import java.util.Arrays;

/**
 *
 * @author Developer
 */
public class Variables {
     public static void SetCounter(int counter) {
     
     byte[] APDUkey = createAPDUComm((byte) 0x80, (byte) 0xA2, (byte) counter, (byte) 0x00,
                    (byte) 0x00,
                    null,
                    (byte) 0x00);
            byte[] rKEY = sendAPDU(APDUkey);
     }
    
    public static void SetVariable(byte what, byte[] key) {
        //byte[] a1= Arrays.copyOfRange(key, 0, 255);
        // byte[] a2= Arrays.copyOfRange(key, 255, key.length);
        //  System.out.println(print1(a1));
        //  System.out.println(print1(a2)); 

        if (key.length > 255 && key.length <= 510) {
            byte[] APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x00, (byte) 0x00,
                    (byte) 255,
                    Arrays.copyOfRange(key, 0, 255),
                    (byte) 255);
            byte[] rKEY = sendAPDU(APDUkey);

            APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x01, (byte) 0x00,
                    (byte) key.length - 255,
                    Arrays.copyOfRange(key, 255, key.length),
                    (byte) (key.length - 255));
            rKEY = sendAPDU(APDUkey);

        } else if (key.length > 510) {
            byte[] APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x00, (byte) 0x00,
                    (byte) 255,
                    Arrays.copyOfRange(key, 0, 255),
                    (byte) 255);
            byte[] rKEY = sendAPDU(APDUkey);

            APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x01, (byte) 0x00,
                    (byte) 255,
                    Arrays.copyOfRange(key, 255, 510),
                    (byte) 255);
            rKEY = sendAPDU(APDUkey);

            APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x02, (byte) 0x00,
                    (byte) key.length - 510,
                    Arrays.copyOfRange(key, 510, key.length),
                    (byte) (key.length - 510));
            rKEY = sendAPDU(APDUkey);

        } else {
            byte[] APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x00, (byte) 0x00,
                    (byte) key.length,
                    key,
                    (byte) key.length);
            byte[] rKEY = sendAPDU(APDUkey);
        }
    }
    
    
    
    
    
    
    
    public static byte[] GetVariable(byte what, byte whole) {
        byte[] two = null;
        byte[] APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x00, whole, (byte) 0x00, null, (byte) 255);
        byte[] one = sendAPDU(APDUkey);

        if (one.length == 255) {
            APDUkey = createAPDUComm((byte) 0x80, what, (byte) 0x01, whole, (byte) 0x00, null, (byte) 255);
            two = sendAPDU(APDUkey);

            byte[] combined = new byte[one.length + two.length];

            System.arraycopy(one, 0, combined, 0, one.length);
            System.arraycopy(two, 0, combined, one.length, two.length);
            return combined;
        } else {
            return one;
        }
    }
    
}
