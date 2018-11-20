/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package helloworldmultos;

import static helloworldmultos.Variables.GetVariable;
import static helloworldmultos.Variables.SetCounter;
import static helloworldmultos.Variables.SetVariable;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.lang.reflect.Array;
import java.math.BigInteger;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.util.Arrays;
import java.util.Base64;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.TreeMap;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.SecretKeyFactory;
import javax.crypto.spec.DESKeySpec;
import javax.smartcardio.Card;
import javax.smartcardio.CardChannel;
import javax.smartcardio.CardException;
import javax.smartcardio.CardTerminal;
import javax.smartcardio.CommandAPDU;
import javax.smartcardio.ResponseAPDU;
import javax.smartcardio.TerminalFactory;

/**
 *
 * @author DzurendaPetr(106420)
 */
public class HelloWorldMultOS {

    public static boolean DEBUG = true;
   // public boolean ML3 = true;

   public boolean ML3=false;
    public static int counter;
    public static Map<String,Long> mapa;
    protected List<CardTerminal> terminals;
    protected TerminalFactory factory;
    private CardTerminal terminal;
    private Card card;
    private static CardChannel channel;
    private ResponseAPDU rAPDU;
    private static byte[] baCommandAPDU;
 public static PrintWriter writer;
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception {
        // TODO code application logic here

        try{
            Files.delete(Paths.get("APDUs.txt"));
            Files.delete(Paths.get("result.txt"));
            
            
        }
        catch(IOException e){
            ;
        }
        HelloWorldMultOS helloWorld = new HelloWorldMultOS();
        try {
            helloWorld.run();
        } catch (NoSuchAlgorithmException ex) {
            Logger.getLogger(HelloWorldMultOS.class.getName()).log(Level.SEVERE, null, ex);
        }

    }

