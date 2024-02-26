


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4); 
/************************Hardware Related Macros************************************/
#define         MQ_PIN_1                       (A1)     //define which analog input channel you are going to use
#define         MQ_PIN_2                       (A0)
#define         RL_VALUE                     (10)    //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.21)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
 
/***********************Software Related Macros************************************/
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (500)   //define the time interal(in milisecond) between each samples in the
                                                     //cablibration phase
#define         READ_SAMPLE_INTERVAL         (50)    //define how many samples you are going to take in normal operation
#define         READ_SAMPLE_TIMES            (5)     //define the time interal(in milisecond) between each samples in 
                                                     //normal operation
/**********************Application Related Macros**********************************/
#define         GAS_H2                      (0)
/*****************************Globals***********************************************/
float           H2Curve[3]  =  {2.3, 0.93,-1.44};    //two points are taken from the curve in datasheet. 
                                                     //with these two points, a line is formed which is "approximately equivalent" 
                                                     //to the original curve. 
                                                     //data format:{ x, y, slope}; point1: (lg200, lg8.5), point2: (lg10000, lg0.03) 
 
float           Ro1           =  10;                  //Ro is initialized to 10 kilo ohms
float           Ro2           =  10;  
int MAX_PPM_1 = 100;
int MAX_PPM_2 = 100; 

void setup() {
  
  lcd.init();
  lcd.backlight();
  pinMode(12, INPUT);
  pinMode(5, OUTPUT);
  pinMode(2,OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("Calibrating...");               
  //Calibrating both the sensor. Wait for 2 minutes. Please make sure the sensor is in clean air. 
  Ro1 = MQCalibration(MQ_PIN_1);
  lcd.setCursor(0,0);
  lcd.print("1st Done..."); 
  Ro2 = MQCalibration(MQ_PIN_2);                                                   //when you perform the calibration                    
  lcd.setCursor(0,0);
  lcd.print("2nd Done..."); 
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Ro1 = ");
  lcd.print(Ro1);
  lcd.print(" kohms");
  lcd.setCursor(0,1);
  lcd.print("Ro2 = ");
  lcd.print(Ro2);
  lcd.print(" kohms");
  delay(2000);
  lcd.clear();
}
 
void loop() {
   int ppm1 = MQGetGasPercentage(MQRead(MQ_PIN_1)/Ro1,GAS_H2);
   int ppm2 = MQGetGasPercentage(MQRead(MQ_PIN_2)/Ro2,GAS_H2);

   lcd.setCursor(0,0);
   lcd.print("PPM Inside: ");
   lcd.print(ppm1);
   
   lcd.setCursor(0,1);
   lcd.print("PPM Outside: ");
   lcd.print(ppm2);
   
   pinMode(2,HIGH);
   
   if ( ppm1 >= MAX_PPM_1) {
    lcd.clear();
    while (true) {
         ppm1 = MQGetGasPercentage(MQRead(MQ_PIN_1)/Ro1,GAS_H2);
         ppm2 = MQGetGasPercentage(MQRead(MQ_PIN_2)/Ro2,GAS_H2);
         lcd.setCursor(0,0);
         lcd.print("Leakage Inside");
         lcd.setCursor(0,1);
         lcd.print("PPM Outside: ");
         lcd.print(ppm2);

         if(ppm2>=MAX_PPM_2){
          goto both;
         }
      int btnVal = digitalRead(12);
      
      if (btnVal == 1) {
        digitalWrite(5, LOW);
        digitalWrite(2,LOW);
        lcd.clear();
        break;
      }else{
        digitalWrite(5, HIGH);
        digitalWrite(2,HIGH);
      }
    }
    
  }else if(ppm2 >= MAX_PPM_2){
      lcd.clear();
    while (true) {

         ppm1 = MQGetGasPercentage(MQRead(MQ_PIN_1)/Ro1,GAS_H2);
         ppm2 = MQGetGasPercentage(MQRead(MQ_PIN_2)/Ro2,GAS_H2);
         lcd.setCursor(0,0);
         lcd.print("PPM Inside: ");
         lcd.print(ppm1);
         lcd.setCursor(0,1);
         lcd.print("Leakage Outside");
          
         if(ppm1>=MAX_PPM_1){
            goto both;
         }
      
      
      int btnVal = digitalRead(12);
      
      if (btnVal == 1) {
        digitalWrite(5, LOW);
        lcd.clear();
        break;
      }else{
        digitalWrite(5, HIGH);
      }
    }
    
  }
  else if(ppm1 >= MAX_PPM_1 || ppm2 >= MAX_PPM_2 ){
    both:
      lcd.clear();
      while(true){
        lcd.setCursor(0,0);
        lcd.print("Both Leakage");
        int btnVal = digitalRead(12);
        
        if (btnVal == 1) {
          digitalWrite(5, LOW);
          digitalWrite(2,LOW);
          lcd.clear();
          break;
        }else{
          digitalWrite(5, HIGH);
          digitalWrite(2,HIGH);
        }
      }
  }
  else {
    digitalWrite(5, LOW);
  }
}





float MQResistanceCalculation(int raw_adc) {
  return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}


float MQCalibration(int mq_pin) {
  int i;
  float val=0;
 
  for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val/CALIBARAION_SAMPLE_TIMES;                   //calculate the average value
 
  val = val/RO_CLEAN_AIR_FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 
                                                        //according to the chart in the datasheet 
 
  return val; 
}

float MQRead(int mq_pin) {
  int i;
  float rs=0;
 
  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }
 
  rs = rs/READ_SAMPLE_TIMES;
 
  return rs;  
}
int MQGetGasPercentage(float rs_ro_ratio, int gas_id) {
  if ( gas_id == GAS_H2) {
     return MQGetPercentage(rs_ro_ratio,H2Curve);
  }  
  return 0;
}
 
int  MQGetPercentage(float rs_ro_ratio, float *pcurve) {
  return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
