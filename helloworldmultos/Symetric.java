/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package helloworldmultos;

import static helloworldmultos.HelloWorldMultOS.ArrayEquals;
import static helloworldmultos.HelloWorldMultOS.GenerateBigNumberAsArray;
import static helloworldmultos.HelloWorldMultOS.count_time;
import static helloworldmultos.HelloWorldMultOS.createAPDUComm;
import static helloworldmultos.HelloWorldMultOS.isEncipher;
import static helloworldmultos.HelloWorldMultOS.print1;
import static helloworldmultos.HelloWorldMultOS.sendAPDU;
import static helloworldmultos.Variables.GetVariable;
import static helloworldmultos.Variables.SetVariable;

/**
 *
 * @author Developer
 */
public class Symetric {
 
     /**
     * Call des funciton on card
     *
     * @param data dsfafd
     *
     */
    private void DES( boolean Encipher, boolean BlockCipher) {

        byte priznak=0x00;
        if(BlockCipher)
        {
            priznak=0x11;
        }
        else{
            priznak=0x10;
        }
        byte P1 = isEncipher(Encipher);

        byte[] DES = createAPDUComm((byte) 0x80, priznak, P1, (byte) 0x00,
                (byte) 0x00,
                null,
                (byte) 0x00);
        //  System.out.print(print1(DES));
        sendAPDU(DES);
    }
    
    
    /**
     * TESTOVACI metoda pro des funciton on card
     *
     */
    public void Test_DES(int counter, byte[] vstup,  boolean BlockCipher) throws Exception {
//          System.out.println("VSTUP:"+ print1(vstup));
        SetVariable((byte) 0x03, vstup);
        SetVariable((byte) 0x01, GenerateBigNumberAsArray(8));
//System
String output;
        if(BlockCipher)
        {output="BlockCipher()";
        }
        else
            output="ECB()";

        byte[] secured = GetVariable((byte) 0x04, (byte) 0x00);
        long timeStart = System.currentTimeMillis();
        //for (int i = 0; i < counter; i++) {
            DES(true, BlockCipher);
       // }
        count_time(timeStart, counter, "DES "+output+ " Encipher input_lenght" + (vstup.length * 8) + " bits");

        //System.out.println("VYSTUP:"+print1(secured));
        timeStart = System.currentTimeMillis();
        //for (int i = 0; i < counter; i++) {
            DES(false, BlockCipher);
        //}
        count_time(timeStart, counter, "DES "+output+ " Decipher input_lenght" + (vstup.length * 8) + " bits");

        byte[] returnSecured = GetVariable((byte) 0x08, (byte) 0x00);

       System.out.println("VYSTUP:"+print1(secured));
        System.out.println("VYSTUP:"+print1(returnSecured));      
        ArrayEquals(returnSecured, secured, "SOME ERROR WITH DES ENCRYPTION");

    }
    
    
    /**
     * Call triple des funciton on card
     *
     *
     */
    private byte[] tripleDES(byte[] data, boolean Encipher) {

        byte P1 = isEncipher(Encipher);

        byte[] TRIPLEDES = createAPDUComm((byte) 0x80, (byte) 0x12, P1, (byte) 0x00,
                0x00,
                null,
                (byte) 0x00);
        return sendAPDU(TRIPLEDES);
    }

    /**
     * TESTOVACI metoda pro triple des funciton on card
     *
     */
    public void Test_tripleDES(int counter, byte[] vstup) throws Exception {

        SetVariable((byte) 0x03, vstup);
        SetVariable((byte) 0x01, GenerateBigNumberAsArray(24));

        byte[] secured = GetVariable((byte) 0x04, (byte) 0x00);

        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            tripleDES(vstup, true);
       // }

        count_time(timeStart, counter, "3DES BlockCipher() Encipher input_lenght" + (vstup.length * 8) + " bits");

        byte[] cipher = GetVariable((byte) 0x06, (byte) 0x00);
        System.out.println(print1(cipher));

        timeStart = System.currentTimeMillis();
        //for (int i = 0; i < counter; i++) {
            tripleDES(vstup, false);
       // }
        count_time(timeStart, counter, "3DES BlockCipher() Decipher input_lenght" + (vstup.length * 8) + " bits");

        byte[] returnSecured = GetVariable((byte) 0x08, (byte) 0x00);
        System.out.println(print1(secured));

        System.out.println(print1(returnSecured));

        ArrayEquals(returnSecured, secured, "SOME ERROR WITH 3DES ENCRYPTION");

    }
    ///

    /**
     * Call triple AES funciton on card
     *
     *
     */
    private byte[] AES(byte[] data, boolean Encipher) {

        byte P1 = isEncipher(Encipher);

        byte[] AES = createAPDUComm((byte) 0x80, (byte) 0x13, P1, (byte) 0x00,
                (byte) 0x00,
                null,
                (byte) 0x00);
        return sendAPDU(AES);
    }

    /**
     * TESTOVACI metoda pro triple des funciton on card
     *
     */
    public void Test_AES(int counter, byte[] vstup, byte[] key) throws Exception {
        SetVariable((byte) 0x03, vstup);
        SetVariable((byte) 0x01, key);
//System.out.println("vst " + print1(vstup));
       
  //       System.out.println("key "+ print1(key));
       
        byte[] secured = GetVariable((byte) 0x04, (byte) 0x00);

        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            AES(vstup, true);
      //  }
        count_time(timeStart, counter, "AES Encipher input_lenght" + (vstup.length * 8) + " bits, key_lenght" + (key.length * 8) + " bits");
byte[] cipher = GetVariable((byte) 0x06, (byte) 0x00);
    //    System.out.println(print1(cipher));
 timeStart = System.currentTimeMillis();
     //   for (int i = 0; i < counter; i++) {
            AES(vstup, false);
       // }
        count_time(timeStart, counter, "AES Decipher input_lenght" + (vstup.length * 8) + " bits, key_lenght" + (key.length * 8) + " bits");
        byte[] returnSecured = GetVariable((byte) 0x08, (byte) 0x00);
 // System.out.println(print1(secured));

       // System.out.println(print1(returnSecured));

        ArrayEquals(returnSecured, secured, "SOME ERROR WITH AES ENCRYPTION");

    }
    
    
    
    
}
