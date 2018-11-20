/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package helloworldmultos;

import static helloworldmultos.HelloWorldMultOS.ArrayEquals;
import static helloworldmultos.HelloWorldMultOS.GenerateBigNumberAsArray;
import static helloworldmultos.HelloWorldMultOS.byteArrayToBigInt;
import static helloworldmultos.HelloWorldMultOS.count_time;
import static helloworldmultos.HelloWorldMultOS.counter;
import static helloworldmultos.HelloWorldMultOS.createAPDUComm;
import static helloworldmultos.HelloWorldMultOS.print1;
import static helloworldmultos.HelloWorldMultOS.sendAPDU;
import static helloworldmultos.Variables.GetVariable;
import static helloworldmultos.Variables.SetVariable;
import java.math.BigInteger;
import java.security.SecureRandom;
import java.util.Arrays;

/**
 *
 * @author Developer
 */
public class Modular {
    
    public  int[] sizesModulus = new int[]{1024, 2048};
    public  int[] sizes = new int[]{80, 160, 224, 256, 384, 512, 1024, 2048};
                  
    
    
    public void Modular(int priznak) { //0x50 Modulární sčítání 0x51 Modulární násobení
        //0x52 Modulární mocnění 0x53 Modulární redukce  0x54 Modulární inverze

        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x00, (byte) 0x00,
                (byte) 0x00,
                null,
                (byte) 0x00);
        //   System.out.println(print1(ADD1));
        byte[] rMUL1 = sendAPDU(ADD1);

    }

    /**
     * Modularni scitani Modullar Addition
     *
     */
    public void Modular_ADD(byte[] opp1, byte[] opp2, byte[] n) {

        int priznak = 0x50; //scitani

        int count_of_return = 255;
        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x01, (byte) 0x00,
                (byte) opp1.length,
                opp1,
                (byte) 0x01);
        byte[] rMUL1 = sendAPDU(ADD1);

        byte[] ADD2 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x02, (byte) 0x00,
                (byte) opp2.length,
                opp2,
                (byte) 0x01);
        byte[] rMUL2 = sendAPDU(ADD2);

        byte[] ADD3 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x03, (byte) 0x00,
                (byte) n.length,
                n,
                (byte) count_of_return);
        byte[] rMUL3 = sendAPDU(ADD3);
    }

    /**
     * TESTOVACI metoda pro Modularni scitani Modullar Addition
     *
     */
    public void Test_MOD_ADD(int counter, byte[] opp1, byte[] opp2, byte[] modulus, int sizeOPP, int sizeModulus ) throws Exception {
        /*if (opp1.length < modulus.length) {
            return;
        }*/
        if (opp1.length != 128) {
            return;
        }
        SetVariable((byte) 0x03, opp1);
        SetVariable((byte) 0x05, opp2);
        SetVariable((byte) 0x07, modulus);
        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            Modular(0x50);  // N can not be ODD  (sude)
       // }
         
        count_time(timeStart, counter, "MOD_ADD opp_lenght" + (sizeOPP * 8) + " bits n_lenght" + (sizeModulus * 8) + " bits");
        byte[] MYresult = GetVariable((byte) 0xA1, (byte) 0x01);

        BigInteger bopp1 = new BigInteger(opp1);
        BigInteger bopp2 = new BigInteger(opp2);
        BigInteger bmodulus = new BigInteger(modulus);
        BigInteger modMul = bopp1.add(bopp2).mod(bmodulus);
        System.err.println("modMulPC = " + modMul);
        System.err.println("modMulCARD = " + byteArrayToBigInt(MYresult));
        ArrayEquals(byteArrayToBigInt(MYresult).toByteArray(), modMul.toByteArray(), "SOME ERROR WITH MODULAR ADDITION");

    }
    
    
     /**
     * Modularni nasobeni Modullar multiplication
     *
     */
    public void Modular_MUL_ML3() throws Exception {
            byte[] opp1;
            byte[] opp2;
            byte[] modulus;

        for (int i = 0; i < sizesModulus.length; i = i + 1) {
           for (int j = 0; j < sizes.length; j = j + 1) {
               modulus = GenerateBigNumberAsArray(sizesModulus[i] / 8);
                   byte[] newOpp1 = GenerateBigNumberAsArray(sizes[j] / 8);
                     opp1 = new byte[modulus.length];
                   byte[] newOpp2 = GenerateBigNumberAsArray(sizes[j] / 8);
                     opp2  = new byte[modulus.length];
                BigInteger bopp1 = new BigInteger(opp1);
                    BigInteger bopp2 = new BigInteger(opp2);
                    BigInteger bmodulus = new BigInteger(modulus);
                    System.err.println("for opp1 = " + bopp1);
                    System.err.println("for opp2 = " + bopp2);
                    System.err.println("for opp3 = " + bmodulus);  
                   /* 
                    if (opp1.length != modulus.length) {
                        continue;
                    }*/
                    if (!(sizes[j] <= sizesModulus[i])) {
                        continue;
                    }

                    if (bopp1.compareTo(bmodulus) != -1) {
                        j--;

                        System.out.println("BOOP1");
                        continue;
                    }
                    if (bopp2.compareTo(bmodulus) != -1) {
                        j--;
                        System.out.println("BOOP2");
                        continue;
                    }
                    
                    
                    System.arraycopy(newOpp1, 0, opp1, (modulus.length - newOpp1.length), (newOpp1.length));
                    System.arraycopy(newOpp2, 0, opp2, (modulus.length - newOpp2.length), (newOpp2.length));
                    System.err.println(print1(opp1));
                    System.err.println(print1(opp2));

                    System.out.println("size:" + opp1.length + " sizesModulus: " + modulus.length + " ");
                    System.out.println("size:" + opp2.length + " sizesModulus: " + modulus.length + " ");
                    
                    
                    
                    
        Test_MOD_MUL_ML3(counter, opp1, opp2, modulus, sizes[j] / 8, sizesModulus[i] / 8);
        
            
           }}
    }
    
      

    /**
     * TESTOVACI metoda pro Modularni nasobeni Modullar multiplication
     * @param opp1
     * @param opp2
     * @param modulus
     * @param sizeOPP
     * @param sizeModulus
     * @throws java.lang.Exception
     */
    public void Test_MOD_MUL_ML3(int counter, byte[] opp1, byte[] opp2, byte[] modulus, int sizeOPP, int sizeModulus)throws Exception {
       // if (opp1.length != modulus.length) {
        //    return;
        //}
        SetVariable((byte) 0x03, opp1);
         SetVariable((byte) 0xA0, opp1);
        SetVariable((byte) 0x05, opp2);
        SetVariable((byte) 0x07, modulus);

        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            Modular(0x51);  // N can not be ODD  (sude)
       //}
        count_time(timeStart, counter, "MOD_MULTIPLICATION opp_lenght" + (sizeOPP * 8) + " bits n_lenght" + (sizeModulus * 8) + " bits");

        BigInteger bopp1 = new BigInteger(opp1);
        BigInteger bopp2 = new BigInteger(opp2);
        BigInteger bmodulus = new BigInteger(modulus);
        BigInteger modMul;
       //for(int j=0;j<counter;j=j+1){
         bopp1 = bopp1.multiply(bopp2).mod(bmodulus);
         
      // }
       modMul=bopp1;
        ///  System.out.println(print1(MYresult));
        byte[] test = modMul.toByteArray();
        //System.out.println(print1(test));
        byte[] MYresult;

        if (sizeModulus != sizeOPP) {
            byte[] MYresult2 = GetVariable((byte) 0x04, (byte) 0x00);
            MYresult = new byte[sizeOPP * 2];
            System.arraycopy(MYresult2, 0, MYresult,0 , sizeOPP*2);
            System.out.println("PCresult2 >>>: " + print1(modMul.toByteArray()));
            
            System.out.println("MYresult2 >>>: " + print1(MYresult2));
            System.out.println("MYresult >>>: " + print1(MYresult));
            System.out.println("size2 >>>: " + MYresult2.length);
            System.out.println("size >>>: " + MYresult.length);
            if (!Arrays.equals(MYresult, modMul.toByteArray())) {
                MYresult = new byte[sizeOPP * 2-1];
                System.arraycopy(MYresult2, 0, MYresult, 0, sizeOPP * 2 - 1);

            }
            if (!Arrays.equals(MYresult, modMul.toByteArray())) {
               MYresult = new byte[sizeOPP * 2+1];
                System.arraycopy(MYresult2, 0, MYresult, 0, sizeOPP * 2 + 1);

            }

        } else {
            MYresult = GetVariable((byte) 0x04, (byte) 0x01);
        }

        System.err.println("modMulPC = " + modMul);
        System.err.println("modMulCARD = " + byteArrayToBigInt(MYresult));

        ArrayEquals(MYresult, modMul.toByteArray(), "SOME ERROR WITH MODULAR MULTIPLICATION");

        /* 
         long timeStart = System.currentTimeMillis();
        for (int i = 0; i < counter; i++) {
                Modular_MUL(opp1,opp2,modulus);  // N can not be ODD  (sude)
        }
        count_time(timeStart, counter,"MOD_MUL opp_lenght"+(opp1.length*8)+" bits n_lenght"+(modulus.length*8)+" bits");
         */
    }
    public void MODULAR_OPERATION(boolean ML3, int mod) throws Exception{ //0 multiplication //1 exp // 2 add
    
         byte[] opp1;
            byte[] opp2;
            byte[] modulus;

        
        
         for (int i = 0; i < sizesModulus.length; i = i + 1) {
               // modulus = GenerateBigNumberAsArray(sizesModulus[i]/8);

                for (int j = 0; j < sizes.length; j = j + 1) {
                 //   sizesModulus[i]=1024;
                  modulus = GenerateBigNumberAsArray(sizesModulus[i]/8);

                    byte[] newOpp1 = GenerateBigNumberAsArray(sizes[j] / 8);
                     opp1 = new byte[modulus.length];
                   byte[] newOpp2 = GenerateBigNumberAsArray(sizes[j] / 8);
                     opp2  = new byte[modulus.length];
                BigInteger bopp1 = new BigInteger(opp1);
                    BigInteger bopp2 = new BigInteger(opp2);
                    BigInteger bmodulus = new BigInteger(modulus);
                    System.err.println("for opp1 = " + bopp1);
                    System.err.println("for opp2 = " + bopp2);
                    System.err.println("for opp3 = " + bmodulus);  
                   /* 
                    if (opp1.length != modulus.length) {
                        continue;
                    }*/
                    if (!(sizes[j] <= sizesModulus[i])) {
                        continue;
                    }

                    if (bopp1.compareTo(bmodulus) != -1) {
                        j--;

                        System.out.println("BOOP1");
                        continue;
                    }
                    if (bopp2.compareTo(bmodulus) != -1) {
                        j--;
                        System.out.println("BOOP2");
                        continue;
                    }
                    
            
                   //Test_MOD_ADD(1, newOpp1, newOpp2, modulus,sizes[j] / 8, sizesModulus[i] / 8);
  
                    System.arraycopy(newOpp1, 0, opp1, (modulus.length - newOpp1.length), (newOpp1.length));
                    System.arraycopy(newOpp2, 0, opp2, (modulus.length - newOpp2.length), (newOpp2.length));
                    System.err.println(print1(opp1));
                    System.err.println(print1(opp2));

                    System.out.println("size:" + opp1.length + " sizesModulus: " + modulus.length + " ");
                    System.out.println("size:" + opp2.length + " sizesModulus: " + modulus.length + " ");
            if(mod==0){      
                    if (ML3){
                      Test_MOD_MUL_ML3(counter, opp1, opp2, modulus, sizes[j] / 8, sizesModulus[i] / 8);
                  
                    }
                    else{
                Test_MOD_MUL_UBM(counter, opp1, opp2, modulus, sizes[j] / 8, sizesModulus[i] / 8);
       // Test_MOD_EXP_ML3(counter, opp1, opp2, modulus,  sizes[j] / 8, sizesModulus[i] / 8);
                
                    }
                  }
            else if(mod==1) {      
       Test_MOD_EXP_ML3(counter, opp1, opp2, modulus,  sizes[j] / 8, sizesModulus[i] / 8);
            }   
            else if(mod==2)
            {
                Test_MOD_ADD(counter, opp1, opp2, modulus, sizes[j] / 8, sizesModulus[i] / 8); //pouze do 1024
            }
                }
                
         }    
         
    }
    
    
    public void Test_MOD_MUL_UBM(int counter, byte[] opp1, byte[] opp2, byte[] modulus, int sizeOPP, int sizeModulus)throws Exception {
      //  if (opp1.length != modulus.length) {
      //      return;
      //  }
        SetVariable((byte) 0x03, opp1);
        SetVariable((byte) 0x05, opp2);
        SetVariable((byte) 0x07, modulus);

        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            Modular(0x51);  // N can not be ODD  (sude)
      // }
        count_time(timeStart, counter, "MOD_MULTIPLICATION opp_lenght" + (sizeOPP * 8) + " bits n_lenght" + (sizeModulus * 8) + " bits");

        BigInteger bopp1 = new BigInteger(opp1);
        BigInteger bopp2 = new BigInteger(opp2);
         BigInteger mul=bopp1.multiply(bopp2);
         System.out.println(mul);
         System.out.println(print1(mul.toByteArray()));
        BigInteger bmodulus = new BigInteger(modulus);
        BigInteger modMul = bopp1.multiply(bopp2).mod(bmodulus);
        ///  System.out.println(print1(MYresult));
       // byte[] resultPC = modMul.toByteArray();
        //System.out.println(print1(test));
        byte[] MYresult;
/*
        if (sizeModulus != sizeOPP) {
            byte[] MYresult2 = GetVariable((byte) 0x04, (byte) 0x00);
            MYresult = new byte[sizeOPP * 2];
            System.arraycopy(MYresult2, 0, MYresult,0 , sizeOPP*2);
            System.out.println("MYresult2 >>>: " + print1(MYresult2));
            System.out.println("MYresult >>>: " + print1(MYresult));
            System.out.println("size2 >>>: " + MYresult2.length);
            System.out.println("size >>>: " + MYresult.length);
            if (!Arrays.equals(MYresult, modMul.toByteArray())) {
                //MYresult = new byte[sizeOPP * 2-1];
                System.arraycopy(MYresult2, 0, MYresult, 0, sizeOPP * 2 - 1);

            }
            if (!Arrays.equals(MYresult, modMul.toByteArray())) {
               MYresult = new byte[sizeOPP * 2+1];
                System.arraycopy(MYresult2, 0, MYresult, 0, sizeOPP * 2 + 1);

            }

        } else {*/
            MYresult = GetVariable((byte) 0x04, (byte) 0x01);
        //}

        System.err.println("modMulPC = " + modMul);
        System.err.println("modMulCARD = " + byteArrayToBigInt(MYresult));
       if(modMul.compareTo(byteArrayToBigInt(MYresult))!=0){
        throw new Exception("SOME ERROR WITH MODULAR MULTIPLICATION");
       }
        
   

           
        }
    

    /**
     * Modularni mocneni exponantion
     *
     */
    public void Modular_EXP(byte[] opp1, byte[] opp2, byte[] n) {

        /* SecureRandom srnd = new SecureRandom();
        
        BigInteger bn;
        //n = bigIntToByteArray(bn);
        
       // opp1 = bigIntToByteArray(new BigInteger(1024, srnd).mod(bn));
       // opp2 = bigIntToByteArray(new BigInteger(1024, srnd).mod(bn));
         String test;
         int tes;
       do{
           bn = new BigInteger(1024, srnd);
           n = bn.toByteArray();
           test=bytesToHex(new byte[] { n[n.length-1]});
            System.out.println("test:"+ test);
            System.out.println("n.lenght:"+ (n.length !=128));
            System.out.println("nttttt:"+ (bn.mod(new BigInteger("2")).intValue()));
            tes=bn.mod(new BigInteger("2")).intValue();
            
            System.out.println("true or false:"+Objects.equals(test, "94"));
       }while( bn.mod(new BigInteger("2")).intValue()==0 || n.length !=128 );
       
        System.out.println("n:"+ n[n.length-1]);
        
        System.out.println("nHEX:"+ bytesToHex(new byte[] { n[n.length-1]}));
        BigInteger bopp1;
        do{
        bopp1  = new BigInteger(1024, srnd);
       opp1 = bopp1.toByteArray();
      } while(  opp1.length !=128 );
        
        BigInteger  bopp2;
       do{
        bopp2 = new BigInteger(1024, srnd);
        opp2 = bopp2.toByteArray();
        } while(  opp1.length !=128 );
        System.out.println("OPP1XOPP2:"+ bopp1.compareTo(bopp2));
        System.out.println("OPP1X N:"+ bopp1.compareTo(bn));
        System.out.println("OPP2X n:"+ bopp2.compareTo(bn));
       
        System.out.println("OPP1 >>>: " + bopp1);
        System.out.println("OPP2 >>>: " + bopp2);
        
        System.out.println("n >>>: " + bn);
        
        
        
        System.out.println("OPP1 >>>: " + bytesToHex(opp1));
        System.out.println("OPP2 >>>: " + bytesToHex(opp2));
        
        System.out.println("n >>>: " + bytesToHex(n));
        
        System.out.println("VELIKOST OPP1: "+opp1.length);
        System.out.println("VELIKOST OPP2: "+opp2.length);
        System.out.println("VELIKOST n: "+n.length);

         */
        int priznak = 0x52; //scitani

        int count_of_return = 255;
        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x01, (byte) 0x00,
                (byte) opp1.length,
                opp1,
                (byte) 0x01);
        byte[] rMUL1 = sendAPDU(ADD1);

        byte[] ADD2 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x02, (byte) 0x00,
                (byte) opp2.length,
                opp2,
                (byte) 0x01);
        byte[] rMUL2 = sendAPDU(ADD2);

        byte[] ADD3 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x03, (byte) 0x00,
                (byte) n.length,
                n,
                (byte) n.length);
        byte[] rMUL3 = sendAPDU(ADD3);
        /*
        BigInteger modMul = bopp1.modPow(bopp2, bn);
        System.err.println("modMulPC = "+modMul);
        System.err.println("modMulCARD = "+byteArrayToBigInt(rMUL3));
         */

    }

    /**
     * TESTOVACI metoda pro Modularni mocneni exponantion
     *
     */
    public void Test_MOD_EXP_ML3(int counter, byte[] opp1, byte[] opp2, byte[] modulus, int sizeOPP, int sizeModulus) throws Exception {
      /* if (opp1.length != modulus.length) {
            return;
        }
        if (!(opp2.length <= opp1.length)) {
            return;
        }
*/
        BigInteger bopp1 = new BigInteger(opp1);
        BigInteger bopp2 = new BigInteger(opp2);
        BigInteger bmodulus = new BigInteger(modulus);
       if (bopp1.compareTo(bmodulus) != -1) {
            return;
        }
        if (bopp2.compareTo(bmodulus) != -1) {
            return;
        }

        SetVariable((byte) 0x03, opp1);
        SetVariable((byte) 0x05, opp2);
        SetVariable((byte) 0x07, modulus);
        //      byte[]MYresult1=GetVariable((byte)0x08, (byte)0x01);

//         BigInteger bopp1=new BigInteger(opp1);
//         BigInteger bopp2=new BigInteger(opp2);
//         BigInteger bmodulus=new BigInteger(modulus);
        System.err.println("opp1 = " + bopp1);
        System.err.println("opp2 = " + bopp2);
        System.err.println("opp3 = " + bmodulus);

        BigInteger modMul = bopp1.modPow(bopp2, bmodulus);
        System.err.println("modMulPC = " + modMul);
        byte[] MYresult = null;
        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            Modular(0x52);  // N can not be ODD  (sude)
        //}
        
        count_time(timeStart, counter, "MOD_EXP opp_lenght" + (sizeOPP * 8) + " bits n_lenght" + (sizeModulus * 8) + " bits");
        MYresult = GetVariable((byte) 0xA1, (byte) 0x01);

        // System.err.println("modMulPC = "+modMul);
        System.err.println("modMulCARD = " + byteArrayToBigInt(MYresult));
        ArrayEquals(MYresult, modMul.toByteArray(), "SOME ERROR WITH MODULAR EXP");

        /*  long timeStart = System.currentTimeMillis();
        for (int i = 0; i < counter; i++) {
                Modular_EXP(opp1,opp2,modulus);  // N can not be ODD  (sude)
        }
             count_time(timeStart, counter,"MOD_EXP opp_lenght"+(opp1.length*8)+" bits n_lenght"+(modulus.length*8)+" bits");
         */
    }
     /**
     * TESTOVACI metoda pro Modularni mocneni exponantion
     *
     */
    public void Test_MOD_EXP_UBM(int counter, byte[] opp1, byte[] opp2, byte[] modulus, int sizeOPP, int sizeModulus) throws Exception {
      /* if (opp1.length != modulus.length) {
            return;
        }
        if (!(opp2.length <= opp1.length)) {
            return;
        }
*/
        BigInteger bopp1 = new BigInteger(opp1);
        BigInteger bopp2 = new BigInteger(opp2);
        BigInteger bmodulus = new BigInteger(modulus);
       if (bopp1.compareTo(bmodulus) != -1) {
            return;
        }
        if (bopp2.compareTo(bmodulus) != -1) {
            return;
        }

        SetVariable((byte) 0x03, opp1);
        SetVariable((byte) 0x05, opp2);
        SetVariable((byte) 0x07, modulus);
        //      byte[]MYresult1=GetVariable((byte)0x08, (byte)0x01);

//         BigInteger bopp1=new BigInteger(opp1);
//         BigInteger bopp2=new BigInteger(opp2);
//         BigInteger bmodulus=new BigInteger(modulus);
        System.err.println("opp1 = " + bopp1);
        System.err.println("opp2 = " + bopp2);
        System.err.println("opp3 = " + bmodulus);

        BigInteger modMul = bopp1.modPow(bopp2, bmodulus);
        System.err.println("modMulPC = " + modMul);
        byte[] MYresult = null;
        long timeStart = System.currentTimeMillis();
     //   for (int i = 0; i < counter; i++) {
            Modular(0x52);  // N can not be ODD  (sude)
       // }
        count_time(timeStart, counter, "MOD_EXP opp_lenght" + (opp1.length * 8) + " bits n_lenght" + (modulus.length * 8) + " bits");
        MYresult = GetVariable((byte) 0xA1, (byte) 0x01);

        System.err.println("modMulPC = "+modMul);
        System.err.println("modMulCARD = " + byteArrayToBigInt(MYresult));
        ArrayEquals(MYresult, modMul.toByteArray(), "SOME ERROR WITH MODULAR EXP");

        /*  long timeStart = System.currentTimeMillis();
        for (int i = 0; i < counter; i++) {
                Modular_EXP(opp1,opp2,modulus);  // N can not be ODD  (sude)
        }
             count_time(timeStart, counter,"MOD_EXP opp_lenght"+(opp1.length*8)+" bits n_lenght"+(modulus.length*8)+" bits");
         */
    }

    /**
     * Modularni redukce reduction
     *
     */
    public void Modular_Redcution(byte[] opp1, byte[] n) {

        /* SecureRandom srnd = new SecureRandom();
        
        BigInteger bn;
        //n = bigIntToByteArray(bn);
        
       // opp1 = bigIntToByteArray(new BigInteger(1024, srnd).mod(bn));
       // opp2 = bigIntToByteArray(new BigInteger(1024, srnd).mod(bn));
         String test;
         int tes;
       do{
           bn = new BigInteger(80, srnd);
           n = bn.toByteArray();
           test=bytesToHex(new byte[] { n[n.length-1]});
            System.out.println("test:"+ test);
            System.out.println("n.lenght:"+ (n.length !=8));
            System.out.println("nttttt:"+ (bn.mod(new BigInteger("2")).intValue()));
            tes=bn.mod(new BigInteger("2")).intValue();
            
            System.out.println("true or false:"+Objects.equals(test, "94"));
            
       }while( bn.mod(new BigInteger("2")).intValue()==0 || n.length !=10 );
       
        System.out.println("n:"+ n[n.length-1]);
        
        System.out.println("nHEX:"+ bytesToHex(new byte[] { n[n.length-1]}));
        BigInteger bopp1;
        do{
        bopp1  = new BigInteger(80, srnd);
       opp1 = bopp1.toByteArray();
            System.out.println("helloworldmultos.HelloWorldMultOS.Modular_Redcution()");
      } while(  opp1.length !=10 );
        
       // BigInteger  bopp2;
       //do{
      //  bopp2 = new BigInteger(1024, srnd);
       // opp2 = bopp2.toByteArray();
      //  } while(  opp1.length !=128 );
    //    System.out.println("OPP1XOPP2:"+ bopp1.compareTo(bopp2));
        System.out.println("OPP1X N:"+ bopp1.compareTo(bn));
      //  System.out.println("OPP2X n:"+ bopp2.compareTo(bn));
       
        System.out.println("OPP1 >>>: " + bopp1);
       // System.out.println("OPP2 >>>: " + bopp2);
        
        System.out.println("n >>>: " + bn);
        
        
        
        System.out.println("OPP1 >>>: " + bytesToHex(opp1));
     //   System.out.println("OPP2 >>>: " + bytesToHex(opp2));
        
        System.out.println("n >>>: " + bytesToHex(n));
        
        System.out.println("VELIKOST OPP1: "+opp1.length);
      //  System.out.println("VELIKOST OPP2: "+opp2.length);
        System.out.println("VELIKOST n: "+n.length);

        
        
        
         */
        int priznak = 0x53; //reduction

        int count_of_return = 255;
        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x01, (byte) 0x00,
                (byte) opp1.length,
                opp1,
                (byte) 0x01);
        byte[] rMUL1 = sendAPDU(ADD1);

        byte[] ADD2 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x02, (byte) 0x00,
                (byte) n.length,
                n,
                (byte) count_of_return);
        byte[] rMUL2 = sendAPDU(ADD2);

        /*   BigInteger modMul = bopp1.modPow(new BigInteger("1"), bn);
        System.err.println("modMulPC = "+modMul);
        System.err.println("modMulCARD = "+byteArrayToBigInt(rMUL2));
         */
    }

    /**
     * TESTOVACI metoda pro Modularni redukci
     *
     */
    public void Test_MOD_RED(int counter, byte[] opp1, byte[] modulus) throws Exception {
        SetVariable((byte) 0x03, opp1);
        //   SetVariable((byte)0x05, opp2);
        SetVariable((byte) 0x07, modulus);
        long timeStart = System.currentTimeMillis();
        //for (int i = 0; i < counter; i++) {
            Modular(0x53);  // N can not be ODD  (sude)
       // }
        count_time(timeStart, counter, "MOD_REDUCTION opp_lenght" + (opp1.length * 8) + " bits n_lenght" + (modulus.length * 8) + " bits");
        byte[] MYresult = GetVariable((byte) 0x04, (byte) 0x01);

        BigInteger bopp1 = new BigInteger(opp1);
        // BigInteger bopp2=new BigInteger(opp2);
        BigInteger bmodulus = new BigInteger(modulus);
        BigInteger modMul = bopp1.modPow(new BigInteger("1"), bmodulus);
        // System.err.println("modMulPC = "+modMul);
        // System.err.println("modMulCARD = "+byteArrayToBigInt(MYresult));
        ArrayEquals(MYresult, modMul.toByteArray(), "SOME ERROR WITH MODULAR REDUCTION");

        /*   long timeStart = System.currentTimeMillis();
        for (int i = 0; i < counter; i++) {
                Modular_Redcution(opp1,modulus);  // N can not be ODD  (sude)
        }
        count_time(timeStart, counter,"MOD_Reduction opp_lenght"+(opp1.length*8)+" bits n_lenght"+(modulus.length*8)+" bits");
         */
    }

    /**
     * Modularni inverze inversion
     *
     */
    public void Modular_Inversion(byte[] opp1, byte[] n) {
        /*
         SecureRandom srnd = new SecureRandom();
        
        BigInteger bn;
        //n = bigIntToByteArray(bn);
        
       // opp1 = bigIntToByteArray(new BigInteger(1024, srnd).mod(bn));
       // opp2 = bigIntToByteArray(new BigInteger(1024, srnd).mod(bn));
         String test;
         int tes;
       boolean provedeno=false;
         do{
         do{
           bn = new BigInteger(1024, srnd);
           n = bn.toByteArray();
           test=bytesToHex(new byte[] { n[n.length-1]});
            System.out.println("test:"+ test);
            System.out.println("n.lenght:"+ (n.length !=8));
            System.out.println("nttttt:"+ (bn.mod(new BigInteger("2")).intValue()));
            tes=bn.mod(new BigInteger("2")).intValue();
            
            System.out.println("true or false:"+Objects.equals(test, "94"));
            
       }while( bn.mod(new BigInteger("2")).intValue()==0 || n.length !=128 );
       
        System.out.println("n:"+ n[n.length-1]);
        
        System.out.println("nHEX:"+ bytesToHex(new byte[] { n[n.length-1]}));
        BigInteger bopp1;
        do{
        bopp1  = new BigInteger(1024, srnd);
       opp1 = bopp1.toByteArray();
      } while(  opp1.length !=128 );
        
       // BigInteger  bopp2;
       //do{
      //  bopp2 = new BigInteger(1024, srnd);
       // opp2 = bopp2.toByteArray();
      //  } while(  opp1.length !=128 );
    //    System.out.println("OPP1XOPP2:"+ bopp1.compareTo(bopp2));
        System.out.println("OPP1X N:"+ bopp1.compareTo(bn));
      //  System.out.println("OPP2X n:"+ bopp2.compareTo(bn));
       
        System.out.println("OPP1 >>>: " + bopp1);
       // System.out.println("OPP2 >>>: " + bopp2);
        
        System.out.println("n >>>: " + bn);
        
        
        
        System.out.println("OPP1 >>>: " + bytesToHex(opp1));
     //   System.out.println("OPP2 >>>: " + bytesToHex(opp2));
        
        System.out.println("n >>>: " + bytesToHex(n));
        
        System.out.println("VELIKOST OPP1: "+opp1.length);
      //  System.out.println("VELIKOST OPP2: "+opp2.length);
        System.out.println("VELIKOST n: "+n.length);

         */

        int priznak = 0x54; //inversrion

        int count_of_return = 255;
        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x01, (byte) 0x00,
                (byte) opp1.length,
                opp1,
                (byte) opp1.length);
        byte[] rMUL1 = sendAPDU(ADD1);

        byte[] ADD2 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x02, (byte) 0x00,
                (byte) n.length,
                n,
                (byte) n.length);
        byte[] rMUL2 = sendAPDU(ADD2);

        /*
        try {
            
          BigInteger modMul = bopp1.modInverse(bn);
        System.err.println("modMulPC = "+modMul);
        System.err.println("modMulCARD = "+byteArrayToBigInt(rMUL2));
        provedeno=true;
        } 
        
        catch (Exception e) {
             System.err.println("modMulPC = ");
             provedeno=false;
                     
        }
        }
        while(!provedeno);
         */
    }

    /**
     * TESTOVACI metoda pro Modularni inverzi
     *
     */
    public void Test_MOD_INV(int counter, int numberOfBytes, int numberOfBytesModulus) throws Exception {
        SecureRandom srnd = new SecureRandom();
        byte[] opp1;
        byte[] modulus;
        BigInteger bn;
        BigInteger modMul = new BigInteger("0");
        boolean provedeno = false;
        do {
            do {
                bn = new BigInteger(numberOfBytesModulus * 8, srnd);
                modulus = bn.toByteArray();

            } while (bn.mod(new BigInteger("2")).intValue() == 0 || modulus.length != numberOfBytesModulus);

            BigInteger bopp1;
            do {
                bopp1 = new BigInteger(numberOfBytes * 8, srnd);
                opp1 = bopp1.toByteArray();
            } while (opp1.length != numberOfBytes);

            try {

                modMul = bopp1.modInverse(bn);
                // System.err.println("modMulPC = "+modMul);
                //  System.err.println("modMulCARD = "+byteArrayToBigInt(rMUL2));
                provedeno = true;
            } catch (Exception e) {
                //        System.err.println("modMulPC = ");
                provedeno = false;

            }
        } while (!provedeno);

        // System.out.println("TEST MODULAR INVERSE");
        //System.out.println("|a|=|n|:"+modulus.length*8 + "bit");
        SetVariable((byte) 0x03, opp1);
        SetVariable((byte) 0x07, modulus);

        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            Modular(0x54);  // N can not be ODD  (sude)
        //}
        count_time(timeStart, counter, "MOD_Inversion opp_lenght: " + (opp1.length * 8) + " bits n_lenght: " + (modulus.length * 8) + " bits");

        byte[] MYresult = GetVariable((byte) 0x91, (byte) 0x01);

        ArrayEquals(MYresult, modMul.toByteArray(), "SOME ERROR WITH MODULAR EXP");

        /* SetVariable((byte)0x03, opp1);
         SetVariable((byte)0x05, opp2);
         SetVariable((byte)0x07, modulus);
         long timeStart = System.currentTimeMillis();
        for (int i = 0; i < counter; i++) {
                Modular(0x50);  // N can not be ODD  (sude)
        }
          count_time(timeStart, counter,"MOD_EXP opp_lenght"+(opp1.length*8)+" bits n_lenght"+(modulus.length*8)+" bits");
         byte[]MYresult=GetVariable((byte)0x91, (byte)0x01);
        
         BigInteger bopp1=new BigInteger(opp1);
         BigInteger bopp2=new BigInteger(opp2);
         BigInteger bmodulus=new BigInteger(modulus);
     //    BigInteger modMul = bopp1.modPow(bopp2, bn);
         BigInteger modMul = bopp1.modPow(bopp2, bmodulus);
       // System.err.println("modMulPC = "+modMul);
       // System.err.println("modMulCARD = "+byteArrayToBigInt(MYresult));
          ArrayEquals(MYresult, modMul.toByteArray(),"SOME ERROR WITH MODULAR EXP");*/
    }
    
    
    
    
}