    public void run() throws NoSuchAlgorithmException, Exception {

        try {

            try {
                factory = TerminalFactory.getDefault();
                terminals = factory.terminals().list();
                terminal = terminals.get(0);

                System.out.println("Terminals: " + terminals);
                System.out.println("Selected Terminal: " + terminal);

            } catch (Exception e) {
                e.printStackTrace();
            }

            try {

                while (!terminal.isCardPresent()) {
                };

                card = terminal.connect("*");
                System.out.println("ATR: " + bytesToHex(card.getATR().getBytes()));
                channel = card.getBasicChannel();

                System.out.println("\nCard info: " + card);

            } catch (CardException ce) {
                ce.printStackTrace();
            }

            byte[] AID = new byte[]{(byte) 0x00, (byte) 0xA4, (byte) 0x04, (byte) 0x00,
                (byte) 0x04,
                (byte) 0xF0, (byte) 0x00, (byte) 0x00, (byte) 0x01,
                (byte) 0x00};

            byte[] rData = sendAPDU(AID);

            byte[] vstupniText = new byte[]{(byte) 0x01, (byte) 0x02, (byte) 0x03, (byte) 0x04, (byte) 0x05, (byte) 0x06, (byte) 0x07, (byte) 0x08, (byte) 0x09, (byte) 0x10, (byte) 0x11, (byte) 0x12, (byte) 0x13, (byte) 0x14, (byte) 0x15, (byte) 0x16};

            byte[] opp1;
            byte[] opp2;
            byte[] modulus;
            byte[] key;
            int[] sizesKeys = new int[]{128, 192, 256};
            mapa=new LinkedHashMap<String, Long>();
            String format;
            String size;
            String p;
            String A;
            String B;
            String x;
            String y;
            String q;
            String h;
/*
              /// 160bit/// nefungje na ML3 
               format=new String("00");
              size=new String("14");
              p=new String("E95E4A5F737059DC60DFC7AD95B3D8139515620F");
              A=new String("340E7BE2A280EB74E2BE61BADA745D97E8F7C300");
             B=new String("1E589A8595423412134FAA2DBDEC95C8D8675E58");
             x=new String("BED5AF16EA3F6A4F62938C4631EB5AF7BDBCDBC3");
              y=new String("1667CB477A1A8EC338F94741669C976316DA6321");
              q=new String("E95E4A5F737059DC60DF5991D45029409E60FC09");
              h=new String("01");
             ////end 160///
            
            */
 //192
             format=new String("00");
              size=new String("18");
              p=new String("C302F41D932A36CDA7A3463093D18DB78FCE476DE1A86297");
              A=new String("6A91174076B1E0E19C39C031FE8685C1CAE040E5C69A28EF");
             B=new String("469A28EF7C28CCA3DC721D044F4496BCCA7EF4146FBF25C9");
             x=new String("C0A0647EAAB6A48753B033C56CB0F0900A2F5C4853375FD6");
              y=new String("14B690866ABD5BB88B5F4828C1490002E6773FA2FA299B8F");
              q=new String("C302F41D932A36CDA7A3462F9E9E916B5BE8F1029AC4ACC1");
              h=new String("01");
              //end 192 
             
 /*
            //224
             format=new String("00");
              size=new String("1C");
              p=new String("D7C134AA264366862A18302575D1D787B09F075797DA89F57EC8C0FF");
              A=new String("68A5E62CA9CE6C1C299803A6C1530B514E182AD8B0042A59CAD29F43");
             B=new String("2580F63CCFE44138870713B1A92369E33E2135D266DBB372386C400B");
             x=new String("0D9029AD2C7E5CF4340823B2A87DC68C9E4CE3174C1E6EFDEE12C07D");
              y=new String("58AA56F772C0726F24C6B89E4ECDAC24354B9E99CAA3F6D3761402CD");
              q=new String("D7C134AA264366862A18302575D0FB98D116BC4B6DDEBCA3A5A7939F");
              h=new String("01");
              //end 224 
             */
   /*
               //256
             format=new String("00");
              size=new String("20");
              p=new String("A9FB57DBA1EEA9BC3E660A909D838D726E3BF623D52620282013481D1F6E5377");
              A=new String("7D5A0975FC2C3057EEF67530417AFFE7FB8055C126DC5C6CE94A4B44F330B5D9");
             B=new String("26DC5C6CE94A4B44F330B5D9BBD77CBF958416295CF7E1CE6BCCDC18FF8C07B6");
             x=new String("8BD2AEB9CB7E57CB2C4B482FFC81B7AFB9DE27E1E3BD23C23A4453BD9ACE3262");
              y=new String("547EF835C3DAC4FD97F8461A14611DC9C27745132DED8E545C1D54C72F046997");
              q=new String("A9FB57DBA1EEA9BC3E660A909D838D718C397AA3B561A6F7901E0E82974856A7");
              h=new String("01");
              //end 256 
         */    
   //320
   /*
             format=new String("00");
              size=new String("28");
              p=new String("D35E472036BC4FB7E13C785ED201E065F98FCFA6F6F40DEF4F92B9EC7893EC" +
"28FCD412B1F1B32E27");
              A=new String("3EE30B568FBAB0F883CCEBD46D3F3BB8A2A73513F5EB79DA66190EB085FFA9" +
"F492F375A97D860EB4");
             B=new String("520883949DFDBC42D3AD198640688A6FE13F41349554B49ACC31DCCD884539" +
"816F5EB4AC8FB1F1A6");
             x=new String("43BD7E9AFB53D8B85289BCC48EE5BFE6F20137D10A087EB6E7871E2A10A599" +
"C710AF8D0D39E20611");
              y=new String("14FDD05545EC1CC8AB4093247F77275E0743FFED117182EAA9C77877AAAC6A" +
"C7D35245D1692E8EE1");
              q=new String("D35E472036BC4FB7E13C785ED201E065F98FCFA5B68F12A32D482EC7EE8658" +
"E98691555B44C59311");
              h=new String("01");
              //end 320*/
         /*    
               //384
             format=new String("00");
              size=new String("30");
              p=new String("8CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B412B1DA197FB711" +
"23ACD3A729901D1A71874700133107EC53");
              A=new String("7BC382C63D8C150C3C72080ACE05AFA0C2BEA28E4FB22787139165EFBA91F9" +
"0F8AA5814A503AD4EB04A8C7DD22CE2826");
             B=new String("04A8C7DD22CE28268B39B55416F0447C2FB77DE107DCD2A62E880EA53EEB62" +
"D57CB4390295DBC9943AB78696FA504C11");
             x=new String("1D1C64F068CF45FFA2A63A81B7C13F6B8847A3E77EF14FE3DB7FCAFE0CBD10" +
"E8E826E03436D646AAEF87B2E247D4AF1E");
              y=new String("8ABE1D7520F9C2A45CB1EB8E95CFD55262B70B29FEEC5864E19C054FF99129" +
"280E4646217791811142820341263C5315");
              q=new String("8CB91E82A3386D280F5D6F7E50E641DF152F7109ED5456B31F166E6CAC0425" +
"A7CF3AB6AF6B7FC3103B883202E9046565");
              h=new String("01");
              //end 384 
        */     
/*
            ///512///
            format = new String("00");
            size = new String("40");
            p = new String("AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA703308"
                    + "717D4D9B009BC66842AECDA12AE6A380E62881FF2F2D82C68528AA6056583A48F3");
            A = new String("7830A3318B603B89E2327145AC234CC594CBDD8D3DF91610A83441CAEA9863"
                    + "BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117A72BF2C7B9E7C1AC4D77FC94CA");
            B = new String("3DF91610A83441CAEA9863BC2DED5D5AA8253AA10A2EF1C98B9AC8B57F1117"
                    + "A72BF2C7B9E7C1AC4D77FC94CADC083E67984050B75EBAE5DD2809BD638016F723");
            x = new String("81AEE4BDD82ED9645A21322E9C4C6A9385ED9F70B5D916C1B43B62EEF4D009"
                    + "8EFF3B1F78E2D0D48D50D1687B93B97D5F7C6D5047406A5E688B352209BCB9F822");
            y = new String("7DDE385D566332ECC0EABFA9CF7822FDF209F70024A57B1AA000C55B881F81"
                    + "11B2DCDE494A5F485E5BCA4BD88A2763AED1CA2B2FA8F0540678CD1E0F3AD80892");
            q = new String("AADD9DB8DBE9C48B3FD4E6AE33C9FC07CB308DB3B3C9D20ED6639CCA703308"
                    + "70553E5C414CA92619418661197FAC10471DB1D381085DDADDB58796829CA90069");
            h = new String("01");

            ///END 512//*/
            String finalString = new String(format + size + p + A + B + x + y + q + h);
            byte[] domainParams = hexStringToByteArray(finalString);
           
            counter =10;
            SetCounter(counter);
       
             ///////////////WORKING /////
             
           if (ML3) {
                SetVariable((byte) 0x92, domainParams);
                ECCGenerateKeyPair(1, domainParams);
                Test_ECC(counter, Integer.decode("0x" + size));
            }
         
            Symetric symetric = new Symetric();

            int[] sizesSymetric = new int[]{128, 1024, 2048}; //bits

            for (int j = 0; j < sizesSymetric.length; j = j + 1) {
                vstupniText = GenerateBigNumberAsArray((sizesSymetric[j]) / 8);
                
                /////DES
                symetric.Test_DES(counter, vstupniText, false);
                    /////3DES
                symetric.Test_tripleDES(counter, vstupniText);

                ///AES
                if (ML3) {
                    for (int i = 0; i < sizesKeys.length; i = i + 1) {
                        key = GenerateBigNumberAsArray((sizesKeys[i]) / 8);
                        
                        symetric.Test_AES(counter, vstupniText, key);

                    }
                }
            }

            ///////SHA
            int[] sizesInputSHA = new int[]{8, 20, 32, 64, 128, 256, 512}; //byte
            int[] sizesSHA = new int[]{20, 28, 32, 48, 64};

            if (ML3) {
                sizesSHA = new int[]{20, 28, 32, 48, 64};
            } else {
                sizesSHA = new int[]{20, 32};
            }
            for (int i = 0; i < sizesSHA.length; i = i + 1) {
                for (int j = 0; j < sizesInputSHA.length; j = j + 1) {
                    vstupniText = GenerateBigNumberAsArray(sizesInputSHA[j]);
                    Test_SHA(counter, sizesSHA[i], vstupniText); // 20 sha1 , 28 sha224 ,32 sha256
                }
            }
            ////RSA
            Test_RSA(counter);

            
            ////GENERATE RANDOM NUMBERS
            int[] sizesGenerateRandomNumbers = new int[]{1, 3, 4, 8, 16, 32, 64};
            // sizesGenerateRandomNumbers=new int[]{8,20,32,64,128,256,512};

            for (int i = 0; i < sizesGenerateRandomNumbers.length; i = i + 1) {
                Test_generateRandomNumbers(counter, sizesGenerateRandomNumbers[i]);
            }

            ////////////////TEST SPEED//////////////
            int[] sizesTESTSpeed = new int[]{8, 20, 32, 64, 128, 255};
            for (int i = 0; i < sizesTESTSpeed.length; i = i + 1) {
                testTestSpeed(counter, (sizesTESTSpeed[i]));
            }

            ////////////////TEST SPEED  ////////////////
            Modular modular = new Modular();

            modular.MODULAR_OPERATION(ML3, 0);
            modular.MODULAR_OPERATION(ML3, 1);
            modular.MODULAR_OPERATION(ML3, 2);

            printResults();
   
        } catch (Exception ex) {
            Logger.getLogger(HelloWorldMultOS.class.getName()).log(Level.SEVERE, null, ex);
        }

    }
    public static void printResults(){
       for (Map.Entry<String, Long> entry : mapa.entrySet())
{
    String result = entry.getKey() + ": " + entry.getValue()+" ms";
    List<String> myAPDU= Arrays.asList(result);
        try {
            Files.write(Paths.get("result.txt"), myAPDU, StandardCharsets.UTF_8,StandardOpenOption.CREATE,StandardOpenOption.APPEND);
        } catch (IOException ex) {
            Logger.getLogger(HelloWorldMultOS.class.getName()).log(Level.SEVERE, null, ex);
        }
    System.out.println(result);
}
    }
    
    
    public void SetKeyForSymetricCrypto(byte[] key) {

        int priznak = 0x01;

        byte[] APDUkey = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x00, (byte) 0x00,
                (byte) key.length,
                key,
                (byte) key.length);
        byte[] rKEY = sendAPDU(APDUkey);
    }

    ///

    public byte[] getMODLEN() {

        int priznak = 0x94;
        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, (byte) 0x00, (byte) 0x00,
                (byte) 0x00,
                null,
                (byte) 0x02);
        return sendAPDU(ADD1);
    }

    public void RSA(boolean Encipher) {

        byte P1 = isEncipher(Encipher);
        int priznak = 0x20; //RSA
        byte[] ADD1 = createAPDUComm((byte) 0x80, (byte) priznak, P1, (byte) 0x00,
                (byte) 0x00,
                null,
                (byte) 0x00);
        byte[] rMUL1 = sendAPDU(ADD1);
    }

    private void Test_RSA(int counter) throws Exception {
        //   System.out.println("RSA");
        byte[] MODLEN = getMODLEN();
        byte[] vstup = GenerateBigNumberAsArray(Integer.decode("0x" + bytesToHex(MODLEN)));
        SetVariable((byte) 0x03, vstup);
        //SetVariable((byte)0x01, key);

        byte[] secured = GetVariable((byte) 0x04, (byte) 0x00);

        long timeStart = System.currentTimeMillis();
      //  for (int i = 0; i < counter; i++) {
            RSA(true);
      //  }
        count_time(timeStart, counter, "RSA Encipher input_lenght" + (vstup.length * 8) + " bits");

       // for (int i = 0; i < counter; i++) {
            RSA(false);
       // }
        count_time(timeStart, counter, "RSA Decipher input_lenght" + (vstup.length * 8) + " bits");

        byte[] returnSecured = GetVariable((byte) 0x08, (byte) 0x01);

        //    System.out.println("VYSTUP:"+print1(Arrays.copyOf(secured,  returnSecured.length)));
        // System.out.println("VYSTUP:"+print1(returnSecured));      
        ArrayEquals(returnSecured, Arrays.copyOf(secured, returnSecured.length), "SOME ERROR WITH RSA ENCRYPTION");

    }

    public void ECCGenerateKeyPair(int velikost, byte[] data) {

        byte[] SHA2 = createAPDUComm((byte) 0x80, (byte) 0x22, (byte) velikost, (byte) 0x00,
                0x00,
                null,
                (byte) 0x00);
        byte[] rSHA2 = sendAPDU(SHA2);
    }

    public void ECDH() {

        byte[] ECC = createAPDUComm((byte) 0x80, (byte) 0x23, (byte) 0x00, (byte) 0x00,
                0x00,
                null,
                (byte) 0x00);
        byte[] rECC = sendAPDU(ECC);
    }

    public void ECDSA(boolean sign) {
        byte P1=isEncipher(sign);
        byte[] ECC = createAPDUComm((byte) 0x80, (byte) 0x24, P1, (byte) 0x00,
                0x00,
                null,
                (byte) 0x00);
        byte[] rECC = sendAPDU(ECC);
    }

    /**
     * TESTOVACI metoda pro ECC
     *
     */
    public void Test_ECC(int counter, int velikost) {
        long timeStart = System.currentTimeMillis();
            ECDH();
        count_time(timeStart, counter, "ECDH-" + velikost * 8);

        SetVariable((byte) 0x03, GenerateBigNumberAsArray(128));
        timeStart = System.currentTimeMillis();
            ECDSA(true);
        count_time(timeStart, counter, "ECDSA SIGN-" + velikost * 8);

        timeStart = System.currentTimeMillis();
            ECDSA(true);
        count_time(timeStart, counter, "ECDSA VERIFY-" + velikost * 8);

        //  byte[] returnSecured=GetVariable((byte)0x06, (byte)0x00);
        // System.out.println(print1(returnSecured));
    }

    /**
     * Call SHA
     *
     *
     */
    public void SHA(int velikost, byte[] data) {

        byte[] SHA2 = createAPDUComm((byte) 0x80, (byte) 0x31, (byte) velikost, (byte) 0x00,
                0x00,
                null,
                (byte) 0x00);
        byte[] rSHA2 = sendAPDU(SHA2);
    }

    /**
     * TESTOVACI metoda pro SHA
     *
     */
    private void Test_SHA(int counter, int velikost, byte[] vstup) {
        SetVariable((byte) 0x03, vstup);
        byte[] secured = GetVariable((byte) 0x04, (byte) 0x00);

        //  System.out.println(print1(secured));
        long timeStart = System.currentTimeMillis();
        //for (int i = 0; i < counter; i++) {
            SHA(velikost, vstup);
        //}
        count_time(timeStart, counter, "SHA-" + velikost * 8 + " input_lenght" + vstup.length);
        byte[] returnSecured = GetVariable((byte) 0x06, (byte) 0x00);
//         System.out.println(print1(returnSecured));
    }
    
      private void testTestSpeed(int counter, int howMuchBytes) {
          if(howMuchBytes>255){
              System.out.println("TOO MUCH BYTES FOR ONE MESSAGE");
              return;}
       byte[] data= GenerateBigNumberAsArray(howMuchBytes);
        byte[] RNG = createAPDUComm((byte) 0x80, (byte) 0x41, (byte) 0x00, (byte) 0x00,
                (byte) data.length,
                data,
                (byte) 0x00);
          long timeStart = System.currentTimeMillis();
      //  for (int i = 0; i < counter; i++) {
              sendAPDU(RNG);
        //}
         long timeIssueAtt = (System.currentTimeMillis() - timeStart) / counter;
                                                            //5 byte to card, 2 bytes respond
        System.out.println("SPEED transmission: "+(howMuchBytes+5+2)*counter/(timeIssueAtt) +" bytes per second "+ "("+ (howMuchBytes+5+2)+ " bytes)");
        
         List<String> myAPDU= Arrays.asList("SPEED transmission: "+(howMuchBytes+5+2)*counter/(timeIssueAtt) +" bytes per second "+ "("+ (howMuchBytes+5+2)+ " bytes)");
        try {
            Files.write(Paths.get("result.txt"), myAPDU, StandardCharsets.UTF_8,StandardOpenOption.CREATE,StandardOpenOption.APPEND);
        } catch (IOException ex) {
            Logger.getLogger(HelloWorldMultOS.class.getName()).log(Level.SEVERE, null, ex);
        }
        
        
      }
        
    
    

    /**
     * Call generate random numbers funciton on card
     *
     *
     */
    public void generateRandomNumbers(int howMuchBytes) {

        byte[] RNG = createAPDUComm((byte) 0x80, (byte) 0x40, (byte) howMuchBytes, (byte) 0x00,
                (byte) 0x00,
                null,
                (byte) howMuchBytes);
        byte[] rRNG = sendAPDU(RNG);
    }
   
    

    /**
     * TESTOVACI metoda pro random numbers funciton on card
     *
     */
    private void Test_generateRandomNumbers(int counter, int howMuchBytes) {
        long timeStart = System.currentTimeMillis();
       // for (int i = 0; i < counter; i++) {
            generateRandomNumbers(howMuchBytes);
       // }
        count_time(timeStart, counter, "Generate Random Numbers: " + howMuchBytes + " bytes ");
    }


    public static void count_time(long timeStart, int counter, String description) {
        long timeIssueAtt = (System.currentTimeMillis() - timeStart) / counter;
        // System.out.println("TIME START:"+timeStart);
        String result= description + " TIME: " + timeIssueAtt+" ms" ;
        System.out.println(result);
        mapa.put(description, timeIssueAtt );
         
  
    }

    public static BigInteger byteArrayToBigInt(byte[] array) {

        byte[] directArray = new byte[array.length + 1];

        System.arraycopy(array, 0, directArray, 1, array.length);
        directArray[0] = (byte) 0x00;

        return new BigInteger(directArray);
    }

   

    public static byte[] GenerateBigNumberAsArray(int lenghtBytes) {
        byte[] n;
        SecureRandom srnd = new SecureRandom();
        BigInteger bn;

        do {
            bn = new BigInteger(lenghtBytes * 8, srnd);
            n = bn.toByteArray();
        } while (bn.mod(new BigInteger("2")).intValue() == 0 || n.length != lenghtBytes);

        return n;

    }

   

    public static byte[] hexStringToByteArray(String s) {
        int len = s.length();
        byte[] data = new byte[len / 2];
        for (int i = 0; i < len; i += 2) {
            data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
                    + Character.digit(s.charAt(i + 1), 16));
        }
        return data;
    }

    
     public static void ArrayEquals(byte[] returnSecured, byte[] vstup, String text) throws Exception {
        if (!Arrays.equals(returnSecured, vstup)) {
            printResults();
            throw new Exception(text);
            
        }
    }
    
    
    /**
     * Create APDU command
     *
     * @param CLA
     * @param INS
     * @param P1
     * @param P2
     * @param Lc
     * @param data
     * @param Le
     * @return
     */
    public static byte[] createAPDUComm(byte CLA, byte INS, byte P1, byte P2, int Lc, byte[] data, byte Le) {

        byte[] apduComm;
        int LcSize = 1;
        int LeSize = 1;

        if (Lc > 255) {
            LcSize = 3;
        }

        if (Lc > 255) {
            LeSize = 3;
        }

        if (Le == 0 && Lc == 0) {                                         //Case 1
            apduComm = new byte[4];
        } else if (Le != 0 && Lc == 0) {                                   //Case 2
            apduComm = new byte[5];
            apduComm[apduComm.length - 1] = Le;

        } else if (Le == 0 && Lc != 0) //Case 3
        {
            apduComm = new byte[4 + LcSize + data.length];
        } else if (Le != 0 && Lc != 0) {                                   //Case 4
            apduComm = new byte[4 + LcSize + data.length + LeSize];
            apduComm[apduComm.length - 1] = Le;
        } else {
            return null;
        }

        apduComm[0] = CLA;
        apduComm[1] = INS;
        apduComm[2] = P1;
        apduComm[3] = P2;

        if (Lc != 0) {
            apduComm[4] = (byte) data.length;
            System.arraycopy(data, 0, apduComm, 5, data.length);
        }

        return apduComm;
    }

    /**
     * Convert bytes to hexadecimal string
     *
     * @param bytes
     * @return
     */
    public static String bytesToHex(byte[] bytes) {

        char[] hexArray = "0123456789ABCDEF".toCharArray();
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++) {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    /**
     * Send APDU command to the card
     */
    public static byte[] sendAPDU(byte[] data) {

        byte[] baResponceAPDU = null;

        baCommandAPDU = data;
        if (DEBUG) {
            System.out.println("APDU >>>: " + bytesToHex(baCommandAPDU));
        }
       List<String> myAPDU= Arrays.asList("-apdu " + bytesToHex(baCommandAPDU));
        try {
            Files.write(Paths.get("APDUs.txt"), myAPDU, StandardCharsets.UTF_8,StandardOpenOption.CREATE,StandardOpenOption.APPEND);
        } catch (IOException ex) {
            Logger.getLogger(HelloWorldMultOS.class.getName()).log(Level.SEVERE, null, ex);
        }
    //    writer.println("-apdu " + bytesToHex(baCommandAPDU));
//writer.close();


        try {

            ResponseAPDU r = channel.transmit(new CommandAPDU(baCommandAPDU));

            baResponceAPDU = r.getBytes();
            if (DEBUG) {
                System.out.println("APDU <<<: " + bytesToHex(baResponceAPDU) + " SW =" + bytesToHex(new byte[]{(byte) r.getSW1(), (byte) r.getSW2()}));
            }
            return r.getData();

        } catch (CardException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
            return null;
        }

    }
    
    
    
     public static byte isEncipher(boolean Encipher) {
        byte P1;
        if (Encipher) {
            P1 = (byte) 0x00;
        } else {
            P1 = (byte) 0x01;
        }
        return P1;
    }

    public static String print1(byte[] bytes) {
        StringBuilder sb = new StringBuilder();
        sb.append("[ ");
        for (byte b : bytes) {
            sb.append(String.format("0x%02X ", b));
        }
        sb.append("]");
        return sb.toString();
    }

}
